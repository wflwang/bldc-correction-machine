/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_divsqrt.c
* @brief:     This file provides all the DIVSQRT firmware functions.
* @author:    AE Team
* @version:   V1.0.0/2024-12-23
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_divsqrt.h"


/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup DIVSQRT DIVSQRT
  * @brief DIVSQRT driver modules
  * @{
*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup DIVSQRT_Private_Functions DIVSQRT_Private_Functions
  * @{
  */

/** @defgroup DIVSQRT_Group1 DIVSQRT Initialization and Configuration functions
 *  @brief   DIVSQRT Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
        ##### DIVSQRT Initialization and Configuration functions #####
 ===============================================================================
     [..] This section provides functions allowing to:
        (+) DVSQ Quick unsigned division operation
        (+) DVSQ Quick signed division operation
        (+) DVSQ normal square root operation
        (+) DVSQ high precision square root operation
        (+) DVSQ Write the dividend
        (+) DVSQ Write the divisor
        (+) DVSQ Write the Radicand
        (+) DVSQ Get Res register
        (+) DVSQ Get Remainder register
        (+) Enable or Disable DVSQ Divide Fast Start Mode
        (+) Enable or Disable DVSQ Unsigned Divide mode
        (+) Enable or Disable DVSQ high precision square root operation
        (+) DVSQ Divide operation start
@endverbatim
  * @{
  */

/**
  * @brief  DVSQ Quick unsigned division operation
  * @param  Dividend:    32bit unsigned dividend
  * @param  Divisor:     32bit unsigned Divisor
  * @param  *pRemainder: Used to store the remainder
  * @retval Calculation results
  */
uint32_t DVSQ_QWKUDivsion(uint32_t Dividend, uint32_t Divisor, uint32_t *pRemainder)
{
    /* DVSQ Waits idle */
    while (DVSQ_GetFlagStatus(DVSQ_FLAG_BUSY) == SET)
    {
    }

    /* Configure the DVSQ to perform unsigned division mode */
    DVSQ->CSR |= DVSQ_CSR_UNSIGN_DIV;
    /* Enables the quick division function */
    DVSQ->CSR &= ~DVSQ_CSR_DFS;
    /* Write the DIVIDEND register */
    DVSQ->DIVIDEND = Dividend;
    /* Write the DIVISOR register */
    DVSQ->DIVISOR  = Divisor;

    /* DVSQ Waits idle */
    while (DVSQ_GetFlagStatus(DVSQ_FLAG_BUSY) == SET)
    {
    }

    /* Get the REMAINDER register */
    if (pRemainder != NULL)
    {
        *pRemainder = DVSQ->REMAINDER;
    }

    /* Return RES register */
    return DVSQ->RES;
}

/**
  * @brief  DVSQ Quick signed division operation
  * @param  Dividend:    32bit signed dividend
  * @param  Divisor:     32bit signed Divisor
  * @param  *pRemainder: Used to store the remainder
  * @retval Calculation results
  */
int32_t DVSQ_QWKDivsion(int32_t Dividend, int32_t Divisor, int32_t *pRemainder)
{
    /* DVSQ Waits idle */
    while (DVSQ_GetFlagStatus(DVSQ_FLAG_BUSY) == SET)
    {
    }

    /* Configure the DVSQ to perform signed division mode */
    DVSQ->CSR &= ~DVSQ_CSR_UNSIGN_DIV;
    /* Enables the quick division function */
    DVSQ->CSR &= ~DVSQ_CSR_DFS;
    /* Write the DIVIDEND register */
    DVSQ->DIVIDEND = Dividend;
    /* Write the DIVISOR register */
    DVSQ->DIVISOR  = Divisor;

    /* DVSQ Waits idle */
    while (DVSQ_GetFlagStatus(DVSQ_FLAG_BUSY) == SET)
    {
    }

    /* Get the REMAINDER register */
    if (pRemainder != NULL)
    {
        *pRemainder = DVSQ->REMAINDER;
    }

    /* Return RES register */
    return DVSQ->RES;
}

/**
  * @brief  DVSQ normal square root operation
  * @param  Radicand: Radicand
  * @retval Calculation results
  */
uint32_t DVSQ_Sqrt(uint32_t Radicand)
{
    /* DVSQ Waits idle */
    while (DVSQ_GetFlagStatus(DVSQ_FLAG_BUSY) == SET)
    {
    }

    /* Configure DVSQ as normal square root operation mode */
    DVSQ->CSR &= ~DVSQ_CSR_HPRESQRT;
    /* Write the RADICAND register */
    DVSQ->RADICAND = Radicand;

    /* DVSQ Waits idle */
    while (DVSQ_GetFlagStatus(DVSQ_FLAG_BUSY) == SET)
    {
    }

    /* Return RES register */
    return DVSQ->RES;
}

/**
  * @brief  DVSQ high precision square root operation
  * @param  Radicand:   Radicand
  * @param  *pFraction: Used to store the fractional part of the result
  * @retval Calculation results
  */
uint16_t DVSQ_HSqrt(uint32_t Radicand, uint16_t *pFraction)
{
    uint16_t Result;

    /* DVSQ Waits idle */
    while (DVSQ_GetFlagStatus(DVSQ_FLAG_BUSY) == SET)
    {
    }

    /* Configure DVSQ as high precision square root operation mode */
    DVSQ->CSR |= DVSQ_CSR_HPRESQRT;
    /* Write the RADICAND register */
    DVSQ->RADICAND = Radicand;

    /* DVSQ Waits idle */
    while (DVSQ_GetFlagStatus(DVSQ_FLAG_BUSY) == SET)
    {
    }

    /* Get the Result */
    Result = (uint16_t)((DVSQ->RES >> 16) & 0xffff);
    /* Get the Fraction */
    *pFraction = (uint16_t)(DVSQ->RES & 0xffff);
    /* Return Result */
    return Result;
}

/**
  * @brief  DVSQ Write the dividend
  * @param  Dividend: 32bit dividend
  * @retval None
  */
void DVSQ_WriteDividend(uint32_t Dividend)
{
    /* Write the DIVIDEND register */
    DVSQ->DIVIDEND = Dividend;
}

/**
  * @brief  DVSQ Write the divisor
  * @param  Divisor: 32bit divisor
  * @retval None
  */
void DVSQ_WriteDivisor(uint32_t Divisor)
{
    /* Write the DIVISOR register */
    DVSQ->DIVISOR = Divisor;
}

/**
  * @brief  DVSQ Write the Radicand
  * @param  Radicand: 32bit Radicand
  * @retval None
  */
void DVSQ_WriteRadicand(uint32_t Radicand)
{
    /* Write the RADICAND register */
    DVSQ->RADICAND = Radicand;
}

/**
  * @brief  DVSQ Get Res register
  * @retval Res
  */
uint32_t DVSQ_GetRes(void)
{
    return DVSQ->RES;
}

/**
  * @brief  DVSQ Get Remainder register
  * @retval Remainder
  */
uint32_t DVSQ_GetRemainder(void)
{
    return DVSQ->REMAINDER;
}

/**
  * @brief  Enable or Disable DVSQ Divide Fast Start Mode
  * @param  NewState: new state of the specified DVSQ fast start mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DVSQ_ConfigDivFastStart(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable DVSQ Divide Fast Start Mode */
        DVSQ->CSR &= (~(uint32_t)DVSQ_CSR_DFS);
    }
    else
    {
        /* Disable DVSQ Divide Fast Start Mode */
        DVSQ->CSR |= (uint32_t)DVSQ_CSR_DFS;
    }
}

/**
  * @brief  Enable or Disable DVSQ Unsigned Divide mode
  * @param  NewState: new state of the specified DVSQ unsigned divide Mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DVSQ_ConfigDivUnsigned(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable DVSQ Divide Unsigned Mode */
        DVSQ->CSR |= (uint32_t)DVSQ_CSR_UNSIGN_DIV;
    }
    else
    {
        /* Disable DVSQ Divide Unsigned Mode */
        DVSQ->CSR &= (~(uint32_t)DVSQ_CSR_UNSIGN_DIV);
    }
}

/**
  * @brief  Enable or Disable DVSQ high precision square root operation
  * @param  NewState: new state of the specified DVSQ high precision square root.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DVSQ_ConfigHighSqrt(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable DVSQ high precision square root operation */
        DVSQ->CSR |= (uint32_t)DVSQ_CSR_HPRESQRT;
    }
    else
    {
        /* Disable DVSQ high precision square root operation */
        DVSQ->CSR &= (~(uint32_t)DVSQ_CSR_HPRESQRT);
    }
}

/**
  * @brief  DVSQ Divide operation start
  * @retval None
  */
void DVSQ_Start(void)
{
    DVSQ->CSR |= (uint32_t)DVSQ_CSR_DIV_SRT;
}

/**
  * @}
  */

/** @defgroup DIVSQRT_Group2 DIVSQRT Interrupts and flags management functions
 *  @brief   DIVSQRT Interrupts and flags management functions
 *
@verbatim
 ===============================================================================
        ##### DIVSQRT Interrupts and flags management functions #####
 ===============================================================================
     [..] This section provides functions allowing to:
        (+) Checks whether the specified DVSQ flag is set or not
        (+) Clears the DVSQ flags
        (+) Enables or disables the specified DVSQ interrupts
@endverbatim
  * @{
  */

/**
  * @brief  Checks whether the specified DVSQ flag is set or not.
  * @param  DVSQ_FLAG: specifies the DVSQ flag to check.
  *         This parameter can be one of the following values:
  *            @arg DVSQ_FLAG_BUSY: DVSQ Busy flag.
  *            @arg DVSQ_FLAG_DIV:  DVSQ Divide operation flag.
  *            @arg DVSQ_FLAG_SQRT: DVSQ Square root operation flag.
  *            @arg DVSQ_FLAG_OV:   DVSQ Overflow flag.
  *            @arg DVSQ_FLAG_DZ:   DVSQ Zero divisor flag.
  * @retval The new state of DVSQ_FLAG (SET or RESET).
  */
FlagStatus DVSQ_GetFlagStatus(uint32_t DVSQ_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_DVSQ_GET_FLAG(DVSQ_FLAG));

    /* Check the status of the specified DVSQ flag */
    if ((DVSQ->CSR & DVSQ_FLAG) != (uint8_t)RESET)
    {
        /* DVSQ_FLAG is set */
        bitstatus = SET;
    }
    else
    {
        /* DVSQ_FLAG is reset */
        bitstatus = RESET;
    }

    /* Return the DVSQ_FLAG status */
    return  bitstatus;
}

/**
  * @brief  Clears the DVSQ flags.
  * @param  DVSQ_FLAG: specifies the flag to clear.
  *         This parameter can be one of the following values:
  *            @arg DVSQ_FLAG_OV: DVSQ Overflow flag.
  *            @arg DVSQ_FLAG_DZ: DVSQ Zero divisor flag.
  * @retval None
  */
void DVSQ_ClearFlag(uint32_t DVSQ_FLAG)
{
    /* Check the parameters */
    assert_param(IS_DVSQ_CLEAR_FLAG(DVSQ_FLAG));
    /* Clear the selected DVSQ flags */
    DVSQ->CSR &= (~(uint32_t)DVSQ_FLAG);
}

/**
  * @brief  Enables or disables the specified DVSQ interrupts.
  * @param  DVSQ_IT: specifies the DVSQ interrupt source to be enabled or disabled.
  *         This parameter can be one of the following values:
  *            @arg DVSQ_IT_OV: DVSQ Overflow interrupt
  *            @arg DVSQ_IT_DZ: DVSQ Zero divisor interrupt
  * @param  NewState: new state of the specified DVSQ interrupt.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DVSQ_ITConfig(uint32_t DVSQ_IT, FunctionalState NewState)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_DVSQ_IT(DVSQ_IT));
    /* Read the CSR Register */
    tmpreg = DVSQ->CSR;

    if (NewState != DISABLE)
    {
        /* Enable the selected DVSQ interrupt */
        tmpreg |= (uint32_t)(DVSQ_IT);
    }
    else
    {
        /* Disable the selected QSPI interrupt */
        tmpreg &= (~(uint32_t)DVSQ_IT);
    }

    /* Write the CSR Register */
    DVSQ->CSR = tmpreg;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
