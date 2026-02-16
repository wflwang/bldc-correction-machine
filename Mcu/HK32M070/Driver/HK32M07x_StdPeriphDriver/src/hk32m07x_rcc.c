/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_rcc.c
* @brief:     RCC initialization and configuration.
* @author:    AE Team
* @version:   V1.0.0/2024-12-20
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_rcc.h"

/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup RCC RCC
  * @brief RCC driver modules
  * @{
  @verbatim
   ===============================================================================
                        ##### RCC specific features #####
   ===============================================================================
    [..] After reset the device is running from HSI (8 MHz) with Flash 0 WS,
         all peripherals are off except internal SRAM, Flash and SWD.
                 (#)Each clock source can be switched on or off independently when it is not used, to optimize
                        power consumption.
         (#) Several prescalers can be used to configure the frequency of the AHB and the APB
                         domains. The AHB and the APB domains maximum frequency is 64 MHz.
         (#) The clock for all peripherals is switched off, except the SRAM and FLASH.
         (#) All GPIOs are in input floating state, except the SWD pins which
             are assigned to be used for debug purpose.
    [..] Once the device started from reset, the user application has to:
         (#) Configure the clock source to be used to drive the System clock
             (if the application needs higher frequency/performance)
         (#) Configure the System clock frequency and Flash settings
         (#) Configure the AHB and APB busses prescalers
         (#) Enable the clock for the peripheral(s) to be used
         (#) Configure the clock source(s) for peripherals which clocks are not
             derived from the System clock (ADC, I2C, UART and IWDG)

  *  @endverbatim
  */

/** @defgroup RCC_Macro_Define RCC_Macro_Define
  * @brief   RCC Macro Define table
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* ---------------------- RCC registers mask -------------------------------- */
/* RCC Flag Mask */
#define FLAG_MASK                 ((uint8_t)0x1F)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @}
  */
/** @defgroup RCC_Private_Functions RCC_Private_Functions
  * @{
  */

/** @defgroup RCC_Group1 Internal clocks and MCO configuration functions
 *  @brief   Internal clocks and MCO configuration functions
 *
    @verbatim
 ===============================================================================
 ##### Internal clocks and MCO configuration functions #####
 ===============================================================================
    [..] This section provides functions allowing to configure the internal clocks and MCO.
         (#) HSI8 (high-speed internal), 8 MHz factory-trimmed RC used directly
             or through the PLL as System clock source.Divided 8 from HSI64MHz frequency.
             The HSI clock can be used also to clock the FLITF and I2C peripherals.
         (#) HSI (high-speed internal RC oscillator 64MHz).
         (#) LSI (low-speed internal), 32.768 KHz low consumption RC used as IWDG clock.
         (#) MCO (microcontroller clock output), used to output HCLK, HSI, HSI8, LSI lock on PA1, PA5 or PB4 pin.
    @endverbatim
  * @{
  */

/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  * @note   HSI ON and used as system clock source
  * @note   AHB, APB prescaler set to 1.
  * @note   MCO OFF
  * @note   All interrupts disabled
  * @note   However, this function doesn't modify the configuration of the
  * @note   Peripheral clocks LSI clocks.
  * @retval none.
  */
void RCC_DeInit(void)
{
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;

    /* Reset SW[1:0], HPRE[3:0], PPRE[2:0] ,MCO[2:0] bits */
    RCC->CFGR &= (uint32_t)0x0E00000C;

    /* Reset EXTCLK_SEL[1:0] and FLITFCLK_PRE[3:0]*/
    RCC->CFGR4 &= (uint32_t)0x000000FF;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000;
}

/**
  * @brief  Adjusts the Internal High Speed oscillator (HSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal HSI RC.
  *         Refer to the Application Note AN4067 for more details on how to
  *         calibrate the HSI.
  * @param  HSITRIMValue: specifies the HSI calibration trimming value.
  *         This parameter must be a number between 0 and 0x1FF.
  * @retval none.
  */
void RCC_HSITRIM(uint16_t HSITRIMValue)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_RCC_HSI_TRIM_VALUE(HSITRIMValue));

    tmpreg = RCC->CR;

    /* Clear HSIRIM[8:0] bits */
    tmpreg &= ~RCC_CR_HSITRIM;

    /* Set the HSIRIM[8:0] bits according to HSICalibrationValue value */
    tmpreg |= (uint32_t)HSITRIMValue << 2;

    /* Store the new value */
    RCC->CR = tmpreg;
}

/**
  * @brief  Enables or disables the Internal High Speed oscillator (HSI).
  * @note   After enabling the HSI, the application software should wait on
  *         HSIRDY flag to be set indicating that HSI clock is stable and can
  *         be used to clock the system clock.
  * @note   HSI can not be stopped if it is used directly as system clock. In
  *         this case, you have to select another source of the system clock
  *         then stop the HSI.
  * @note   The HSI is stopped by hardware when entering STOP modes.
  * @param  NewState: new state of the HSI.
  *         This parameter can be: ENABLE or DISABLE.
  * @note   When the HSI is stopped, HSIRDY flag goes low after 1 HSI oscillator
  *         clock cycles.
  * @retval none.
  */
void RCC_HSICmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->CR |= RCC_CR_HSION;
    }
    else
    {
        RCC->CR &= ~RCC_CR_HSION;
    }
}

/**
  * @brief  Adjusts the Internal Low Speed oscillator (LSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal LSI RC.
  * @param  HSIFineTRIMValue: specifies the HSI fine calibration trimming value.
  *         This parameter must be a number between 0 and 0x07.
  * @param  HSICoarseTRIMValue: specifies the HSI coarse calibration trimming value.
  *         This parameter must be a number between 0 and 0x1F.
  * @retval none.
  */
void RCC_LSITRIM(uint8_t HSIFineTRIMValue, uint8_t HSICoarseTRIMValue)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_RCC_LSI_FINE_TRIM_VALUE(HSIFineTRIMValue));
    assert_param(IS_RCC_LSI_COARSE_TRIM_VALUE(HSICoarseTRIMValue));

    tmpreg = RCC->CSR;

    /* Clear HSIRIM[8:0] bits */
    tmpreg &= ~RCC_CSR_LSI_FINE;
    tmpreg &= ~RCC_CSR_LSI_COARSE;

    /* Set the HSIRIM[8:0] bits according to HSICalibrationValue value */
    tmpreg |= ((uint32_t)HSIFineTRIMValue << 8) | ((uint32_t)HSICoarseTRIMValue << 2);

    /* Store the new value */
    RCC->CSR = tmpreg;
}

/**
  * @brief  Enables or disables the Internal Low Speed oscillator (LSI).
  * @note   After enabling the LSI, the application software should wait on
  *         LSIRDY flag to be set indicating that LSI clock is stable and can
  *         be used to clock the IWDG.
  * @note   LSI can not be disabled if the IWDG is running.
  * @param  NewState: new state of the LSI.
  *         This parameter can be: ENABLE or DISABLE.
  * @note   When the LSI is stopped, LSIRDY flag goes low after 3 LSI oscillator
  *         clock cycles.
  * @retval none.
  */
void RCC_LSICmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->CSR |= RCC_CSR_LSION;
    }
    else
    {
        RCC->CSR &= ~RCC_CSR_LSION;
    }
}

/**
  * @brief  Selects the clock source to output on MCO pin.
  * @note   MCO pin should be configured in alternate function mode.
  * @param  RCC_MCOSource: specifies the clock source to output.
  *         This parameter can be one of the following values:
  *            @arg RCC_MCOSource_NoClock: No clock selected.
  *            @arg RCC_MCOSource_HCLK: HCLK clock selected.
  *            @arg RCC_MCOSource_LSI: LSI oscillator clock selected.
  *            @arg RCC_MCOSource_HSI8M: HSI8 oscillator clock selected.
  *            @arg RCC_MCOSource_HSI: HSI64 oscillator clock selected.
  * @retval none.
  */
void RCC_MCOConfig(uint8_t RCC_MCOSource)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_RCC_MCO_SOURCE(RCC_MCOSource));

    /* Get CFGR value */
    tmpreg = RCC->CFGR;

    /* Clear MCOPRE[2:0] bits and MCO[3:0] bits*/
    tmpreg &= ~RCC_CFGR_MCO;

    /* Set the RCC_MCOSource and RCC_MCOPrescaler */
    tmpreg |= (uint32_t)RCC_MCOSource << RCC_CFGR_MCO_Pos;

    /* Store the new value */
    RCC->CFGR = tmpreg;
}


/**
  * @}
  */

/** @defgroup RCC_Group2 System AHB and APB busses clocks configuration functions
 *  @brief   System, AHB and APB busses clocks configuration functions
 *
@verbatim
 ===============================================================================
     ##### System, AHB and APB busses clocks configuration functions #####
 ===============================================================================

    [..] This section provide functions allowing to configure the System, AHB and
         APB busses clocks.
         (#) Several clock sources can be used to drive the System clock (SYSCLK): HSI,
             LSI and EXTCLK.
             The AHB clock (HCLK) is derived from System clock through configurable prescaler
             and used to clock the CPU, memory and peripherals mapped on AHB bus.
             and APB (PCLK) clocks are derived from AHB clock through
             configurable prescalers and used to clock the peripherals mapped on these busses.
             You can use "RCC_GetClocksFreq()" function to retrieve the frequencies of these clocks.

         -@- All the peripheral clocks are derived from the System clock (SYSCLK) except:
             (+@) IWDG clock which is always the LSI clock.

         (#) The maximum frequency of the SYSCLK, HCLK and PCLK is 64 MHz.
             Depending on the maximum frequency, the FLASH wait states (WS) should be
             adapted accordingly:
        +--------------------------------------------- +
        |  Wait states  |   HCLK clock frequency (MHz) |
        |---------------|------------------------------|
        |0WS(1CPU cycle)|       0 < HCLK <= 24         |
        |---------------|------------------------------|
        |1WS(2CPU cycle)|       24 < HCLK <= 48        |
        |---------------|------------------------------|
        |2WS(3CPU cycle)|       48 < HCLK <= 64        |
        +----------------------------------------------+

         (#) After reset, the System clock source is the HSI (8 MHz) with 0 WS and
             prefetch is disabled.

    [..] It is recommended to use the following software sequences to tune the number
         of wait states needed to access the Flash memory with the CPU frequency (HCLK).
         (+) Increasing the CPU frequency
         (++) Program the Flash Prefetch buffer, using "FLASH_PrefetchBufferCmd(ENABLE)"
              function
         (++) Check that Flash Prefetch buffer activation is taken into account by
              reading FLASH_ACR using the FLASH_GetPrefetchBufferStatus() function
         (++) Program Flash WS to 1, using "FLASH_SetLatency(FLASH_Latency_1)" function
         (++) Check that the new number of WS is taken into account by reading FLASH_ACR
         (++) Modify the CPU clock source, using "RCC_SYSCLKConfig()" function
         (++) If needed, modify the CPU clock prescaler by using "RCC_HCLKConfig()" function
         (++) Check that the new CPU clock source is taken into account by reading
              the clock source status, using "RCC_GetSYSCLKSource()" function
         (+) Decreasing the CPU frequency
         (++) Modify the CPU clock source, using "RCC_SYSCLKConfig()" function
         (++) If needed, modify the CPU clock prescaler by using "RCC_HCLKConfig()" function
         (++) Check that the new CPU clock source is taken into account by reading
              the clock source status, using "RCC_GetSYSCLKSource()" function
         (++) Program the new number of WS, using "FLASH_SetLatency()" function
         (++) Check that the new number of WS is taken into account by reading FLASH_ACR
         (++) Disable the Flash Prefetch buffer using "FLASH_PrefetchBufferCmd(DISABLE)"
              function
         (++) Check that Flash Prefetch buffer deactivation is taken into account by reading FLASH_ACR
              using the FLASH_GetPrefetchBufferStatus() function.
   @endverbatim
  * @{
  */

/**
  * @brief  Configures the system clock (SYSCLK).
  * @note   The HSI is used (enabled by hardware) as system clock source after
  *         startup from Reset, wake-up from STOP mode.
  * @note   A switch from one clock source to another occurs only if the target
  *         clock source is ready (clock stable after startup delay).
  *         If a clock source which is not yet ready is selected, the switch will
  *         occur when the clock source will be ready.
  *         You can use RCC_GetSYSCLKSource() function to know which clock is
  *         currently used as system clock source.
  * @param  RCC_SYSCLKSource: specifies the clock source used as system clock source
  *         This parameter can be one of the following values:
  *            @arg RCC_SYSCLKSource_HSI:    HSI64 selected as system clock source
  *            @arg RCC_SYSCLKSource_LSI:    LSI selected as system clock source
  *            @arg RCC_SYSCLKSource_HSI8:   HSI8 selected as system clock source
  * @retval none.
  */
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource)
{
    uint32_t tmpreg = 0, tmp = 0;
    uint32_t StartUpCounter = 0;

    /* Check the parameters */
    assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));

    /* wait for CLK Ready*/
    do
    {
        switch (RCC_SYSCLKSource)
        {
            case RCC_SYSCLKSource_HSI:
                tmp = RCC->CR & RCC_CR_HSIRDY;
                break;

            case RCC_SYSCLKSource_LSI:
                tmp = RCC->CSR & RCC_CSR_LSIRDY;
                break;

            case RCC_SYSCLKSource_HSI8:
                tmp = RCC->CR & RCC_CR_HSIRDY;
                break;

            default:
                break;
        }

        StartUpCounter++;
    } while ((tmp == 0) && (StartUpCounter != 0xFFFF));

    /* Clear ESSS bits */
    RCC->CFGR4 &= ~RCC_CFGR4_ESSS;

    tmp = RCC_SYSCLKSource >> 4;

    if (tmp == 0)
    {
        tmpreg = RCC->CFGR;

        /* Clear SW[1:0] bits */
        tmpreg &= ~RCC_CFGR_SW;

        /* Set SW[1:0] bits according to RCC_SYSCLKSource value */
        tmpreg |= RCC_SYSCLKSource;

        /* Store the new value */
        RCC->CFGR = tmpreg;
    }
    else
    {
        tmpreg = RCC->CFGR4;

        /* Clear ESW[2:0] bits */
        tmpreg &= ~RCC_CFGR4_ESW;

        /* Set SW[1:0] bits according to RCC_SYSCLKSource value */
        tmpreg |= RCC_SYSCLKSource | RCC_CFGR4_ESSS;

        /* Store the new value */
        RCC->CFGR4 = tmpreg;
    }
}

/**
  * @brief  Returns the clock source used as system clock.
  * @retval The clock source used as system clock. The returned value can be one
  *         of the following values:
  *            @arg RCC_SYSCLKSource_HSI:    HSI64 selected as system clock source
  *            @arg RCC_SYSCLKSource_LSI:    LSI selected as system clock source
  *            @arg RCC_SYSCLKSource_HSI8:   HSI8 selected as system clock source
  *            @arg RCC_SYSCLKSource_EXTCLK: EXTCLK selected as system clock source
  * @retval none.
  */
uint8_t RCC_GetSYSCLKSource(void)
{
    uint32_t tmp = 0;

    tmp = (RCC->CFGR4 & RCC_CFGR4_ESSS) >> 7;

    /* RCC_CFGR.SWS[1:0] bits */
    if (tmp == 0)
    {
        return ((uint8_t)(RCC->CFGR & RCC_CFGR_SWS)) >> 2;
    }
    /* RCC_CFGR4.ESWS][2:0] bits */
    else
    {
        return ((uint8_t)((RCC->CFGR4 & RCC_CFGR4_ESWS)) >> 3 | 0x10);
    }
}

/**
  * @brief  Configures the AHB clock (HCLK).
  * @param  RCC_SYSCLK: defines the AHB clock divider. This clock is derived from
  *         the system clock (SYSCLK).
  *         This parameter can be one of the following values:
  *            @arg RCC_SYSCLK_Div1:   AHB clock = SYSCLK
  *            @arg RCC_SYSCLK_Div2:   AHB clock = SYSCLK/2
  *            @arg RCC_SYSCLK_Div4:   AHB clock = SYSCLK/4
  *            @arg RCC_SYSCLK_Div8:   AHB clock = SYSCLK/8
  *            @arg RCC_SYSCLK_Div16:  AHB clock = SYSCLK/16
  *            @arg RCC_SYSCLK_Div64:  AHB clock = SYSCLK/64
  *            @arg RCC_SYSCLK_Div128: AHB clock = SYSCLK/128
  *            @arg RCC_SYSCLK_Div256: AHB clock = SYSCLK/256
  *            @arg RCC_SYSCLK_Div512: AHB clock = SYSCLK/512
  * @retval none.
  */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_RCC_HCLK(RCC_SYSCLK));

    tmpreg = RCC->CFGR;

    /* Clear HPRE[3:0] bits */
    tmpreg &= ~RCC_CFGR_HPRE;

    /* Set HPRE[3:0] bits according to RCC_SYSCLK value */
    tmpreg |= RCC_SYSCLK;

    /* Store the new value */
    RCC->CFGR = tmpreg;
}

/**
  * @brief  Configures the APB clock (PCLK).
  * @param  RCC_HCLK: defines the APB clock divider. This clock is derived from
  *         the AHB clock (HCLK).
  *         This parameter can be one of the following values:
  *            @arg RCC_HCLK_Div1:  APB clock = HCLK
  *            @arg RCC_HCLK_Div2:  APB clock = HCLK/2
  *            @arg RCC_HCLK_Div4:  APB clock = HCLK/4
  *            @arg RCC_HCLK_Div8:  APB clock = HCLK/8
  *            @arg RCC_HCLK_Div16: APB clock = HCLK/16
  * @retval none.
  */
void RCC_PCLKConfig(uint32_t RCC_HCLK)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_RCC_PCLK(RCC_HCLK));

    tmpreg = RCC->CFGR;

    /* Clear PPRE[2:0] bits */
    tmpreg &= ~RCC_CFGR_PPRE;

    /* Set PPRE[2:0] bits according to RCC_HCLK value */
    tmpreg |= RCC_HCLK;

    /* Store the new value */
    RCC->CFGR = tmpreg;
}

/**
  * @brief  Configures the FLITF clock prescaler factor.
  * @param  RCC_FLITFPRE_VALUE: defines the FLITF clock divider. This clock is derived from
  *         the FLITF clock source .
  *         This parameter can be one of the following values:
  *            @arg RCC_FLITF_Div1: prescaler factor is 1
  *            @arg RCC_FLITF_Div2: prescaler factor is 2
  *            @arg RCC_FLITF_Div3: prescaler factor is 3
  *            @arg RCC_FLITF_Div4: prescaler factor is 4
  *            @arg RCC_FLITF_Div5: prescaler factor is 5
  *            @arg RCC_FLITF_Div6: prescaler factor is 6
  *            @arg RCC_FLITF_Div7: prescaler factor is 7
  *            @arg RCC_FLITF_Div8: prescaler factor is 8
  *            @arg RCC_FLITF_Div9: prescaler factor is 9
  *            @arg RCC_FLITF_Div10: prescaler factor is 10
  *            @arg RCC_FLITF_Div11: prescaler factor is 11
  *            @arg RCC_FLITF_Div12: prescaler factor is 12
  * @retval none.
  */
void RCC_FLITFPREDIVConfig(uint32_t RCC_FLITFPRE_VALUE)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_RCC_FLITFCLK_PREDIV(RCC_FLITFPRE_VALUE));

    tmpreg = RCC->CFGR4;

    /* Clear FLITFCLK_PRE[3:0] bits */
    tmpreg &= ~RCC_CFGR4_FLITFCLK_PRE;

    /* Set FLITFCLK_PRE[3:0] bits */
    tmpreg |= RCC_FLITFPRE_VALUE << RCC_CFGR4_FLITFCLK_PRE_Pos;

    /* Store the new value */
    RCC->CFGR4 = tmpreg;
}

/**
  * @brief  Returns the frequencies of the System, AHB and APB busses clocks.
  * @note   The frequency returned by this function is not the real frequency
  *         in the chip. It is calculated based on the predefined constant and
  *         the source selected by RCC_SYSCLKConfig():
  * @note   If SYSCLK source is HSI8, function returns constant HSI8_VALUE(*)
  * @note   If SYSCLK source is HSI, function returns constant HSI64_VALUE(**)
  * @note   If SYSCLK source is LSI, function returns constant LSI_VALUE(***)
  * @param  RCC_Clocks: pointer to a RCC_ClocksTypeDef structure which will hold
  *         the clocks frequencies.
  * @note   This function can be used by the user application to compute the
  *         baudrate for the communication peripherals or configure other parameters.
  * @note   Each time SYSCLK, HCLK and/or PCLK clock changes, this function
  *         must be called to update the structure's field. Otherwise, any
  *         configuration based on this function will be incorrect.
  * @retval none.
  */
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
    uint32_t tmp = 0, presc = 0;

    /* Get SYSCLK source -------------------------------------------------------*/
    if ((RCC->CFGR4 & RCC_CFGR4_ESSS) != 0)
    {
        /* SYSCLK defined by RCC_CFGR4.ESW bits*/
        tmp = (RCC->CFGR4 & RCC_CFGR4_ESWS) >> RCC_CFGR4_ESWS_Pos;
        tmp = tmp | 0x10;
    }
    else
    {
        tmp = (RCC->CFGR & RCC_CFGR_SWS) >> RCC_CFGR_SWS_Pos;
    }

    switch (tmp)
    {
        case 0x00:  /* HSI8 used as system clock */
            RCC_Clocks->SYSCLK_Frequency = HSI8_VALUE;
            break;

        case 0x10:  /* LSI used as system clock */
            RCC_Clocks->SYSCLK_Frequency = LSI_VALUE;
            break;

        case 0x12:  /* HSI64 used as system clock */
            RCC_Clocks->SYSCLK_Frequency = HSI64_VALUE;
            break;

        case 0x14:  /* EXTCLK_GPIOINPUT used as system clock */
            RCC_Clocks->SYSCLK_Frequency = EXTCLK_GPIOINPUT_VALUE;
            break;

        default: /* HSI8 used as system clock */
            RCC_Clocks->SYSCLK_Frequency = HSI8_VALUE;
            break;
    }

    /* Compute HCLK, PCLK clocks frequencies -----------------------------------*/
    /* Get HCLK prescaler */
    tmp = RCC->CFGR & RCC_CFGR_HPRE;
    tmp = tmp >> RCC_CFGR_HPRE_Pos;

    presc = APBAHBPrescTable[tmp];

    /* HCLK clock frequency */
    RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;

    /* Get PCLK1(APB1) prescaler */
    tmp = RCC->CFGR & RCC_CFGR_PPRE;
    tmp = tmp >> RCC_CFGR_PPRE_Pos;
    presc = APBAHBPrescTable[tmp];
    /* PCLK clock frequency */
    RCC_Clocks->PCLK_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

}


/**
  * @}
  */

/** @defgroup RCC_Group3 Peripheral clocks configuration functions
 *  @brief   Peripheral clocks configuration functions
 *
@verbatim
 ===============================================================================
             ##### Peripheral clocks configuration functions #####
 ===============================================================================

    [..] This section provide functions allowing to configure the Peripheral clocks.
         (#) After restart from Reset , all peripherals are off
             except internal SRAM, Flash and SWD. Before to start using a peripheral you
             have to enable its interface clock. You can do this using RCC_AHBPeriphClockCmd(),
             RCC_APBPeriphClockCmd1() and RCC_APBPeriphClockCmd2() functions.
         (#) To reset the peripherals configuration (to the default state after device reset)
             you can use RCC_AHBPeriphResetCmd(), RCC_APBPeriphResetCmd1() and
             RCC_APBPeriphResetCmd2() functions.

@endverbatim
  * @{
  */


/**
  * @brief  Enables or disables the AHB peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @param  RCC_AHBPeriph: specifies the AHB peripheral to gates its clock.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_AHBPeriph_GPIOA:   GPIOA clock
  *            @arg RCC_AHBPeriph_GPIOB:   GPIOB clock
  *            @arg RCC_AHBPeriph_DIVSQ:   DVSQ clock
  *            @arg RCC_AHBPeriph_FLITF:   FLITF clock(It has effect only when the Flash memory is in power down mode)
  *            @arg RCC_AHBPeriph_SRAM:    SRAM clock
  * @param  NewState: new state of the specified peripheral clock.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval none.
  */
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->AHBENR |= RCC_AHBPeriph;
    }
    else
    {
        RCC->AHBENR &= ~RCC_AHBPeriph;
    }
}

/**
  * @brief  Enables or disables the High Speed APB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @param  RCC_APB2Periph: specifies the APB2 peripheral to gates its clock.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_APB2Periph_PGA: PGA clock
  *            @arg RCC_APB2Periph_DBG: DBG clock
  *            @arg RCC_APB2Periph_I2C1: I2C1 clock
  *            @arg RCC_APB2Periph_COMP: COMP clock
  *            @arg RCC_APB2Periph_UTU2: UTU2 clock
  *            @arg RCC_APB2Periph_UART2: UART2 clock
  *            @arg RCC_APB2Periph_SYSCFG: SYSCFG clock
  * @param  NewState: new state of the specified peripheral clock.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval none.
  */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->APB2ENR |= RCC_APB2Periph;
    }
    else
    {
        RCC->APB2ENR &= ~RCC_APB2Periph;
    }
}

/**
  * @brief  Enables or disables the Low Speed APB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @param  RCC_APB1Periph: specifies the APB1 peripheral to gates its clock.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_APB1Periph_VDIV: VDIV clock
  *            @arg RCC_APB1Periph_PWR: PWR clock
  *            @arg RCC_APB1Periph_UTU1: UTU1 clock
  *            @arg RCC_APB1Periph_ATU1: ATU1 clock
  *            @arg RCC_APB1Periph_HTU1: HTU1 clock
  *            @arg RCC_APB1Periph_SPI1: SPI1 clock
  *            @arg RCC_APB1Periph_WWDG: WWDG clock
  *            @arg RCC_APB1Periph_UART1: UART1 clock
  * @param  NewState: new state of the specified peripheral clock.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval none.
  */
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->APB1ENR |= RCC_APB1Periph;
    }
    else
    {
        RCC->APB1ENR &= ~RCC_APB1Periph;
    }
}

/**
  * @brief  Forces or releases AHB peripheral reset.
  * @param  RCC_AHBPeriph: specifies the AHB peripheral to reset.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_AHBPeriph_GPIOA:   GPIOA clock
  *            @arg RCC_AHBPeriph_GPIOB:   GPIOB clock
  *            @arg RCC_AHBPeriph_DIVSQ:   DVSQ clock
  *            @arg RCC_AHBPeriph_FLITF:   FLITF clock(It has effect only when the Flash memory is in power down mode)
  *            @arg RCC_AHBPeriph_SRAM:    SRAM clock
  * @param  NewState: new state of the specified peripheral reset.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval none.
  */
void RCC_AHBPeriphResetCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_AHB_RST_PERIPH(RCC_AHBPeriph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->AHBRSTR |= RCC_AHBPeriph;
    }
    else
    {
        RCC->AHBRSTR &= ~RCC_AHBPeriph;
    }
}

/**
  * @brief  Forces or releases High Speed APB2 peripheral reset.
  * @param  RCC_APB2Periph: specifies the APB2 peripheral to reset.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_APB2Periph_PGA: PGA clock
  *            @arg RCC_APB2Periph_DBG: DBG clock
  *            @arg RCC_APB2Periph_I2C1: I2C1 clock
  *            @arg RCC_APB2Periph_COMP: COMP clock
  *            @arg RCC_APB2Periph_UTU2: UTU2 clock
  *            @arg RCC_APB2Periph_UART2: UART2 clock
  *            @arg RCC_APB2Periph_SYSCFG: SYSCFG clock
  * @param  NewState: new state of the specified peripheral reset.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval none.
  */
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->APB2RSTR |= RCC_APB2Periph;
    }
    else
    {
        RCC->APB2RSTR &= ~RCC_APB2Periph;
    }
}

/**
  * @brief  Forces or releases Low Speed APB1 peripheral reset.
  * @param  RCC_APB1Periph: specifies the APB1 peripheral to reset.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_APB1Periph_VDIV: VDIV clock
  *            @arg RCC_APB1Periph_PWR: PWR clock
  *            @arg RCC_APB1Periph_UTU1: UTU1 clock
  *            @arg RCC_APB1Periph_ATU1: ATU1 clock
  *            @arg RCC_APB1Periph_HTU1: HTU1 clock
  *            @arg RCC_APB1Periph_SPI1: SPI1 clock
  *            @arg RCC_APB1Periph_WWDG: WWDG clock
  *            @arg RCC_APB1Periph_UART1: UART1 clock
  *            @arg RCC_APB1Periph_ADC: ADC clock
  * @param  NewState: new state of the specified peripheral clock.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval none.
  */
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RCC->APB1RSTR |= RCC_APB1Periph;
    }
    else
    {
        RCC->APB1RSTR &= ~RCC_APB1Periph;
    }
}

/**
  * @}
  */

/** @defgroup RCC_Group4 Interrupts and flags management functions
 *  @brief   Interrupts and flags management functions
 *
@verbatim
 ===============================================================================
             ##### Interrupts and flags management functions #####
 ===============================================================================
@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the specified RCC interrupts.
  * @param  RCC_IT: specifies the RCC interrupt sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_IT_LSIRDY: LSI ready interrupt
  *            @arg RCC_IT_HSIRDY: HSI ready interrupt
  * @param  NewState: new state of the specified RCC interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval none.
  */
void RCC_ITConfig(uint32_t RCC_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RCC_IT(RCC_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /*Enable the selected interrupts */
        RCC->CIR |= (uint32_t)RCC_IT << 8;
    }
    else
    {
        /*Disable the selected interrupts */
        RCC->CIR &= ~((uint32_t)RCC_IT) << 8;
    }
}

/**
  * @brief  Checks whether the specified RCC flag is set or not.
  * @param  RCC_FLAG: specifies the flag to check.
  *         This parameter can be one of the following values:
  *         ** CR **
  *            @arg RCC_FLAG_HSIRDY: HSI8/64 oscillator clock ready
  *                 ** CSR **
  *            @arg RCC_FLAG_WWDGRST: Window Watchdog reset
  *            @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
  *            @arg RCC_FLAG_SFTRST: Software reset
  *            @arg RCC_FLAG_PORRST: POR/PDR reset
  *            @arg RCC_FLAG_PINRST: Pin reset
  *            @arg RCC_FLAG_OBLRST: Option Byte Loader (OBL) reset
  *            @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
  * @retval The new state of RCC_FLAG (SET or RESET).
  */
FlagStatus RCC_GetFlagStatus(uint32_t RCC_FLAG)
{
    uint32_t tmp = 0;
    uint32_t statusreg = 0;
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_RCC_FLAG(RCC_FLAG));

    /* Get the RCC register index */
    tmp = RCC_FLAG >> 5;

    if (tmp == 0)               /* The flag to check is in CR register */
    {
        statusreg = RCC->CR;
    }
    else if (tmp == 2)          /* The flag to check is in CSR register */
    {
        statusreg = RCC->CSR;
    }

    /* Get the flag position */
    tmp = RCC_FLAG & FLAG_MASK;

    if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the flag status */
    return bitstatus;
}

/**
  * @brief  Clears the RCC reset flags.
  *         The reset flags are:
  *         RCC_FLAG_OBLRST
  *         RCC_FLAG_PINRST
  *         RCC_FLAG_PORRST
  *         RCC_FLAG_SFTRST
  *         RCC_FLAG_IWDGRST
  *         RCC_FLAG_WWDGRST
  * @retval none.
  */
void RCC_ClearFlag(void)
{
    /* Set RMVF bit to clear the reset flags */
    RCC->CSR |= RCC_CSR_RMVF;
}

/**
  * @brief  Checks whether the specified RCC interrupt has occurred or not.
  * @param  RCC_IT: specifies the RCC interrupt source to check.
  *         This parameter can be one of the following values:
  *            @arg RCC_IT_LSIRDY: LSI ready interrupt
  *            @arg RCC_IT_HSIRDY: HSI8/64 ready interrupt
  * @retval The new state of RCC_IT (SET or RESET).
  */
ITStatus RCC_GetITStatus(uint32_t RCC_IT)
{
    ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_RCC_GET_IT(RCC_IT));

    /* Check the status of the specified RCC interrupt */
    if ((RCC->CIR & RCC_IT) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the RCC_IT status */
    return  bitstatus;
}

/**
  * @brief  Clears the RCC's interrupt pending bits.
  * @param  RCC_IT: specifies the interrupt pending bit to clear.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_IT_LSIRDY: LSI ready interrupt
  *            @arg RCC_IT_HSIRDY: HSI8/64 ready interrupt
  * @retval none.
  */
void RCC_ClearITPendingBit(uint32_t RCC_IT)
{
    /* Check the parameters */
    assert_param(IS_RCC_CLEAR_IT(RCC_IT));

    /* Clear the selected interrupt pending bits */
    RCC->CIR |= (uint32_t)RCC_IT<<16 ;
}

/**
  * @brief  Configures the external clock source(GPIOCLK).
  * @param  RCC_EXTCLK_GPIO: defines the external clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_EXTCLK_GPIO_PA0  : external clock = GPIOA_0
  *            @arg RCC_EXTCLK_GPIO_PB15  : external clock = GPIOB_15
  * @retval none.
  */
void RCC_EXTCLKGPIOConfig(uint32_t RCC_EXTCLK_GPIO)
{
    /* Check the parameters */
    assert_param(IS_RCC_EXTCLK_GPIO(RCC_EXTCLK_GPIO));

    /* Clear EXTCLK_SEL[1:0] bits */
    RCC->CFGR4 &= ~RCC_CFGR4_EXTCLK_SEL;

    /* Set EXTCLK_SEL[1:0] bits according to RCC_BEEPERCLK value */
    RCC->CFGR4 |= RCC_EXTCLK_GPIO;
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

