/*
 * Hardware configuration for HK32M070-based BLDC controller
 */

#ifndef HW_CONF_H_
#define HW_CONF_H_

#include "hk32m07x.h"
#include "hk32m07x_gpio.h"

// Hardware name
#define HW_NAME "HK32M070_BLDC"

// Firmware name
#define FW_NAME "HK32M070_BLDC"

// Hardware source and header
#define HW_SOURCE "hwconf/hw_conf.c"

// GPIO definitions
#define HW_HALL_ENC_GPIO1 GPIOA
#define HW_HALL_ENC_PIN1 GPIO_Pin_5
#define HW_HALL_ENC_GPIO2 GPIOA
#define HW_HALL_ENC_PIN2 GPIO_Pin_6
#define HW_HALL_ENC_GPIO3 GPIOA
#define HW_HALL_ENC_PIN3 GPIO_Pin_7

// ADC channels
#define ADC_IND_CURR1 0
#define ADC_IND_CURR2 1
#define ADC_IND_EXT 2
#define ADC_IND_VIN 3
#define ADC_IND_TEMP_MOTOR 4

// Current sensing
#define CURRENT_SHUNT_RES 0.001
#define CURRENT_AMP_GAIN 50
#define V_REG 3.3

// Voltage sensing
#define VIN_R1 33000
#define VIN_R2 2200

// PWM configuration
#define HW_HAS_3_SHUNTS 0
#define HW_HAS_PHASE_SHUNTS 1

// Other hardware features
#define HW_HAS_DUAL_MOTORS 0
#define HW_HAS_DUAL_PARALLEL 0
#define HW_HAS_NO_CAN 1

// Default ID
#define HW_DEFAULT_ID 1

// Hardware limits
#define HW_LIM_CURRENT_MIN -20.0
#define HW_LIM_CURRENT_MAX 20.0
#define HW_LIM_CURRENT_ABS_MIN 0.0
#define HW_LIM_CURRENT_ABS_MAX 25.0
#define HW_LIM_FOC_CTRL_LOOP_FREQ_MIN 5000.0
#define HW_LIM_FOC_CTRL_LOOP_FREQ_MAX 30000.0
#define HW_FOC_CURRENT_FILTER_LIM_MIN 0.05
#define HW_FOC_CURRENT_FILTER_LIM_MAX 1.0

// Macros for hardware control
#define ENABLE_GATE()
#define DISABLE_GATE()

#define CURRENT_FILTER_ON()
#define CURRENT_FILTER_OFF()
#define CURRENT_FILTER_ON_M2()
#define CURRENT_FILTER_OFF_M2()

#define DCCAL_ON()
#define DCCAL_OFF()

// Read Hall sensors
#define READ_HALL1() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1)
#define READ_HALL2() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2)
#define READ_HALL3() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3)

// GPIO helper functions
#define palSetPadMode(port, pin, mode) GPIO_Init(port, &(GPIO_InitTypeDef){pin, GPIO_Mode_IN, GPIO_Speed_Level_4, GPIO_PuPd_NOPULL, GPIO_Schmit_Disable})
#define palReadPad(port, pin) GPIO_ReadInputDataBit(port, pin)

// STM32 UUID
#define STM32_UUID ((uint32_t*)0x1FFFF7E8)
#define STM32_UUID_8 ((uint8_t*)0x1FFFF7E8)

#endif /* HW_CONF_H_ */