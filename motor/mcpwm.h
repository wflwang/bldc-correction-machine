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

#ifndef MCPWM_H_
#define MCPWM_H_

#include "conf_general.h"

// Functions
void mcpwm_init(volatile mc_configuration *configuration);
void mcpwm_deinit(void);
bool mcpwm_init_done(void);
void mcpwm_set_configuration(volatile mc_configuration *configuration);
void mcpwm_stop_pwm(void);
bool mcpwm_is_dccal_done(void);

// Interrupt handlers
void mcpwm_adc_inj_int_handler(void);
void mcpwm_adc_int_handler(void *p, uint32_t flags);

// External variables
extern volatile float mcpwm_detect_currents[];
extern volatile float mcpwm_detect_voltages[];
extern volatile float mcpwm_detect_currents_diff[];
extern volatile int mcpwm_vzero;

/*
 * Fixed parameters
 */
#define MCPWM_RPM_TIMER_FREQ			1000000.0	// Frequency of the RPM measurement timer
#define MCPWM_CMD_STOP_TIME				0		// Ignore commands for this duration in msec after a stop has been sent
#define MCPWM_DETECT_STOP_TIME			500		// Ignore commands for this duration in msec after a detect command

// Speed PID parameters
#define MCPWM_PID_TIME_K				0.001	// Pid controller sample time in seconds

#endif /* MC_PWM_H_ */
