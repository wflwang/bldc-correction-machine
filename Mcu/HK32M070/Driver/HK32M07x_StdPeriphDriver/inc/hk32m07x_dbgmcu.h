/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_dbgmcu.h
* @brief:     This file contains all the functions prototypes for the DBGMCU firmware
*             library.
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_DBGMCU_H
#define __HK32M07X_DBGMCU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup DBGMCU DBGMCU
  * @{
  */
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


/** @defgroup DBGMCU_Exported_Constants DBGMCU_Exported_Constants
  * @{
  */

#define DBGMCU_STOP                  DBGMCU_CR_DBG_STOP
#define IS_DBGMCU_PERIPH(PERIPH)     ((((PERIPH) & 0xFFFFFFFD) == 0x00) && ((PERIPH) != 0x00))

#define DBGMCU_WWDG_STOP             DBGMCU_APB_FZ_DBG_WWDG_STOP
#define DBGMCU_IWDG_STOP             DBGMCU_APB_FZ_DBG_IWDG_STOP

#define IS_DBGMCU_APBPERIPH(PERIPH)  ((((PERIPH) & 0xFFFFE7FF) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */

/** @defgroup DBGMCU_Exported_Functions DBGMCU_Exported_Functions
  * @{
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* Device and Revision ID management functions ********************************/
uint32_t DBGMCU_GetREVID(void);
uint32_t DBGMCU_GetDEVID(void);

/* Peripherals Configuration functions ****************************************/
void DBGMCU_Config(uint32_t DBGMCU_Periph, FunctionalState NewState);
void DBGMCU_APBPeriphConfig(uint32_t DBGMCU_Periph, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /*__HK32M07X_DBGMCU_H */

/**
  * @}
  */

/**
  * @}
  */

