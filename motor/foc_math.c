/*
	Copyright 2016 - 2022 Benjamin Vedder	benjamin@vedder.se

	This file is part of the VESC firmware.

	The VESC firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The VESC firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "foc_math.h"
#include "utils_math.h"
#include <math.h>

// See http://cas.ensmp.fr/~praly/Telechargement/Journaux/2010-IEEE_TPEL-Lee-Hong-Nam-Ortega-Praly-Astolfi.pdf
// MXlemming 观测方法（定点数版本），适用于M0内核
void foc_observer_update(int32_t v_alpha, int32_t v_beta, int32_t i_alpha, int32_t i_beta,
		int32_t dt, observer_state *state, int32_t *phase, motor_all_state_t *motor) {

	mc_configuration *conf_now = motor->m_conf;

	// 定点数实现 - 使用Q12.8格式
	int32_t R = FLOAT_TO_Q12(conf_now->foc_motor_r); 	//马达的电阻 (Q12.8)
	int32_t L = FLOAT_TO_Q12(conf_now->foc_motor_l); 	//马达的电感 (Q12.8)
	int32_t lambda = FLOAT_TO_Q12(conf_now->foc_motor_flux_linkage); 	//电机的磁链 (Q12.8)

	// 磁饱和补偿 (Q12.8)
	int32_t i_abs_filter = motor->m_motor_state.i_abs_filter; 	//滤波后电流绝对值 (Q12.8)
	int32_t sat_comp = FLOAT_TO_Q12(conf_now->foc_sat_comp);
	int32_t l_current_max = FLOAT_TO_Q12(conf_now->l_current_max);
	
	// comp_fact = sat_comp * i_abs_filter / l_current_max
	int32_t comp_fact = Q12_DIV(Q12_MUL(sat_comp, i_abs_filter), l_current_max);
	
	// L -= L * comp_fact
	L = L - Q12_MUL(L, comp_fact);
	lambda = lambda - Q12_MUL(lambda, comp_fact);

	// 温度补偿
	if (conf_now->foc_temp_comp) { 	//电阻的温度补偿是否打开
		R = motor->m_res_temp_comp; 	//电机阻值温度补偿 (Q12.8)
	}

	// 凸极效应补偿（简化实现）
	int32_t ld_lq_diff = FLOAT_TO_Q12(conf_now->foc_motor_ld_lq_diff);
	int32_t id = motor->m_motor_state.id;
	int32_t iq = motor->m_motor_state.iq;
	
	// 0.1A阈值 (Q12.8)
	int32_t threshold = FLOAT_TO_Q12(0.1f);
	if (ABS(id) > threshold || ABS(iq) > threshold) {
		int32_t iq_sq = Q12_MUL(iq, iq);
		int32_t id_sq = Q12_MUL(id, id);
		int32_t denom = id_sq + iq_sq;
		
		if (denom > 0) {
			// saliency = ld_lq_diff * iq_sq / denom
			int32_t saliency = Q12_DIV(Q12_MUL(ld_lq_diff, iq_sq), denom);
			
			// L = L - ld_lq_diff * 0.5 + saliency * 0.5
			int32_t half_ld_lq = ld_lq_diff >> 1;
			int32_t half_saliency = saliency >> 1;
			L = L - half_ld_lq + half_saliency;
		}
	}

	// MXlemming观测器核心算法（定点数实现）
	// v_alpha_R_ia = v_alpha - R * i_alpha
	int32_t v_alpha_R_ia = v_alpha - Q12_MUL(R, i_alpha);
	// v_beta_R_ib = v_beta - R * i_beta
	int32_t v_beta_R_ib = v_beta - Q12_MUL(R, i_beta);
	
	// delta_i_alpha = i_alpha - state->i_alpha_last
	int32_t delta_i_alpha = i_alpha - state->i_alpha_last;
	// delta_i_beta = i_beta - state->i_beta_last
	int32_t delta_i_beta = i_beta - state->i_beta_last;

	// 更新磁链观测值
	// state->x1 += v_alpha_R_ia * dt - L * delta_i_alpha
	state->x1 += Q12_MUL(v_alpha_R_ia, dt) - Q12_MUL(L, delta_i_alpha);
	// state->x2 += v_beta_R_ib * dt - L * delta_i_beta
	state->x2 += Q12_MUL(v_beta_R_ib, dt) - Q12_MUL(L, delta_i_beta);

	// 磁链幅值自适应
	#ifdef OBServerMXlemming_LambdaCompEn
	int32_t x1_sq = Q12_MUL(state->x1, state->x1);
	int32_t x2_sq = Q12_MUL(state->x2, state->x2);
	int32_t lambda_est_sq = Q12_MUL(state->lambda_est, state->lambda_est);
	int32_t err = lambda_est_sq - (x1_sq + x2_sq);
	
	int32_t gamma_half = motor->m_gamma_now >> 1;
	int32_t update = Q12_MUL(Q12_MUL(Q12_MUL(FLOAT_TO_Q12(0.1f), gamma_half), state->lambda_est), -err);
	update = Q12_MUL(update, dt);
	state->lambda_est += update;
	
	// 限幅
	int32_t lambda_min = Q12_MUL(lambda, FLOAT_TO_Q12(0.3f));
	int32_t lambda_max = Q12_MUL(lambda, FLOAT_TO_Q12(2.5f));
	if (state->lambda_est < lambda_min) state->lambda_est = lambda_min;
	if (state->lambda_est > lambda_max) state->lambda_est = lambda_max;
	
	// 磁链限幅
	if (state->x1 > state->lambda_est) state->x1 = state->lambda_est;
	if (state->x1 < -state->lambda_est) state->x1 = -state->lambda_est;
	if (state->x2 > state->lambda_est) state->x2 = state->lambda_est;
	if (state->x2 < -state->lambda_est) state->x2 = -state->lambda_est;
	#else
	// 不带磁链补偿的限幅
	if (state->x1 > lambda) state->x1 = lambda;
	if (state->x1 < -lambda) state->x1 = -lambda;
	if (state->x2 > lambda) state->x2 = lambda;
	if (state->x2 < -lambda) state->x2 = -lambda;
	#endif

	// 保存电流值
	state->i_alpha_last = i_alpha;
	state->i_beta_last = i_beta;

	// 防止磁链幅值过低
	int32_t mag_sq = Q12_MUL(state->x1, state->x1) + Q12_MUL(state->x2, state->x2);
	
	// 使用快速开方函数
	int32_t mag = utils_sqrt(mag_sq);
	
	int32_t half_lambda = lambda >> 1;
	if (mag < half_lambda) {
		// 适当放大 1.1倍 (Q12.8格式)
		int32_t scale = FLOAT_TO_Q12(1.1f);
		state->x1 = Q12_MUL(state->x1, scale);
		state->x2 = Q12_MUL(state->x2, scale);
	}

	// 计算相位 - 使用快速反正切函数
	if (phase) {
		*phase = utils_fast_atan2(state->x2, state->x1);
	}

	// 保存观测器角度
	if (motor) {
		motor->m_phase_now_observer = *phase;
	}
}
/**
 * 
 * 参数说明：
 * phase：输入相位（观测器直接输出的原始角度，可能有噪声/跳变）
 * dt：控制周期（秒）
 * phase_var：输出相位（平滑后的连续角度）上次用的角度
 * speed_var：输出速度（估算的角速度）
 * conf：配置参数，包含PLL的PI控制器参数
 * // 监控这些变量来调试PLL：
 * int32_t phase_error = delta_theta;      // 相位误差，应趋近于0
 * int32_t pll_bandwidth = Q12_DIV(conf->foc_pll_kp, FLOAT_TO_Q12(2.0f * 3.1415926f));  // 近似带宽(Hz)
 * PLL的-3dB带宽 ≈ Kp/(2π)，积分时间常数 ≈ Kp/Ki
 */
void foc_pll_run(int32_t phase, int32_t dt, int32_t *phase_var,
					int32_t *speed_var, mc_configuration *conf) {
	// 定点数版本 (Q12.8格式)
	int32_t delta_theta = phase - *phase_var;
	
	// 归一化角度 (Q12.8格式)
	// 2π弧度 = 6.2831853 = FLOAT_TO_Q12(6.2831853f)
	int32_t two_pi = FLOAT_TO_Q12(2.0f * 3.1415926535f);
	while (delta_theta < -two_pi / 2) delta_theta += two_pi;
	while (delta_theta >= two_pi / 2) delta_theta -= two_pi;
	
	// 转换KP和KI到Q12.8格式
	int32_t kp = FLOAT_TO_Q12(conf->foc_pll_kp);
	int32_t ki = FLOAT_TO_Q12(conf->foc_pll_ki);
	
	// *phase_var += (*speed_var + conf->foc_pll_kp * delta_theta) * dt
	*phase_var += Q12_MUL(*speed_var + Q12_MUL(kp, delta_theta), dt);
	
	// 归一化相位
	while (*phase_var < -two_pi / 2) *phase_var += two_pi;
	while (*phase_var >= two_pi / 2) *phase_var -= two_pi;
	
	// *speed_var += conf->foc_pll_ki * delta_theta * dt
	*speed_var += Q12_MUL(Q12_MUL(ki, delta_theta), dt);
}

/**
 * @brief svm Space vector modulation. Magnitude must not be larger than sqrt(3)/2, or 0.866 to avoid overmodulation.
 *        See https://github.com/vedderb/bldc/pull/372#issuecomment-962499623 for a full description.
 * @param alpha voltage
 * @param beta Park transformed and normalized voltage
 * @param PWMFullDutyCycle is the peak value of the PWM counter.
 * @param tAout PWM duty cycle phase A (0 = off all of the time, PWMFullDutyCycle = on all of the time)
 * @param tBout PWM duty cycle phase B
 * @param tCout PWM duty cycle phase C
 */
void foc_svm(int32_t alpha, int32_t beta, int32_t max_mod, uint32_t PWMFullDutyCycle,
			uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector) {
	uint32_t sector;

	if (beta >= 0) {
		if (alpha >= 0) {
			//quadrant I
			if (Q12_MUL(FLOAT_TO_Q12(ONE_BY_SQRT3), beta) > alpha) {
				sector = 2;
			} else {
				sector = 1;
			}
		} else {
			//quadrant II
			if (Q12_MUL(FLOAT_TO_Q12(-ONE_BY_SQRT3), beta) > alpha) {
				sector = 3;
			} else {
				sector = 2;
			}
		}
	} else {
		if (alpha >= 0) {
			//quadrant IV5
			if (Q12_MUL(FLOAT_TO_Q12(-ONE_BY_SQRT3), beta) > alpha) {
				sector = 5;
			} else {
				sector = 6;
			}
		} else {
			//quadrant III
			if (Q12_MUL(FLOAT_TO_Q12(ONE_BY_SQRT3), beta) > alpha) {
				sector = 4;
			} else {
				sector = 5;
			}
		}
	}

	// PWM timings
	int tA, tB, tC;

	switch (sector) {

	// sector 1-2
	case 1: {
		// Vector on-times
		int t1 = Q12_TO_FLOAT(alpha - Q12_MUL(FLOAT_TO_Q12(ONE_BY_SQRT3), beta)) * PWMFullDutyCycle;
		int t2 = Q12_TO_FLOAT(Q12_MUL(FLOAT_TO_Q12(TWO_BY_SQRT3), beta)) * PWMFullDutyCycle;

		// PWM timings
		tA = (PWMFullDutyCycle + t1 + t2) / 2;
		tB = tA - t1;
		tC = tB - t2;

		break;
	}

	// sector 2-3
	case 2: {
		// Vector on-times
		int t2 = Q12_TO_FLOAT(alpha + Q12_MUL(FLOAT_TO_Q12(ONE_BY_SQRT3), beta)) * PWMFullDutyCycle;
		int t3 = Q12_TO_FLOAT(-alpha + Q12_MUL(FLOAT_TO_Q12(ONE_BY_SQRT3), beta)) * PWMFullDutyCycle;

		// PWM timings
		tB = (PWMFullDutyCycle + t2 + t3) / 2;
		tA = tB - t3;
		tC = tA - t2;

		break;
	}

	// sector 3-4
	case 3: {
		// Vector on-times
		int t3 = (TWO_BY_SQRT3 * beta) * PWMFullDutyCycle;
		int t4 = (-alpha - ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;

		// PWM timings
		tB = (PWMFullDutyCycle + t3 + t4) / 2;
		tC = tB - t3;
		tA = tC - t4;

		break;
	}

	// sector 4-5
	case 4: {
		// Vector on-times
		int t4 = (-alpha + ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;
		int t5 = (-TWO_BY_SQRT3 * beta) * PWMFullDutyCycle;

		// PWM timings
		tC = (PWMFullDutyCycle + t4 + t5) / 2;
		tB = tC - t5;
		tA = tB - t4;

		break;
	}

	// sector 5-6
	case 5: {
		// Vector on-times
		int t5 = (-alpha - ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;
		int t6 = (alpha - ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;

		// PWM timings
		tC = (PWMFullDutyCycle + t5 + t6) / 2;
		tA = tC - t5;
		tB = tA - t6;

		break;
	}

	// sector 6-1
	case 6: {
		// Vector on-times
		int t6 = (-TWO_BY_SQRT3 * beta) * PWMFullDutyCycle;
		int t1 = (alpha + ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;

		// PWM timings
		tA = (PWMFullDutyCycle + t6 + t1) / 2;
		tC = tA - t1;
		tB = tC - t6;

		break;
	}
	}

	int t_max = PWMFullDutyCycle * (1.0 - (1.0 - max_mod) * 0.5);
	utils_truncate_number_int(&tA, 0, t_max);
	utils_truncate_number_int(&tB, 0, t_max);
	utils_truncate_number_int(&tC, 0, t_max);

	*tAout = tA;
	*tBout = tB;
	*tCout = tC;
	*svm_sector = sector;
}

void foc_run_pid_control_pos(bool index_found, int32_t dt, motor_all_state_t *motor) {
	mc_configuration *conf_now = motor->m_conf;

	float angle_now = motor->m_pos_pid_now;
	float angle_set = motor->m_pos_pid_set;

	float p_term;
	float d_term;
	float d_term_proc;

	// PID is off. Return.
	if (motor->m_control_mode != CONTROL_MODE_POS) {
		motor->m_pos_i_term = 0;
		motor->m_pos_prev_error = 0;
		motor->m_pos_prev_proc = angle_now;
		motor->m_pos_d_filter = 0.0;
		motor->m_pos_d_filter_proc = 0.0;
		return;
	}

	// Compute parameters
	float error = utils_angle_difference(angle_set, angle_now);
	float error_sign = 1.0;

	if (conf_now->m_sensor_port_mode != SENSOR_PORT_MODE_HALL) {
		if (conf_now->foc_encoder_inverted) {
			error_sign = -1.0;
		}
	}

	error *= error_sign;

	float kp = conf_now->p_pid_kp;
	float ki = conf_now->p_pid_ki;
	float kd = conf_now->p_pid_kd;
	float kd_proc = conf_now->p_pid_kd_proc;

	if (conf_now->p_pid_gain_dec_angle > 0.1) {
		float min_error = conf_now->p_pid_gain_dec_angle / conf_now->p_pid_ang_div;
		float error_abs = fabs(error);

		if (error_abs < min_error) {
			float scale = error_abs / min_error;
			kp *= scale;
			ki *= scale;
			kd *= scale;
			kd_proc *= scale;
		}
	}

	p_term = error * kp;
	float pos_i_term = motor->m_pos_i_term;
	pos_i_term += error * (ki * (float)dt);

	// Average DT for the D term when the error does not change. This likely
	// happens at low speed when the position resolution is low and several
	// control iterations run without position updates.
	// TODO: Are there problems with this approach?
	motor->m_pos_dt_int += dt;
	if (error == motor->m_pos_prev_error) {
		d_term = 0.0;
	} else {
		d_term = (error - motor->m_pos_prev_error) * (kd / (float)motor->m_pos_dt_int);
		motor->m_pos_dt_int = 0.0;
	}

	// Filter D
	UTILS_LP_FAST(motor->m_pos_d_filter, d_term, conf_now->p_pid_kd_filter);
	d_term = motor->m_pos_d_filter;

	// Process D term
	motor->m_pos_dt_int_proc += dt;
	if (angle_now == motor->m_pos_prev_proc) {
		d_term_proc = 0.0;
	} else {
		d_term_proc = -utils_angle_difference(angle_now, motor->m_pos_prev_proc) * error_sign * (kd_proc / (float)motor->m_pos_dt_int_proc);
		motor->m_pos_dt_int_proc = 0.0;
	}

	// Filter D process
	UTILS_LP_FAST(motor->m_pos_d_filter_proc, d_term_proc, conf_now->p_pid_kd_filter);
	d_term_proc = motor->m_pos_d_filter_proc;

	// I-term wind-up protection
	float p_tmp = p_term;
	utils_truncate_number_abs(&p_tmp, 1.0);
	utils_truncate_number_abs(&pos_i_term, 1.0 - fabsf(p_tmp));
	motor->m_pos_i_term = pos_i_term;

	// Store previous error
	motor->m_pos_prev_error = error;
	motor->m_pos_prev_proc = angle_now;

	// Calculate output
	float output = p_term + motor->m_pos_i_term + d_term + d_term_proc;
	utils_truncate_number(&output, -1.0, 1.0);

	if (conf_now->m_sensor_port_mode != SENSOR_PORT_MODE_HALL) {
		if (index_found) {
			motor->m_iq_set = output * conf_now->l_current_max * conf_now->l_current_max_scale;;
		} else {
			// Rotate the motor with 40 % power until the encoder index is found.
			motor->m_iq_set = 0.4 * conf_now->l_current_max * conf_now->l_current_max_scale;;
		}
	} else {
		motor->m_iq_set = output * conf_now->l_current_max * conf_now->l_current_max_scale;;
	}
}

void foc_run_pid_control_speed(bool index_found, int32_t dt, motor_all_state_t *motor) {
	mc_configuration *conf_now = motor->m_conf;
	float p_term;
	float d_term;

	// PID is off. Return.
	if (motor->m_control_mode != CONTROL_MODE_SPEED) {
		motor->m_speed_i_term = 0.0;
		motor->m_speed_prev_error = 0.0;
		motor->m_speed_d_filter = 0.0;
		return;
	}

	if (conf_now->s_pid_ramp_erpms_s > 0.0) {
		float speed_set = motor->m_speed_pid_set_rpm;
		utils_step_towards(&speed_set, motor->m_speed_command_rpm, conf_now->s_pid_ramp_erpms_s * dt);
		if (!index_found) {
			utils_truncate_number_abs(&speed_set, conf_now->foc_openloop_rpm);
		}
		utils_truncate_number(&speed_set, conf_now->l_min_erpm, conf_now->l_max_erpm);
		motor->m_speed_pid_set_rpm = speed_set;
	}

	float rpm = 0.0;
	switch (conf_now->s_pid_speed_source) {
	case S_PID_SPEED_SRC_PLL:
		rpm = RADPS2RPM_f(motor->m_pll_speed);
		break;
	case S_PID_SPEED_SRC_FAST:
		rpm = RADPS2RPM_f(motor->m_speed_est_fast);
		break;
	case S_PID_SPEED_SRC_FASTER:
		rpm = RADPS2RPM_f(motor->m_speed_est_faster);
		break;
	}

	float error = motor->m_speed_pid_set_rpm - rpm;

	// Too low RPM set. Reset state, release motor and return.
	if (fabsf(motor->m_speed_pid_set_rpm) < conf_now->s_pid_min_erpm) {
		motor->m_speed_i_term = 0.0;
		motor->m_speed_prev_error = error;
		motor->m_iq_set = 0.0;
		return;
	}

	// Compute parameters
	p_term = error * conf_now->s_pid_kp * (1.0 / 20.0);
	d_term = (error - motor->m_speed_prev_error) * (conf_now->s_pid_kd / (float)dt) * (1.0 / 20.0);

	// Filter D
	UTILS_LP_FAST(motor->m_speed_d_filter, d_term, conf_now->s_pid_kd_filter);
	d_term = motor->m_speed_d_filter;

	// Store previous error
	motor->m_speed_prev_error = error;

	// Calculate output
	float speed_i_term = motor->m_speed_i_term;
	float output = p_term + speed_i_term + d_term;
	utils_truncate_number_abs(&output, 1.0);

	// Integrator windup protection
	speed_i_term += error * conf_now->s_pid_ki * (float)dt * (1.0 / 20.0);
	utils_truncate_number_abs(&speed_i_term, 1.0);
	motor->m_speed_i_term = speed_i_term;

	if (conf_now->s_pid_ki < 1e-9) {
		motor->m_speed_i_term = 0.0;
	}

	// Optionally disable braking
	if (!conf_now->s_pid_allow_braking) {
		if (rpm > 20.0 && output < 0.0) {
			output = 0.0;
		}

		if (rpm < -20.0 && output > 0.0) {
			output = 0.0;
		}
	}

	motor->m_iq_set = output * conf_now->lo_current_max * conf_now->l_current_max_scale;
}

int32_t foc_correct_encoder(int32_t obs_angle, int32_t enc_angle, int32_t speed,
							 int32_t sl_erpm, motor_all_state_t *motor) {
	// 定点数版本 (Q12.8格式)
	// 将speed从rad/s转换为rpm
	int32_t radps2rpm = FLOAT_TO_Q12(9.54929658551f);
	int32_t rpm_abs = ABS(Q12_MUL(speed, radps2rpm));

	// Hysteresis 5 % of total speed (Q12.8)
	int32_t hyst = Q12_MUL(sl_erpm, FLOAT_TO_Q12(0.05f));
	if (motor->m_using_encoder) {
		if (rpm_abs > (sl_erpm + hyst)) {
			motor->m_using_encoder = false;
		}
	} else {
		if (rpm_abs < (sl_erpm - hyst)) {
			motor->m_using_encoder = true;
		}
	}

	return motor->m_using_encoder ? enc_angle : obs_angle;
}
/**
 * 
 * @param angle 当前(观测器)的角度
 * @param dt 处理的间隔时间 s
 * @param motor 当前选择马达的结构体
 * @param val 当前的霍尔值
 */
int32_t foc_correct_hall(int32_t angle, int32_t dt, motor_all_state_t *motor, int hall_val) {
	mc_configuration *conf_now = motor->m_conf;
	motor->m_hall_dt_diff_now += dt;

	// 将speed从rad/s转换为rpm (Q12.8)
	int32_t radps2rpm = FLOAT_TO_Q12(9.54929658551f);
	int32_t rpm_abs = ABS(Q12_MUL(motor->m_pll_speed, radps2rpm));

	// rad_per_sec_hall = (π/3) / m_hall_dt_diff_last (Q12.8)
	int32_t pi_div_3 = FLOAT_TO_Q12(3.1415926535f / 3.0f);
	int32_t rad_per_sec_hall = Q12_DIV(pi_div_3, motor->m_hall_dt_diff_last);
	
	// rpm_abs_hall = abs(RADPS2RPM_f(rad_per_sec_hall)) (Q12.8)
	int32_t rpm_abs_hall = ABS(Q12_MUL(rad_per_sec_hall, radps2rpm));

	// 检测霍尔错误
	if ((hall_val == 0) || (hall_val == 7)) {
		// 霍尔错误，切换到观测器
		motor->m_using_hall = false;
		return angle;
	}

	//hall table[] = 000 001 011 010 110 100 101 111
	int ang_hall_int = conf_now->foc_hall_table[hall_val];	//当前值对应表格位置的角度值

	// Only override the observer if the hall sensor value is valid.
	if (ang_hall_int < 201) {
		// Scale to the circle and convert to radians (Q12.8)
		// ang_hall_now = (ang_hall_int / 200.0) * 2π
		int32_t scale = Q12_DIV(ang_hall_int << Q12_SHIFT, 200);
		int32_t two_pi = FLOAT_TO_Q12(2.0f * 3.1415926535f);
		int32_t ang_hall_now = Q12_MUL(scale, two_pi);

		if (motor->m_ang_hall_int_prev < 0) {
			//上次角度不是正确值
			// Previous angle not valid
			motor->m_ang_hall_int_prev = ang_hall_int;
			motor->m_ang_hall = ang_hall_now;
		} else if (ang_hall_int != motor->m_ang_hall_int_prev) {
			int diff = ang_hall_int - motor->m_ang_hall_int_prev; 	//角度变化超过一半时候自动增加一个周期
			if (diff > 100) {
				diff -= 200;
			} else if (diff < -100) {
				diff += 200;
			}

			// This is only valid if the direction did not just change. If it did, we use the
			// last speed together with the sign right now.
			int sign_diff = (diff >= 0) ? 1 : -1;
			int sign_last = (motor->m_hall_dt_diff_last >= 0) ? 1 : -1;
			
			if (sign_diff == sign_last) {	//与上次误差方向是否一致
				if (diff > 0) {
					motor->m_hall_dt_diff_last = motor->m_hall_dt_diff_now; 	//上前时间增量赋值 表示霍尔变化花费的时间
				} else {
					motor->m_hall_dt_diff_last = -motor->m_hall_dt_diff_now;
				}
			} else {
				motor->m_hall_dt_diff_last = -motor->m_hall_dt_diff_last;
			}

			motor->m_hall_dt_diff_now = 0;

			// A transition was just made. The angle is in the middle of the new and old angle.
			int ang_avg = motor->m_ang_hall_int_prev + diff / 2; //取霍尔角度变化的中点值
			ang_avg %= 200;
			if (ang_avg < 0) ang_avg += 200;

			// Scale to the circle and convert to radians (Q12.8)
			scale = Q12_DIV(ang_avg << Q12_SHIFT, 200);
			motor->m_ang_hall = Q12_MUL(scale, two_pi);
		}

		motor->m_ang_hall_int_prev = ang_hall_int;

		// 计算当前速度并比较
		int32_t max_dt = (ABS(motor->m_hall_dt_diff_now) > ABS(motor->m_hall_dt_diff_last)) ? 
						motor->m_hall_dt_diff_now : motor->m_hall_dt_diff_last;
		int32_t current_rpm = ABS(Q12_MUL(Q12_DIV(pi_div_3, max_dt), radps2rpm));
		
		int32_t hall_interp_erpm = FLOAT_TO_Q12(conf_now->foc_hall_interp_erpm);
		
		if (current_rpm < hall_interp_erpm) {
			// Don't interpolate on very low speed, just use the closest hall sensor.
			motor->m_ang_hall = ang_hall_now;
		} else {
			// Interpolate 插值
			// 计算角度差
			int32_t diff = motor->m_ang_hall - ang_hall_now;
			
			// 归一化角度差
			while (diff < -two_pi / 2) diff += two_pi;
			while (diff >= two_pi / 2) diff -= two_pi;
			
			int32_t pi_div_6 = FLOAT_TO_Q12(3.1415926535f / 6.0f);
			int sign_rad = (rad_per_sec_hall >= 0) ? 1 : -1;
			int sign_diff = (diff >= 0) ? 1 : -1;
			
			if (ABS(diff) < pi_div_6 || sign_diff != sign_rad) {
				// 正常插值：角度 = 角度 + 角速度 × 时间
				motor->m_ang_hall += Q12_MUL(rad_per_sec_hall, dt);
			} else {
				// 角度偏差过大：缓慢修正 (0.01倍)
				int32_t correction = Q12_MUL(diff, FLOAT_TO_Q12(0.01f));
				motor->m_ang_hall -= correction;
			}
		}

		// 归一化角度
		while (motor->m_ang_hall < -two_pi / 2) motor->m_ang_hall += two_pi;
		while (motor->m_ang_hall >= two_pi / 2) motor->m_ang_hall -= two_pi;

		// Limit hall sensor rate of change.
		// angle_step = (max(rpm_abs_hall, foc_hall_interp_erpm) / 60.0) * 2π * dt * 1.5
		int32_t max_rpm = (rpm_abs_hall > hall_interp_erpm) ? rpm_abs_hall : hall_interp_erpm;
		int32_t rpm_div_60 = Q12_DIV(max_rpm, FLOAT_TO_Q12(60.0f));
		int32_t angle_step = Q12_MUL(Q12_MUL(Q12_MUL(rpm_div_60, two_pi), dt), FLOAT_TO_Q12(1.5f));
		
		// 计算角度差
		int32_t angle_diff = motor->m_ang_hall - motor->m_ang_hall_rate_limited;
		while (angle_diff < -two_pi / 2) angle_diff += two_pi;
		while (angle_diff >= two_pi / 2) angle_diff -= two_pi;
		
		if (ABS(angle_diff) < angle_step) {
			motor->m_ang_hall_rate_limited = motor->m_ang_hall;
		} else {
			int32_t step_sign = (angle_diff >= 0) ? 1 : -1;
			motor->m_ang_hall_rate_limited += angle_step * step_sign;
		}

		// 归一化角度
		while (motor->m_ang_hall_rate_limited < -two_pi / 2) motor->m_ang_hall_rate_limited += two_pi;
		while (motor->m_ang_hall_rate_limited >= two_pi / 2) motor->m_ang_hall_rate_limited -= two_pi;
	}

	// 速度判断，决定使用霍尔还是观测器
	int32_t sl_erpm = FLOAT_TO_Q12(conf_now->foc_sl_erpm);
	if (rpm_abs < sl_erpm) {
		motor->m_using_hall = true;
	} else {
		motor->m_using_hall = false;
	}

	// 平滑过渡
	int32_t angle_old = angle;
	if (motor->m_using_hall && ang_hall_int < 201) {
		angle = motor->m_ang_hall_rate_limited;
	}

	// Map output angle between hall angle and observer angle in transition region to make
	// a smooth transition.
	if (angle_old != angle) {
		// weight_hall = utils_map(rpm_abs, sl_erpm_start, sl_erpm, 1.0, 0.0)
		int32_t sl_erpm_start = FLOAT_TO_Q12(conf_now->foc_sl_erpm_start);
		int32_t weight_hall = Q12_DIV((rpm_abs - sl_erpm_start) << Q12_SHIFT, (sl_erpm - sl_erpm_start));
		weight_hall = FLOAT_TO_Q12(1.0f) - weight_hall;
		
		// 限幅
		if (weight_hall < 0) weight_hall = 0;
		if (weight_hall > FLOAT_TO_Q12(1.0f)) weight_hall = FLOAT_TO_Q12(1.0f);
		
		// 插值：angle = angle * (1-weight) + angle_old * weight
		int32_t angle_new = Q12_MUL(angle, FLOAT_TO_Q12(1.0f) - weight_hall) + Q12_MUL(angle_old, weight_hall);
		angle = angle_new;
	}

	return angle;
}

void foc_run_fw(motor_all_state_t *motor, int32_t dt) {
	// 转换配置参数到Q12.8格式
	int32_t foc_fw_current_max = FLOAT_TO_Q12(motor->m_conf->foc_fw_current_max);
	int32_t cc_min_current = FLOAT_TO_Q12(motor->m_conf->cc_min_current);
	int32_t min_threshold = (cc_min_current > FLOAT_TO_Q12(0.001f)) ? cc_min_current : FLOAT_TO_Q12(0.001f);
	
	if (foc_fw_current_max < min_threshold) {
		return;
	}

	// Field Weakening
	if (motor->m_state == MC_STATE_RUNNING &&
			(motor->m_control_mode == CONTROL_MODE_CURRENT ||
					motor->m_control_mode == CONTROL_MODE_CURRENT_BRAKE ||
					motor->m_control_mode == CONTROL_MODE_SPEED ||
					motor->m_i_fw_set > min_threshold)) {
		int32_t fw_current_now = 0;
		int32_t duty_abs = motor->m_duty_abs_filtered;

		int32_t foc_fw_duty_start = FLOAT_TO_Q12(motor->m_conf->foc_fw_duty_start);
		int32_t l_max_duty = FLOAT_TO_Q12(motor->m_conf->l_max_duty);
		int32_t threshold_duty = Q12_MUL(foc_fw_duty_start, l_max_duty);
		
		if (foc_fw_duty_start < FLOAT_TO_Q12(0.99f) && duty_abs > threshold_duty) {
			// fw_current_now = utils_map(duty_abs, threshold_duty, l_max_duty, 0, foc_fw_current_max)
			int32_t range = l_max_duty - threshold_duty;
			int32_t position = duty_abs - threshold_duty;
			fw_current_now = Q12_DIV(Q12_MUL(position, foc_fw_current_max), range);

			motor->m_current_off_delay = FLOAT_TO_Q12(1.0f);
		}

		int32_t foc_fw_ramp_time = FLOAT_TO_Q12(motor->m_conf->foc_fw_ramp_time);
		
		if (foc_fw_ramp_time < dt) {
			motor->m_i_fw_set = fw_current_now;
		} else {
			// step = (dt / foc_fw_ramp_time) * foc_fw_current_max
			int32_t step = Q12_MUL(Q12_DIV(dt, foc_fw_ramp_time), foc_fw_current_max);
			
			// utils_step_towards
			if (motor->m_i_fw_set < fw_current_now) {
				if ((motor->m_i_fw_set + step) < fw_current_now) {
					motor->m_i_fw_set += step;
				} else {
					motor->m_i_fw_set = fw_current_now;
				}
			} else if (motor->m_i_fw_set > fw_current_now) {
				if ((motor->m_i_fw_set - step) > fw_current_now) {
					motor->m_i_fw_set -= step;
				} else {
					motor->m_i_fw_set = fw_current_now;
				}
			}
		}
	}
}


/**
 * @brief FOC 参数初始化
 * 
 * 
 */
void foc_precalc_values(motor_all_state_t *motor) {
	const mc_configuration *conf_now = motor->m_conf;
	
	// 转换配置参数到Q12.8格式
	int32_t foc_motor_l = FLOAT_TO_Q12(conf_now->foc_motor_l);
	int32_t foc_motor_ld_lq_diff = FLOAT_TO_Q12(conf_now->foc_motor_ld_lq_diff);
	int32_t foc_motor_flux_linkage = FLOAT_TO_Q12(conf_now->foc_motor_flux_linkage);
	int32_t foc_overmod_factor = FLOAT_TO_Q12(conf_now->foc_overmod_factor);
	
	// p_lq = foc_motor_l + foc_motor_ld_lq_diff * 0.5
	motor->p_lq = foc_motor_l + (foc_motor_ld_lq_diff >> 1);
	// p_ld = foc_motor_l - foc_motor_ld_lq_diff * 0.5
	motor->p_ld = foc_motor_l - (foc_motor_ld_lq_diff >> 1);
	
	//凸极补偿相关参数
	// p_inv_ld_lq = (1.0 / p_lq - 1.0 / p_ld)
	int32_t one_q12 = FLOAT_TO_Q12(1.0f);
	int32_t inv_lq = Q12_DIV(one_q12, motor->p_lq);
	int32_t inv_ld = Q12_DIV(one_q12, motor->p_ld);
	motor->p_inv_ld_lq = inv_lq - inv_ld;
	
	//电压限制计算
	// p_v2_v3_inv_avg_half = (0.5 / p_lq + 0.5 / p_ld) * 0.9
	int32_t half_inv_lq = inv_lq >> 1;
	int32_t half_inv_ld = inv_ld >> 1;
	int32_t avg = half_inv_lq + half_inv_ld;
	motor->p_v2_v3_inv_avg_half = Q12_MUL(avg, FLOAT_TO_Q12(0.9f));
	
	//将观测器的磁链估计初始化为电机的额定磁链
	motor->m_observer_state.lambda_est = foc_motor_flux_linkage;
	
	//数学原理：
	//TWO_BY_SQRT3= 2/√3 ≈ 1.1547
	//这是空间矢量调制（SVPWM）​ 的最大调制系数
	//foc_overmod_factor是过调制因子，允许超过六边形调制区域
	int32_t two_by_sqrt3 = FLOAT_TO_Q12(TWO_BY_SQRT3);
	motor->p_duty_norm = Q12_DIV(two_by_sqrt3, foc_overmod_factor);

	// 零矢量采样频率 (Q12.8)
	int32_t foc_f_zv = FLOAT_TO_Q12(conf_now->foc_f_zv);
	motor->p_fs = foc_f_zv >> 1;
	
	// 得出每次中断的时间 (Q12.8)
	motor->p_dt = Q12_DIV(one_q12, motor->p_fs);
}
