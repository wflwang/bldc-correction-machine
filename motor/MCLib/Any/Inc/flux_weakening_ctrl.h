/*
  ******************************************************************************
  * @file    flux_weakening_ctrl.h
	* @version V1.0.1
	* @date    2021-07-23
	 ===========================================================================
                     ##### How to use flux weakening control #####
   ===========================================================================
		 [..]
     (#) This file provides firmware functions that implement the Flux Weakening
  *      Control component of the Motor Control SDK.
    @endverbatim
  ****************************************************************************** 
  */

/* Includes ------------------------------------------------------------------*/
#ifndef __FLUXWEAKENINGCTRL_H
#define __FLUXWEAKENINGCTRL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
#include "pid_regulator.h"
/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup FluxWeakeningCtrl
  * @{
  */

/**
  * @brief  Flux Weakening Control Component handle structure
  */
typedef struct
{
  PID_Handle_t        *       pFluxWeakeningPID; /**< PI object used for flux weakening */
  PID_Handle_t        *       pSpeedPID;         /**< PI object used for speed control */
  uint16_t        hFW_V_Ref;              /**< Voltage reference, tenth of
                                                 percentage points */
  Volt_Components AvVolt_qd;              /**< Average stator voltage in qd
                                                 reference frame */
  int16_t         AvVoltAmpl;             /**< Average stator voltage amplitude */
  int16_t         hIdRefOffset;           /**< Id reference offset */
  uint16_t        hMaxModule;             /**< Circle limitation maximum allowed module */

  uint16_t        hDefaultFW_V_Ref;       /**< Default flux weakening voltage reference,
                                               tenth of percentage points*/
  int16_t         hDemagCurrent;          /**< Demagnetization current in s16A:
                                               Current(Amp) = [Current(s16A) * Vdd micro]/
                                               [65536 * Rshunt * Aop] */
  int32_t         wNominalSqCurr;         /**< Squared motor nominal current in (s16A)^2
                                               where:
                                               Current(Amp) = [Current(s16A) * Vdd micro]/
                                               [65536 * Rshunt * Aop] */
  uint16_t        hVqdLowPassFilterBW;    /**< Use this parameter to configure the Vqd
                                               first order software filter bandwidth.
                                               hVqdLowPassFilterBW = FOC_CurrController
                                               call rate [Hz]/ FilterBandwidth[Hz] in
                                               case FULL_MISRA_COMPLIANCY is defined.
                                               On the contrary, if FULL_MISRA_COMPLIANCY
                                               is not defined, hVqdLowPassFilterBW is
                                               equal to log with base two of previous
                                               definition */
  uint16_t        hVqdLowPassFilterBWLOG; /**< hVqdLowPassFilterBW expressed as power of 2.
                                               E.g. if gain divisor is 512 the value
                                               must be 9 because 2^9 = 512 */
} FW_Handle_t;


/** @defgroup Initialization and reset module 
  * @brief FluxWeakeningCtrl control component initialization
 
@verbatim
 ===============================================================================
       ##### Initialize and reset FluxWeakeningCtrl related component  #####
 ===============================================================================
@endverbatim
  * @{
  */
/* Initializes all the object variables, usually it has to be called once right after object creation. */
void FW_Init( FW_Handle_t * pHandle, PID_Handle_t * pPIDSpeed, PID_Handle_t * pPIDFluxWeakeningHandle );

/* It should be called before each motor restart and clears the Flux weakening internal variables */
void FW_Clear( FW_Handle_t * pHandle );

/** @defgroup Current Iq calculation module 
  * @brief Calculate the new current value in real time
@verbatim
 ===============================================================================
                      ##### Iq calculation function #####
 ===============================================================================
@endverbatim
  * @{
  */
/* It computes Iqdref according the flux weakening algorithm.*/
Curr_Components FW_CalcCurrRef( FW_Handle_t * pHandle, Curr_Components Iqdref );

/** @defgroup Voltage get module 
  * @brief Get target voltage reference voltage current voltage and average voltage
@verbatim
 ===============================================================================
                      ##### Voltage get related function #####
 ===============================================================================
 @endverbatim
  * @{
  */
/*Use this method to set a new value for the voltage reference used by flux weakening algorithm */
void FW_SetVref( FW_Handle_t * pHandle, uint16_t hNewVref );

/* It returns the present value of target voltage used by flux weakening algorihtm. */
uint16_t FW_GetVref( FW_Handle_t * pHandle );

/* It returns the present value of voltage actually used by flux weakening algorihtm. */
int16_t FW_GetAvVAmplitude( FW_Handle_t * pHandle );

/* It returns the measure of present voltage actually used by flux weakening algorihtm as percentage of available voltage. */
uint16_t FW_GetAvVPercentage( FW_Handle_t * pHandle );

/** @defgroup Voltage filter module 
  * @brief The voltage Vqd filter function 
@verbatim
 ===============================================================================
                   ##### The voltage Vqd is low pass filtered #####
 ===============================================================================
@endverbatim
  * @{
  */

/* It low-pass filters both the Vqd voltage components. */
void FW_DataProcess( FW_Handle_t * pHandle, Volt_Components Vqd );
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __FLUXWEAKENINGCTRL_H */

