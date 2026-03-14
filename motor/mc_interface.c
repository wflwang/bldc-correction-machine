/*
	Copyright 2016 Benjamin Vedder	benjamin@vedder.se

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

#include "mc_interface.h"
#include "mcpwm.h"
#include "mcpwm_foc.h"
#include <string.h>

// Global variables
static mc_configuration mcconf;
static volatile mc_fault_code fault_code = FAULT_CODE_NONE;
static volatile mc_state motor_state = MC_STATE_OFF;

// External variables
volatile uint16_t ADC_Value[] = {0};
volatile float ADC_curr_norm_value[] = {0.0};
volatile float ADC_curr_raw[] = {0.0};

void mc_interface_init(void) {
	// Initialize motor configuration
	conf_general_read_mc_configuration(&mcconf);
}



const volatile mc_configuration* mc_interface_get_configuration(void) {
	return &mcconf;
}

void mc_interface_set_configuration(mc_configuration *configuration) {
	mcconf = *configuration;
}

unsigned mc_interface_calc_crc(mc_configuration* conf) {
	// Simplified: just return 0 for now
	return 0;
}

bool mc_interface_dccal_done(void) {
	// Simplified: just return true for now
	return true;
}

void mc_interface_set_pwm_callback(void (*p_func)(void)) {
	// Not implemented
}

void mc_interface_lock(void) {
	// Not implemented
}

void mc_interface_unlock(void) {
	// Not implemented
}

void mc_interface_lock_override_once(void) {
	// Not implemented
}

mc_fault_code mc_interface_get_fault(void) {
	return fault_code;
}

const char* mc_interface_fault_to_string(mc_fault_code fault) {
	// Simplified: just return a generic string
	return "No fault";
}

mc_state mc_interface_get_state(void) {
	return motor_state;
}

mc_control_mode mc_interface_get_control_mode(void) {
	// Simplified: just return duty mode
	return CONTROL_MODE_DUTY;
}

void mc_interface_set_duty(float dutyCycle) {
	// Not implemented
}

void mc_interface_set_duty_noramp(float dutyCycle) {
	// Not implemented
}

void mc_interface_set_pid_speed(float rpm) {
	// Not implemented
}

void mc_interface_set_pid_pos(float pos) {
	// Not implemented
}

void mc_interface_set_current(float current) {
	// Not implemented
}

void mc_interface_set_brake_current(float current) {
	// Not implemented
}

void mc_interface_set_current_rel(float val) {
	// Not implemented
}

void mc_interface_set_brake_current_rel(float val) {
	// Not implemented
}

void mc_interface_set_handbrake(float current) {
	// Not implemented
}

void mc_interface_set_handbrake_rel(float val) {
	// Not implemented
}

void mc_interface_set_openloop_current(float current, float rpm) {
	// Not implemented
}

void mc_interface_set_openloop_phase(float current, float phase) {
	// Not implemented
}

void mc_interface_set_openloop_duty(float dutyCycle, float rpm) {
	// Not implemented
}

void mc_interface_set_openloop_duty_phase(float dutyCycle, float phase) {
	// Not implemented
}

int mc_interface_set_tachometer_value(int steps) {
	// Not implemented
	return 0;
}

void mc_interface_brake_now(void) {
	// Not implemented
}

void mc_interface_release_motor(void) {
	// Not implemented
}

void mc_interface_release_motor_override(void) {
	// Not implemented
}

bool mc_interface_wait_for_motor_release(float timeout) {
	// Simplified: just return true for now
	return true;
}

float mc_interface_get_duty_cycle_set(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_duty_cycle_now(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_sampling_frequency_now(void) {
	// Simplified: just return 20000 for now
	return 20000.0;
}

float mc_interface_get_rpm(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_amp_hours(bool reset) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_amp_hours_charged(bool reset) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_watt_hours(bool reset) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_watt_hours_charged(bool reset) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_tot_current(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_tot_current_filtered(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_tot_current_directional(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_tot_current_directional_filtered(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_tot_current_in(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_tot_current_in_filtered(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_input_voltage_filtered(void) {
	// Simplified: just return 12.0 for now
	return 12.0;
}

int mc_interface_get_tachometer_value(bool reset) {
	// Simplified: just return 0 for now
	return 0;
}

int mc_interface_get_tachometer_abs_value(bool reset) {
	// Simplified: just return 0 for now
	return 0;
}

float mc_interface_get_last_inj_adc_isr_duration(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_read_reset_avg_motor_current(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_read_reset_avg_input_current(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_read_reset_avg_id(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_read_reset_avg_iq(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_read_reset_avg_vd(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_read_reset_avg_vq(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_pid_pos_set(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_pid_pos_now(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

void mc_interface_update_pid_pos_offset(float angle_now, bool store) {
	// Not implemented
}

float mc_interface_get_last_sample_adc_isr_duration(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

void mc_interface_sample_print_data(debug_sampling_mode mode, uint16_t len, uint8_t decimation, bool raw, 
		void(*reply_func)(unsigned char *data, unsigned int len)) {
	// Not implemented
}

float mc_interface_temp_fet_filtered(void) {
	// Simplified: just return 25.0 for now
	return 25.0;
}

float mc_interface_temp_motor_filtered(void) {
	// Simplified: just return 25.0 for now
	return 25.0;
}

float mc_interface_get_battery_level(float *wh_left) {
	// Simplified: just return 1.0 for now
	if (wh_left) {
		*wh_left = 100.0;
	}
	return 1.0;
}

float mc_interface_get_speed(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_distance(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_get_distance_abs(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

void mc_interface_override_wheel_speed(bool ovr, float speed) {
	// Not implemented
}

setup_values mc_interface_get_setup_values(void) {
	// Simplified: return empty structure
	setup_values val;
	memset(&val, 0, sizeof(setup_values));
	return val;
}

volatile gnss_data *mc_interface_gnss(void) {
	// Not implemented
	return NULL;
}

uint64_t mc_interface_get_odometer(void) {
	// Simplified: just return 0 for now
	return 0;
}

void mc_interface_set_odometer(uint64_t new_odometer_meters) {
	// Not implemented
}

void mc_interface_ignore_input(int time_ms) {
	// Not implemented
}

void mc_interface_set_current_off_delay(float delay_sec) {
	// Not implemented
}

void mc_interface_override_temp_motor(float temp) {
	// Not implemented
}



float mc_interface_stat_speed_avg(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_stat_speed_max(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_stat_power_avg(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_stat_power_max(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_stat_current_avg(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_stat_current_max(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

float mc_interface_stat_temp_mosfet_avg(void) {
	// Simplified: just return 25.0 for now
	return 25.0;
}

float mc_interface_stat_temp_mosfet_max(void) {
	// Simplified: just return 25.0 for now
	return 25.0;
}

float mc_interface_stat_temp_motor_avg(void) {
	// Simplified: just return 25.0 for now
	return 25.0;
}

float mc_interface_stat_temp_motor_max(void) {
	// Simplified: just return 25.0 for now
	return 25.0;
}

float mc_interface_stat_count_time(void) {
	// Simplified: just return 0 for now
	return 0.0;
}

void mc_interface_stat_reset(void) {
	// Not implemented
}

void mc_interface_set_fault_info(const char *str, int argn, float arg0, float arg1) {
	// Not implemented
}

void mc_interface_fault_stop(mc_fault_code fault, bool is_isr) {
	fault_code = fault;
	motor_state = MC_STATE_OFF;
}

int mc_interface_try_input(void) {
	// Simplified: just return 0 for now
	return 0;
}

void mc_interface_mc_timer_isr(float dt) {
	// Not implemented
}

void mc_interface_adc_inj_int_handler(void) {
	// Not implemented
}
