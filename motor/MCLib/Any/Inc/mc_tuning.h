/**
  ******************************************************************************
  * @file    mc_tuning.h 
  * @version V1.0.0
  * @date    2021-07-27
  * @brief   This file contains all definitions and functions prototypes for the
  *          motor control tuning component.	
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MC_TUNING_H
#define __MC_TUNING_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
#include "pid_regulator.h"
#include "revup_ctrl.h"
#include "sto_pll_speed_pos_fdbk.h"
//#include "sto_cordic_speed_pos_fdbk.h"
#include "virtual_speed_sensor.h"
#include "speed_torq_ctrl.h"
#include "motor_power_measurement.h"
#include "pqd_motor_power_measurement.h"
#include "ntc_temperature_sensor.h"
#include "bus_voltage_sensor.h"
#include "feed_forward_ctrl.h"
#include "flux_weakening_ctrl.h"
#include "state_machine.h"


/**
 * @addtogroup MOTOR
  * @{
  */

/**
 * @defgroup MCTuning Motor Control Fine Tuning interface
 *
 * @brief This interface provides access to the internals of the Motor Control Subsystem
 *
 * @todo Complete documentation
 * @{
 */



/**
  * @brief  MC tuning internal objects initialization structure type;
  */
typedef struct
{
  PID_Handle_t * pPIDSpeed;
  PID_Handle_t * pPIDIq;
  PID_Handle_t * pPIDId;
  PID_Handle_t * pPIDFluxWeakening;
  PWMC_Handle_t * pPWMnCurrFdbk;
  RevUpCtrl_Handle_t * pRevupCtrl;
  SpeednPosFdbk_Handle_t * pSpeedSensorMain;
  SpeednPosFdbk_Handle_t * pSpeedSensorAux;
  VirtualSpeedSensor_Handle_t * pSpeedSensorVirtual;
  SpeednTorqCtrl_Handle_t * pSpeednTorqueCtrl;
  STM_Handle_t * pStateMachine;
  NTC_Handle_t * pTemperatureSensor;
  BusVoltageSensor_Handle_t * pBusVoltageSensor;
  MotorPowMeas_Handle_t * pMPM;
  FW_Handle_t  * pFW;
  FF_Handle_t  * pFF;
} MCT_Handle_t;


/**
  * @brief  Use this method to set a new value for the voltage reference used by
  *         flux weakening algorithm.
  * @param  pHandle Flux weakening init strutcture.
  * @param  uint16_t New target voltage value, expressend in tenth of percentage
  *         points of available voltage.
  * @retval none
  */
void FW_SetVref( FW_Handle_t * pHandle, uint16_t hNewVref );

/**
  * @brief  It returns the present value of target voltage used by flux
  *         weakening algorihtm.
  * @param  pHandle Flux weakening init strutcture.
  * @retval int16_t Present target voltage value expressed in tenth of
  *         percentage points of available voltage.
  */
uint16_t FW_GetVref( FW_Handle_t * pHandle );

/**
  * @brief  It returns the present value of voltage actually used by flux
  *         weakening algorihtm.
  * @param  pHandle Flux weakening init strutcture.
  * @retval int16_t Present averaged phase stator voltage value, expressed
  *         in s16V (0-to-peak), where
  *         PhaseVoltage(V) = [PhaseVoltage(s16A) * Vbus(V)] /[sqrt(3) *32767].
  */
int16_t FW_GetAvVAmplitude( FW_Handle_t * pHandle );

/**
  * @brief  It returns the measure of present voltage actually used by flux
  *         weakening algorihtm as percentage of available voltage.
  * @param  pHandle Flux weakening init strutcture.
  * @retval uint16_t Present averaged phase stator voltage value, expressed in
  *         tenth of percentage points of available voltage.
  */
uint16_t FW_GetAvVPercentage( FW_Handle_t * pHandle );

/**
  * @brief  Use this method to set new values for the constants utilized by
  *         feed-forward algorithm.
  * @param  pHandle Feed forward  strutcture.
  * @param  sNewConstants The FF_TuningStruct_t containing constants utilized by
  *         feed-forward algorithm.
  * @retval none
  */
void FF_SetFFConstants( FF_Handle_t * pHandle, FF_TuningStruct_t sNewConstants );

/**
  * @brief  Use this method to get present values for the constants utilized by
  *         feed-forward algorithm.
  * @param  pHandle Feed forward  strutcture.
  * @retval FF_TuningStruct_t Values of the constants utilized by
  *         feed-forward algorithm.
  */
FF_TuningStruct_t FF_GetFFConstants( FF_Handle_t * pHandle );

/**
  * @brief  Use this method to get present values for the Vqd feed-forward
  *         components.
  * @param  pHandle Feed forward  strutcture.
  * @retval Volt_Components Vqd feed-forward components.
  */
Volt_Components FF_GetVqdff( FF_Handle_t * pHandle );

/**
  * @brief  Use this method to get values of the averaged output of qd axes
  *         currents PI regulators.
  * @param  pHandle Feed forward  strutcture.
  * @retval Volt_Components Averaged output of qd axes currents PI regulators.
  */
Volt_Components FF_GetVqdAvPIout( FF_Handle_t * pHandle );


/**
  * @brief  It updates the Kp gain
  * @param  CPI PI object
  * @param  int16_t New Kp gain
  * @retval None
  */
void PID_SetKP( PID_Handle_t * pHandle, int16_t hKpGain );

/**
  * @brief  It updates the Ki gain
  * @param  CPI PI object
  * @param  int16_t New Ki gain
  * @retval None
  */
void PID_SetKI( PID_Handle_t * pHandle, int16_t hKiGain );

/**
  * @brief  It returns the Kp gain of the passed PI object
  * @param  CPI PI regulator object
  * @retval int16_t Kp gain
  */
int16_t PID_GetKP( PID_Handle_t * pHandle );

/**
  * @brief  It returns the Kp gain divisor of the passed PI object
  * @param  CPI PI regulator object
  * @retval int16_t Kp gain
  */
uint16_t PID_GetKPDivisor( PID_Handle_t * pHandle );

/**
  * @brief  It returns the Ki gain of the passed PI object
  * @param  CPI PI regulator object
  * @retval int16_t Ki gain
  */
int16_t PID_GetKI( PID_Handle_t * pHandle );

/**
  * @brief  It returns the Ki gain divisor of the passed PI object
  * @param  CPI PI regulator object
  * @retval int16_t Ki gain
  */
uint16_t PID_GetKIDivisor( PID_Handle_t * pHandle );

/**
  * @brief  It returns the Default Kp gain of the passed PI object
  * @param  CPI PI regulator object
  * @retval int16_t Kp gain
  */
int16_t PID_GetDefaultKP( PID_Handle_t * pHandle );

/**
  * @brief  It returns the Default Ki gain of the passed PI object
  * @param  CPI PI regulator object
  * @retval int16_t Ki gain
  */
int16_t PID_GetDefaultKI( PID_Handle_t * pHandle );

/**
  * @brief  It set a new value into the PI integral term
  * @param  CPI PI regulator object
  * @param  int32_t New integral term value
  * @retval None
  */
void PID_SetIntegralTerm( PID_Handle_t * pHandle, int32_t wIntegralTermValue );

/**
  * @brief  It set a new value into the PID Previous error variable required to
  *         compute derivative term
  * @param  pHandle regulator object
  * @param  wPrevProcessVarError New integral term value
  * @retval None
  */
void PID_SetPrevError( PID_Handle_t * pHandle, int32_t wPrevProcessVarError );

/**
  * @brief  It updates the Kd gain
  * @param  pHandle PID regulator object
  * @param  hKdGain New Kd gain
  * @retval None
  */
void PID_SetKD( PID_Handle_t * pHandle, int16_t hKdGain );

/**
  * @brief  It returns the Kd gain of the PID object passed
  * @param  pHandle PID regulator object
  * @retval int16_t Kd gain
  */
int16_t PID_GetKD( PID_Handle_t * pHandle );

/**
* @brief  Execute a regular conversion using ADC1.
*         The function is not re-entrant (can't executed twice at the same time)
*         It returns 0xFFFF in case of conversion error.
* @param  pHandle PWM component handler
* @param  bChannel ADC channel used for the regular conversion
* @retval It returns converted value or oxFFFF for conversion error
*/
uint16_t PWMC_ExecRegularConv( PWMC_Handle_t * pHandle, uint8_t bChannel );

/**
* @brief  It sets the specified sampling time for the specified ADC channel
*         on ADC1. It must be called once for each channel utilized by user
* @param  pHandle PWM component handler
* @param  ADConv_struct struct containing ADC channel and sampling time
* @retval none
*/
void PWMC_ADC_SetSamplingTime( PWMC_Handle_t * pHandle, ADConv_t ADConv_struct );

/**
  * @brief  It is used to modify the default value of duration of a specific
  *         rev up phase.
  *         Note: The module can be also compiled commenting the
  *         define RUC_ALLOWS_TUNING to optimize the flash memory occupation
  *         and the RAM usage if the tuning is not required in this case this
  *         function has no effect.
  * @param  pHandle related object of class CRUC.
  * @param  bPhase is the rev up phase, zero based, to be modified.
  * @param  hDurationms is the new value of duration for that phase.
  * @retval none.
  */
void RUC_SetPhaseDurationms( RevUpCtrl_Handle_t * pHandle, uint8_t bPhase, uint16_t hDurationms );

/**
  * @brief  It is used to modify the default value of mechanical speed at the
  *         end of a specific rev up phase.
  *         Note: The module can be also compiled commenting the
  *         define RUC_ALLOWS_TUNING to optimize the flash memory occupation
  *         and the RAM usage if the tuning is not required in this case this
  *         function has no effect.
  * @param  pHandle related object of class CRUC.
  * @param  bPhase is the rev up phase, zero based, to be modified.
  * @param  hFinalMecSpeed01Hz is the new value of mechanical speed at the end
  *         of that phase expressed in 0.1Hz.
  * @retval none.
  */
void RUC_SetPhaseFinalMecSpeed01Hz( RevUpCtrl_Handle_t * pHandle, uint8_t bPhase,
                                    int16_t hFinalMecSpeed01Hz );

/**
  * @brief  It is used to modify the default value of motor torque at the end of
  *         a specific rev up phase.
  *         Note: The module can be also compiled commenting the
  *         define RUC_ALLOWS_TUNING to optimize the flash memory occupation
  *         and the RAM usage if the tuning is not required in this case this
  *         function has no effect.
  * @param  pHandle related object of class CRUC.
  * @param  bPhase is the rev up phase, zero based, to be modified.
  * @param  hFinalTorque is the new value of motor torque at the end of that
  *         phase. This value represents actually the Iq current expressed in
  *         digit.
  * @retval none.
  */
void RUC_SetPhaseFinalTorque( RevUpCtrl_Handle_t * pHandle, uint8_t bPhase, int16_t hFinalTorque );

/**
  * @brief  It is used to read the current value of duration of a specific rev
  *         up phase.
  *         Note: The module can be also compiled commenting the
  *         define RUC_ALLOWS_TUNING to optimize the flash memory occupation
  *         and the RAM usage if the tuning is not required in this case this
  *         function has no effect.
  * @param  pHandle related object of class CRUC.
  * @param  bPhase is the rev up phase, zero based, to be read.
  * @retval uint16_t The current value of duration for that phase expressed in
  *         milliseconds.
  */
uint16_t RUC_GetPhaseDurationms( RevUpCtrl_Handle_t * pHandle, uint8_t bPhase );

/**
  * @brief  It is used to read the current value of mechanical speed at the end
  *         of a specific rev up phase.
  *         Note: The module can be also compiled commenting the
  *         define RUC_ALLOWS_TUNING to optimize the flash memory occupation
  *         and the RAM usage if the tuning is not required in this case this
  *         function has no effect.
  * @param  pHandle related object of class CRUC.
  * @param  bPhase is the rev up phase, zero based, to be read.
  * @retval int16_t The current value of mechanical speed at the end of that
  *         phase expressed in 0.1Hz.
  */
int16_t RUC_GetPhaseFinalMecSpeed01Hz( RevUpCtrl_Handle_t * pHandle, uint8_t bPhase );

/**
  * @brief  It is used to read the current value of motor torque at the end of a
  *         specific rev up phase.
  *         Note: The module can be also compiled commenting the
  *         define RUC_ALLOWS_TUNING to optimize the flash memory occupation
  *         and the RAM usage if the tuning is not required in this case this
  *         function has no effect.
  * @param  pHandle related object of class CRUC.
  * @param  bPhase is the rev up phase, zero based, to be read.
  * @retval int16_t The current value of motor torque at the end of that phase.
  *         This value represents actually the Iq current expressed in digit.
  */
int16_t RUC_GetPhaseFinalTorque( RevUpCtrl_Handle_t * pHandle, uint8_t bPhase );

/**
  * @brief  It is used to get information about the number of phases relative to
  *         the programmed rev up.
  *         Note: The module can be also compiled commenting the
  *         define RUC_ALLOWS_TUNING to optimize the flash memory occupation
  *         and the RAM usage if the tuning is not required in this case this
  *         function has no effect.
  * @param  pHandle related object of class CRUC.
  * @retval uint8_t The number of phases relative to the programmed rev up.
  */
uint8_t RUC_GetNumberOfPhases( RevUpCtrl_Handle_t * pHandle );

/**
  * @brief  Returns latest averaged temperature measurement expressed in Celsius degrees
  *
  * @param  pHandle: Pointer on Handle structure of TemperatureSensor component
  *
  * @retval Latest averaged temperature measurement in Celsius degrees
  */
int16_t NTC_GetAvTemp_C( NTC_Handle_t * pHandle );

/**
  * @brief  Returns Temperature mesurement fault status
  * Fault can be either MC_OVER_TEMP or MC_NO_ERROR according on protection threshold values set
  *
  * @param  pHandle: Pointer on Handle structure of TemperatureSensor component.
  *
  * @retval Fault code error
  */
uint16_t NTC_CheckTemp( NTC_Handle_t * pHandle );



/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __MC_TUNING_H */

