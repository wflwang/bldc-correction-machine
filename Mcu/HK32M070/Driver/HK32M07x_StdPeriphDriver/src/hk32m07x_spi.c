/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_spi.c
* @brief:     SPI Initialization and Configuration functions.
* @author:    AE Team
* @version:   V1.0.0/2024-12-20
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_spi.h"
#include "hk32m07x_rcc.h"

/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup SPI SPI
  * @brief SPI driver modules
  * @{
  @verbatim
 ===============================================================================
                       ##### How to use this driver #####
 ===============================================================================
    [..]
        (#) Enable peripheral clock using RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI1, ENABLE)
            function for SPI1.

        (#) Enable SCK, MOSI, MISO and NSS GPIO clocks using
            RCC_AHBPeriphClockCmd() function.

        (#) Peripherals alternate function:
            (++) Connect the pin to the desired peripherals' Alternate
                 Function (AF) using GPIO_PinAFConfig() function.
            (++) Configure the desired pin in alternate function by:
                 GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF.
            (++) Select the type, pull-up/pull-down and output speed via
                 GPIO_PuPd, GPIO_OType and GPIO_Speed members.
            (++) Call GPIO_Init() function.

        (#) Program the Polarity, Phase, First Data, Baud Rate Prescaler, Slave
            Management, Peripheral Mode using the SPI_Init()
            function.

        (#) Configure the FIFO threshold using SPI_RxFIFOThresholdConfig() to select
            at which threshold the RXNE event is generated.

        (#) Enable the NVIC and the corresponding interrupt using the function
            SPI_ITConfig() if you need to use interrupt mode.

        (#) Enable the SPI using the SPI_Cmd() function.

        (#) Optionally, you can enable/configure the following parameters without
            re-initialization (i.e there is no need to call again SPI_Init() function):
            (++) When bidirectional mode (SPI_Direction_1Line_Rx or SPI_Direction_1Line_Tx)
                 is programmed as Data direction parameter using the SPI_Init()
                 function it can be possible to switch between SPI_Direction_Tx
                 or SPI_Direction_Rx using the SPI_BiDirectionalLineConfig() function.
            (++) When SPI_NSS_Soft is selected as Slave Select Management parameter
                 using the SPI_Init() function it can be possible to manage the
                 NSS internal signal using the SPI_NSSInternalSoftwareConfig() function.
            (++) Reconfigure the data size using the SPI_DataSizeConfig() function.
            (++) Enable or disable the SS output using the SPI_SSOutputCmd() function.

    @endverbatim
  ******************************************************************************
  */  

/** @defgroup SPI_Private_Defines SPI Private Defines
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* SPI registers Masks */
#define CR1_CLEAR_MASK       ((uint16_t)0x3040)
#define CR1_CLEAR_MASK2      ((uint16_t)0xFFFB)

/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup SPI_Private_Functions SPI Private Functions
  * @{
  */

/** @defgroup SPI_Group1 Initialization and Configuration functions
 *  @brief   Initialization and Configuration functions
 *  @{
@verbatim
 ===============================================================================
           ##### Initialization and Configuration functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to initialize the SPI Direction,
         SPI Mode, SPI Data Size, SPI Polarity, SPI Phase, SPI NSS Management, SPI Baud
         Rate Prescaler, SPI First Bit.

    [..] The SPI_Init() function follows the SPI configuration procedures for Master mode
         and Slave mode (details for these procedures are available in reference manual).

    [..] When the Software NSS management (SPI_InitStruct->SPI_NSS = SPI_NSS_Soft) is selected,
         use the following function to manage the NSS bit:
         void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft);

    [..] In Master mode, when the Hardware NSS management (SPI_InitStruct->SPI_NSS = SPI_NSS_Hard)
         is selected, use the follwoing function to enable the NSS output feature.
         void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState);

    [..] The NSS pulse mode can be managed by the SPI TI mode when enabling it using the following function:
         void SPI_TIModeCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
         And it can be managed by software in the SPI Motorola mode using this function:
         void SPI_NSSPulseModeCmd(SPI_TypeDef* SPIx, FunctionalState NewState);

@endverbatim
  */

/**
  * @brief  Deinitializes the SPI1 peripheral registers to their default
  *         reset values.
  * @param  SPIx: where x can be 1 o to select the SPI1 peripheral.
  * @retval None
  */
void SPI_DeInit(SPI_TypeDef* SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Enable SPI1 reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI1, ENABLE);
    /* Release SPI1 from reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI1, DISABLE);

}

/**
  * @brief  Fills each SPI_InitStruct member with its default value.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
    /*--------------- Reset SPI init structure parameters values -----------------*/
    /* Initialize the SPI_Direction member */
    SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    /* Initialize the SPI_Mode member */
    SPI_InitStruct->SPI_Mode = SPI_Mode_Slave;
    /* Initialize the SPI_DataSize member */
    SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;
    /* Initialize the SPI_CPOL member */
    SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;
    /* Initialize the SPI_CPHA member */
    SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;
    /* Initialize the SPI_NSS member */
    SPI_InitStruct->SPI_NSS = SPI_NSS_Hard;
    /* Initialize the SPI_BaudRatePrescaler member */
    SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    /* Initialize the SPI_FirstBit member */
    SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;
}

/**
  * @brief  Initializes the SPIx peripheral according to the specified
  *         parameters in the SPI_InitStruct.
  * @param  SPIx: where x can be 1 o to select the SPI1 peripheral.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
  *         contains the configuration information for the specified SPI peripheral.
  * @retval None
  */
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
    uint16_t tmpreg = 0;

    /* check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Check the SPI parameters */
    assert_param(IS_SPI_DIRECTION_MODE(SPI_InitStruct->SPI_Direction));
    assert_param(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
    assert_param(IS_SPI_DATA_SIZE(SPI_InitStruct->SPI_DataSize));
    assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
    assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
    assert_param(IS_SPI_NSS(SPI_InitStruct->SPI_NSS));
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
    assert_param(IS_SPI_FIRST_BIT(SPI_InitStruct->SPI_FirstBit));

    /*---------------------------- SPIx CR1 Configuration ------------------------*/
    /* Get the SPIx CR1 value */
    tmpreg = SPIx->CR1;
    /* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, CPOL and CPHA bits */
    tmpreg &= CR1_CLEAR_MASK;
    /* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
    master/slave mode, CPOL and CPHA */
    /* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value */
    /* Set SSM, SSI bit according to SPI_NSS values */
    /* Set LSBFirst bit according to SPI_FirstBit value */
    /* Set BR bits according to SPI_BaudRatePrescaler value */
    /* Set CPOL bit according to SPI_CPOL value */
    /* Set CPHA bit according to SPI_CPHA value */
    tmpreg |= (uint16_t)((uint32_t)SPI_InitStruct->SPI_Direction | SPI_InitStruct->SPI_FirstBit |
                         SPI_InitStruct->SPI_CPOL | SPI_InitStruct->SPI_CPHA |
                         SPI_InitStruct->SPI_NSS | SPI_InitStruct->SPI_BaudRatePrescaler);
    /* Write to SPIx CR1 */
    SPIx->CR1 = tmpreg;
    /*-------------------------Data Size Configuration -----------------------*/
    /* Get the SPIx CR2 value */
    tmpreg = SPIx->CR2;
    /* Clear DS[3:0] bits */
    tmpreg &= (uint16_t)~SPI_CR2_DS;
    /* Configure SPIx: Data Size */
    tmpreg |= (uint16_t)(SPI_InitStruct->SPI_DataSize);
    /* Write to SPIx CR2 */
    SPIx->CR2 = tmpreg;

    /*---------------------------- SPIx CR1 Configuration ------------------------*/
    /* Get the SPIx CR1 value */
    tmpreg = SPIx->CR1;
    /* Clear MSTR bit */
    tmpreg &= CR1_CLEAR_MASK2;
    /* Configure SPIx: master/slave mode */
    /* Set MSTR bit according to SPI_Mode */
    tmpreg |= (uint16_t)((uint32_t)SPI_InitStruct->SPI_Mode);
    /* Write to SPIx CR1 */
    SPIx->CR1 = tmpreg;

}



/**
  * @brief  Enables or disables the specified SPI peripheral.
  * @param  SPIx: where x can be 1 o to select the SPI1 peripheral.
  * @param  NewState: new state of the SPIx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected SPI peripheral */
        SPIx->CR1 |= SPI_CR1_SPE;
    }
    else
    {
        /* Disable the selected SPI peripheral */
        SPIx->CR1 &= (uint16_t)~((uint16_t)SPI_CR1_SPE);
    }
}

/**
  * @brief  Configures the data size for the selected SPI.
  * @param  SPIx: where x can be 1 o to select the SPI1 peripheral.
  * @param  SPI_DataSize: specifies the SPI data size.
  *         For the SPIx peripheral this parameter can be one of the following values:
  *            @arg SPI_DataSize_4b: Set data size to 4 bits
  *            @arg SPI_DataSize_5b: Set data size to 5 bits
  *            @arg SPI_DataSize_6b: Set data size to 6 bits
  *            @arg SPI_DataSize_7b: Set data size to 7 bits
  *            @arg SPI_DataSize_8b: Set data size to 8 bits
  *            @arg SPI_DataSize_9b: Set data size to 9 bits
  *            @arg SPI_DataSize_10b: Set data size to 10 bits
  *            @arg SPI_DataSize_11b: Set data size to 11 bits
  *            @arg SPI_DataSize_12b: Set data size to 12 bits
  *            @arg SPI_DataSize_13b: Set data size to 13 bits
  *            @arg SPI_DataSize_14b: Set data size to 14 bits
  *            @arg SPI_DataSize_15b: Set data size to 15 bits
  *            @arg SPI_DataSize_16b: Set data size to 16 bits
  * @retval None
  */
void SPI_DataSizeConfig(SPI_TypeDef* SPIx, uint16_t SPI_DataSize)
{
    uint16_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_DATA_SIZE(SPI_DataSize));
    /* Read the CR2 register */
    tmpreg = SPIx->CR2;
    /* Clear DS[3:0] bits */
    tmpreg &= (uint16_t)~SPI_CR2_DS;
    /* Set new DS[3:0] bits value */
    tmpreg |= SPI_DataSize;
    SPIx->CR2 = tmpreg;
}

/**
  * @brief  Configures the FIFO reception threshold for the selected SPI.
  * @param  SPIx: where x can be 1 o to select the SPI1 peripheral.
  * @param  SPI_RxFIFOThreshold: specifies the FIFO reception threshold.
  *         This parameter can be one of the following values:
  *            @arg SPI_RxFIFOThreshold_HF: RXNE event is generated if the FIFO
  *                 level is greater or equal to 1/2.
  *            @arg SPI_RxFIFOThreshold_QF: RXNE event is generated if the FIFO
  *                 level is greater or equal to 1/4.
  * @retval None
  */
void SPI_RxFIFOThresholdConfig(SPI_TypeDef* SPIx, uint16_t SPI_RxFIFOThreshold)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_RX_FIFO_THRESHOLD(SPI_RxFIFOThreshold));

    /* Clear FRXTH bit */
    SPIx->CR2 &= (uint16_t)~((uint16_t)SPI_CR2_FRXTH);

    /* Set new FRXTH bit value */
    SPIx->CR2 |= SPI_RxFIFOThreshold;
}

/**
  * @brief  Selects the data transfer direction in bidirectional mode for the specified SPI.
  * @param  SPIx: where x can be 1 o to select the SPI1 peripheral.
  * @param  SPI_Direction: specifies the data transfer direction in bidirectional mode.
  *         This parameter can be one of the following values:
  *            @arg SPI_Direction_Tx: Selects Tx transmission direction
  *            @arg SPI_Direction_Rx: Selects Rx receive direction
  * @retval None
  */
void SPI_BiDirectionalLineConfig(SPI_TypeDef* SPIx, uint16_t SPI_Direction)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_DIRECTION(SPI_Direction));

    if (SPI_Direction == SPI_Direction_Tx)
    {
        /* Set the Tx only mode */
        SPIx->CR1 |= SPI_Direction_Tx;
    }
    else
    {
        /* Set the Rx only mode */
        SPIx->CR1 &= SPI_Direction_Rx;
    }
}

/**
  * @brief  Configures internally by software the NSS pin for the selected SPI.
  * @note   This function can be called only after the SPI_Init() function has
  *         been called.
  * @param  SPIx: where x can be 1 o to select the SPI1 peripheral.
  * @param  SPI_NSSInternalSoft: specifies the SPI NSS internal state.
  *         This parameter can be one of the following values:
  *            @arg SPI_NSSInternalSoft_Set: Set NSS pin internally
  *            @arg SPI_NSSInternalSoft_Reset: Reset NSS pin internally
  * @retval None
  */
void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_NSS_INTERNAL(SPI_NSSInternalSoft));

    if (SPI_NSSInternalSoft != SPI_NSSInternalSoft_Reset)
    {
        /* Set NSS pin internally by software */
        SPIx->CR1 |= SPI_NSSInternalSoft_Set;
    }
    else
    {
        /* Reset NSS pin internally by software */
        SPIx->CR1 &= SPI_NSSInternalSoft_Reset;
    }
}

/**
  * @brief  Enables or disables the SS output for the selected SPI.
  * @note   This function can be called only after the SPI_Init() function has
  *         been called and the NSS hardware management mode is selected.
  * @param  SPIx: where x can be 1 o to select the SPI1 peripheral.
  * @param  NewState: new state of the SPIx SS output.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected SPI SS output */
        SPIx->CR2 |= SPI_CR2_SSOE;
    }
    else
    {
        /* Disable the selected SPI SS output */
        SPIx->CR2 &= (uint16_t)~((uint16_t)SPI_CR2_SSOE);
    }
}

/**
  * @brief  Enables or disables the NSS pulse management mode.
  * @note   This function can be called only after the SPI_Init() function has
  *         been called.
  * @note   When TI mode is selected, the control bits NSSP is not taken into
  *         consideration and are configured by hardware respectively to the
  *         TI mode requirements.
  * @param  SPIx: where x can be 1 o to select the SPI1 peripheral.
  * @param  NewState: new state of the NSS pulse management mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_NSSPulseModeCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the NSS pulse management mode */
        SPIx->CR2 |= SPI_CR2_NSSP;
    }
    else
    {
        /* Disable the NSS pulse management mode */
        SPIx->CR2 &= (uint16_t)~((uint16_t)SPI_CR2_NSSP);
    }
}

/**
  * @}
  */

/** @defgroup SPI_Group2 Data transfers functions
 *  @brief   Data transfers functions
 *
@verbatim
 ===============================================================================
                    ##### Data transfers functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to manage the SPI
         data transfers.

    [..] In reception, data are received and then stored into an internal Rx buffer while
         In transmission, data are first stored into an internal Tx buffer before being
         transmitted.

    [..] The read access of the SPI_DR register can be done using
         SPI_ReceiveData8() (when data size is equal or inferior than 8bits) and.
         SPI_ReceiveData16() (when data size is superior than 8bits)function
         and returns the Rx buffered value. Whereas a write access to the SPI_DR
         can be done using SPI_SendData8() (when data size is equal or inferior than 8bits)
         and SPI_SendData16() (when data size is superior than 8bits) function
         and stores the written data into Tx buffer.

@endverbatim
  * @{
  */

/**
  * @brief  Transmits a Data through the SPIx peripheral.
  * @param  SPIx: where x can be 1 o to select the SPI1 peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void SPI_SendData8(SPI_TypeDef* SPIx, uint8_t Data)
{
    uint32_t spixbase = 0x00;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    spixbase = (uint32_t)SPIx;
    spixbase += 0x0C;

    *(__IO uint8_t *) spixbase = Data;
}

/**
  * @brief  Transmits a Data through the SPIx peripheral.
  * @param  SPIx: where x can be 1 in SPI mode to select
  *         the SPI peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void SPI_SendData16(SPI_TypeDef* SPIx, uint16_t Data)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    SPIx->DR = (uint16_t)Data;
}

/**
  * @brief  Returns the most recent received data by the SPIx peripheral.
  * @param  SPIx: where x can be 1 in SPI mode to select the SPI peripheral.
  * @retval The value of the received data.
  */
uint8_t SPI_ReceiveData8(SPI_TypeDef* SPIx)
{
    uint32_t spixbase = 0x00;

    spixbase = (uint32_t)SPIx;
    spixbase += 0x0C;

    return *(__IO uint8_t *) spixbase;
}

/**
  * @brief  Returns the most recent received data by the SPIx peripheral.
  * @param  SPIx: where x can be 1 in SPI mode to select the SPI peripheral.
  *         the SPI peripheral.
  * @retval The value of the received data.
  */
uint16_t SPI_ReceiveData16(SPI_TypeDef* SPIx)
{
    return SPIx->DR;
}
/**
  * @}
  */

/** @defgroup SPI_Group3 Interrupts and flags management functions
 *  @brief   Interrupts and flags management functions
  *
@verbatim
 ===============================================================================
             ##### Interrupts and flags management functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to configure the SPI Interrupts
         sources and check or clear the flags or pending bits status.
         The user should identify which mode will be used in his application to manage
         the communication: Polling mode, Interrupt mode.

  *** Polling Mode ***
  ====================
    [..] In Polling Mode, the SPIcommunication can be managed by 9 flags:
        (#) SPI_FLAG_TXE : to indicate the status of the transmit buffer register
        (#) SPI_FLAG_RXNE : to indicate the status of the receive buffer register
        (#) SPI_FLAG_BSY : to indicate the state of the communication layer of the SPI.
        (#) SPI_FLAG_MODF : to indicate if a Mode Fault error occur
        (#) SPI_FLAG_OVR : to indicate if an Overrun error occur
        (#) SPI_FLAG_FRE: to indicate a Frame Format error occurs.

    [..]
        (@)Do not use the BSY flag to handle each data transmission or reception. It is better
           to use the TXE and RXNE flags instead.

    [..] In this Mode it is advised to use the following functions:
        (+) FlagStatus SPI_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_FLAG);
        (+) void SPI_ClearFlag(SPI_TypeDef* SPIx, uint16_t SPI_FLAG);

  *** Interrupt Mode ***
  ======================
    [..] In Interrupt Mode, the SPI communication can be managed by 3 interrupt sources
         and 5 pending bits:
    [..] Pending Bits:
        (#) SPI_IT_TXE : to indicate the status of the transmit buffer register
        (#) SPI_IT_RXNE : to indicate the status of the receive buffer register
        (#) SPI_IT_OVR : to indicate if an Overrun error occur
        (#) SPI_FLAG_FRE : to indicate a Frame Format error occurs.

    [..] Interrupt Source:
        (#) SPI_IT_TXE: specifies the interrupt source for the Tx buffer empty
            interrupt.
        (#) SPI_IT_RXNE : specifies the interrupt source for the Rx buffer not
            empty interrupt.
        (#) SPI_IT_ERR : specifies the interrupt source for the errors interrupt.

    [..] In this Mode it is advised to use the following functions:
         (+) void SPI_ITConfig(SPI_TypeDef* SPIx, uint8_t SPI_IT, FunctionalState NewState);
         (+) ITStatus SPI_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_IT);

  *** FIFO Status ***
  ===================
    [..] It is possible to monitor the FIFO status when a transfer is ongoing using the
         following function:
         (+) uint32_t SPI_GetFIFOStatus(uint8_t SPI_FIFO_Direction);

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the specified SPI interrupts.
  * @param  SPIx: where x can be 1 in SPI mode  to select
  *         the SPI peripheral.
  * @param  SPI_IT: specifies the SPI interrupt source to be enabled or disabled.
  *         This parameter can be one of the following values:
  *            @arg SPI_IT_TXE: Tx buffer empty interrupt mask
  *            @arg SPI_IT_RXNE: Rx buffer not empty interrupt mask
  *            @arg SPI_IT_ERR: Error interrupt mask
  *            @arg SPI_IT_TXF: Tx finish interrupt mask
  * @param  NewState: new state of the specified SPI interrupt.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_ITConfig(SPI_TypeDef* SPIx, uint8_t SPI_IT, FunctionalState NewState)
{
    uint16_t itpos = 0, itmask = 0 ;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_SPI_CONFIG_IT(SPI_IT));

    /* Get the SPI IT index */
    itpos = SPI_IT >> 4;

    /* Set the IT mask */
    itmask = (uint16_t)1 << (uint16_t)itpos;

    if (NewState != DISABLE)
    {
        /* Enable the selected SPI interrupt */
        SPIx->CR2 |= itmask;
    }
    else
    {
        /* Disable the selected SPI interrupt */
        SPIx->CR2 &= (uint16_t)~itmask;
    }
}

/**
  * @brief  Returns the current SPIx Transmission FIFO filled level.
  * @param  SPIx: where x can be 1 to select the SPI peripheral.
  * @retval The Transmission FIFO filling state.
  *         - SPI_TransmissionFIFOStatus_Empty: when FIFO is empty
  *         - SPI_TransmissionFIFOStatus_1QuarterFull: if more than 1 quarter-full.
  *         - SPI_TransmissionFIFOStatus_HalfFull: if more than 1 half-full.
  *         - SPI_TransmissionFIFOStatus_Full: when FIFO is full.
  */
uint16_t SPI_GetTransmissionFIFOStatus(SPI_TypeDef* SPIx)
{
    /* Get the SPIx Transmission FIFO level bits */
    return (uint16_t)((SPIx->SR & SPI_SR_FTLVL));
}

/**
  * @brief  Returns the current SPIx Reception FIFO filled level.
  * @param  SPIx: where x can be 1 to select the SPI peripheral.
  * @retval The Reception FIFO filling state.
  *         - SPI_ReceptionFIFOStatus_Empty: when FIFO is empty
  *         - SPI_ReceptionFIFOStatus_1QuarterFull: if more than 1 quarter-full.
  *         - SPI_ReceptionFIFOStatus_HalfFull: if more than 1 half-full.
  *         - SPI_ReceptionFIFOStatus_Full: when FIFO is full.
  */
uint16_t SPI_GetReceptionFIFOStatus(SPI_TypeDef* SPIx)
{
    /* Get the SPIx Reception FIFO level bits */
    return (uint16_t)((SPIx->SR & SPI_SR_FRLVL));
}

/**
  * @brief  Checks whether the specified SPI flag is set or not.
  * @param  SPIx: where x can be 1 in SPI mode  to select
  *         the SPI peripheral.
  * @param  SPI_FLAG: specifies the SPI flag to check.
  *         This parameter can be one of the following values:
  *            @arg SPI_FLAG_TXE: Transmit buffer empty flag.
  *            @arg SPI_FLAG_RXNE: Receive buffer not empty flag.
  *            @arg SPI_FLAG_BSY: Busy flag.
  *            @arg SPI_FLAG_OVR: Overrun flag.
  *            @arg SPI_FLAG_MODF: Mode Fault flag.
  *            @arg SPI_FLAG_TXF: Transmit finish flag.
  * @retval The new state of SPI_FLAG (SET or RESET).
  */
FlagStatus SPI_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_GET_FLAG(SPI_FLAG));

    /* Check the status of the specified SPI flag */
    if ((SPIx->SR & SPI_FLAG) != (uint16_t)RESET)
    {
        /* SPI_FLAG is set */
        bitstatus = SET;
    }
    else
    {
        /* SPI_FLAG is reset */
        bitstatus = RESET;
    }

    /* Return the SPI_FLAG status */
    return  bitstatus;
}

/**
  * @brief  Checks whether the specified SPI interrupt has occurred or not.
  * @param  SPIx: where x can be 1 in SPI mode  to select the SPI peripheral.
  * @param  SPI_IT: specifies the SPI interrupt source to check.
  *         This parameter can be one of the following values:
  *            @arg SPI_IT_TXE: Transmit buffer empty interrupt.
  *            @arg SPI_IT_RXNE: Receive buffer not empty interrupt.
  *            @arg SPI_IT_MODF: Mode Fault interrupt.
  *            @arg SPI_IT_OVR: Overrun interrupt.
  *            @arg SPI_IT_TXF: Transmit finish interrupt.
  * @retval The new state of SPI_IT (SET or RESET).
  */
ITStatus SPI_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_IT)
{
    ITStatus bitstatus = RESET;
    uint16_t itpos = 0, itmask = 0, enablestatus = 0;

    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_GET_IT(SPI_IT));

    /* Get the SPI_IT index */
    itpos = 0x01 << (SPI_IT & 0x0F);

    /* Get the SPI_IT IT mask */
    itmask = SPI_IT >> 4;

    /* Set the IT mask */
    itmask = 0x01 << itmask;

    /* Get the SPI_IT enable bit status */
    enablestatus = (SPIx->CR2 & itmask) ;

    /* Check the status of the specified SPI interrupt */
    if (((SPIx->SR & itpos) != (uint16_t)RESET) && enablestatus)
    {
        /* SPI_IT is set */
        bitstatus = SET;
    }
    else
    {
        /* SPI_IT is reset */
        bitstatus = RESET;
    }

    /* Return the SPI_IT status */
    return bitstatus;
}


/**
  * @brief  Clear the specified SPI flag.
  * @param  SPIx: where x can be 1 in SPI mode to select
  *         the SPI peripheral.
  * @param  SPI_FLAG: specifies the SPI flag to check.
  *         This parameter can be one of the following values:
  *            @arg SPI_FLAG_TXF: Transmit finish flag.
  * @retval None.
  */
void SPI_ClearFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_FLAG)
{
    /* Clear the status of the specified SPI flag */
    SPIx->SR &= ~(SPI_FLAG);
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

