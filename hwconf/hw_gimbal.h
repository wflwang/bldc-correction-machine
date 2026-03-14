/*
 * Hardware configuration for HK32M070-based BLDC controller
 * Gimbal application
 */

#ifndef HW_HEADER
#define HW_HEADER "hw_gimbal.h"
#endif

#ifndef HW_NAME
#define HW_NAME "HK32M070-Gimbal"
#endif

// Hall sensor pins
#define HW_HALL_ENC_GPIO1 GPIOB
#define HW_HALL_ENC_PIN1 GPIO_Pin_0
#define HW_HALL_ENC_PIN2 GPIO_Pin_1
#define HW_HALL_ENC_PIN3 GPIO_Pin_2

// ADC channels
#define ADC_IND_CURR1 0
#define ADC_IND_CURR2 1
#define ADC_IND_VIN 2
#define ADC_IND_EXT 3

// Motor control pins
#define HW_PWM1_PORT GPIOA
#define HW_PWM1_PIN GPIO_Pin_8
#define HW_PWM2_PORT GPIOA
#define HW_PWM2_PIN GPIO_Pin_9
#define HW_PWM3_PORT GPIOA
#define HW_PWM3_PIN GPIO_Pin_10

// Direction pin
#define HW_DIR_PORT GPIOB
#define HW_DIR_PIN GPIO_Pin_3

// Read Hall sensor functions
#define READ_HALL1() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1)
#define READ_HALL2() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN2)
#define READ_HALL3() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN3)

// Get input voltage
#define GET_INPUT_VOLTAGE() 12.0

// Motor control limits
#define HW_LIM_CURRENT -20.0, 20.0
#define HW_LIM_CURRENT_ABS 0.0, 30.0

// FOC control loop frequency
#define HW_LIM_FOC_CTRL_LOOP_FREQ 10000.0, 20000.0

// Current filter limits
#define HW_FOC_CURRENT_FILTER_LIM 0.05, 0.5

// Default configuration values
#define MCCONF_L_CURRENT_MAX 10.0
#define MCCONF_L_CURRENT_MIN -10.0
#define MCCONF_L_MAX_ABS_CURRENT 15.0
#define MCCONF_L_CURRENT_MAX_SCALE 1.0
#define MCCONF_L_CURRENT_MIN_SCALE -1.0
#define MCCONF_L_WATT_MAX 120.0
#define MCCONF_L_WATT_MIN -120.0
#define MCCONF_L_RPM_MAX 10000.0
#define MCCONF_L_RPM_MIN -10000.0

// FOC configuration
#define FOC_SENSOR_MODE_HALL 1
#define FOC_SENSOR_MODE_SENSORLESS 2
#define MOTOR_TYPE_FOC 2

// Fault codes
#define FAULT_CODE_NONE 0
#define FAULT_CODE_HALL_SENSOR_ERROR 1
