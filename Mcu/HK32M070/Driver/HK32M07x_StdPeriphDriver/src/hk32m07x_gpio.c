/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_gpio.c
* @brief:     API file of gpio module
* @author:    AE Team
* @version:   V1.0.0/2024-03-17
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_gpio.h"
#include "hk32m07x_rcc.h"


/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup GPIO GPIO
  * @brief GPIO modules
  * @{
  * @verbatim
  ==============================================================================
                    ##### GPIO Peripheral features #####
  ==============================================================================
  [..]
    (+) Each port bit of the general-purpose I/O (GPIO) ports can be individually
        configured by software in several modes:
        (++) Input mode
        (++) Analog mode
        (++) Output mode
        (++) Alternate function mode
        (++) External interrupt/event lines

                     ##### How to use this driver #####
  ==============================================================================
  [..]
    (#) Enable the GPIO AHB clock using the following function: RCC_AHBPeriphClockCmd().

    (#) Configure the GPIO pin(s) using GPIO_Init().
        (++) Configure the IO mode using "Mode" member from GPIO_InitTypeDef structure
        (++) Activate Pull-up, Pull-down resistor using "Pull" member from GPIO_InitTypeDef
             structure.
        (++) In case of Output or alternate function mode selection: the speed is
             configured through "Speed" member from GPIO_InitTypeDef structure.
        (++) In alternate mode is selection, the alternate function connected to the IO
             is configured through "Alternate" member from GPIO_InitTypeDef structure.
        (++) Analog mode is required when a pin is to be used as ADC channel
             or DAC output.
        (++) In case of external interrupt/event selection the "Mode" member from
             GPIO_InitTypeDef structure select the type (interrupt or event) and
             the corresponding trigger event (rising or falling or both).

  @endverbatim
  ******************************************************************************
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup GPIO_Private_Functions GPIO_Private_Functions
  * @{
  */

/** @defgroup GPIO_Group1 Initialization and Configuration
 *  @brief   Initialization and Configuration
 *
@verbatim
 ===============================================================================
                    ##### Initialization and Configuration #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the GPIOx peripheral registers to their default reset
  *         values.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @retval None
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    if (GPIOx == GPIOA)
    {
        RCC_AHBPeriphResetCmd(RCC_AHBPeriph_GPIOA, ENABLE);
        RCC_AHBPeriphResetCmd(RCC_AHBPeriph_GPIOA, DISABLE);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_AHBPeriphResetCmd(RCC_AHBPeriph_GPIOB, ENABLE);
        RCC_AHBPeriphResetCmd(RCC_AHBPeriph_GPIOB, DISABLE);
    }
    else
    {

    }
}

/**
  * @brief  Fills each GPIO_InitStruct member with its default value.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
    /* Reset GPIO init structure parameters values */
    GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStruct->GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct->GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct->GPIO_Schmit = GPIO_Schmit_Disable;
}

/**
  * @brief  Initializes the GPIOx peripheral according to the specified
  *         parameters in the GPIO_InitStruct.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct )
{
    uint32_t cfg = 0;
    uint32_t pinpos = 0x00, pos = 0x00, currentpin = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
    assert_param(IS_GPIO_PUPD(GPIO_InitStruct->GPIO_PuPd));

    /*-------------------------- Configure the port pins -----------------------*/
    cfg = GPIO_InitStruct->GPIO_Mode | (GPIO_InitStruct->GPIO_OType << 4) | \
          (GPIO_InitStruct->GPIO_Speed << 2) |  (GPIO_InitStruct->GPIO_PuPd << 5 ) | (GPIO_InitStruct->GPIO_Schmit << 7);

    /*-- GPIO Mode Configuration --*/
    for (pinpos = 0x00; pinpos < 0x10; pinpos++)
    {
        pos = ((uint32_t)0x01) << pinpos;
        /* Get the port pins position */
        currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

        if (currentpin == pos)
        {
            if (GPIOx == GPIOA)
            {
                *(uint32_t *)(GPIOA_BASE + (pinpos) * 4) = cfg | (*(uint32_t *)(GPIOA_BASE + (pinpos) * 4) & 0x0F00);
            }

            if (GPIOx == GPIOB)
            {
                *(uint32_t *)(GPIOB_BASE + (pinpos) * 4) = cfg | (*(uint32_t *)(GPIOB_BASE + (pinpos) * 4) & 0x0F00);
            }
        }
    }
}

/**
  * @brief  selects the pin to used as Alternate function.
  * @param  GPIOx: where x can be (A,B) to select the GPIO peripheral.
  * @param  GPIO_PinSource: specifies the pin for the Alternate function.
  *         This parameter can be GPIO_PinSourcex where x can be (0..7) for GPIOA/B
  * @param  GPIO_AF: selects the pin to used as Alternate function.
  *         This parameter can be one of the following value:
  *            @arg GPIO_AF_0: ATU
  *            @arg GPIO_AF_1: HTU
  *            @arg GPIO_AF_2: UTU
  *            @arg GPIO_AF_3: UART
  *            @arg GPIO_AF_4: IIC
  *            @arg GPIO_AF_5: SPI
  *            @arg GPIO_AF_6: N/A
  *            @arg GPIO_AF_7: ADC/ATU/CMP/RCC
  * @retval None
  * @note   The pin should already been configured in Alternate Function mode(AF)
  *         using GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF
  * @note   Refer to the Alternate function mapping table in the device datasheet
  *         for the detailed mapping of the system and peripherals'alternate
  *         function I/O pins.
  * @retval None
  */
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF)
{
    uint32_t temp = 0x00;
    /* Check the parameters */
    assert_param(IS_GPIO_AF(GPIO_AF));

    temp = (uint32_t)(GPIO_AF << 8);

    if (GPIOx == GPIOA)
    {
        *(uint32_t *)(GPIOA_BASE + (GPIO_PinSource) * 4) = temp | (*(uint32_t *)(GPIOA_BASE + (GPIO_PinSource) * 4) & 0x00FF);
    }

    if (GPIOx == GPIOB)
    {
        *(uint32_t *)(GPIOB_BASE + (GPIO_PinSource) * 4) = temp | (*(uint32_t *)(GPIOB_BASE + (GPIO_PinSource) * 4) & 0x00FF);
    }
}

/**
  * @brief  Locks GPIO Pins configuration registers.
  * @note   The locked registers are GPIOx_MODER, GPIOx_OTYPER, GPIOx_OSPEEDR,
  *         GPIOx_PUPDR, GPIOx_AFRL and GPIOx_AFRH.
  * @note   The configuration of the locked GPIO pins can no longer be modified
  *         until the next device reset.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval None
  */
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    __IO uint32_t tmp = 0x55aa;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    /* Set LCKK bit */
    GPIOx->LCKR = tmp << 16 | GPIO_Pin;

    /* Read LCKK bit */
    tmp = GPIOx->LCKR;
    /* Read LCKK bit */
    tmp = GPIOx->LCKR;
}

/**
  * @}
  */

/** @defgroup GPIO_Group2 GPIO Read and Write
 *  @brief   GPIO Read and Write
 *
@verbatim
 ===============================================================================
                      ##### GPIO Read and Write #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to read.
  * @note   This parameter can be GPIO_Pin_x where x can be:GPIOA, GPIOB
  * @retval The input port pin value.
  */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }

    return bitstatus;
}

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @retval The input port pin value.
  */
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    return ((uint16_t)GPIOx->IDR);
}

/**
  * @brief  Reads the specified output data port bit.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @param  GPIO_Pin: Specifies the port bit to read.
  * @note   This parameter can be GPIO_Pin_x where x can be (A, B).
  * @retval The output port pin value.
  */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->ODR & GPIO_Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }

    return bitstatus;
}

/**
  * @brief  Reads the specified GPIO output data port.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @retval GPIO output data port value.
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    return ((uint16_t)GPIOx->ODR);
}

/**
  * @brief  Sets the selected data port bits.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  * @note   This parameter can be GPIO_Pin_x where x can be (A, B).
  * @retval None
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    GPIOx->BSRR = GPIO_Pin;
}

/**
  * @brief  Clears the selected data port bits.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  * @note   This parameter can be GPIO_Pin_x where x can be (A, B).
  * @retval None
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    GPIOx->BSRR = (GPIO_Pin ) << 16;
}

/**
  * @brief  Sets or clears the selected data port bit.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  * @param  BitVal: specifies the value to be written to the selected bit.
  *         This parameter can be one of the BitAction enumeration values:
  *         Bit_RESET: to clear the port pin
  *         Bit_SET: to set the port pin
  * @note   This parameter can be GPIO_Pin_x
  * @retval None
  */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_BIT_ACTION(BitVal));

    if (BitVal != Bit_RESET)
    {
        GPIOx->BSRR = GPIO_Pin;
    }
    else
    {
        GPIOx->BSRR = GPIO_Pin << 16;
    }
}

/**
  * @brief  Writes data to the specified GPIO data port.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @param  PortVal: specifies the value to be written to the port output data register.
  * @retval None
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    GPIOx->ODR = PortVal;
}

/**
  * @brief  Toggle the GPIO port.
  * @param  GPIOx: where x can be (A, B) to select the GPIO peripheral.
  * @param  GPIO_Pin: sGPIO_pins_define
  *         GPIO_Pin_0
  *         GPIO_Pin_1
  *         GPIO_Pin_2
  *         GPIO_Pin_3
  *         GPIO_Pin_4
  *         GPIO_Pin_5
  *         GPIO_Pin_6
  *         GPIO_Pin_7
  *         GPIO_Pin_8
  *         GPIO_Pin_9
  *         GPIO_Pin_10
  *         GPIO_Pin_11
  *         GPIO_Pin_12
  *         GPIO_Pin_13
  *         GPIO_Pin_14
  *         GPIO_Pin_15
  * @retval None
  */
void GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin ;
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

