/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_pwr.c
* @brief:     This file provides firmware functions to manage the following
*             functionalities of the PWR peripheral:
*             + lower power mode management
*             + flags management
* @author:    AE Team
* @version:   V1.0.0/2024-12-17
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_pwr.h"
#include "hk32m07x_rcc.h"

/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup PWR  PWR
  * @brief PWR driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup PWR_Private_Defines PWR_Private_Defines
  * @{
*/

/* ------------------ PWR registers bit mask ------------------------ */
#define CR_DS_MASK               ((uint32_t)0xFFFFFFFE)
#define CR_PLS_MASK              ((uint32_t)0xFFFFFFE0)

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup PWR_Private_Functions PWR_Private_Functions
  * @{
  */

/** @defgroup PWR_Group1 Initialization and Configuration functions
 *  @brief   Initialization and Configuration functions
 *
@verbatim
  ==============================================================================
                   ##### Initialization and Configuration functions #####
  ==============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the PWR peripheral registers to their default reset values.
  * @retval None
  */
void PWR_DeInit(void)
{
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, DISABLE);
}

/**
  * @}
  */

/** @defgroup PWR_Group2 PVD configuration functions
 *  @brief   PVD configuration functions
 *
@verbatim
  ==============================================================================
                    ##### PVD configuration functions #####
  ==============================================================================
  [..]
  (+) The PVD is used to monitor the VDD power supply by comparing it to a threshold
      selected by the PVD Level (PVD_CFG[2:0] bits in the PWR_PVDCFG).

  (+) A PVDO flag is available to indicate if VDD/VDDA is higher or lower than the
      PVD threshold. This event is internally connected to the EXTI line16
      and can generate an interrupt if enabled through the EXTI registers.

@endverbatim
  * @{
  */

/**
  * @brief  Configures the voltage threshold detected by the Power Voltage Detector(PVD).
  * @param  PWR_PVDLevel: specifies the PVD detection level
  *         This parameter can be one of the following values:
  *            @arg PWR_PVDLevel_0
  *            @arg PWR_PVDLevel_1
  *            @arg PWR_PVDLevel_2
  *            @arg PWR_PVDLevel_3
  *            @arg PWR_PVDLevel_4
  *            @arg PWR_PVDLevel_5
  *            @arg PWR_PVDLevel_6
  *            @arg PWR_PVDLevel_7
  *            @arg PWR_PVDLevel_8
  *            @arg PWR_PVDLevel_9
  *            @arg PWR_PVDLevel_10
  *            @arg PWR_PVDLevel_11
  *            @arg PWR_PVDLevel_12
  *            @arg PWR_PVDLevel_13
  * @note   Refer to the electrical characteristics of your device datasheet for
  *         more details about the voltage threshold corresponding to each
  *         detection level.
  * @retval None
  */
void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_PWR_PVD_LEVEL(PWR_PVDLevel));
    
    tmpreg = PWR->PVDCFG;
    /* Clear [4:0] bits */
    tmpreg &= CR_PLS_MASK;
    /* Set PWR_PVDLevel[3:0] bits according to PWR_PVDLevel value */
    tmpreg |= PWR_PVDLevel;
    /* Store the new value */
    PWR->PVDCFG = tmpreg;
}

/**
  * @brief  Enables or disables the Power Voltage Detector(PVD).
  * @param  NewState: new state of the PVD.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_PVDCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the PVD */
        PWR->PVDCFG |= PWR_PVDCFG_PVD_EN;
    }
    else
    {
        /* Disable the PVD */
        PWR->PVDCFG &= (uint32_t)~((uint32_t)PWR_PVDCFG_PVD_EN);
    }
}

/**
  * @}
  */


/** @defgroup PWR_Group3 Low Power modes configuration functions
 *  @brief   Low Power modes configuration functions
 *
@verbatim
  ==============================================================================
              ##### Low Power modes configuration functions #####
  ==============================================================================

    [..] The devices feature three low-power modes:
    (+) Sleep mode: Cortex-M0 core stopped, peripherals kept running.
    (+) Stop mode: all clocks are stopped, regulator running, regulator in low power mode

  *** Sleep mode ***
  ==================
  [..]
    (+) Entry:
        (++) The Sleep mode is entered by executing the WFE() or WFI() instructions.
    (+) Exit:
        (++) Any peripheral interrupt acknowledged by the nested vectored interrupt
             controller (NVIC) can wake up the device from Sleep mode.

  *** Stop mode ***
  =================
  [..] In Stop mode, all clocks in the VCORE domain are stopped, the PLL, the HSI,
       are disabled. Internal SRAM and register
       contents are preserved.
       The voltage regulator can be configured either in normal or low-power mode.

    (+) Entry:
        (++) The Stop mode is entered using the PWR_EnterSTOPMode(PWR_Regulator_LowPower,)
             function with regulator in LowPower or with Regulator ON.
    (+) Exit:
        (++) Any EXTI Line (Internal or External) configured in Interrupt/Event mode
             or any internal IPs (UART ) wakeup event..

@endverbatim
  * @{
  */

/**
  * @brief  Enters Sleep mode.
  * @note   In Sleep mode, all I/O pins keep the same state as in Run mode.
  * @param  PWR_SLEEPEntry: specifies if SLEEP mode in entered with WFI or WFE instruction.
  *         This parameter can be one of the following values:
  *            @arg PWR_SLEEPEntry_WFI: enter SLEEP mode with WFI instruction
  *            @arg PWR_SLEEPEntry_WFE: enter SLEEP mode with WFE instruction
  * @retval None
  */
void PWR_EnterSleepMode(uint8_t PWR_SLEEPEntry)
{
    /* Check the parameters */
    assert_param(IS_PWR_SLEEP_ENTRY(PWR_SLEEPEntry));

    /* Clear SLEEPDEEP bit of Cortex-M0 System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);

    /* Select SLEEP mode entry -------------------------------------------------*/
    if (PWR_SLEEPEntry == PWR_SLEEPEntry_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
}

/**
  * @brief  Enters STOP mode.
  * @note   In Stop mode, all I/O pins keep the same state as in Run mode.
  * @note   When exiting Stop mode by issuing an interrupt or a wakeup event,
  *         the HSI RC oscillator is selected as system clock.
  *         By keeping the internal regulator ON during Stop mode, the consumption
  *         is higher although the startup time is reduced.
  * @param  PWR_Regulator: Turn off the internal LDO option.
  *            @arg PWR_Regulator_ON      :Internal LDO open.
  *            @arg PWR_Regulator_LowPower: Internal LDO closed.
  * @param  PWR_Entry: specifies if STOP mode in entered with WFI or WFE instruction.
  *         This parameter can be one of the following values:
  *            @arg PWR_STOPEntry_WFI: enter STOP mode with WFI instruction
  *            @arg PWR_STOPEntry_WFE: enter STOP mode with WFE instruction
  *            @arg PWR_STOPEntry_SLEEPONEXIT: enter STOP mode with SLEEPONEXIT instruction
  * @retval None
  */
void PWR_EnterStopMode(uint32_t PWR_Regulator, uint8_t PWR_Entry)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_PWR_REGULATOR(PWR_Regulator));
    assert_param(IS_PWR_ENTRY(PWR_Entry));

    /* Select the regulator state in Stop mode ---------------------------------*/
    tmpreg = PWR->CR;
    /* Clear PDDS and LPDS bits */
    tmpreg &= CR_DS_MASK;
    /* Set LPDS bit according to PWR_Regulator value */
    tmpreg |= PWR_Regulator;
    /* Store the new value */
    PWR->CR = tmpreg;
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* Select Stop mode entry --------------------------------------------------*/
    if (PWR_Entry == PWR_Entry_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();


    }
    else if (PWR_Entry == PWR_Entry_WFE)
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
    else
    {
        /* Set SLEEP on exit bit of Cortex-M0 System Control Register */
        SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    }

    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
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
