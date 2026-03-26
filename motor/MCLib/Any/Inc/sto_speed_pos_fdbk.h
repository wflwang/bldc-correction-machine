/**
  ******************************************************************************
  * @file    sto_speed_pos_fdbk.h
	* @version V1.0.1
	* @date    2021-07-23
  ******************************************************************************
*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STO_SPEEDNPOSFDBK_H
#define __STO_SPEEDNPOSFDBK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "speed_pos_fdbk.h"

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup SpeednPosFdbk
  * @{
  */

/** @addtogroup SpeednPosFdbk_STO
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @brief PWM & Current Sensing component handle type */
typedef struct STO_Handle STO_Handle_t;

typedef void ( *STO_ForceConvergency1_Cb_t )( STO_Handle_t * pHandle );
typedef void ( *STO_ForceConvergency2_Cb_t )( STO_Handle_t * pHandle );
typedef void ( *STO_OtfResetPLL_Cb_t )( STO_Handle_t * pHandle );
typedef bool ( *STO_SpeedReliabilityCheck_Cb_t )( STO_Handle_t * pHandle );

/**
  * @brief  SpeednPosFdbk  handle definition
  */
struct STO_Handle
{
  SpeednPosFdbk_Handle_t     *     _Super;
  STO_ForceConvergency1_Cb_t       pFctForceConvergency1;
  STO_ForceConvergency2_Cb_t       pFctForceConvergency2;
  STO_OtfResetPLL_Cb_t             pFctStoOtfResetPLL;
  STO_SpeedReliabilityCheck_Cb_t   pFctSTO_SpeedReliabilityCheck;
};

/**
  * @}
  */

/**
  * @}
  */

/** @} */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /*__STO_SPEEDNPOSFDBK_H*/


