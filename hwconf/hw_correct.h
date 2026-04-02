/*
 * Hardware configuration for HK32M070-based BLDC controller
 * Gimbal application
 */

#ifndef HW_HEADER
#define HW_HEADER "hw_correct.h"
#endif

#ifndef HW_NAME
#define HW_NAME "HK32M070-correct"
#endif

// Hall sensor pins
#define HW_HALL_ENC_GPIO1 GPIOA
#define HW_HALL_ENC_PIN1 GPIO_Pin_0
#define HW_HALL_ENC_PIN1_No GPIO_PinSource0
#define HW_HALL_ENC_PIN2 GPIO_Pin_1
#define HW_HALL_ENC_PIN2_No GPIO_PinSource1
#define HW_HALL_ENC_PIN3 GPIO_Pin_2
#define HW_HALL_ENC_PIN3_No GPIO_PinSource3
#define HW_HALL_TIMER   HTU

// ADC channels
#define ADC_IND_CURR1 0
#define ADC_IND_CURR2 1
#define ADC_IND_VIN 2
#define ADC_IND_EXT 3

#define default_KV  125 //默认KV值125

// 电压值
#define vMinBus 1200    //最低电压 12V*100
#define vMaxBus 5300   //最高电压 53V*100
#define Int16FilterDiv 32768    //int16 滤波分母
#define VBusFilterConstant (int16_t)(0.04*Int16FilterDiv) //母线电压滤波常数 0.02 100次更新大约63% 200次更新大约86% 400次更新大约95% 800次更新大约98% 1600次更新大约99%

#define MotorTempEn     //使能motor temp
#define MosTempEn       //使能MOS temp
#define TempBeta    3490    //温度传感器B值
//#define vMinMotorTemp 1200    //最低电机 temp 12V*100
#define vMaxMotorTemp 15000   //最高电机 temp 150c*100
//#define vMinMosTemp 1200    //最低mos temp 12V*100
#define vMaxMosTemp 10000   //最高mos temp 100c*100
#define VTempMotorFilterConstant (int16_t)(0.02*Int16FilterDiv) //母线电压滤波常数 0.02 100次更新大约63% 200次更新大约86% 400次更新大约95% 800次更新大约98% 1600次更新大约99%
#define VTempPCBFilterConstant (int16_t)(0.02*Int16FilterDiv) //母线电压滤波常数 0.02 100次更新大约63% 200次更新大约86% 400次更新大约95% 800次更新大约98% 1600次更新大约99%

//获取AD值
#define GetVBusAD()         (ADC->ADDR1B)
#define GetPCBTempAD()      (ADC->ADDR2B)
#define GetMotorTempAD()    (ADC->ADDR3B)

// Motor control pins
#define HW_PWM1_PORT GPIOA
#define HW_PWM1_PIN GPIO_Pin_8
#define HW_PWM2_PORT GPIOA
#define HW_PWM2_PIN GPIO_Pin_9
#define HW_PWM3_PORT GPIOA
#define HW_PWM3_PIN GPIO_Pin_10

#define PWMIO_PB10      ATU_PWM_REMAP_SOURCE_TIO2B
#define PWMIO_PB11      ATU_PWM_REMAP_SOURCE_TIO2A
#define PWMIO_PB12      ATU_PWM_REMAP_SOURCE_TIO1B
#define PWMIO_PB13      ATU_PWM_REMAP_SOURCE_TIO1A
#define PWMIO_PB14      ATU_PWM_REMAP_SOURCE_TIO0B
#define PWMIO_PB15      ATU_PWM_REMAP_SOURCE_TIO0A

// Direction pin
#define HW_DIR_PORT GPIOB
#define HW_DIR_PIN GPIO_Pin_3

// Read Hall sensor functions
#define READ_HALL1() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1)
#define READ_HALL2() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN2)
#define READ_HALL3() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN3)

#ifndef V_REG
#define V_REG   3.3
#endif
#ifndef VIN_R1  
#define VIN_R1  47000.0
#endif
#ifndef VIN_R2
#define VIN_R2  2200.0
#endif
#ifndef CURRENT_AMP_GAIN
#define CURRENT_AMP_GAIN    12.0
#endif
#ifndef CURRENT_SHUNT_RES
#define CURRENT_SHUNT_RES   0.005
#endif

// Get input voltage 得出的是当前电压
#define GET_INPUT_VOLTAGE() 	((uint16_t)(100.0 * (V_REG / 4095.0) * (float)GetVBusAD()  * ((VIN_R1 + VIN_R2) / VIN_R2)))
//最终温度扩大100倍保留精度
#define NTC_RES_MOTOR(adc_val)	(10000.0 / ((4095.0 / (float)adc_val) - 1.0)) // Motor temp sensor on low side
#define NTC_TEMP_MOTOR(beta)	((uint16_t)(100.0f*(1.0 / ((logf(NTC_RES_MOTOR(GetMotorTempAD()) / 10000.0) / beta) + (1.0 / 298.15)) - 273.15)))
#define NTC_RES_PCB(adc_val)	(10000.0 / ((4095.0 / (float)adc_val) - 1.0)) // Motor temp sensor on low side
#define NTC_TEMP_PCB(beta)	    ((uint16_t)(100.0f*(1.0 / ((logf(NTC_RES_MOTOR(GetPCBTempAD()) / 10000.0) / beta) + (1.0 / 298.15)) - 273.15)))



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