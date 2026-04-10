/**
  ******************************************************************************
  * @file    mc_tasks.h 
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   This file implementes tasks definition.
  ******************************************************************************
  * @ingroup MCTasks
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCTASKS_H
#define __MCTASKS_H

/* Includes ------------------------------------------------------------------*/
#include "mc_tuning.h"
#include "mc_interface.h"
#include "app.h"
#include "foc_hall.h"

/** @addtogroup MCSDK
  * @{
  */

/** @defgroup MCTasks Motor Control Tasks
  * 
  * @brief Motor Control subsystem configuration and operation routines.  
  *
  * @{
  */

/* Initializes the Motor subsystem core according to user defined parameters. */
void MCboot( MCI_Handle_t* pMCIList[], MCT_Handle_t* pMCTList[] );

/* Runs all the Tasks of the Motor Control cockpit */
void MC_RunMotorControlTasks(void);

/* Executes the Medium Frequency Task functions for each drive instance */
void MC_Scheduler(void);

/* Executes safety checks (e.g. bus voltage and temperature) for all drive instances */
void TSK_SafetyTask(void);

/* Executes the Motor Control duties that require a high frequency rate and a precise timing */
uint8_t TSK_HighFrequencyTask(void);

/* Reserves FOC execution on ADC ISR half a PWM period in advance */
void TSK_DualDriveFIFOUpdate(void *pDrive);

/* Puts the Motor Control subsystem in in safety conditions on a Hard Fault */
void TSK_HardwareFaultTask(void);
uint8_t GetMaxTerefReady(void);
void GetMCConfig(void);     //flash读取motor
void GetAPPConfig(void);  //读取APP data
int32_t GetMaxSpeed(void);  //获取最大转速

 /* Locks GPIO pins used for Motor Control to prevent accidental reconfiguration */
void mc_lock_pins (void);
void clearRefIdq(void);
app_mode_t GetAPPMode(void);
bool GetISChangeState(void);
int16_t GetNowVBusAD(void);
void Hall_CalcAvrgMecSpeed01Hz( foc_hall_t * pHandle);
void SetMSpeechEN(int state);
int GetMSpeechEN(void);
void Delay_ms(__IO uint32_t Delay);
void printfMC(uint8_t mode);
void ScanUartRX(void);
/**
  * @}
  */

/**
  * @}
  */

#endif /* __MCTASKS_H */


