/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_vdiv.h
* @brief:     This file contains all the functions prototypes for the Voltage divider firmware
*             library.
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_VDIV_H
#define __HK32M07X_VDIV_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup VDIV
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @defgroup SPI_Function_Declaration SPI_Function_Declaration
  * @{
  */

void VDIV_DeInit(void);
void VDIV_SetChannelData(uint16_t Data);
void VDIV_Cmd(FunctionalState NewState);
void VDIV_RemoveWriteProtection(FunctionalState NewState);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__HK32M07X_VDIV_H */

/***************************** (C) COPYRIGHT HKMicroChip  *******************************/
