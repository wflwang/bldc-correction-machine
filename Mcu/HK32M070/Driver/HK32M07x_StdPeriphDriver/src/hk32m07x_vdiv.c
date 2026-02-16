/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_vdiv.c
* @brief:     Voltage divider init and functions configure.
* @author:    AE Team
* @version:   V1.0.0/2024-12-20
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_vdiv.h"


/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup VDIV VDIV
  * @brief VDIV driver modules
  * @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup VDIV_Private_Functions VDIV Private Functions
  * @{
  */

/**
  * @brief  Deinitializes the VDIV peripheral registers to their default reset values.
  * @retval None
  */
void VDIV_DeInit(void)
{
    /* Enable VDIV reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_VDIV, ENABLE);
    /* Release VDIV from reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_VDIV, DISABLE);
}

/**
  * @brief  Set the DVID data output register.
  * @param  Data : Data to be loaded in the selected data holding register.
  * @retval None
  */
void VDIV_SetChannelData(uint16_t Data)
{
    /* Set the DVID data output register */
    VDIV->DOR = Data;
}

/**
  * @brief  Enables or disables the specified VDIV.
  * @param  NewState: new state of the VDIV.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void VDIV_Cmd(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        /* Enable the selected VDIV */
        VDIV->CSR |= VDIV_CSR_EN;
    }
    else
    {
        /* Disable the selected VDIV */
        VDIV->CSR &= ~VDIV_CSR_EN;
    }
}

/**
  * @brief  Enables or disables the VDIV write protection.
  * @param  NewState: new state of the VDIV.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void VDIV_RemoveWriteProtection(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        /* Remove write protection */
        VDIV->WPTR = 0xA5A5;
    }
    else
    {
        /* Enable write protection */
        VDIV->WPTR = 0x0000;
    }
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
