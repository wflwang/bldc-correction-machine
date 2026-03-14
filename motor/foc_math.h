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

#ifndef FOC_MATH_H_
#define FOC_MATH_H_

#include "datatypes.h"

// 定点数格式定义
// Q12.8: 12位整数 + 8位小数，范围±4095.996，精度0.0039
#define Q12_SHIFT 8
#define Q12_SCALE (1 << Q12_SHIFT)

// Q4.12: 4位整数 + 12位小数，范围±7.999，精度0.00024
#define Q4_SHIFT 12
#define Q4_SCALE (1 << Q4_SHIFT)

// Q16.16: 16位整数 + 16位小数，范围±32767.999，精度0.000015
#define Q16_SHIFT 16
#define Q16_SCALE (1 << Q16_SHIFT)

// 定点数转换宏
#define FLOAT_TO_Q12(f) ((int32_t)((f) * Q12_SCALE))
#define Q12_TO_FLOAT(q) ((float)(q) / Q12_SCALE)

#define FLOAT_TO_Q4(f) ((int16_t)((f) * Q4_SCALE))
#define Q4_TO_FLOAT(q) ((float)(q) / Q4_SCALE)

#define FLOAT_TO_Q16(f) ((int32_t)((f) * Q16_SCALE))
#define Q16_TO_FLOAT(q) ((float)(q) / Q16_SCALE)

// 定点数乘法（Q格式）
#define Q12_MUL(a, b) (((int64_t)(a) * (b)) >> Q12_SHIFT)
#define Q4_MUL(a, b) (((int32_t)(a) * (b)) >> Q4_SHIFT)
#define Q16_MUL(a, b) (((int64_t)(a) * (b)) >> Q16_SHIFT)

// 定点数除法（Q格式）
#define Q12_DIV(a, b) (((int64_t)(a) << Q12_SHIFT) / (b))
#define Q4_DIV(a, b) (((int32_t)(a) << Q4_SHIFT) / (b))

// Types
typedef struct {
	int32_t va;
	int32_t vb;
	int32_t vc;
	int32_t v_mag_filter;
	int32_t mod_alpha_filter;
	int32_t mod_beta_filter;
	int32_t mod_alpha_measured;
	int32_t mod_beta_measured;
	int32_t mod_alpha_raw;
	int32_t mod_beta_raw;
	int32_t id_target;
	int32_t iq_target;
	int32_t max_duty;
	int32_t duty_now;
	int32_t phase;
	int32_t phase_cos;
	int32_t phase_sin;
	int32_t i_alpha;
	int32_t i_beta;
	int32_t i_abs;
	int32_t i_abs_filter;
	int32_t i_bus;
	int32_t v_bus;
	int32_t v_alpha;
	int32_t v_beta;
	int32_t mod_d;
	int32_t mod_q;
	int32_t mod_q_filter;
	int32_t id;
	int32_t iq;
	int32_t id_filter;
	int32_t iq_filter;
	int32_t vd;
	int32_t vq;
	int32_t vd_int;
	int32_t vq_int;
	uint32_t svm_sector;
	bool is_using_phase_filters;
} motor_state_t;

typedef struct {
	int sample_num;
	float avg_current_tot;
	float avg_voltage_tot;
} mc_sample_t;



typedef struct {
	int32_t x1;
	int32_t x2;
	int32_t lambda_est;
	int32_t i_alpha_last;
	int32_t i_beta_last;
} observer_state;

//all data >>32 定点数
typedef struct {
    int32_t x1;             // α轴磁链估计 (Q12.8)
    int32_t x2;             // β轴磁链估计 (Q12.8)
    int32_t lambda_est;     // 估计磁链幅值 (Q12.8)
    int16_t i_alpha_last;   // 上次α电流 (Q4.12)
    int16_t i_beta_last;    // 上次β电流 (Q4.12)
} observer_state_int32;

#define MC_AUDIO_CHANNELS	4

typedef enum {
	MC_AUDIO_OFF = 0,
	MC_AUDIO_TABLE,
	MC_AUDIO_SAMPLED,
} mc_audio_mode;

typedef struct {
	mc_audio_mode mode;

	const float *table[MC_AUDIO_CHANNELS];
	int table_len[MC_AUDIO_CHANNELS];
	float table_voltage[MC_AUDIO_CHANNELS];
	float table_freq[MC_AUDIO_CHANNELS];
	float table_pos[MC_AUDIO_CHANNELS];

	// Double-buffered sampled audio
	const int8_t *sample_table[2];
	int sample_table_len[2];
	bool sample_table_filled[2];
	int sample_table_now;
	float sample_freq;
	float sample_pos;
	float sample_voltage;
} mc_audio_state;

typedef enum {
	FOC_PWM_DISABLED = 0,
	FOC_PWM_ENABLED,
	FOC_PWM_FULL_BRAKE
} foc_pwm_mode;

typedef struct {
	mc_configuration *m_conf;
	mc_state m_state;
	mc_control_mode m_control_mode;
	motor_state_t m_motor_state;
	int32_t m_curr_unbalance;
	int32_t m_currents_adc[3];
	bool m_phase_override;
	int32_t m_phase_now_override;
	int32_t m_duty_cycle_set;
	int32_t m_id_set;
	int32_t m_iq_set;
	int32_t m_i_fw_set;
	int32_t m_i_fw_override;
	int32_t m_current_off_delay;
	int32_t m_openloop_speed;
	int32_t m_openloop_phase;
	foc_pwm_mode m_pwm_mode;
	int32_t m_pos_pid_set;
	int32_t m_speed_pid_set_rpm;
	int32_t m_speed_command_rpm;
	int32_t m_phase_now_observer;
	int32_t m_phase_now_observer_override;
	int32_t m_observer_x1_override;
	int32_t m_observer_x2_override;
	bool m_phase_observer_override;
	int32_t m_phase_now_encoder;
	int32_t m_phase_now_encoder_no_index;
	observer_state m_observer_state;
	int32_t m_pll_phase;
	int32_t m_pll_speed;
	int32_t m_speed_est_fast;
	int32_t m_speed_est_fast_corrected; // Same as m_speed_est_fast, but always based on the corrected position
	int32_t m_speed_est_faster;
	mc_sample_t m_samples;
	int m_tachometer;
	int m_tachometer_abs;
	int32_t m_pos_pid_now;
	int32_t m_gamma_now;
	bool m_using_encoder;
	int m_duty1_next, m_duty2_next, m_duty3_next;
	bool m_duty_next_set;
	int32_t m_i_alpha_sample_next;
	int32_t m_i_beta_sample_next;
	int32_t m_i_alpha_sample_with_offset;
	int32_t m_i_beta_sample_with_offset;
	int32_t m_i_alpha_beta_has_offset;


	// Audio Modulation
	mc_audio_state m_audio;

	// For braking
	int32_t m_br_speed_before;
	int32_t m_br_vq_before;
	int m_br_no_duty_samples;

	int32_t m_duty_abs_filtered;
	int32_t m_duty_filtered;
	bool m_was_control_duty;
	int32_t m_duty_i_term;
	bool duty_was_pi;
	int32_t duty_pi_duty_last;
	int32_t m_openloop_angle;
	int32_t m_x1_prev;
	int32_t m_x2_prev;
	int32_t m_phase_before_speed_est;
	int32_t m_phase_before_speed_est_corrected;
	int m_tacho_step_last;
	int32_t m_pid_div_angle_last;
	int32_t m_pid_div_angle_accumulator;
	int32_t m_min_rpm_hyst_timer;
	int32_t m_min_rpm_timer;

	int32_t m_pos_i_term;
	int32_t m_pos_prev_error;
	int32_t m_pos_dt_int;
	int32_t m_pos_prev_proc;
	int32_t m_pos_dt_int_proc;
	int32_t m_pos_d_filter;
	int32_t m_pos_d_filter_proc;
	int32_t m_speed_i_term;
	int32_t m_speed_prev_error;
	int32_t m_speed_d_filter;
	int m_ang_hall_int_prev;
	bool m_using_hall;
	int32_t m_ang_hall;
	int32_t m_ang_hall_rate_limited;
	int32_t m_hall_dt_diff_last;
	int32_t m_hall_dt_diff_now;
	bool m_motor_released;
	bool m_hall_error;
	uint8_t hall_val;
	int last_ang_diff;

	// Resistance observer
	int32_t m_res_est;
	int32_t m_r_est_state;

	// Temperature-compensated parameters
	int32_t m_res_temp_comp;	//温度补偿后的电阻
	int32_t m_current_ki_temp_comp;

	// Pre-calculated values
	int32_t p_lq;
	int32_t p_ld;
	int32_t p_inv_ld_lq; // (1.0/lq - 1.0/ld)
	int32_t p_v2_v3_inv_avg_half; // (0.5/ld + 0.5/lq)
	int32_t p_duty_norm;
	int32_t p_fs;
	int32_t p_dt;
} motor_all_state_t;

// Functions
void foc_observer_update(int32_t v_alpha, int32_t v_beta, int32_t i_alpha, int32_t i_beta,
		int32_t dt, observer_state *state, int32_t *phase, motor_all_state_t *motor);
void foc_pll_run(int32_t phase, int32_t dt, int32_t *phase_var,
		int32_t *speed_var, mc_configuration *conf);
void foc_svm(int32_t alpha, int32_t beta, int32_t max_mod, uint32_t PWMFullDutyCycle,
		uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector);
void foc_run_pid_control_pos(bool index_found, int32_t dt, motor_all_state_t *motor);
void foc_run_pid_control_speed(bool index_found, int32_t dt, motor_all_state_t *motor);
int32_t foc_correct_encoder(int32_t obs_angle, int32_t enc_angle, int32_t speed, int32_t sl_erpm, motor_all_state_t *motor);
int32_t foc_correct_hall(int32_t angle, int32_t dt, motor_all_state_t *motor, int hall_val);
void foc_run_fw(motor_all_state_t *motor, int32_t dt);
void foc_precalc_values(motor_all_state_t *motor);

#endif /* FOC_MATH_H_ */
