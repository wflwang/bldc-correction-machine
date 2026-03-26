
/**
  ******************************************************************************
  * @file    motorcontrol.h
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   Motor Control Subsystem initialization functions.
  ******************************************************************************
  * @ingroup MCAPI
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTORCONTROL_H
#define __MOTORCONTROL_H

#include "parameters_conversion.h"
#include "mc_api.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup MCAPI
  * @{
  */

/* Initializes the Motor Control Subsystem */
void MX_MotorControl_Init(void);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __MOTORCONTROL_H */

