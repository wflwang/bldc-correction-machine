/*
	Copyright 2019 Benjamin Vedder	benjamin@vedder.se

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

#include "conf_general.h"
#include "utils_math.h"
#include "hk32m07x.h"
#include "hk32m07x_gpio.h"
#include "hk32m07x_rcc.h"
#include <math.h>

// Hardware source file is not included here

// Simple delay function
void delay_ms(uint32_t ms) {
	for (uint32_t i = 0; i < ms * 16000; i++) {
		__NOP();
	}
}

uint8_t hw_id_from_uuid(void) {
	// Use a simple ID for now
	return 1;
}

#if defined(HW_ID_PIN_GPIOS) && defined(HW_ID_PIN_PINS)
uint8_t hw_id_from_pins(void) {
	// Simplified version for HK32M070
	return hw_id_from_uuid();
}
#endif //defined(HW_ID_PIN_GPIOS) && defined(HW_ID_PIN_PINS)

