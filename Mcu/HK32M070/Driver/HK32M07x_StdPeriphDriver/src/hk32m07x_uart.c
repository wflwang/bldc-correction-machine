/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_uart.c
* @brief:     This file provides all the UART firmware functions.
* @author:    AE Team
* @version:   V1.0.0/2024-12-23
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_uart.h"

/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup UART UART
  * @brief UART driver modules
  * @{
*/

/** @defgroup UART_Private_Defines UART_Private_Defines
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*!< UART CR1 register clear Mask */
#define CR1_CLEAR_MASK            ((uint32_t)(UART_CR1_M0 | UART_CR1_PCE | UART_CR1_PS \
                                  | UART_CR1_TE | UART_CR1_RE))
/*!< UART Position of the corresponding Interrupt mask */
#define IT_MASK                   ((uint32_t)0x000000FF)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @}
  */

/** @defgroup UART_Private_Functions UART_Private_Functions
  * @{
  */

/** @defgroup UART_Group1 Initialization And Configuration Functions
 *  @brief   Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
          ##### Initialization and Configuration functions #####
 ===============================================================================
     [..] This section provides functions allowing to:
        (+) Deinitializes the UARTx peripheral registers to their default reset values
        (+) Initializes the UARTx peripheral
        (+) Fills each UART_InitStruct member with its default value
        (+) Enables or disables the specified UART peripheral
        (+) Enables or disables the UART's transmitter or receiver
        (+) Enables or disables the UART's 8x oversampling mode
        (+) Enables or disables the UART's one bit sampling method
        (+) Enables or disables the UART's most significant bit first
        (+) Enables or disables the binary data inversion
        (+) Enables or disables the Pin(s) active level inversion
        (+) Enables or disables the swap Tx/Rx pins
        (+) Enables or disables the UART's Overrun detection
        (+) Enables the specified UART's Request
        (+) Configure noise filtering level
        (+) Enables or disables the resynchronization
        (+) Enables or disables the half duplex
@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the UARTx peripheral registers to their default reset values.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @retval None
  */
void UART_DeInit(UART_TypeDef* UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));

    if (UARTx == UART1)
    {
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART1, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART1, DISABLE);
    }
    else if (UARTx == UART2)
    {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_UART2, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_UART2, DISABLE);
    }
}

/**
  * @brief  Initializes the UARTx peripheral according to the specified
  *         parameters in the UART_InitStruct .
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure that contains
  *         the configuration information for the specified UART peripheral.
  * @retval None
  */
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct)
{
    uint32_t divider = 0, apbclock = 0, tmpreg = 0;
    RCC_ClocksTypeDef RCC_ClocksStatus;
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_BAUDRATE(UART_InitStruct->UART_BaudRate));
    assert_param(IS_UART_WORD_LENGTH(UART_InitStruct->UART_WordLength));
    assert_param(IS_UART_STOPBITS(UART_InitStruct->UART_StopBits));
    assert_param(IS_UART_PARITY(UART_InitStruct->UART_Parity));
    assert_param(IS_UART_MODE(UART_InitStruct->UART_Mode));
    assert_param(IS_FUNCTIONAL_STATE(UART_InitStruct->UART_RESYN));
    /* Disable UART */
    UARTx->CR1 &= (uint32_t)~((uint32_t)UART_CR1_UE);  
    /*---------------------------- UART NFCR Configuration -----------------------*/
    if (UART_InitStruct->UART_RESYN != DISABLE)
    {
        /* Enable the Resynchronization */
        UARTx->NFCR |= UART_NFCR_RESYN;
    }
    else
    {
        /* Disable the Resynchronization */
        UARTx->NFCR &= (uint32_t)~((uint32_t)UART_NFCR_RESYN);
    }
    /*---------------------------- UART CR2 Configuration -----------------------*/
    tmpreg = UARTx->CR2;
    /* Clear STOP[13:12] bits */
    tmpreg &= (uint32_t)~((uint32_t)UART_CR2_STOP);
    /* Configure the UART Stop Bits, Clock, CPOL, CPHA and LastBit ------------*/
    /* Set STOP[13:12] bits according to UART_StopBits value */
    tmpreg |= (uint32_t)UART_InitStruct->UART_StopBits;
    /* Write to UART CR2 */
    UARTx->CR2 = tmpreg;
    /*---------------------------- UART CR1 Configuration -----------------------*/
    tmpreg = UARTx->CR1;
    /* Clear M0, PCE, PS, TE and RE bits */
    tmpreg &= (uint32_t)~((uint32_t)CR1_CLEAR_MASK);
    /* Configure the UART Word Length, Parity and mode ----------------------- */
    /* Set the M0 bits according to UART_WordLength value */
    /* Set PCE and PS bits according to UART_Parity value */
    /* Set TE and RE bits according to UART_Mode value    */
    tmpreg |= (uint32_t)UART_InitStruct->UART_WordLength | UART_InitStruct->UART_Parity |
              UART_InitStruct->UART_Mode;
    /* Write to UART CR1 */
    UARTx->CR1 = tmpreg;
    /*---------------------------- UART BRR Configuration -----------------------*/
    /* Configure the UART Baud Rate -------------------------------------------*/
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    apbclock = RCC_ClocksStatus.PCLK_Frequency;

    /* Determine the integer part */
    if ((UARTx->CR1 & UART_CR1_OVER8) != 0)
    {
        /* (divider * 10) computing in case Oversampling mode is 8 Samples */
        divider = (uint32_t)((2 * apbclock) / (UART_InitStruct->UART_BaudRate));
        tmpreg  = (uint32_t)((2 * apbclock) % (UART_InitStruct->UART_BaudRate));
    }
    else /* if ((UARTx->CR1 & CR1_OVER8_Set) == 0) */
    {
        /* (divider * 10) computing in case Oversampling mode is 16 Samples */
        divider = (uint32_t)((apbclock) / (UART_InitStruct->UART_BaudRate));
        tmpreg  = (uint32_t)((apbclock) % (UART_InitStruct->UART_BaudRate));
    }

    /* round the divider : if fractional part i greater than 0.5 increment divider */
    if (tmpreg >=  (UART_InitStruct->UART_BaudRate) / 2)
    {
        divider++;
    }

    /* Implement the divider in case Oversampling mode is 8 Samples */
    if ((UARTx->CR1 & UART_CR1_OVER8) != 0)
    {
        /* get the LSB of divider and shift it to the right by 1 bit */
        tmpreg = (divider & (uint16_t)0x000F) >> 1;
        /* update the divider value */
        divider = (divider & (uint16_t)0xFFF0) | tmpreg;
    }

    /* Write to UART BRR */
    UARTx->BRR = (uint16_t)divider;
}

/**
  * @brief  Fills each UART_InitStruct member with its default value.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
    /* UART_InitStruct members default value */
    UART_InitStruct->UART_BaudRate = 9600;
    UART_InitStruct->UART_WordLength = UART_WordLength_8b;
    UART_InitStruct->UART_StopBits = UART_StopBits_1;
    UART_InitStruct->UART_Parity = UART_Parity_No ;
    UART_InitStruct->UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_InitStruct->UART_RESYN = ENABLE;
}

/**
  * @brief  Enables or disables the specified UART peripheral.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new state of the UARTx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_Cmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected UART by setting the UE bit in the CR1 register */
        UARTx->CR1 |= UART_CR1_UE;
    }
    else
    {
        /* Disable the selected UART by clearing the UE bit in the CR1 register */
        UARTx->CR1 &= (uint32_t)~((uint32_t)UART_CR1_UE);
    }
}

/**
  * @brief  Enables or disables the UART's transmitter or receiver.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_DirectionMode: specifies the UART direction.
  *         This parameter can be any combination of the following values:
  *            @arg UART_Mode_Tx: UART Transmitter
  *            @arg UART_Mode_Rx: UART Receiver
  * @param  NewState: new state of the UART transfer direction.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_DirectionModeCmd(UART_TypeDef* UARTx, uint32_t UART_DirectionMode, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_MODE(UART_DirectionMode));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the UART's transfer interface by setting the TE and/or RE bits
           in the UART CR1 register */
        UARTx->CR1 |= UART_DirectionMode;
    }
    else
    {
        /* Disable the UART's transfer interface by clearing the TE and/or RE bits
           in the UART CR3 register */
        UARTx->CR1 &= (uint32_t)~UART_DirectionMode;
    }
}

/**
  * @brief  Enables or disables the UART's 8x oversampling mode.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new state of the UART 8x oversampling mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @note   This function has to be called before calling UART_Init() function
  *         in order to have correct baudrate Divider value.
  * @retval None
  */
void UART_OverSampling8Cmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the 8x Oversampling mode by setting the OVER8 bit in the CR1 register */
        UARTx->CR1 |= UART_CR1_OVER8;
    }
    else
    {
        /* Disable the 8x Oversampling mode by clearing the OVER8 bit in the CR1 register */
        UARTx->CR1 &= (uint32_t)~((uint32_t)UART_CR1_OVER8);
    }
}

/**
  * @brief  Enables or disables the UART's one bit sampling method.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new state of the UART one bit sampling method.
  *         This parameter can be: ENABLE or DISABLE.
  * @note   This function has to be called before calling UART_Cmd() function.
  * @retval None
  */
void UART_OneBitMethodCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the one bit method by setting the ONEBITE bit in the CR3 register */
        UARTx->CR3 |= UART_CR3_ONEBIT;
    }
    else
    {
        /* Disable the one bit method by clearing the ONEBITE bit in the CR3 register */
        UARTx->CR3 &= (uint32_t)~((uint32_t)UART_CR3_ONEBIT);
    }
}

/**
  * @brief  Enables or disables the UART's most significant bit first
  *         transmitted/received following the start bit.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new state of the UART most significant bit first
  *         transmitted/received following the start bit.
  *         This parameter can be: ENABLE or DISABLE.
  * @note   This function has to be called before calling UART_Cmd() function.
  * @retval None
  */
void UART_MSBFirstCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the most significant bit first transmitted/received following the
           start bit by setting the MSBFIRST bit in the CR2 register */
        UARTx->CR2 |= UART_CR2_MSBFIRST;
    }
    else
    {
        /* Disable the most significant bit first transmitted/received following the
           start bit by clearing the MSBFIRST bit in the CR2 register */
        UARTx->CR2 &= (uint32_t)~((uint32_t)UART_CR2_MSBFIRST);
    }
}

/**
  * @brief  Enables or disables the binary data inversion.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new defined levels for the UART data.
  *         This parameter can be:
  *            @arg ENABLE:  Logical data from the data register are send/received in negative
  *                 logic (1=L, 0=H). The parity bit is also inverted.
  *            @arg DISABLE: Logical data from the data register are send/received in positive
  *                 logic (1=H, 0=L)
  * @note   This function has to be called before calling UART_Cmd() function.
  * @retval None
  */
void UART_DataInvCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the binary data inversion feature by setting the DATAINV bit in
           the CR2 register */
        UARTx->CR2 |= UART_CR2_DATAINV;
    }
    else
    {
        /* Disable the binary data inversion feature by clearing the DATAINV bit in
           the CR2 register */
        UARTx->CR2 &= (uint32_t)~((uint32_t)UART_CR2_DATAINV);
    }
}

/**
  * @brief  Enables or disables the Pin(s) active level inversion.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_InvPin: specifies the UART pin(s) to invert.
  *         This parameter can be any combination of the following values:
  *            @arg UART_InvPin_Tx: UART Tx pin active level inversion.
  *            @arg UART_InvPin_Rx: UART Rx pin active level inversion.
  * @param  NewState: new active level status for the UART pin(s).
  *         This parameter can be:
  *            @arg ENABLE: pin(s) signal values are inverted (Vdd =0, Gnd =1).
  *            @arg DISABLE: pin(s) signal works using the standard logic levels (Vdd =1, Gnd =0).
  * @note   This function has to be called before calling UART_Cmd() function.
  * @retval None
  */
void UART_InvPinCmd(UART_TypeDef* UARTx, uint32_t UART_InvPin, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_INVERSTION_PIN(UART_InvPin));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the active level inversion for selected pins by setting the TXINV
           and/or RXINV bits in the UART CR2 register */
        UARTx->CR2 |= UART_InvPin;
    }
    else
    {
        /* Disable the active level inversion for selected requests by clearing the
           TXINV and/or RXINV bits in the UART CR2 register */
        UARTx->CR2 &= (uint32_t)~UART_InvPin;
    }
}

/**
  * @brief  Enables or disables the swap Tx/Rx pins.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new state of the UARTx TX/RX pins pinout.
  *         This parameter can be:
  *            @arg ENABLE: The TX and RX pins functions are swapped.
  *            @arg DISABLE: TX/RX pins are used as defined in standard pinout
  * @note   This function has to be called before calling UART_Cmd() function.
  * @retval None
  */
void UART_SWAPPinCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the SWAP feature by setting the SWAP bit in the CR2 register */
        UARTx->CR2 |= UART_CR2_SWAP;
    }
    else
    {
        /* Disable the SWAP feature by clearing the SWAP bit in the CR2 register */
        UARTx->CR2 &= (uint32_t)~((uint32_t)UART_CR2_SWAP);
    }
}

/**
  * @brief  Enables the specified UART's Request.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_Request: specifies the UART request.
  *         This parameter can be any combination of the following values:
  *            @arg UART_Request_RXFRQ: Receive data flush ReQuest
  *            @arg UART_Request_MMRQ:  Mute Mode ReQuest
  *            @arg UART_Request_SBKRQ: Send Break ReQuest
  * @param  NewState: new state of the UARTx's Request
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_RequestCmd(UART_TypeDef* UARTx, uint32_t UART_Request, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_REQUEST(UART_Request));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the UART ReQuest by setting the dedicated request bit in the RQR
           register.*/
        UARTx->RQR |= UART_Request;
    }
    else
    {
        /* Disable the UART ReQuest by clearing the dedicated request bit in the RQR
           register.*/
        UARTx->RQR &= (uint32_t)~UART_Request;
    }
}

/**
  * @brief  Enables or disables the UART's Overrun detection.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_OVRDetection: specifies the OVR detection status in case of OVR error.
  *         This parameter can be any combination of the following values:
  *            @arg UART_OVRDetection_Enable: OVR error detection enabled when
  *                 the UART OVR error is asserted.
  *            @arg UART_OVRDetection_Disable: OVR error detection disabled when
  *                 the UART OVR error is asserted.
  * @retval None
  */
void UART_OverrunDetectionConfig(UART_TypeDef* UARTx, uint32_t UART_OVRDetection)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_OVRDETECTION(UART_OVRDetection));
    /* Clear the OVR detection bit */
    UARTx->CR3 &= (uint32_t)~((uint32_t)UART_CR3_OVRDIS);
    /* Set the new value for the OVR detection bit */
    UARTx->CR3 |= UART_OVRDetection;
}

/**
  * @brief  Configure noise filtering level.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NFLevel: Noise filtering level, Can be configured as 0~7.
  *         If configured to 0, Disable noise filtering
  * @note   This function has to be called before calling UART_Cmd() function.
  * @retval None
  */
void UART_NFConfig(UART_TypeDef* UARTx, uint8_t NFLevel)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_NF_LEVEL(NFLevel));
    /* Read the NFCR Register */
    tmpreg = UARTx->NFCR;
    /* Clear the noise filter bit */
    tmpreg &= (~(uint32_t)UART_NFCR_NFCR_Mask);
    /* Write the noise filter bit */
    tmpreg |= NFLevel;
    /* Write the NFCR Register */
    UARTx->NFCR = tmpreg;
}

/**
  * @brief  Enables or disables the resynchronization.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new state of the UARTx Resynchronizations.
  *         This parameter can be: ENABLE or DISABLE.
  * @note   This function has to be called before calling UART_Cmd() function.
  * @retval None
  */
void UART_RESYNCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Resynchronization */
        UARTx->NFCR |= UART_NFCR_RESYN;
    }
    else
    {
        /* Disable the Resynchronization */
        UARTx->NFCR &= (uint32_t)~((uint32_t)UART_NFCR_RESYN);
    }
}

/**
  * @brief  Enables or disables the half duplex.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new state of the UARTx half duplex.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_HDUPECmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the half duplex */
        UARTx->CR1 |= UART_CR1_HDUPE;
    }
    else
    {
        /* Disable the half duplex */
        UARTx->CR1 &= (uint32_t)~((uint32_t)UART_CR1_HDUPE);
    }
}

/**
  * @}
  */

/** @defgroup UART_Group2 Data Transfers Functions
 *  @brief   Data transfers functions
 *
@verbatim
 ===============================================================================
                    ##### Data transfers functions #####
 ===============================================================================
      [..] This section provides functions allowing to:
         (+) Transmits single data through the UARTx peripheral
         (+) Returns the most recent received data by the UARTx peripheral

@endverbatim
  * @{
  */

/**
  * @brief  Transmits single data through the UARTx peripheral.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void UART_SendData(UART_TypeDef* UARTx, uint16_t Data)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_DATA(Data));
    /* Transmit Data */
    UARTx->TDR = (Data & (uint16_t)0x01FF);
}

/**
  * @brief  Returns the most recent received data by the UARTx peripheral.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @retval The received data.
  */
uint16_t UART_ReceiveData(UART_TypeDef* UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    /* Receive Data */
    return (uint16_t)(UARTx->RDR & (uint16_t)0x01FF);
}

/**
  * @}
  */

/** @defgroup UART_Group3 MultiProcessor Communication Functions
 *  @brief   MultiProcessor Communication functions
 *
@verbatim
 ===============================================================================
             ##### MultiProcessor Communication functions #####
 ===============================================================================
      [..] This section provides functions allowing to:
         (+) Sets the address of the UART node
         (+) Enables or disables the UART's mute mode
         (+) Selects the UART WakeUp method from mute mode
         (+) Configure the the UART Address detection length

@endverbatim
  * @{
  */

/**
  * @brief  Sets the address of the UART node.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_Address: Indicates the address of the UART node.
  * @retval None
  */
void UART_SetAddress(UART_TypeDef* UARTx, uint8_t UART_Address)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    /* Clear the UART address */
    UARTx->CR2 &= (uint32_t)~((uint32_t)UART_CR2_ADD);
    /* Set the UART address node */
    UARTx->CR2 |= ((uint32_t)UART_Address << (uint32_t)0x18);
}

/**
  * @brief  Enables or disables the UART's mute mode.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new state of the UART mute mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_MuteModeCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the UART mute mode by setting the MME bit in the CR1 register */
        UARTx->CR1 |= UART_CR1_MME;
    }
    else
    {
        /* Disable the UART mute mode by clearing the MME bit in the CR1 register */
        UARTx->CR1 &= (uint32_t)~((uint32_t)UART_CR1_MME);
    }
}

/**
  * @brief  Selects the UART WakeUp method from mute mode.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_WakeUp: specifies the UART wakeup method.
  *         This parameter can be one of the following values:
  *            @arg UART_WakeUp_IdleLine: WakeUp by an idle line detection
  *            @arg UART_WakeUp_AddressMark: WakeUp by an address mark
  * @retval None
  */
void UART_MuteModeWakeUpConfig(UART_TypeDef* UARTx, uint32_t UART_WakeUp)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_MUTEMODE_WAKEUP(UART_WakeUp));
    UARTx->CR1 &= (uint32_t)~((uint32_t)UART_CR1_WAKE);
    UARTx->CR1 |= UART_WakeUp;
}

/**
  * @brief  Configure the the UART Address detection length.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_AddressLength: specifies the UART address length detection.
  *         This parameter can be one of the following values:
  *            @arg UART_AddressLength_4b: 4-bit address length detection
  *            @arg UART_AddressLength_7b: 7-bit address length detection
  * @retval None
  */
void UART_AddressDetectionConfig(UART_TypeDef* UARTx, uint32_t UART_AddressLength)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_ADDRESS_DETECTION(UART_AddressLength));
    UARTx->CR2 &= (uint32_t)~((uint32_t)UART_CR2_ADDM7);
    UARTx->CR2 |= UART_AddressLength;
}

/**
  * @}
  */

/** @defgroup UART_Group4 Half-duplex Mode Functions
 *  @brief   Half-duplex mode functions
 *
@verbatim
 ===============================================================================
                   ##### Half-duplex mode function #####
 ===============================================================================
      [..] This section provides functions allowing to:
         (+) Enables or disables the UART's Half Duplex communication

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the UART's Half Duplex communication.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new state of the UART Communication.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_HalfDuplexCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Half-Duplex mode by setting the HDSEL bit in the CR3 register */
        UARTx->CR3 |= UART_CR3_HDSEL;
    }
    else
    {
        /* Disable the Half-Duplex mode by clearing the HDSEL bit in the CR3 register */
        UARTx->CR3 &= (uint32_t)~((uint32_t)UART_CR3_HDSEL);
    }
}

/**
  * @}
  */


/** @defgroup UART_Group5 LIN Mode Functions
 *  @brief   LIN mode functions
 *
@verbatim
 ===============================================================================
                       ##### LIN mode functions #####
 ===============================================================================
      [..] This section provides functions allowing to:
         (+) Sets the UART LIN Break detection length
         (+) Enables or disables the UART's LIN mode
         (+) Transmits break characters

@endverbatim
  * @{
  */

/**
  * @brief  Sets the UART LIN Break detection length.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_LINBreakDetectLength: specifies the LIN break detection length.
  *         This parameter can be one of the following values:
  *            @arg UART_LINBreakDetectLength_10b: 10-bit break detection
  *            @arg UART_LINBreakDetectLength_11b: 11-bit break detection
  * @retval None
  */
void UART_LINBreakDetectLengthConfig(UART_TypeDef* UARTx, uint32_t UART_LINBreakDetectLength)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_LIN_BREAK_DETECT_LENGTH(UART_LINBreakDetectLength));
    UARTx->CR2 &= (uint32_t)~((uint32_t)UART_CR2_LBDL);
    UARTx->CR2 |= UART_LINBreakDetectLength;
}

/**
  * @brief  Enables or disables the UART's LIN mode.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  NewState: new state of the UART LIN mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_LINCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the LIN mode by setting the LINEN bit in the CR2 register */
        UARTx->CR2 |= UART_CR2_LINEN;
    }
    else
    {
        /* Disable the LIN mode by clearing the LINEN bit in the CR2 register */
        UARTx->CR2 &= (uint32_t)~((uint32_t)UART_CR2_LINEN);
    }
}


/**
  * @brief  Transmits break characters.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @retval None
  */
void UART_SendBreak(UART_TypeDef* UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    /* Send break characters */
    UARTx->RQR |= UART_RQR_SBKRQ;
}

/**
  * @}
  */

/** @defgroup UART_Group6 Interrupts And Flags Management Functions
 *  @brief   Interrupts and flags management functions
 *
@verbatim
 ===============================================================================
            ##### Interrupts and flags management functions #####
 ===============================================================================
      [..] This section provides functions allowing to:
         (+) Enables or disables the specified UART interrupts
         (+) Checks whether the specified UART interrupt has occurred or not
         (+) Clears the UARTx's interrupt pending bits
         (+) Checks whether the specified UART flag is set or not
         (+) Clears the UARTx's pending flags

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the specified UART interrupts.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_IT: specifies the UART interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  *            @arg UART_IT_CM:   Character match interrupt.
  *            @arg UART_IT_LBD:  LIN break detection interrupt.
  *            @arg UART_IT_TXE:  Tansmit Data Register empty interrupt.
  *            @arg UART_IT_TC:   Transmission complete interrupt.
  *            @arg UART_IT_RXNE: Receive Data register not empty interrupt.
  *            @arg UART_IT_IDLE: Idle line detection interrupt.
  *            @arg UART_IT_PE:   Parity Error interrupt.
  *            @arg UART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @param  NewState: new state of the specified UARTx interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_ITConfig(UART_TypeDef* UARTx, uint32_t UART_IT, FunctionalState NewState)
{
    uint32_t uartreg = 0, itpos = 0, itmask = 0;
    uint32_t uartxbase = 0;
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_CONFIG_IT(UART_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    uartxbase = (uint32_t)UARTx;
    /* Get the UART register index */
    uartreg = (((uint16_t)UART_IT) >> 0x08);
    /* Get the interrupt position */
    itpos = UART_IT & IT_MASK;
    itmask = (((uint32_t)0x01) << itpos);

    if (uartreg == 0x02) /* The IT is in CR2 register */
    {
        uartxbase += 0x04;
    }
    else if (uartreg == 0x03) /* The IT is in CR3 register */
    {
        uartxbase += 0x08;
    }
    else /* The IT is in CR1 register */
    {
    }

    if (NewState != DISABLE)
    {
        *(__IO uint32_t*)uartxbase  |= itmask;
    }
    else
    {
        *(__IO uint32_t*)uartxbase &= ~itmask;
    }
}

/**
  * @brief  Checks whether the specified UART flag is set or not.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_FLAG: specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg UART_FLAG_REACK: Receive Enable acknowledge flag.
  *            @arg UART_FLAG_TEACK: Transmit Enable acknowledge flag.
  *            @arg UART_FLAG_RWU:   Receive Wake up flag
  *            @arg UART_FLAG_SBK:   Send Break flag.
  *            @arg UART_FLAG_CM:    Character match flag.
  *            @arg UART_FLAG_BUSY:  Busy flag.
  *            @arg UART_FLAG_LBDF:  LIN break detection flag.
  *            @arg UART_FLAG_TXE:   Transmit data register empty flag.
  *            @arg UART_FLAG_TC:    Transmission Complete flag.
  *            @arg UART_FLAG_RXNE:  Receive data register not empty flag.
  *            @arg UART_FLAG_IDLE:  Idle Line detection flag.
  *            @arg UART_FLAG_ORE:   OverRun Error flag.
  *            @arg UART_FLAG_NE:    Noise Error flag.
  *            @arg UART_FLAG_FE:    Framing Error flag.
  *            @arg UART_FLAG_PE:    Parity Error flag.
  * @retval The new state of UART_FLAG (SET or RESET).
  */
FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint32_t UART_FLAG)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_FLAG(UART_FLAG));
    FlagStatus bitstatus = RESET;

    if ((UARTx->ISR & UART_FLAG) != (uint16_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
  * @brief  Clears the UARTx's pending flags.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_FLAG: specifies the flag to clear.
  *         This parameter can be any combination of the following values:
  *            @arg UART_FLAG_CM:   Character match flag.
  *            @arg UART_FLAG_LBDF: LIN break detection flag.
  *            @arg UART_FLAG_TC:   Transmission Complete flag.
  *            @arg UART_FLAG_IDLE: IDLE line detected flag.
  *            @arg UART_FLAG_ORE:  OverRun Error flag.
  *            @arg UART_FLAG_NE:   Noise Error flag.
  *            @arg UART_FLAG_FE:   Framing Error flag.
  *            @arg UART_FLAG_PE:   Parity Errorflag.
  * @note   RXNE pending bit is cleared by a read to the UART_RDR register
  *         (UART_ReceiveData()) or by writing 1 to the RXFRQ in the register
  *         UART_RQR (UART_RequestCmd()).
  * @note   TC flag can be also cleared by software sequence: a read operation
  *         to UART_SR register (UART_GetFlagStatus()) followed by a write
  *         operation to UART_TDR register (UART_SendData()).
  * @note   TXE flag is cleared by a write to the UART_TDR register (UART_SendData())
  *         or by writing 1 to the TXFRQ in the register UART_RQR (UART_RequestCmd()).
  * @note   SBKF flag is cleared by 1 to the SBKRQ in the register UART_RQR
  *         (UART_RequestCmd()).
  * @retval None
  */
void UART_ClearFlag(UART_TypeDef* UARTx, uint32_t UART_FLAG)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_CLEAR_FLAG(UART_FLAG));
    UARTx->ICR = UART_FLAG;
}

/**
  * @brief  Checks whether the specified UART interrupt has occurred or not.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_IT: specifies the UART interrupt source to check.
  *         This parameter can be one of the following values:
  *            @arg UART_IT_CM:   Character match interrupt.
  *            @arg UART_IT_LBD:  LIN Break detection interrupt
  *            @arg UART_IT_TXE:  Tansmit Data Register empty interrupt.
  *            @arg UART_IT_TC:   Transmission complete interrupt.
  *            @arg UART_IT_RXNE: Receive Data register not empty interrupt.
  *            @arg UART_IT_IDLE: Idle line detection interrupt.
  *            @arg UART_IT_ORE:  OverRun Error interrupt.
  *            @arg UART_IT_NE:   Noise Error interrupt.
  *            @arg UART_IT_FE:   Framing Error interrupt.
  *            @arg UART_IT_PE:   Parity Error interrupt.
  * @retval The new state of UART_IT (SET or RESET).
  */
ITStatus UART_GetITStatus(UART_TypeDef* UARTx, uint32_t UART_IT)
{
    uint32_t bitpos = 0, itmask = 0, uartreg = 0;
    ITStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_GET_IT(UART_IT));
    /* Get the UART register index */
    uartreg = (((uint16_t)UART_IT) >> 0x08);
    /* Get the interrupt position */
    itmask = UART_IT & IT_MASK;
    itmask = (uint32_t)0x01 << itmask;

    if (uartreg == 0x01) /* The IT  is in CR1 register */
    {
        itmask &= UARTx->CR1;
    }
    else if (uartreg == 0x02) /* The IT  is in CR2 register */
    {
        itmask &= UARTx->CR2;
    }
    else /* The IT  is in CR3 register */
    {
        itmask &= UARTx->CR3;
    }

    bitpos = UART_IT >> 0x10;
    bitpos = (uint32_t)0x01 << bitpos;
    bitpos &= UARTx->ISR;

    if ((itmask != (uint16_t)RESET) && (bitpos != (uint16_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
  * @brief  Clears the UARTx's interrupt pending bits.
  * @param  UARTx: where x can be from 1 to 2 to select the UART peripheral.
  * @param  UART_IT: specifies the interrupt pending bit to clear.
  *         This parameter can be one of the following values:
  *            @arg UART_IT_CM:   Character match interrupt.
  *            @arg UART_IT_LBD:  LIN break detection interrupt.
  *            @arg UART_IT_TC:   Transmission complete interrupt.
  *            @arg UART_IT_IDLE: IDLE line detected interrupt.
  *            @arg UART_IT_ORE:  OverRun Error interrupt.
  *            @arg UART_IT_NE:   Noise Error interrupt.
  *            @arg UART_IT_FE:   Framing Error interrupt.
  *            @arg UART_IT_PE:   Parity Error interrupt.
  * @note   RXNE pending bit is cleared by a read to the UART_RDR register
  *         (UART_ReceiveData()) or by writing 1 to the RXFRQ in the register
  *         UART_RQR (UART_RequestCmd()).
  * @note   TC pending bit can be also cleared by software sequence: a read
  *         operation to UART_SR register (UART_GetITStatus()) followed by
  *         a write operation to UART_TDR register (UART_SendData()).
  * @note   TXE pending bit is cleared by a write to the UART_TDR register
  *         (UART_SendData()) or by writing 1 to the TXFRQ in the register
  *         UART_RQR (UART_RequestCmd()).
  * @retval None
  */
void UART_ClearITPendingBit(UART_TypeDef* UARTx, uint32_t UART_IT)
{
    uint32_t bitpos = 0, itmask = 0;
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_CLEAR_IT(UART_IT));
    bitpos = UART_IT >> 0x10;
    itmask = ((uint32_t)0x01 << (uint32_t)bitpos);
    UARTx->ICR = (uint32_t)itmask;
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
