/**
  ******************************************************************************
  * @file    mc_config.h 
  * @version V1.0.0
  * @date    2021-07-30
  ******************************************************************************
  */  
#ifndef __MC_CONFIG_H
#define __MC_CONFIG_H

#include "pid_regulator.h"
#include "revup_ctrl.h"
#include "speed_torq_ctrl.h"
#include "virtual_speed_sensor.h"
//#include "ntc_temperature_sensor.h"
#include "pwm_curr_fdbk.h"
#include "r_divider_bus_voltage_sensor.h"
#include "virtual_bus_voltage_sensor.h"
//#include "pqd_motor_power_measurement.h"
#include "r3_f0xx_pwm_curr_fdbk.h"
#include "hall_speed_pos_fdbk.h"
 
extern RevUpCtrl_Handle_t RevUpControlM1;
#include "ramp_ext_mngr.h"
#include "circle_limitation.h"
#include "sto_speed_pos_fdbk.h"
#include "sto_pll_speed_pos_fdbk.h"
//#include "mc_config.h"
#include "foc_hall.h"

extern PID_Handle_t PIDSpeedHandle_M1;
extern PID_Handle_t PIDIqHandle_M1;
extern PID_Handle_t PIDIdHandle_M1;
//extern NTC_Handle_t TempSensorParamsM1;

extern PWMC_R3_F0_Handle_t PWM_Handle_M1;

extern SpeednTorqCtrl_Handle_t SpeednTorqCtrlM1;
// PQD_MotorPowMeas_Handle_t PQD_MotorPowMeasM1;
//extern PQD_MotorPowMeas_Handle_t *pPQD_MotorPowMeasM1; 
extern VirtualSpeedSensor_Handle_t VirtualSpeedSensorM1;
extern STO_Handle_t STO_M1;
extern STO_PLL_Handle_t STO_PLL_M1;
extern RDivider_Handle_t RealBusVoltageSensorParamsM1;
extern CircleLimitation_Handle_t CircleLimitationM1;
extern foc_hall_t HALL_M1; 

extern RampExtMngr_Handle_t RampExtMngrHFParamsM1;
void DefaultMCConfig(mc_config_t *mcconf);

#define NBR_OF_MOTORS 1
#endif /* __MC_CONFIG_H */

