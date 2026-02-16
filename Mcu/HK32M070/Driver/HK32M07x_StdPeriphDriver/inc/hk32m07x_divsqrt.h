/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_divsqrt.h
* @brief:     DIVSQRT initialization and configuration
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_DIVSQRT_H
#define __HK32M07X_DIVSQRT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup DIVSQRT DIVSQRT
  * @{
  */
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup DIVSQRT_Exported_Constants DIVSQRT_Exported_Constants
  * @{
  */

/** @defgroup DIVSQRT_flags_definition DIVSQRT_flags_definition
  * @{
  */
#define DVSQ_FLAG_BUSY                              DVSQ_CSR_BUSY         /*!< DVSQ Busy flag */
#define DVSQ_FLAG_DIV                               DVSQ_CSR_DIV          /*!< DVSQ Divide operation flag */
#define DVSQ_FLAG_SQRT                              DVSQ_CSR_SQRT         /*!< DVSQ Square root operation flag */
#define DVSQ_FLAG_OV                                DVSQ_CSR_OV_FLAG      /*!< DVSQ Overflow flag */
#define DVSQ_FLAG_DZ                                DVSQ_CSR_DZ_FLAG      /*!< DVSQ Zero divisor flag */

#define IS_DVSQ_GET_FLAG(DVSQ_FLAG)   (((DVSQ_FLAG) == DVSQ_FLAG_BUSY) || ((DVSQ_FLAG) == DVSQ_FLAG_DIV) || \
                                       ((DVSQ_FLAG) == DVSQ_FLAG_SQRT) || ((DVSQ_FLAG) == DVSQ_FLAG_OV)  || \
                                       ((DVSQ_FLAG) == DVSQ_FLAG_DZ))
#define IS_DVSQ_CLEAR_FLAG(DVSQ_FLAG) (((DVSQ_FLAG) == DVSQ_FLAG_OV) || ((DVSQ_FLAG) == DVSQ_FLAG_DZ))
/**
  * @}
  */

/** @defgroup DIVSQRT_interrupts_definition DIVSQRT_interrupts_definition
  * @{
  */
#define DVSQ_IT_OV                                  DVSQ_CSR_OV_INT_EN    /*!< DVSQ Overflow interrupt */
#define DVSQ_IT_DZ                                  DVSQ_CSR_DZ_INT_EN    /*!< DVSQ Zero divisor interrupt */
#define IS_DVSQ_IT(DVSQ_IT)           (((DVSQ_IT) == DVSQ_IT_OV) || ((DVSQ_IT) == DVSQ_IT_DZ))
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @defgroup DIVSQRT_Exported_Functions DIVSQRT_Exported_Functions
  * @{
  */

/* DIVSQRT Initialization and Configuration functions */
uint32_t DVSQ_QWKUDivsion(uint32_t Dividend, uint32_t Divisor, uint32_t *pRemainder);
int32_t DVSQ_QWKDivsion(int32_t Dividend, int32_t Divisor, int32_t *pRemainder);
uint32_t DVSQ_Sqrt(uint32_t Radicand);
uint16_t DVSQ_HSqrt(uint32_t Radicand, uint16_t *pFraction);
void DVSQ_WriteDividend(uint32_t Dividend);
void DVSQ_WriteDivisor(uint32_t Divisor);
void DVSQ_WriteRadicand(uint32_t Radicand);
uint32_t DVSQ_GetRes(void);
uint32_t DVSQ_GetRemainder(void);
void DVSQ_ConfigDivFastStart(FunctionalState NewState);
void DVSQ_ConfigDivUnsigned(FunctionalState NewState);
void DVSQ_ConfigHighSqrt(FunctionalState NewState);
void DVSQ_Start(void);

/* DIVSQRT Interrupts and flags management functions */
FlagStatus DVSQ_GetFlagStatus(uint32_t DVSQ_FLAG);
void DVSQ_ClearFlag(uint32_t DVSQ_FLAG);
void DVSQ_ITConfig(uint32_t DVSQ_IT, FunctionalState NewState);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__HK32M07X_DIVSQRT_H */

/**
  * @}
  */
