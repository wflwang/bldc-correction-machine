/*
  ******************************************************************************
  * @file    speed_torq_ctrl.h
	* @version V1.0.1
	* @date    2021-07-22
 ===========================================================================
                    ##### How to control speed and torque #####
 ===========================================================================
		 [..]
     (#) This file provides firmware functions that implement the following features
         of the Speed & Torque Control component of the Motor Control SDK.
    @endverbatim
  ****************************************************************************** 
  */
/* Includes ------------------------------------------------------------------*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPEEDNTORQCTRLCLASS_H
#define __SPEEDNTORQCTRLCLASS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
#include "pid_regulator.h"
#include "speed_pos_fdbk.h"

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup SpeednTorqCtrl
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  Speed & Torque Control parameters definition
  */
typedef struct
{
  STC_Modality_t Mode;   /*!< Modality of STC. It can be one of these two
                               settings: STC_TORQUE_MODE to enable the
                               Torque mode or STC_SPEED_MODE to enable the
                               Speed mode.*/
  int16_t TargetFinal;  /*!< Backup of hTargetFinal to be applied in the
                             last step.*/
  int32_t SpeedRef01HzExt; /*!< Current mechanical rotor speed reference
                                     expressed in tenths of HZ multiplied by
                                     65536.*/
  int32_t TorqueRef;     /*!< Current motor torque reference. This value
                                     represents actually the Iq current
                                     expressed in digit multiplied by 65536.*/
  uint32_t RampRemainingStep;/*!< Number of steps remaining to complete the
                                     ramp.*/
  PID_Handle_t * PISpeed;   /*!< The regulator used to perform the speed
                                     control loop.*/
  SpeednPosFdbk_Handle_t * SPD;/*!< The speed sensor used to perform the speed
                                     regulation.*/
  int32_t IncDecAmount; /*!< Increment/decrement amount to be applied to
                                     the reference value at each
                                     CalcTorqueReference.*/

  uint16_t STCFrequencyHz;             /*!< Frequency on which the user updates
                                             the torque reference calling
                                             STC_CalcTorqueReference method
                                             expressed in Hz */
  uint16_t MaxAppPositiveMecSpeed01Hz; /*!< Application maximum positive value
                                             of rotor speed. It's expressed in
                                             tenth of mechanical Hertz.*/
  uint16_t MinAppPositiveMecSpeed01Hz; /*!< Application minimum positive value
                                             of rotor speed. It's expressed in
                                             tenth of mechanical Hertz.*/
  int16_t MaxAppNegativeMecSpeed01Hz;  /*!< Application maximum negative value
                                             of rotor speed. It's expressed in
                                             tenth of mechanical Hertz.*/
  int16_t MinAppNegativeMecSpeed01Hz;  /*!< Application minimum negative value
                                             of rotor speed. It's expressed in
                                             tenth of mechanical Hertz.*/
  uint16_t MaxPositiveTorque;          /*!< Maximum positive value of motor
                                             torque. This value represents
                                             actually the maximum Iq current
                                             expressed in digit.*/
  int16_t MinNegativeTorque;           /*!< Minimum negative value of motor
                                             torque. This value represents
                                             actually the maximum Iq current
                                             expressed in digit.*/
  STC_Modality_t ModeDefault;          /*!< Default STC modality.*/
  int16_t MecSpeedRef01HzDefault;      /*!< Default mechanical rotor speed
                                             reference expressed in tenths of
                                             HZ.*/
  int16_t TorqueRefDefault;            /*!< Default motor torque reference.
                                             This value represents actually the
                                             Iq current reference expressed in
                                             digit.*/
  int16_t IdrefDefault;                /*!< Default Id current reference expressed
                                             in digit.*/
} SpeednTorqCtrl_Handle_t;


/** @defgroup initialization and reset module  
  * @brief Speed & torque control related variables initialization
 
@verbatim
 ===============================================================================
      ##### Initialize and reset SpeednTorqCtrl related component #####
 ===============================================================================
@endverbatim
  * @{
  */
/* Initializes all the object variables, usually it has to be called once right after object creation. */
void STC_Init( SpeednTorqCtrl_Handle_t * pHandle, PID_Handle_t * oPI, SpeednPosFdbk_Handle_t * oSPD );

/* It should be called before each motor restart. If STC is set in speed mode, this method resets the integral term of speed regulator. */
void STC_Clear( SpeednTorqCtrl_Handle_t * pHandle );

/** @defgroup  Speed Control related module
  * @brief Speed Control component of the Motor Control SDK
 
@verbatim
 ===============================================================================
                    ##### Speed control functions #####
 ===============================================================================
@endverbatim
  * @{
  */
/* It sets in real time the speed sensor utilized by the STC.*/
void STC_SetSpeedSensor( SpeednTorqCtrl_Handle_t * pHandle, SpeednPosFdbk_Handle_t * oSPD );
	
/* It returns the speed sensor utilized by the FOC. */
SpeednPosFdbk_Handle_t * STC_GetSpeedSensor( SpeednTorqCtrl_Handle_t * pHandle );
	
/* Get the Default mechanical rotor speed reference expressed in tenths of HZ.*/
int16_t STC_GetMecSpeedRef01HzDefault( SpeednTorqCtrl_Handle_t * pHandle );

/* Get the current mechanical rotor speed reference expressed in tenths of HZ.*/
int16_t STC_GetMecSpeedRef01Hz( SpeednTorqCtrl_Handle_t * pHandle );

/* Get the Application maximum positive value of rotor speed. It's expressed in tenth of mechanical Hertz.*/
uint16_t STC_GetMaxAppPositiveMecSpeed01Hz( SpeednTorqCtrl_Handle_t * pHandle );

/* Stop the execution of speed ramp. */
bool STC_StopSpeedRamp( SpeednTorqCtrl_Handle_t * pHandle );

/*  Force the speed reference to the curren speed. It is used at the START_RUN state to initialize the speed reference. */
void STC_ForceSpeedReferenceToCurrentSpeed( SpeednTorqCtrl_Handle_t * pHandle );

/*  Get the Application minimum negative value of rotor speed. It's expressed in tenth of mechanical Hertz.*/
int16_t STC_GetMinAppNegativeMecSpeed01Hz( SpeednTorqCtrl_Handle_t * pHandle );

/** @defgroup  Speed & torque control related module
  * @brief Speed and torque mode configuration related functions
 
@verbatim
 ===============================================================================
              ##### Speed & torque control functions #####
 ===============================================================================
@endverbatim
  * @{
  */
	
/* Set the modality of the speed and torque controller. Two modality are available Torque mode and Speed mode.*/
void STC_SetControlMode( SpeednTorqCtrl_Handle_t * pHandle, STC_Modality_t bMode );


/* Get the mode of the speed and torque controller. */
STC_Modality_t STC_GetControlMode( SpeednTorqCtrl_Handle_t * pHandle );

/* Starts the execution of a ramp using new target and duration. */
bool STC_ExecRamp( SpeednTorqCtrl_Handle_t * pHandle, int16_t hTargetFinal, uint32_t hDurationms );

/* Check if the settled speed or torque ramp has been completed.*/
bool STC_RampCompleted( SpeednTorqCtrl_Handle_t * pHandle );

/* It interrupts the execution of any previous ramp command.*/
void STC_StopRamp( SpeednTorqCtrl_Handle_t * pHandle );

/** @defgroup Torque Control function module
  * @brief  Torque Control component of the Motor Control SDK
 
@verbatim
 ===============================================================================
                   ##### Torque control functions #####
 ===============================================================================
@endverbatim
  * @{
 */
/* It computes the new value of motor torque reference */
Curr_Components STC_CalcTorqueReference( SpeednTorqCtrl_Handle_t * pHandle );

/*  Get the current motor torque reference. */
int16_t STC_GetTorqueRef( SpeednTorqCtrl_Handle_t * pHandle );

/* It returns the default values of Iqdref. */
Curr_Components STC_GetDefaultIqdref( SpeednTorqCtrl_Handle_t * pHandle );

/* It sets the nominal current */
void STC_SetNominalCurrent( SpeednTorqCtrl_Handle_t * pHandle, uint16_t hNominalCurrent );

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __SPEEDNTORQCTRLCLASS_H */


