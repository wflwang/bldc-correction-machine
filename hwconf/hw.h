/*
	Copyright 2012 - 2020 Benjamin Vedder	benjamin@vedder.se

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

#ifndef HW_H_
#define HW_H_

#include "hk32m07x_conf.h"
#include HW_HEADER

#ifndef HW_NAME
#error "No hardware name set"
#endif

#ifndef FW_NAME
#ifdef DISABLE_HW_LIMITS
#define FW_NAME "no_hw_limits"
#else
#define FW_NAME ""
#endif
#endif

// Possible HW properties.


// Default macros in case there is no hardware support or no need to change them.

//可能连接一个开关,改变电流采样电阻配置
//开关直流校准功能
#ifndef DCCAL_ON
#define DCCAL_ON()
#endif
#ifndef DCCAL_OFF
#define DCCAL_OFF()
#endif

// Double samples in beginning and end for positive current measurement.
// Useful when the shunt sense traces have noise that causes offset.
#ifndef CURR1_DOUBLE_SAMPLE
#define CURR1_DOUBLE_SAMPLE		0
#endif
#ifndef CURR2_DOUBLE_SAMPLE
#define CURR2_DOUBLE_SAMPLE		0
#endif
#ifndef CURR3_DOUBLE_SAMPLE
#define CURR3_DOUBLE_SAMPLE		0
#endif

#ifndef AUX_ON
#define AUX_ON()
#endif
#ifndef AUX_OFF
#define AUX_OFF()
#endif
#ifndef AUX2_ON
#define AUX2_ON()
#endif
#ifndef AUX2_OFF
#define AUX2_OFF()
#endif

#ifndef PHASE_FILTER_ON
#define PHASE_FILTER_ON()
#endif
#ifndef PHASE_FILTER_OFF
#define PHASE_FILTER_OFF()
#endif
#ifndef PHASE_FILTER_ON_M2
#define PHASE_FILTER_ON_M2()
#endif
#ifndef PHASE_FILTER_OFF_M2
#define PHASE_FILTER_OFF_M2()
#endif

#ifndef CURRENT_FILTER_ON
#define CURRENT_FILTER_ON()
#endif
#ifndef CURRENT_FILTER_OFF
#define CURRENT_FILTER_OFF()
#endif
#ifndef CURRENT_FILTER_ON_M2
#define CURRENT_FILTER_ON_M2()
#endif
#ifndef CURRENT_FILTER_OFF_M2
#define CURRENT_FILTER_OFF_M2()
#endif

// VCC net voltage
#ifndef V_REG
#define V_REG				3.3
#endif

// Individual MOSFET temperature sensors. Override if available.
#ifndef NTC_TEMP_MOS1
#define NTC_TEMP_MOS1()		0.0
#endif
#ifndef NTC_TEMP_MOS2
#define NTC_TEMP_MOS2()		0.0
#endif
#ifndef NTC_TEMP_MOS3
#define NTC_TEMP_MOS3()		0.0
#endif


#ifndef TEMP_MOTOR_1
#define TEMP_MOTOR_1(beta)	0.0
#endif

// Sin/Cos Encoder Signals. Override if available
#ifndef ENCODER_SIN_VOLTS
#if defined(ADC_IND_EXT) && defined(ADC_VOLTS)
#define ENCODER_SIN_VOLTS		ADC_VOLTS(ADC_IND_EXT)
#else
#define ENCODER_SIN_VOLTS		0.0
#endif
#endif

#ifndef ENCODER_COS_VOLTS
#if defined(ADC_IND_EXT2) && defined(ADC_VOLTS)
#define ENCODER_COS_VOLTS		ADC_VOLTS(ADC_IND_EXT2)
#else
#define ENCODER_COS_VOLTS		0.0
#endif
#endif

// Current ADC macros. Override them for custom current measurement functions.
#ifndef GET_CURRENT1
#ifdef INVERTED_SHUNT_POLARITY
#define GET_CURRENT1()		(4095.0 - (float)ADC_Value[ADC_IND_CURR1])
#else
#define GET_CURRENT1()		((float)ADC_Value[ADC_IND_CURR1])
#endif
#endif

#ifndef GET_CURRENT2
#ifdef INVERTED_SHUNT_POLARITY
#define GET_CURRENT2()		(4095.0 - (float)ADC_Value[ADC_IND_CURR2])
#else
#define GET_CURRENT2()		((float)ADC_Value[ADC_IND_CURR2])
#endif
#endif

#ifndef ADC_IND_CURR3
#define ADC_IND_CURR3		0
#endif
#define GET_CURRENT3()		0

#ifndef CURRENT_CAL1
#define CURRENT_CAL1				1.0
#endif
#ifndef CURRENT_CAL2
#define CURRENT_CAL2				1.0
#endif
#ifndef CURRENT_CAL3
#define CURRENT_CAL3				1.0
#endif

#ifndef HW_MAX_CURRENT_OFFSET
#define HW_MAX_CURRENT_OFFSET 				620
#endif
#ifndef MCCONF_MAX_CURRENT_UNBALANCE
#define MCCONF_MAX_CURRENT_UNBALANCE		(FAC_CURRENT * 512)
#endif
#ifndef MCCONF_MAX_CURRENT_UNBALANCE_RATE
#define MCCONF_MAX_CURRENT_UNBALANCE_RATE	0.3
#endif

// ADC Channels
#ifndef ADC_IND_EXT2
#define ADC_IND_EXT2 			ADC_IND_EXT
#endif
#ifndef ADC_IND_EXT3
#define ADC_IND_EXT3 			ADC_IND_EXT
#endif
#ifndef ADC_IND_EXT4
#define ADC_IND_EXT4 			ADC_IND_EXT
#endif
#ifndef ADC_IND_EXT5
#define ADC_IND_EXT5 			ADC_IND_EXT
#endif
#ifndef ADC_IND_EXT6
#define ADC_IND_EXT6 			ADC_IND_EXT
#endif
#ifndef ADC_IND_EXT7
#define ADC_IND_EXT7 			ADC_IND_EXT
#endif
#ifndef ADC_IND_EXT8
#define ADC_IND_EXT8 			ADC_IND_EXT
#endif

// Voltage on phase input used for FOC 相电压采样
#ifndef ADC_V_L1_VOLTS
#define ADC_V_L1_VOLTS				((float)ADC_V_L1 / 4096.0 * V_REG)
#endif
#ifndef ADC_V_L2_VOLTS
#define ADC_V_L2_VOLTS				((float)ADC_V_L2 / 4096.0 * V_REG)
#endif
#ifndef ADC_V_L3_VOLTS
#define ADC_V_L3_VOLTS				((float)ADC_V_L3 / 4096.0 * V_REG)
#endif

// Adc voltage scaling on phases and input
#ifndef ADC_VOLTS_PH_FACTOR
#define ADC_VOLTS_PH_FACTOR		1.0
#endif
#ifndef ADC_VOLTS_INPUT_FACTOR
#define ADC_VOLTS_INPUT_FACTOR	1.0
#endif

// Hook to call when trying to initialize the permanent NRF failed. Can be
// used to e.g. reconfigure pins.
#ifndef HW_PERMANENT_NRF_FAILED_HOOK
#define HW_PERMANENT_NRF_FAILED_HOOK()
#endif

#ifndef HW_EARLY_INIT
#define HW_EARLY_INIT()
#endif

// Default ID
#ifndef HW_DEFAULT_ID
#define HW_DEFAULT_ID			(APPCONF_CONTROLLER_ID >= 0 ? APPCONF_CONTROLLER_ID : hw_id_from_uuid())
#endif

#ifndef HW_LIM_CURRENT
#define HW_LIM_CURRENT			-100.0, 100.0
#endif
#ifndef HW_LIM_CURRENT_ABS
#define HW_LIM_CURRENT_ABS		0.0, 140.0
#endif

#ifndef HW_LIM_FOC_CTRL_LOOP_FREQ
#define HW_LIM_FOC_CTRL_LOOP_FREQ	3000.0, 30000.0
#endif

#ifndef HW_FOC_CURRENT_FILTER_LIM
#define HW_FOC_CURRENT_FILTER_LIM	0.05, 1.0
#endif

#ifndef PTC_TEMP_MOTOR
#if defined(NTC_RES_MOTOR) && defined(ADC_IND_TEMP_MOTOR)
#define PTC_TEMP_MOTOR(res, con, tbase)			(((NTC_RES_MOTOR(ADC_Value[ADC_IND_TEMP_MOTOR]) - (res)) / (res)) * 100 / (con) + (tbase))
#define PTC_TEMP_MOTOR_2(res, con, tbase)		(((NTC_RES_MOTOR(ADC_Value[ADC_IND_TEMP_MOTOR_2]) - (res)) / (res)) * 100 / (con) + (tbase))
#else
#define PTC_TEMP_MOTOR(res, con, tbase)			0.0
#define PTC_TEMP_MOTOR_2(res, con, tbase)		0.0
#endif
#endif

#ifndef NTC100K_TEMP_MOTOR
#if defined(NTC_RES_MOTOR) && defined(ADC_IND_TEMP_MOTOR)
#define NTC100K_TEMP_MOTOR(beta)				(1.0 / ((logf(NTC_RES_MOTOR(ADC_Value[ADC_IND_TEMP_MOTOR]) / 100000.0) / beta) + (1.0 / 298.15)) - 273.15)
#define NTC100K_TEMP_MOTOR_2(beta)				(1.0 / ((logf(NTC_RES_MOTOR(ADC_Value[ADC_IND_TEMP_MOTOR_2]) / 100000.0) / beta) + (1.0 / 298.15)) - 273.15)
#else
#define NTC100K_TEMP_MOTOR(beta)				0.0
#define NTC100K_TEMP_MOTOR_2(beta)				0.0
#endif
#endif

#ifndef NTCX_TEMP_MOTOR
#if defined(NTC_RES_MOTOR) && defined(ADC_IND_TEMP_MOTOR)
#define NTCX_TEMP_MOTOR(res, beta, tbase)		(1.0 / ((logf(NTC_RES_MOTOR(ADC_Value[ADC_IND_TEMP_MOTOR]) / (res)) / (beta)) + (1.0 / (273.15 + (tbase)))) - 273.15)
#define NTCX_TEMP_MOTOR_2(res, beta, tbase)		(1.0 / ((logf(NTC_RES_MOTOR(ADC_Value[ADC_IND_TEMP_MOTOR_2]) / (res)) / (beta)) + (1.0 / (273.15 + (tbase)))) - 273.15)
#else
#define NTCX_TEMP_MOTOR(res, beta, tbase)		0.0
#define NTCX_TEMP_MOTOR_2(res, beta, tbase)		0.0
#endif
#endif

// Default second motor defines
#ifndef READ_HALL1_2
#define READ_HALL1_2()			READ_HALL1()
#endif
#ifndef READ_HALL2_2
#define READ_HALL2_2()			READ_HALL2()
#endif
#ifndef READ_HALL3_2
#define READ_HALL3_2()			READ_HALL3()
#endif

#ifndef  MOTOR_TEMP_LPF
#define MOTOR_TEMP_LPF 			0.01
#endif
#ifndef HW_ADC_CHANNELS_EXTRA
#define HW_ADC_CHANNELS_EXTRA	0
#endif
#ifndef ADC_V_L4
#define ADC_V_L4				ADC_V_L1
#endif
#ifndef ADC_V_L5
#define ADC_V_L5				ADC_V_L2
#endif
#ifndef ADC_V_L6
#define ADC_V_L6				ADC_V_L3
#endif

#ifndef HW_PAS1_PORT
#define HW_PAS1_PORT			HW_UART_RX_PORT
#define HW_PAS1_PIN				HW_UART_RX_PIN
#define HW_PAS2_PORT			HW_UART_TX_PORT
#define HW_PAS2_PIN				HW_UART_TX_PIN
#endif

#ifndef HW_TRIM_HSI
#define HW_TRIM_HSI()
#endif

#ifndef HW_RESET_DRV_FAULTS
#define HW_RESET_DRV_FAULTS()
#endif

// For backwards-compatibility with configs that use the old name
#ifdef MCCONF_FOC_F_SW
#ifndef MCCONF_FOC_F_ZV
#define MCCONF_FOC_F_ZV			MCCONF_FOC_F_SW
#warning Please replace `MCCONF_FOC_F_SW` by `MCCONF_FOC_F_ZV`. `MCCONF_FOC_F_SW` is deprecated.
#endif
#endif

#ifndef HW_SPI_PORT_NSS
#define HW_SPI_PORT_NSS HW_HALL_ENC_GPIO3
#endif

#ifndef HW_SPI_PIN_NSS
#define HW_SPI_PIN_NSS HW_HALL_ENC_PIN3
#endif

#ifndef HW_SPI_PORT_SCK
#define HW_SPI_PORT_SCK HW_HALL_ENC_GPIO1
#endif

#ifndef HW_SPI_PIN_SCK
#define HW_SPI_PIN_SCK HW_HALL_ENC_PIN1
#endif

#ifndef HW_SPI_PORT_MISO
#define HW_SPI_PORT_MISO HW_HALL_ENC_GPIO2
#endif

#ifndef HW_SPI_PIN_MISO
#define HW_SPI_PIN_MISO HW_HALL_ENC_PIN2
#endif

// Functions
void hw_init_gpio(void);
void hw_setup_adc_channels(void);
void hw_start_i2c(void);
void hw_stop_i2c(void);
void hw_try_restore_i2c(void);
uint8_t hw_id_from_uuid(void);
uint8_t hw_id_from_pins(void);

#endif /* HW_H_ */
