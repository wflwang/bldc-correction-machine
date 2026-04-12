/*
 * Hardware configuration for HK32M070-based BLDC controller
 * Gimbal application
 */
 
#ifndef __HW_CORRECT_H
#define __HW_CORRECT_H

#include <cstdint>
#ifndef HW_HEADER
#define HW_HEADER "hw_correct.h"
#endif

#ifndef HW_NAME
#define HW_NAME "HK32M070-correct"
#endif

#define cTestSVPWM  2800        //开环电流
#define TesTAngAdd  16       //开环测试每次变化的角度

// Hall sensor pins
#define HW_HALL_ENC_GPIO1 GPIOA
#define HW_HALL_ENC_PIN1 GPIO_Pin_0
#define HW_HALL_ENC_PIN1_No GPIO_PinSource0
#define HW_HALL_ENC_GPIO2 GPIOA
#define HW_HALL_ENC_PIN2 GPIO_Pin_1
#define HW_HALL_ENC_PIN2_No GPIO_PinSource1
#define HW_HALL_ENC_GPIO3 GPIOA
#define HW_HALL_ENC_PIN3 GPIO_Pin_2
#define HW_HALL_ENC_PIN3_No GPIO_PinSource2
#define HW_HALL_TIMER   HTU

// ADC channels
#define ADC_IND_CURR1 0
#define ADC_IND_CURR2 1
#define ADC_IND_VIN 2
#define ADC_IND_EXT 3

#define default_KV  125 //默认KV值125
#define baseSpeed   200 //最小增加转速

//#define MotorTempEn     //使能motor temp
//#define MosTempEn       //使能MOS temp
#define TempBeta    3490    //温度传感器B值
//#define vMinMotorTemp 1200    //最低电机 temp 12V*100
#define vMaxMotorTemp 15000   //最高电机 temp 150c*100
#define vMaxPCBTemp 15000
//#define vMinMosTemp 1200    //最低mos temp 12V*100
#define vMaxMosTemp 10000   //最高mos temp 100c*100
#define VTempMotorFilterConstant (int16_t)(0.02*Int16FilterDiv) //母线电压滤波常数 0.02 100次更新大约63% 200次更新大约86% 400次更新大约95% 800次更新大约98% 1600次更新大约99%
#define VTempPCBFilterConstant (int16_t)(0.02*Int16FilterDiv) //母线电压滤波常数 0.02 100次更新大约63% 200次更新大约86% 400次更新大约95% 800次更新大约98% 1600次更新大约99%

//获取AD值
#define GetVBusAD()         (ADC->ADDR1B>>4)
#define GetPCBTempAD()      (ADC->ADDR2B>>4)
#define GetMotorTempAD()    (ADC->ADDR3B>>4)

// Motor control pins
//#define HW_PWM1_PORT GPIOA
//#define HW_PWM1_PIN GPIO_Pin_8
//#define HW_PWM2_PORT GPIOA
//#define HW_PWM2_PIN GPIO_Pin_9
//#define HW_PWM3_PORT GPIOA
//#define HW_PWM3_PIN GPIO_Pin_10
//uart config

#define MainOutMCTime   1   //打印内部数据的时间间隔 这个数加1

#define bps_rate    115200

#define UartTX_PORT GPIOA
#define UartRX_PORT GPIOA
#define UartTX_PIN  GPIO_Pin_7
#define UartRX_PIN  GPIO_Pin_6
#define UartTX_SOURCE   GPIO_PinSource7
#define UartRX_SOURCE   GPIO_PinSource6
#define UartTX_GPIO_AF  GPIO_AF_3
#define UartRX_GPIO_AF  GPIO_AF_3
#define UartTX_CLK  RCC_AHBPeriph_GPIOA
#define UartRX_CLK  RCC_AHBPeriph_GPIOA
#define UartCLKEn() RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART2, ENABLE);
#define UartCH      UART2


//PWM IO config
#define PWMIO_PB10      ATU_PWM_REMAP_SOURCE_TIO2B
#define PWMIO_PB11      ATU_PWM_REMAP_SOURCE_TIO2A
#define PWMIO_PB12      ATU_PWM_REMAP_SOURCE_TIO1B
#define PWMIO_PB13      ATU_PWM_REMAP_SOURCE_TIO1A
#define PWMIO_PB14      ATU_PWM_REMAP_SOURCE_TIO0B
#define PWMIO_PB15      ATU_PWM_REMAP_SOURCE_TIO0A

// Direction pin
#define HW_DIR_PORT GPIOA
#define HW_DIR_PIN  GPIO_Pin_13
#define HW_En_PORT  GPIOA
#define HW_En_PIN   GPIO_Pin_11
#define SetEn(x)    ((x) ? GPIO_SetBits(HW_En_PORT, HW_En_PIN) : GPIO_ResetBits(HW_En_PORT, HW_En_PIN))
#define XorEn()     GPIO_Toggle(HW_En_PORT, HW_En_PIN)

//LED control



//*********************PPM input
#define AHPSrc          RCC_AHBPeriph_GPIOA
#define UTU_IOA_PORT    GPIOA
#define UTU_IOA_PIN     GPIO_Pin_12
#define UTU_IOA_PIN_SRC GPIO_PinSource12
#define UTU_IOA_AF      GPIO_AF_2

// Read Hall sensor functions
#define READ_HALL1() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1)
#define READ_HALL2() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN2)
#define READ_HALL3() GPIO_ReadInputDataBit(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN3)

#ifndef V_REG
#define V_REG   3.3
#endif
#ifndef VIN_R1  
#define VIN_R1  470.0
#endif
#ifndef VIN_R2
#define VIN_R2  22.0
#endif
#ifndef CURRENT_AMP_GAIN
#define CURRENT_AMP_GAIN    12.0
#endif
#ifndef CURRENT_SHUNT_RES
#define CURRENT_SHUNT_RES   0.005
#endif

// Get input voltage 得出的是当前电压
//#define GET_INPUT_VOLTAGE() 	((uint16_t)(100.0 * (V_REG / 4095.0) * (float)GetVBusAD()  * ((VIN_R1 + VIN_R2) / VIN_R2)))
//最终温度扩大100倍保留精度
#define NTC_RES_MOTOR(adc_val)	(10000.0 / ((4095.0 / (float)adc_val) - 1.0)) // Motor temp sensor on low side
#define NTC_TEMP_MOTOR(beta)	((uint16_t)(100.0f*(1.0 / ((logf(NTC_RES_MOTOR(GetMotorTempAD()) / 10000.0) / beta) + (1.0 / 298.15)) - 273.15)))
#define NTC_RES_PCB(adc_val)	(10000.0 / ((4095.0 / (float)adc_val) - 1.0)) // Motor temp sensor on low side
#define NTC_TEMP_PCB(beta)	    ((uint16_t)(100.0f*(1.0 / ((logf(NTC_RES_MOTOR(GetPCBTempAD()) / 10000.0) / beta) + (1.0 / 298.15)) - 273.15)))

// 电压值
//电压对应的AD值
#define VBusVol(x)  ((uint16_t)(x*VIN_R2*4095.0/(VIN_R1 + VIN_R2)/V_REG)) //AD
#define vMinBus VBusVol(12.0)    //最低电压 12V*100
#define vMaxBus VBusVol(53.0)   //最高电压 53V*100
#define Int16FilterDiv 32768    //int16 滤波分母
#define VBusFilterConstant (int16_t)(0.2*Int16FilterDiv) //母线电压滤波常数 0.02 100次更新大约63% 200次更新大约86% 400次更新大约95% 800次更新大约98% 1600次更新大约99%
//电压AD 换算成电压值
#define ScaleMV (uint16_t)((V_REG * 32768.0 * (VIN_R1 + VIN_R2) / (4095.0 * VIN_R2)) + 0.5)
static inline uint16_t get_input_voltage(uint16_t adc_val) {
    // 单位：0.01V
    return (uint16_t)(((uint32_t)adc_val * ScaleMV)>>15);
}
static inline uint16_t get_input_voltageX10(uint16_t adc_val) {
    // 单位：0.01V
    return (uint16_t)(((uint32_t)adc_val * ScaleMV*10)>>15);
}
static inline uint32_t get_MaxSpeed(uint16_t adc_val,uint16_t kv) {
    return (uint32_t)(((uint64_t)adc_val * kv * ScaleMV) >> 15);
}
//#define GET_INPUT_VOLTAGE(AD) 	((uint16_t)(100.0 * (V_REG / 4095.0) * AD  * ((VIN_R1 + VIN_R2) / VIN_R2)))

//低压级别
#define LvdLVL1     14.0
#define LvdLVL2     11.0
#define LvdLVL3     9.0
#define LvdLVL1_RPM     500
#define LvdLVL2_RPM     200
#define LvdLVL3_RPM     0

#define MaxLimitHVol    52  //最高限制电压

#define GetVBusLimit()  VBusVol(MaxLimitHVol)   

//最高超过限制电压 多少达到最大弱磁电流
#define MaxBoostVol     0.9f

//采样电阻 RSHUNT 0.005 放大倍数12倍 最高知道检测到31.75A
// vref*2/RSHUNT/AMP/sqrt(3) ~= 31.75A -> 32767  ---V_REG = vref*2
//32.75*x/32767 =>
#define  DefaultMaxCurrent       30.0    //最大电流60A？
#define  DefaultMaxBrakeCurrent  30.0
#define  DefaultMinBrakeCurrent  10.0
#define MaxWeakId    -18.0    //最大弱磁电流

//电流换算
#define CurrentInt16(x) (int16_t)(x*31.75*V_REG/RSHUNT/AMPLIFICATION_GAIN/SQRT_3)

// 弱磁-d 过滤系数
#define WEAK_FIELD_ALPHA         30000 //32768  // 0.25的滤波系数
#define LIMIT_SMOOTH_ALPHA       26000 //32768   // 限制值平滑滤波系数 (α=0.25)

//4000转以下 -Iq限制最小(值最大) 10000转以上-Iq限制达到最大(值最小)
#define SpeedLimit_MIN_NIq_TH 1500
#define SpeedLimit_MAX_NIq_TH 2500
#define IQBrakelimit                   6000 //最大刹车限幅

//限制前进时候最大转矩增速
#define ISDelayT        15 //200   //800   //I Speed change delay
#define MaxFBSpeedADD   5350  //最大速度误差
#define LIMIT_AddAcc_ALPHA 18000 //加速幅度   /65536  扭矩变化的一阶滤波
#define MaxTorqAcc      20000   //最大增加的电流扭矩 上个版本每次增加6% 现在最大增加到30%
#define MinTorqAcc      1  //最小增加的电流扭矩 12% 变化样子
//限制刹车时候的最大转矩增速
#define LIMIT_SubAcc_ALPHA 10000  //减速幅度 /65536  
#define MaxTorqDecAcc   8000   //最大减速限制 最大减速扭矩
#define MinTorqDecAcc   1  //最小减速限制

//发声时候的音量
#define speechVol   7000      //  /32768

#define Kpdiff      1   //和固定放大倍数 方便放大整个数据 防止数据过小时候全是0


#define HallCheckAddVd      10   //每次增加的vd
#define HallCheckEndVd       7000   //hall开始校准的最大vd电压
#define HallFastStep        10      //快速步进每次 + 10/65536
#define HallSlowStep        1       //慢速步进每次 + 1/65536

//获取角度值
#define hEdegree(x)     (x*65536/360)
//速度比例常数 64000000*60(1min)/65536(1圈)
#define ScaleErpm       (64000000UL*60UL/65536UL)
//转速前馈补偿
#define fw_pro    93    //前馈补偿的比例  ?*目标速度/128


#define IloopTrigH      500     //切入电流环的速度
#define IloopTrigL      200     //切回无电流环的速度


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

#endif

