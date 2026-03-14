/*
	Copyright 2016 - 2020 Benjamin Vedder	benjamin@vedder.se

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

#include "mcpwm_foc.h"
#include "hw_conf.h"

// Global variables
static bool init_done = false;

void mcpwm_foc_init(mc_configuration *conf) {
	init_done = true;
}

void mcpwm_foc_deinit(void) {
	init_done = false;
}

bool mcpwm_foc_init_done(void) {
	return init_done;
}

void mcpwm_foc_set_configuration(mc_configuration *configuration) {
	// Not implemented
}

mc_state mcpwm_foc_get_state(void) {
	// Not implemented
	return MC_STATE_OFF;
}

mc_control_mode mcpwm_foc_control_mode(void) {
	// Not implemented
	return CONTROL_MODE_DUTY;
}

bool mcpwm_foc_is_dccal_done(void) {
	// Not implemented
	return true;
}

int mcpwm_foc_isr_motor(void) {
	// Not implemented
	return 0;
}

void mcpwm_foc_stop_pwm(void) {
	// Not implemented
}

void mcpwm_foc_set_duty(float dutyCycle) {
	// Not implemented
}

void mcpwm_foc_set_duty_noramp(float dutyCycle) {
	// Not implemented
}

void mcpwm_foc_set_pid_speed(float rpm) {
	// Not implemented
}

void mcpwm_foc_set_pid_pos(float pos) {
	// Not implemented
}

void mcpwm_foc_set_current(float current) {
	// Not implemented
}

void mcpwm_foc_release_motor(void) {
	// Not implemented
}

void mcpwm_foc_set_brake_current(float current) {
	// Not implemented
}

void mcpwm_foc_set_handbrake(float current) {
	// Not implemented
}

void mcpwm_foc_set_openloop_current(float current, float rpm) {
	// Not implemented
}

void mcpwm_foc_set_openloop_phase(float current, float phase) {
	// Not implemented
}

void mcpwm_foc_set_openloop_duty(float dutyCycle, float rpm) {
	// Not implemented
}

void mcpwm_foc_set_openloop_duty_phase(float dutyCycle, float phase) {
	// Not implemented
}

void mcpwm_foc_set_fw_override(float current) {
	// Not implemented
}

int mcpwm_foc_set_tachometer_value(int steps) {
	// Not implemented
	return 0;
}

float mcpwm_foc_get_duty_cycle_set(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_duty_cycle_now(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_pid_speed_set(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_pid_pos_set(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_pid_pos_now(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_switching_frequency_now(void) {
	// Not implemented
	return 20000.0;
}

float mcpwm_foc_get_sampling_frequency_now(void) {
	// Not implemented
	return 20000.0;
}

float mcpwm_foc_get_rpm(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_rpm_fast(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_rpm_faster(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_tot_current(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_tot_current_filtered(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_abs_motor_current(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_abs_motor_current_unbalance(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_abs_motor_voltage(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_abs_motor_current_filtered(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_tot_current_directional(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_tot_current_directional_filtered(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_id(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_iq(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_id_set(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_iq_set(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_id_filter(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_iq_filter(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_tot_current_in(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_tot_current_in_filtered(void) {
	// Not implemented
	return 0.0;
}

int mcpwm_foc_get_tachometer_value(bool reset) {
	// Not implemented
	return 0;
}

int mcpwm_foc_get_tachometer_abs_value(bool reset) {
	// Not implemented
	return 0;
}

float mcpwm_foc_get_phase(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_phase_observer(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_phase_bemf(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_phase_encoder(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_phase_hall(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_vd(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_vq(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_mod_alpha_raw(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_mod_beta_raw(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_mod_alpha_measured(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_mod_beta_measured(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_v_alpha(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_v_beta(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_est_lambda(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_est_res(void) {
	// Not implemented
	return 0.0;
}

float mcpwm_foc_get_est_ind(void) {
	// Not implemented
	return 0.0;
}



int mcpwm_foc_encoder_detect(float current, bool print, float *offset, float *ratio, bool *inverted) {
	// Not implemented
	return 0;
}

int mcpwm_foc_measure_resistance(float current, int samples, bool stop_after, float *resistance) {
	// Not implemented
	return 0;
}

int mcpwm_foc_measure_inductance(float duty, int samples, float *curr, float *ld_lq_diff, float *inductance) {
	// Not implemented
	return 0;
}

int mcpwm_foc_measure_inductance_current(float curr_goal, int samples, float *curr, float *ld_lq_diff, float *inductance) {
	// Not implemented
	return 0;
}

bool mcpwm_foc_beep(float freq, float time, float voltage) {
	// Not implemented
	return false;
}

bool mcpwm_foc_play_tone(int channel, float freq, float voltage) {
	// Not implemented
	return false;
}

void mcpwm_foc_stop_audio(bool reset) {
	// Not implemented
}

bool mcpwm_foc_set_audio_sample_table(int channel, const float *samples, int len) {
	// Not implemented
	return false;
}

const float *mcpwm_foc_get_audio_sample_table(int channel) {
	// Not implemented
	return NULL;
}

bool mcpwm_foc_play_audio_samples(const int8_t *samples, int num_samp, float f_samp, float voltage) {
	// Not implemented
	return false;
}

int mcpwm_foc_measure_res_ind(float *res, float *ind, float *ld_lq_diff) {
	// Not implemented
	return 0;
}

int mcpwm_foc_hall_detect(float current, uint8_t *hall_table, bool *result) {
	// Not implemented
	return 0;
}

int mcpwm_foc_dc_cal(bool cal_undriven) {
	// Not implemented
	return 0;
}

void mcpwm_foc_print_state(void) {
	// Not implemented
}

void mcpwm_foc_get_current_offsets(
		volatile float *curr0_offset,
		volatile float *curr1_offset,
		volatile float *curr2_offset) {
	// Not implemented
}

void mcpwm_foc_set_current_offsets(
		volatile float curr0_offset,
		volatile float curr1_offset,
		volatile float curr2_offset) {
	// Not implemented
}

void mcpwm_foc_get_voltage_offsets(
		float *v0_offset,
		float *v1_offset,
		float *v2_offset) {
	// Not implemented
}

void mcpwm_foc_get_voltage_offsets_undriven(
		float *v0_offset,
		float *v1_offset,
		float *v2_offset) {
	// Not implemented
}

void mcpwm_foc_get_currents_adc(
		float *ph0,
		float *ph1,
		float *ph2) {
	// Not implemented
}

float mcpwm_foc_get_ts(void) {
	// Not implemented
	return 0.00005;
}

bool mcpwm_foc_is_using_encoder(void) {
	// Not implemented
	return false;
}

void mcpwm_foc_get_observer_state(float *x1, float *x2) {
	// Not implemented
}

void mcpwm_foc_set_current_off_delay(float delay_sec) {
	// Not implemented
}



void mcpwm_foc_tim_sample_int_handler(void) {
	// Not implemented
}

void mcpwm_foc_adc_int_handler(void *p, uint32_t flags) {
	// Not implemented
}

// Add the task function that's called from main.c
void mcpwm_foc_task(void) {
	// Not implemented
}
