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

#include "mcpwm.h"
#include "hw_conf.h"

// Global variables
static volatile mc_configuration *mcconf_ptr = NULL;
static bool init_done = false;

// External variables
volatile float mcpwm_detect_currents[] = {0.0};
volatile float mcpwm_detect_voltages[] = {0.0};
volatile float mcpwm_detect_currents_diff[] = {0.0};
volatile int mcpwm_vzero = 0;

void mcpwm_init(volatile mc_configuration *configuration) {
	mcconf_ptr = configuration;
	init_done = true;
}

void mcpwm_deinit(void) {
	init_done = false;
}

bool mcpwm_init_done(void) {
	return init_done;
}

void mcpwm_set_configuration(volatile mc_configuration *configuration) {
	mcconf_ptr = configuration;
}

void mcpwm_stop_pwm(void) {
	// Not implemented
}

bool mcpwm_is_dccal_done(void) {
	// Not implemented
	return true;
}

void mcpwm_adc_inj_int_handler(void) {
	// Not implemented
}

void mcpwm_adc_int_handler(void *p, uint32_t flags) {
	// Not implemented
}
