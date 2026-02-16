/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_atu.c
* @brief:     This file provides firmware functions to manage ATU moudle.
* @author:    AE Team
* @version:   V1.0.0/2024-12-19 
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_atu.h"


/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup ATU ATU
  * @brief ATU driver modules
  * @{
  @verbatim
  
  @endverbatim
*/


/** @defgroup ATU_Private_Defines ATU_Private_Defines
  * @{
  */
/* ---------------------- ATU registers bit mask ------------------------ */
#define TOPC_OUTPUT_CHANNEL_MASK           ((uint32_t)0x000000FF)
#define ATU_CRxA_BASE                      ((uint32_t)(ATU_BASE+0x08))
#define ATU_CRxB_BASE                      ((uint32_t)(ATU_BASE+0x20))
#define TPPS_PROTECT_IF_MASK               ((uint16_t)0x00FF)
#define TOPC_FILTER_MASK                   ((uint16_t)0x00FF)

#define Aligned_8BIT(pos)  ((pos>>3)<<3)

/**
  * @}
  */

/** @defgroup ATU_Private_Functions ATU_Private_Functions
  * @{
  */

/** @defgroup ATU_Group1 TimeBase management functions
 *  @brief   TimeBase management functions
 *
@verbatim
 ===============================================================================
                 ##### TimeBase management functions #####
 ===============================================================================

        *** ATU Driver: how to use it in Timing(Time base) Mode ***
 ===============================================================================
    [..] To use the Timer in Timing(Time base) mode, the following steps are
         mandatory:
         (#) Enable ATU clock using
             RCC_APBxPeriphClockCmd(RCC_APBxPeriph_ATUx, ENABLE) function.
         (#) Fill the ATU_TimeBaseInitStruct with the desired parameters.
         (#) Call ATU_TimeBaseInit(ATUx, &ATU_TimeBaseInitStruct) to configure
             the Time Base unit with the corresponding configuration.
         (#) Enable the NVIC if you need to generate the update interrupt.
         (#) Enable the corresponding interrupt using the function
             ATU_ITConfig(ATUx, ATU_IT_Update).
         (#) Call the ATU_Cmd(ENABLE) function to enable the ATU counter.
    [..]
        (@) All other functions can be used seperatly to modify, if needed,
            a specific feature of the Timer.

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the ATUx peripheral registers to their default reset values.
  * @retval None
  */
void ATU_DeInit(void)
{
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_ATU1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_ATU1, DISABLE);
}

/**
  * @brief  Initializes the ATUx Time Base Unit peripheral according to
  *         the specified parameters in the ATU_TimeBaseInitStruct.
  * @param  ATU_TimeBaseInitStruct: pointer to a ATU_TimeBaseInitTypeDef
  *         structure that contains the configuration information for
  *         the specified ATU peripheral.
  * @retval None
  */
void ATU_TimeBaseInit(ATU_TimeBaseInitTypeDef* ATU_TimeBaseInitStruct)
{
    uint16_t tmptcsr = 0;
    /* Check the parameters */
    assert_param(IS_ATU_COUNTER_MODE(ATU_TimeBaseInitStruct->ATU_CounterMode));
    assert_param(IS_ATU_CKD_DIV(ATU_TimeBaseInitStruct->ATU_ClockDivision));
    assert_param(IS_ATU_PERIOD_RELOAD_MODE(ATU_TimeBaseInitStruct->ATU_PeriodAutoReload));
    tmptcsr = ATU->TCSR;

    /* Select the Counter Mode and Clock division*/
    tmptcsr &= (uint16_t)(~((uint16_t)(ATU_TCSR_CLK_DIV| ATU_TCSR_TCMTH)));
    tmptcsr |= ATU_TimeBaseInitStruct->ATU_CounterMode |\
               ATU_TimeBaseInitStruct->ATU_ClockDivision;

    ATU->TCSR = tmptcsr;
    /* Set the Period reload mode */
    ATU->TARCS &= (uint16_t)(~(uint16_t)(ATU_TARCS_RELTPR_Mask));
    ATU->TARCS |= ATU_TimeBaseInitStruct->ATU_PeriodAutoReload;
    /* Set the Period value */
    ATU->TPR = ATU_TimeBaseInitStruct->ATU_Period;

}

/**
  * @brief  Fills each ATU_TimeBaseInitStruct member with its default value.
  * @param  ATU_TimeBaseInitStruct: pointer to a ATU_TimeBaseInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void ATU_TimeBaseStructInit(ATU_TimeBaseInitTypeDef* ATU_TimeBaseInitStruct)
{
    /* Set the default configuration */
    ATU_TimeBaseInitStruct->ATU_Period = 0xFFFF;
    ATU_TimeBaseInitStruct->ATU_ClockDivision = ATU_CLK_DIV_1;
    ATU_TimeBaseInitStruct->ATU_CounterMode = ATU_COUNTER_MODE_UP;
    ATU_TimeBaseInitStruct->ATU_PeriodAutoReload = ATU_PERIOD_IMMEDIATELY_RELOAD;
}


/**
  * @brief  Specifies the ATU Counter Mode to be used.
  * @param  ATU_CounterMode: specifies the Counter Mode to be used
  *         This parameter can be one of the following values:
  *            @arg ATU_COUNTER_MODE_UP: ATU Up Counting Mode
  *            @arg ATU_COUNTER_MODE_DOWM: ATU Down Counting Mode
  *            @arg ATU_COUNTER_MODE_UPDOWM: ATU Center Aligned Mode
  * @retval None
  */
void ATU_CounterModeConfig(uint16_t ATU_CounterMode)
{
    uint16_t tmptcsr = 0;
    /* Check the parameters */
    assert_param(IS_ATU_COUNTER_MODE(ATU_CounterMode));
    tmptcsr = ATU->TCSR;
    /* Reset the TCMTH Bits */
    tmptcsr &= (uint16_t)(~((uint16_t)(ATU_TCSR_TCMTH)));
    /* Set the Counter Mode */
    tmptcsr |= ATU_CounterMode;
    /* Write to ATUx CR1 register */
    ATU->TCSR = tmptcsr;
}

/**
  * @brief  Sets the ATU Counter Register value
  * @param  Counter: specifies the Counter register new value.
  * @param  ReloadMode: specifies the ATU CNT Reload mode.
  *         This parameter can be one of the following values:
  *            @arg ATU_CNT_IMMEDIATELY_RELOAD
  *            @arg ATU_CNT_AUTO_RELOAD
  * @retval None
  */
void ATU_SetCounter(uint16_t Counter,uint16_t ReloadMode)
{
    /* Check the parameters */
    assert_param(IS_ATU_CNT_RELOAD_MODE(ReloadMode));

    /* Set the CNT reload mode */
    ATU->TARCS &= (uint16_t)(~(uint16_t)(ATU_TARCS_RELTPR_Mask));
    ATU->TARCS |= ReloadMode;
    /* Set the Counter Register value */
    ATU->CNT = Counter;
}


/**
  * @brief  Sets the ATU TimePeriod Register value
  * @param  TimePeriod: specifies the TimePeriod register new value.
  * @param  ReloadMode: specifies the ATU Period Reload mode
  *         This parameter can be one of the following values:
  *            @arg ATU_PERIOD_IMMEDIATELY_RELOAD
  *            @arg ATU_PERIOD_AUTO_RELOAD
  * @retval None
  */
void ATU_SetTimePeriod(uint16_t TimePeriod ,uint16_t ReloadMode)
{
    /* Check the parameters */
    assert_param(IS_ATU_PERIOD_RELOAD_MODE(ReloadMode));

    /* Set the Period reload mode */
    ATU->TARCS &= (uint16_t)(~(uint16_t)(ATU_TARCS_RELCNT_Mask));
    ATU->TARCS |= ReloadMode;
    /* Set the TimePeriod Register value */
    ATU->TPR = TimePeriod;
}

/**
  * @brief  Gets the ATU Counter value.
  * @retval Counter Register value.
  */
uint16_t ATU_GetCounter(void)
{
    /* Get the Counter Register value */
    return ATU->CNT;
}

/**
  * @brief  Gets the ATU ClockDivision value.
  * @retval ClockDivision value.
  */
uint16_t ATU_GetClockDivision(void)
{
    /* Get the ClockDivision value */
    return ATU->TCSR & ATU_TCSR_CLK_DIV_Mask;
}


/**
  * @brief  Configure ATU PWM output channelx.
  * @param  ATU_PWMChannelx: specifies ATU PWM output channel.
  *         This parameter can be one of the following values:
  *            @arg ATU_PWMCHANNEL_0
  *            @arg ATU_PWMCHANNEL_1
  *            @arg ATU_PWMCHANNEL_2
  * @param  ATU_OutputInitStruct: pointer to a ATU_OutputInitTypeDef
  *         structure that contains the configuration information for
  *         the specified ATU PWM output channel.
  * @retval None
  */
void ATU_OutputInit(uint32_t ATU_PWMChannelx,ATU_OutputInitTypeDef* ATU_OutputInitStruct)
{
    #define CRxA_GAP                       0x03    //CR0A and CR1A address gap 0x04,mask the 3LSB.
    #define RELOADxA_BIT_FIELD_WIDTH_MASK  0x02    //REL0A and REL1A location gap 2bit.
    uint32_t tmptopc = 0, cr_offset = 0, reload_offset = 0;

    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_CHANNEL(ATU_PWMChannelx));
    assert_param(IS_ATU_OUTPUT_MODE(ATU_OutputInitStruct->ATU_OutputMode));
    assert_param(IS_ATU_OUTPUT_POLARITYA(ATU_OutputInitStruct->ATU_OutputPolarityA));
    assert_param(IS_ATU_OUTPUT_POLARITYB(ATU_OutputInitStruct->ATU_OutputPolarityB));
    assert_param(IS_ATU_OUTPUT_SOURCEA(ATU_OutputInitStruct->ATU_OutputSourceA));
    assert_param(IS_ATU_OUTPUT_SOURCEB(ATU_OutputInitStruct->ATU_OutputSourceB));
    assert_param(IS_ATU_OUTPUT_LEVEL_SETA(ATU_OutputInitStruct->ATU_OutputLevelSetA));
    assert_param(IS_ATU_OUTPUT_LEVEL_SETB(ATU_OutputInitStruct->ATU_OutputLevelSetB));
    assert_param(IS_ATU_CRxA_RELOAD_MODE(ATU_OutputInitStruct->ATU_CRxAReloadMode));
    assert_param(IS_ATU_CRxB_RELOAD_MODE(ATU_OutputInitStruct->ATU_CRxBReloadMode));

    /* Get the ATU TOPC register value */
    tmptopc = ATU->TPOC;
    /* Clear the Channel x configuration */
    tmptopc &= (uint32_t)(~(uint32_t)(TOPC_OUTPUT_CHANNEL_MASK << ATU_PWMChannelx));
    /* Configure the Channel x  */
    tmptopc |= (ATU_OutputInitStruct->ATU_OutputMode       |\
                ATU_OutputInitStruct->ATU_OutputPolarityA  |\
                ATU_OutputInitStruct->ATU_OutputPolarityB  |\
                ATU_OutputInitStruct->ATU_OutputSourceA    |\
                ATU_OutputInitStruct->ATU_OutputSourceB    |\
                ATU_OutputInitStruct->ATU_OutputLevelSetA  |\
                ATU_OutputInitStruct->ATU_OutputLevelSetB) << ATU_PWMChannelx;

    /* Configure the DBTA and DBTB Register  */   
    if(ATU_OUTPUT_COMPLEMENTARY_PWM == ATU_OutputInitStruct->ATU_OutputMode)
    {
      ATU->DBTA = ATU_OutputInitStruct->ATU_DeadBandTimeA;
      ATU->DBTB = ATU_OutputInitStruct->ATU_DeadBandTimeB;
    }
    /* Get the CRxA,CRxB offset Address  */
    cr_offset =  ATU_PWMChannelx >> CRxA_GAP;
    /* Get the RELxA,RELxB offset bit position  */
    reload_offset =  ATU_PWMChannelx >> RELOADxA_BIT_FIELD_WIDTH_MASK;
    /* Configure the CRxA,CRxB Reload mode */   
    ATU->TARCS &= (uint16_t)(~(uint16_t)((ATU_TARCS_REL0A_Mask |\
                  ATU_TARCS_REL0B_Mask)<<reload_offset));
    ATU->TARCS |= (ATU_OutputInitStruct->ATU_CRxAReloadMode |\
                   ATU_OutputInitStruct->ATU_CRxBReloadMode) << reload_offset;
    /* Configure the CRxA,CRxB Register  */   
    *(uint16_t*)((uint32_t*)ATU_CRxA_BASE + cr_offset) = ATU_OutputInitStruct->ATU_CompareValuexA;
    *(uint16_t*)((uint32_t*)ATU_CRxB_BASE + cr_offset) = ATU_OutputInitStruct->ATU_CompareValuexB;

    /* Configure the TPOC Register  */
    ATU->TPOC = tmptopc;
    
}

/**
  * @brief  Fills each ATU_OutputInitStruct member with its default value.
  * @param  ATU_OutputInitStruct: pointer to a ATU_OutputInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void ATU_OutputStructInit(ATU_OutputInitTypeDef* ATU_OutputInitStruct)
{
    /* Set the default configuration */
    ATU_OutputInitStruct->ATU_CompareValuexA  = 0;
    ATU_OutputInitStruct->ATU_CompareValuexB  = 0;
    ATU_OutputInitStruct->ATU_CRxAReloadMode  = ATU_CRxA_IMMEDIATELY_RELOAD;
    ATU_OutputInitStruct->ATU_CRxBReloadMode  = ATU_CRxB_IMMEDIATELY_RELOAD;
    ATU_OutputInitStruct->ATU_DeadBandTimeA   = 0;
    ATU_OutputInitStruct->ATU_DeadBandTimeB   = 0;
    ATU_OutputInitStruct->ATU_OutputLevelSetA = ATU_OUTPUT_LEVEL_SETA_LOW;
    ATU_OutputInitStruct->ATU_OutputLevelSetB = ATU_OUTPUT_LEVEL_SETB_LOW;
    ATU_OutputInitStruct->ATU_OutputMode      = ATU_OUTPUT_INDEPENDENT_PWM;
    ATU_OutputInitStruct->ATU_OutputPolarityA = ATU_OUTPUT_POLARITYA_NORMAL;
    ATU_OutputInitStruct->ATU_OutputPolarityB = ATU_OUTPUT_POLARITYB_NORMAL;
    ATU_OutputInitStruct->ATU_OutputSourceA   = ATU_OUTPUT_SOURCEA_COUNTER;
    ATU_OutputInitStruct->ATU_OutputSourceB   = ATU_OUTPUT_SOURCEB_COUNTER;
}

/**
  * @brief  Configure ATU protect interface.
  * @param  ATU_ProtectIFx: specifies ATU protect interface.
  *         This parameter can be one of the following values:
  *            @arg ATU_PROTECT_IF_0
  *            @arg ATU_PROTECT_IF_1
  * @param  ATU_ProtectIFInitStruct: pointer to a ATU_ProtectInitTypeDef
  *         structure that contains the configuration information for
  *         the specified ATU protect interface.
  * @retval None
  */
void ATU_ProtectIFConfig(uint32_t ATU_ProtectIFx, ATU_ProtectIFInitTypeDef* ATU_ProtectIFInitStruct)
{
    uint16_t tmptpps = 0;

    /* Check the parameters */
    assert_param(IS_ATU_PROTECT_IF(ATU_ProtectIFx));
    assert_param(IS_ATU_PROTECT_SOURCE(ATU_ProtectIFInitStruct->ATU_ProtectSource));
    assert_param(IS_ATU_PROTECT_POLARITY(ATU_ProtectIFInitStruct->ATU_ProtectPolarity));
    assert_param(IS_ATU_PROTECT_STATE(ATU_ProtectIFInitStruct->ATU_ProtectCmd));
    assert_param(IS_ATU_PROTECT_FILTER(ATU_ProtectIFInitStruct->ATU_Filter));
    assert_param(IS_ATU_PROTECT_AUTOCLEAR(ATU_ProtectIFInitStruct->ATU_AutoClearProtect));

    /* Get the ATU TPPS register value */
    tmptpps = ATU->TPPS;
    /* Clear the Protect IFx configuration */
    tmptpps &= (uint16_t)(~(uint16_t)(TPPS_PROTECT_IF_MASK << ATU_ProtectIFx));
    /* Clear the Protect IFx Filter */
    ATU->NFCR &= (uint16_t)(~(uint16_t)(TOPC_FILTER_MASK << ATU_ProtectIFx));
    /* Configure the Protect IFx */
    tmptpps |= (ATU_ProtectIFInitStruct->ATU_ProtectSource    |\
                ATU_ProtectIFInitStruct->ATU_ProtectPolarity  |\
                ATU_ProtectIFInitStruct->ATU_ProtectCmd       |\
                ATU_ProtectIFInitStruct->ATU_AutoClearProtect) << ATU_ProtectIFx;

    /* Configure the NFCR Register  */
    ATU->NFCR |= ATU_ProtectIFInitStruct->ATU_Filter << ATU_ProtectIFx;
    /* Configure the TPPS Register  */
    ATU->TPPS = tmptpps;
}

/**
  * @brief  Fills each ATU_ProtectIFInitStruct member with its default value.
  * @param  ATU_ProtectIFInitStruct: pointer to a ATU_ProtectIFInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void ATU_ProtectIFInitStructInit(ATU_ProtectIFInitTypeDef* ATU_ProtectIFInitStruct)
{
    /* Set the default configuration */
    ATU_ProtectIFInitStruct->ATU_AutoClearProtect = ATU_PROTECT_AUTOCLEAR_DISABLE;
    ATU_ProtectIFInitStruct->ATU_Filter = 0;
    ATU_ProtectIFInitStruct->ATU_ProtectCmd = ATU_PROTECT_STATE_DISABLE;
    ATU_ProtectIFInitStruct->ATU_ProtectPolarity = ATU_PROTECT_POLARITY_NORMAL;
    ATU_ProtectIFInitStruct->ATU_ProtectSource = ATU_PROTECT_SOURCE_BKIN;
}

/**
  * @brief  Sets the ATU Compare0A and CompareB Register value
  * @param  CompareA: specifies the Compare0A register new value.
  * @param  CompareB: specifies the Compare0B register new value.
  * @retval None
  */
void ATU_SetCompare0(uint16_t CompareA, uint16_t CompareB)
{
    /* Set the Capture Compare0 Register value */
    ATU->CR0A = CompareA;
    ATU->CR0B = CompareB;
}

/**
  * @brief  Sets the ATU Compare1A and CompareB Register value
  * @param  CompareA: specifies the Compare1A register new value.
  * @param  CompareB: specifies the Compare1B register new value.
  * @retval None
  */
void ATU_SetCompare1(uint16_t CompareA, uint16_t CompareB)
{
    /* Set the Capture Compare1 Register value */
    ATU->CR1A = CompareA;
    ATU->CR1B = CompareB;
}

/**
  * @brief  Sets the ATU Compare2A and CompareB Register value
  * @param  CompareA: specifies the Compare2A register new value.
  * @param  CompareB: specifies the Compare2B register new value.
  * @retval None
  */
void ATU_SetCompare2(uint16_t CompareA, uint16_t CompareB)
{
    /* Set the Capture Compare2 Register value */
    ATU->CR2A = CompareA;
    ATU->CR2B = CompareB;
}


/**
  * @brief  Sets the ATU DBTA and DBTB Register value
  * @param  DeadBandTimeA: specifies the DBTA register new value.
  * @param  DeadBandTimeB: specifies the DBTB register new value.
  * @retval None
  */
void ATU_SetDeadBandTime(uint16_t DeadBandTimeA, uint16_t DeadBandTimeB)
{
    /* Set the  DBTA and DBTB Register value */
    ATU->DBTA = DeadBandTimeA;
    ATU->DBTB = DeadBandTimeB;
}

/**
  * @brief  Sets the ATU TBTTR Register value
  * @note   After the blank area is set,the blank signal is generated to the 
  *         comparator for comparator blocking function, so that the comparator 
  *         is not affected by switching noise during PWM output.
  * @param  BlankAreaA: specifies the BlankAreaA new value in TBTTR register.
  * @param  BlankAreaB: specifies the BlankAreaA new value in TBTTR register.
  * @retval None
  */
void ATU_SetBlankAera(uint16_t BlankAreaA, uint16_t BlankAreaB)
{
    /* Set the TBTTR Register value */
    ATU->TBTTR = BlankAreaA | (BlankAreaB << ATU_TBTTR_BLTB_Pos);
}



/**
  * @brief  Sets the ATU TRG0 Register value
  * @note   When the CNT count equals TRG0 value,The ADC is triggered 
  *         or UTU is triggered synchronously.
  * @param  TriggerPoint: specifies the TriggerPoint0 in TRGMC register.
  * @param  ReloadMode: specifies the ATU TriggerPoint0 Reload mode
  *         This parameter can be one of the following values:
  *            @arg ATU_TRG0_IMMEDIATELY_RELOAD
  *            @arg ATU_TRG0_AUTO_RELOAD
  * @param  TriggerValue: specifies the TriggerPoint new value in TRG0 register.
  * @retval None
  */
void ATU_SetTrigger0(uint16_t TriggerPoint, uint16_t ReloadMode, uint16_t TriggerValue)
{
    uint16_t tmptrgmc ,tmptarcs ;

    /* Check the parameters */
    assert_param(IS_ATU_TRG0_RELOAD_MODE(ReloadMode));
    assert_param(IS_ATU_TRG0_POINT(TriggerPoint));

    /* Set the TriggerPoint0 position*/
    tmptrgmc = ATU->TRGMC;
    tmptrgmc &= (uint16_t)(~(uint16_t)(ATU_TRGMC_TRGMS0_Mask));
    tmptrgmc |= TriggerPoint;
    ATU->TRGMC = tmptrgmc;
    /* Set the Trigger reload mode */
    tmptarcs = ATU->TARCS;
    tmptarcs &= (uint16_t)(~(uint16_t)(ATU_TARCS_RELTRG0_Mask));
    tmptarcs |= ReloadMode;
    ATU->TARCS = tmptarcs;
    /* Set the TRG0 Register value */
    ATU->TRG0 = TriggerValue;
}

/**
  * @brief  Sets the ATU TRG1 Register value
  * @note   When the CNT count equals TRG1 value,The ADC is triggered 
  *         or UTU is triggered synchronously.
  * @param  TriggerPoint: specifies the TriggerPoint1 in TRGMC register.
  * @param  ReloadMode: specifies the ATU TriggerPoint1 Reload mode
  *         This parameter can be one of the following values:
  *            @arg ATU_TRG1_IMMEDIATELY_RELOAD
  *            @arg ATU_TRG1_AUTO_RELOAD
  * @param  TriggerValue: specifies the TriggerPoint new value in TRG1 register.
  * @retval None
  */
void ATU_SetTrigger1(uint16_t TriggerPoint, uint16_t ReloadMode, uint16_t TriggerValue)
{
    uint16_t tmptrgmc ,tmptarcs ;
    
    /* Check the parameters */
    assert_param(IS_ATU_TRG1_RELOAD_MODE(ReloadMode));
    assert_param(IS_ATU_TRG1_POINT(TriggerPoint));

    /* Set the TriggerPoint1 position*/
    tmptrgmc = ATU->TRGMC;
    tmptrgmc &= (uint16_t)(~(uint16_t)(ATU_TRGMC_TRGMS1_Mask));
    tmptrgmc |= TriggerPoint;
    ATU->TRGMC = tmptrgmc;
    /* Set the Trigger reload mode */
    tmptarcs = ATU->TARCS;
    tmptarcs &= (uint16_t)(~(uint16_t)(ATU_TARCS_RELTRG1_Mask));
    tmptarcs |= ReloadMode;
    ATU->TARCS = tmptarcs;
    /* Set the TRG1 Register value */
    ATU->TRG1 = TriggerValue;
}


/**
  * @brief  Sets the ATU TRGDB Register value
  * @note   It is used when the ADC GroupB performs double sampling.
  * @param  TriggerPoint: specifies the Trigger double data in TRGMC register.
  * @param  ReloadMode: specifies the ATU TriggerDoubleDataB Reload mode
  *         This parameter can be one of the following values:
  *            @arg ATU_TRIGGER_DOUBLE_DATAB_IMMEDIATELY_RELOAD
  *            @arg ATU_TRIGGER_DOUBLE_DATAB_AUTO_RELOAD
  * @param  TriggerValue: specifies the TriggerPoint new value in TRGDB register.
  * @retval None
  */
void ATU_SetTriggerDoubleDataB(uint16_t TriggerPoint, uint16_t ReloadMode, uint16_t TriggerValue)
{
    uint16_t tmptrgmc ,tmptarcs ;
    
    /* Check the parameters */
    assert_param(IS_ATU_TRIGGER_DOUBLE_DATAB_RELOAD_MODE(ReloadMode));
    assert_param(IS_ATU_TRGDB_POINT(TriggerPoint));
    
    /* Set the Trigger double data Point position*/
    tmptrgmc = ATU->TRGMC;
    tmptrgmc &= (uint16_t)(~(uint16_t)(ATU_TRGMC_TRGMSB_Mask));
    tmptrgmc |= TriggerPoint;
    ATU->TRGMC = tmptrgmc;
    /* Set the Trigger reload mode */
    tmptarcs = ATU->TARCS;
    tmptarcs &= (uint16_t)(~(uint16_t)(ATU_TARCS_RELTDB_Mask));
    tmptarcs |= ReloadMode;
    ATU->TARCS = tmptarcs;

    /* Set the TRGDB Register value */
    ATU->TRGDB = TriggerValue;
}


/**
  * @brief  Sets the ATU Compare0A and Compare0B Register value
  * @note   It is used to change the CRxA and CRxB reload mode.
  * @param  ReloadModeA: specifies the ATU CRxA Reload mode
  *         This parameter can be one of the following values:
  *            @arg ATU_CRxA_IMMEDIATELY_RELOAD
  *            @arg ATU_CRxA_AUTO_RELOAD
  * @param  ReloadModeB: specifies the ATU CRxB Reload mode
  *         This parameter can be one of the following values:
  *            @arg ATU_CRxB_IMMEDIATELY_RELOAD
  *            @arg ATU_CRxB_AUTO_RELOAD
  * @retval None
  */
void ATU_SetReloadModeCompare0(uint16_t ReloadModeA, uint16_t ReloadModeB)
{
    #define REL0A_OFFSET  0
    /* Check the parameters */
    assert_param(IS_ATU_CRxA_RELOAD_MODE(ReloadModeA));
    assert_param(IS_ATU_CRxB_RELOAD_MODE(ReloadModeB));

    /* Set the CR0A and CR0B reload mode */
    ATU->TARCS &= (uint16_t)(~(uint16_t)(ATU_TARCS_REL0A_Mask|ATU_TARCS_REL0B_Mask));
    ATU->TARCS |= (ReloadModeA|ReloadModeB) << REL0A_OFFSET;
}

/**
  * @brief  Sets the ATU Compare1A and Compare1B Register value
  * @note   It is used to change the CRxA and CRxB reload mode.
  * @param  ReloadModeA: specifies the ATU CRxA Reload mode
  *         This parameter can be one of the following values:
  *            @arg ATU_CRxA_IMMEDIATELY_RELOAD
  *            @arg ATU_CRxA_AUTO_RELOAD
  * @param  ReloadModeB: specifies the ATU CRxB Reload mode
  *         This parameter can be one of the following values:
  *            @arg ATU_CRxB_IMMEDIATELY_RELOAD
  *            @arg ATU_CRxB_AUTO_RELOAD
  * @retval None
  */
void ATU_SetReloadModeCompare1(uint16_t ReloadModeA, uint16_t ReloadModeB)
{
    #define REL1A_OFFSET  2
    /* Check the parameters */
    assert_param(IS_ATU_CRxA_RELOAD_MODE(ReloadModeA));
    assert_param(IS_ATU_CRxB_RELOAD_MODE(ReloadModeB));

    /* Set the CR1A and CR1B reload mode */
    ATU->TARCS &= (uint16_t)(~(uint16_t)(ATU_TARCS_REL1A_Mask|ATU_TARCS_REL1B_Mask));
    ATU->TARCS |= (ReloadModeA|ReloadModeB) << REL1A_OFFSET;
}

/**
  * @brief  Sets the ATU Compare2A and Compare2B Register value
  * @note   It is used to change the CRxA and CRxB reload mode.
  * @param  ReloadModeA: specifies the ATU CRxA Reload mode
  *         This parameter can be one of the following values:
  *            @arg ATU_CRxA_IMMEDIATELY_RELOAD
  *            @arg ATU_CRxA_AUTO_RELOAD
  * @param  ReloadModeB: specifies the ATU CRxB Reload mode
  *         This parameter can be one of the following values:
  *            @arg ATU_CRxB_IMMEDIATELY_RELOAD
  *            @arg ATU_CRxB_AUTO_RELOAD
  * @retval None
  */
void ATU_SetReloadModeCompare2(uint16_t ReloadModeA, uint16_t ReloadModeB)
{
    #define REL2A_OFFSET  4
    /* Check the parameters */
    assert_param(IS_ATU_CRxA_RELOAD_MODE(ReloadModeA));
    assert_param(IS_ATU_CRxB_RELOAD_MODE(ReloadModeB));

    /* Set the CR2A and CR2B reload mode */
    ATU->TARCS &= (uint16_t)(~(uint16_t)(ATU_TARCS_REL2A_Mask|ATU_TARCS_REL2B_Mask));
    ATU->TARCS |= (ReloadModeA|ReloadModeB) << REL2A_OFFSET;
}

/**
  * @brief  Configure ATU external trigger source,Sets the ATU TTSS Register value.
  * @param  ExtSource: specifies the ATU External trigger source.
  *         This parameter can be one of the following values:
  *            @arg ATU_EXT_TRIG_UTU1_TRGO
  *            @arg ATU_EXT_TRIG_UTU2_TRGO
  * @param  NewState: new state of the External trigger source command.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ATU_ExternalTriggerConfig(uint16_t ExtSource, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ATU_EXT_TRIG_SOURCE(ExtSource));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Config the external trigger source */
    ATU->TTSS = ExtSource;

    if (NewState != DISABLE)
    {
      /* Enable the External trigger function */
      ATU->TTSS |= ATU_TTSS_TTEN;
    }
    else
    {
      /* Disable the External trigger function */
      ATU->TTSS &= (uint16_t)(~((uint16_t)ATU_TTSS_TTEN));
    }
}

/**
  * @brief  Enables or disables ATU Counter.
  * @param  NewState: new state of ATU counter command.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ATU_CounterCMD(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
      /* Enable the ATU Counter */
      ATU->TCSR |= ATU_TCSR_CNT_EN;
    }
    else
    {
      /* Disable the ATU Counter */
      ATU->TCSR &= (uint16_t)(~((uint16_t)ATU_TCSR_CNT_EN));
    }
}

/**
  * @brief  Configure ATU channel0 output mode.
  * @param  OutputMode: specifies the ATU channel0 output mode.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_INDEPENDENT_PWM
  *            @arg ATU_OUTPUT_COMPLEMENTARY_PWM
  *            @arg ATU_OUTPUT_INDEPENDENT_TOGGLE
  * @retval None
  */
void ATU_ConfigOutputModeChannel0(uint16_t OutputMode)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_MODE(OutputMode));

    /* Change the output channel0 mode */
    ATU->TPOC &= (uint32_t)(~(uint32_t)ATU_TPOC_TOMOD0_Mask);
    ATU->TPOC |= OutputMode << Aligned_8BIT(ATU_TPOC_TOMOD0_Pos);
}

/**
  * @brief  Configure ATU channel1 output mode.
  * @param  OutputMode: specifies the ATU channel1 output mode.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_INDEPENDENT_PWM
  *            @arg ATU_OUTPUT_COMPLEMENTARY_PWM
  *            @arg ATU_OUTPUT_INDEPENDENT_TOGGLE
  * @retval None
  */
void ATU_ConfigOutputModeChannel1(uint16_t OutputMode)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_MODE(OutputMode));

    /* Change the output channel1 mode */
    ATU->TPOC &= (uint32_t)(~(uint32_t)ATU_TPOC_TOMOD1_Mask);
    ATU->TPOC |= OutputMode << Aligned_8BIT(ATU_TPOC_TOMOD1_Pos);
}

/**
  * @brief  Configure ATU channel2 output mode.
  * @param  OutputMode: specifies the ATU channel2 output mode.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_INDEPENDENT_PWM
  *            @arg ATU_OUTPUT_COMPLEMENTARY_PWM
  *            @arg ATU_OUTPUT_INDEPENDENT_TOGGLE
  * @retval None
  */
void ATU_ConfigOutputModeChannel2(uint16_t OutputMode)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_MODE(OutputMode));

    /* Change the output channel2 mode */
    ATU->TPOC &= (uint32_t)(~(uint32_t)ATU_TPOC_TOMOD2_Mask);
    ATU->TPOC |= OutputMode << Aligned_8BIT(ATU_TPOC_TOMOD2_Pos);
}

/**
  * @brief  Configure ATU channel0 output polarity.
  * @param  PolarityA: specifies the ATU channel0 output PolarityA.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_POLARITYA_NORMAL
  *            @arg ATU_OUTPUT_POLARITYA_REVERSE
  * @param  PolarityB: specifies the ATU channel0 output PolarityB.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_POLARITYB_NORMAL
  *            @arg ATU_OUTPUT_POLARITYB_REVERSE
  * @retval None
  */
void ATU_ConfigOutputPolarityChannel0(uint16_t PolarityA, uint16_t PolarityB)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_POLARITYA(PolarityA));
    assert_param(IS_ATU_OUTPUT_POLARITYB(PolarityB));

    /* Change the output channel0 polarity */
    ATU->TPOC &= (uint32_t)(~(uint32_t)(ATU_TPOC_TOPC0A_Mask|ATU_TPOC_TOPC0B_Mask));
    ATU->TPOC |= (PolarityA|PolarityB) << Aligned_8BIT(ATU_TPOC_TOPC0A_Pos);
}

/**
  * @brief  Configure ATU channel1 output polarity.
  * @param  PolarityA: specifies the ATU channel1 output PolarityA.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_POLARITYA_NORMAL
  *            @arg ATU_OUTPUT_POLARITYA_REVERSE
  * @param  PolarityB: specifies the ATU channel1 output PolarityB.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_POLARITYB_NORMAL
  *            @arg ATU_OUTPUT_POLARITYB_REVERSE
  * @retval None
  */
void ATU_ConfigOutputPolarityChannel1(uint16_t PolarityA, uint16_t PolarityB)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_POLARITYA(PolarityA));
    assert_param(IS_ATU_OUTPUT_POLARITYB(PolarityB));

    /* Change the output channel1 polarity */
    ATU->TPOC &= (uint32_t)(~(uint32_t)(ATU_TPOC_TOPC1A_Mask|ATU_TPOC_TOPC1B_Mask));
    ATU->TPOC |= (PolarityA|PolarityB) << Aligned_8BIT(ATU_TPOC_TOPC1A_Pos);
}

/**
  * @brief  Configure ATU channel2 output polarity.
  * @param  PolarityA: specifies the ATU channel2 output PolarityA.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_POLARITYA_NORMAL
  *            @arg ATU_OUTPUT_POLARITYA_REVERSE
  * @param  PolarityB: specifies the ATU channel2 output PolarityB.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_POLARITYB_NORMAL
  *            @arg ATU_OUTPUT_POLARITYB_REVERSE
  * @retval None
  */
void ATU_ConfigOutputPolarityChannel2(uint16_t PolarityA, uint16_t PolarityB)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_POLARITYA(PolarityA));
    assert_param(IS_ATU_OUTPUT_POLARITYB(PolarityB));

    /* Change the output channel2 polarity */
    ATU->TPOC &= (uint32_t)(~(uint32_t)(ATU_TPOC_TOPC2A_Mask|ATU_TPOC_TOPC2B_Mask));
    ATU->TPOC |= (PolarityA|PolarityB) << Aligned_8BIT(ATU_TPOC_TOPC2A_Pos);
}

/**
  * @brief  Configure ATU channel0 output Source.
  * @param  SourceA: specifies the ATU channel0 output SourceA.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_SOURCEA_COUNTER
  *            @arg ATU_OUTPUT_SOURCEA_IO_SET
  * @param  SourceB: specifies the ATU channel0 output SourceB.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_SOURCEB_COUNTER
  *            @arg ATU_OUTPUT_SOURCEB_IO_SET
  * @retval None
  */
void ATU_ConfigOutputSourceChannel0(uint16_t SourceA, uint16_t SourceB)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_SOURCEA(SourceA));
    assert_param(IS_ATU_OUTPUT_SOURCEB(SourceB));

    /* Change the output channel0 Source */
    ATU->TPOC &= (uint32_t)(~(uint32_t)(ATU_TPOC_TOSSL0A_Mask|ATU_TPOC_TOSSL0B_Mask));
    ATU->TPOC |= (SourceA|SourceB) << Aligned_8BIT(ATU_TPOC_TOSSL0A_Pos);
}

/**
  * @brief  Configure ATU channel1 output Source.
  * @param  SourceA: specifies the ATU channel1 output SourceA.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_SOURCEA_COUNTER
  *            @arg ATU_OUTPUT_SOURCEA_IO_SET
  * @param  SourceB: specifies the ATU channel1 output SourceB.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_SOURCEB_COUNTER
  *            @arg ATU_OUTPUT_SOURCEB_IO_SET
  * @retval None
  */
void ATU_ConfigOutputSourceChannel1(uint16_t SourceA, uint16_t SourceB)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_SOURCEA(SourceA));
    assert_param(IS_ATU_OUTPUT_SOURCEB(SourceB));

    /* Change the output channel1 Source */
    ATU->TPOC &= (uint32_t)(~(uint32_t)(ATU_TPOC_TOSSL1A_Mask|ATU_TPOC_TOSSL1B_Mask));
    ATU->TPOC |= (SourceA|SourceB) << Aligned_8BIT(ATU_TPOC_TOSSL1A_Pos);
}

/**
  * @brief  Configure ATU channel2 output Source.
  * @param  SourceA: specifies the ATU channel2 output SourceA.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_SOURCEA_COUNTER
  *            @arg ATU_OUTPUT_SOURCEA_IO_SET
  * @param  SourceB: specifies the ATU channel2 output SourceB.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_SOURCEB_COUNTER
  *            @arg ATU_OUTPUT_SOURCEB_IO_SET
  * @retval None
  */
void ATU_ConfigOutputSourceChannel2(uint16_t SourceA, uint16_t SourceB)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_SOURCEA(SourceA));
    assert_param(IS_ATU_OUTPUT_SOURCEB(SourceB));

    /* Change the output channel2 Source */
    ATU->TPOC &= (uint32_t)(~(uint32_t)(ATU_TPOC_TOSSL2A_Mask|ATU_TPOC_TOSSL2B_Mask));
    ATU->TPOC |= (SourceA|SourceB) << Aligned_8BIT(ATU_TPOC_TOSSL2A_Pos);
}


/**
  * @brief  Configure ATU channel0 output Set-Level.
  * @param  LevelA: specifies the ATU channel0 output Set-LevelA.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_LEVEL_SETA_LOW
  *            @arg ATU_OUTPUT_LEVEL_SETA_HIGH
  * @param  LevelB: specifies the ATU channel0 output Set-LevelB.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_LEVEL_SETB_LOW
  *            @arg ATU_OUTPUT_LEVEL_SETB_HIGH
  * @retval None
  */
void ATU_ConfigOutputLeveLChannel0(uint16_t LevelA, uint16_t LevelB)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_LEVEL_SETA(LevelA));
    assert_param(IS_ATU_OUTPUT_LEVEL_SETB(LevelB));

    /* Change the output channel0 Set Level */
    ATU->TPOC &= (uint32_t)(~(uint32_t)(ATU_TPOC_TIOMS0A_Mask|ATU_TPOC_TIOMS0B_Mask));
    ATU->TPOC |= (LevelA|LevelB) << Aligned_8BIT(ATU_TPOC_TIOMS0A_Pos);
}

/**
  * @brief  Configure ATU channel1 output Set-Level.
  * @param  LevelA: specifies the ATU channel1 output Set-LevelA.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_LEVEL_SETA_LOW
  *            @arg ATU_OUTPUT_LEVEL_SETA_HIGH
  * @param  LevelB: specifies the ATU channel1 output Set-LevelB.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_LEVEL_SETB_LOW
  *            @arg ATU_OUTPUT_LEVEL_SETB_HIGH
  * @retval None
  */
void ATU_ConfigOutputLeveLChannel1(uint16_t LevelA, uint16_t LevelB)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_LEVEL_SETA(LevelA));
    assert_param(IS_ATU_OUTPUT_LEVEL_SETB(LevelB));

    /* Change the output channel1 Set Level */
    ATU->TPOC &= (uint32_t)(~(uint32_t)(ATU_TPOC_TIOMS1A_Mask|ATU_TPOC_TIOMS1B_Mask));
    ATU->TPOC |= (LevelA|LevelB) << Aligned_8BIT(ATU_TPOC_TIOMS1A_Pos);
}

/**
  * @brief  Configure ATU channel2 output Set-Level.
  * @param  LevelA: specifies the ATU channel2 output Set-LevelA.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_LEVEL_SETA_LOW
  *            @arg ATU_OUTPUT_LEVEL_SETA_HIGH
  * @param  LevelB: specifies the ATU channel2 output Set-LevelB.
  *         This parameter can be one of the following values:
  *            @arg ATU_OUTPUT_LEVEL_SETB_LOW
  *            @arg ATU_OUTPUT_LEVEL_SETB_HIGH
  * @retval None
  */
void ATU_ConfigOutputLeveLChannel2(uint16_t LevelA, uint16_t LevelB)
{
    /* Check the parameters */
    assert_param(IS_ATU_OUTPUT_LEVEL_SETA(LevelA));
    assert_param(IS_ATU_OUTPUT_LEVEL_SETB(LevelB));

    /* Change the output channel2 Set Level */
    ATU->TPOC &= (uint32_t)(~(uint32_t)(ATU_TPOC_TIOMS2A_Mask|ATU_TPOC_TIOMS2B_Mask));
    ATU->TPOC |= (LevelA|LevelB) << Aligned_8BIT(ATU_TPOC_TIOMS2A_Pos);
}


/**
  * @brief  Enables or disables ATU PWM output.
  * @note   If disabled ATU PWM output,PWM channels Output protection Default value.
  * @param  NewState: new state of ATU PWM output status.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ATU_PWMOutputCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
      /* Enable the PWM output */
      ATU->TPPS &= (uint32_t)(~((uint32_t)ATU_TPPS_TPOE_Mask));
      ATU->TPPS |= ATU_TPPS_TPOE;
    }
    else
    {
      /* Disable the PWM output,PWM channels Output protection Default value */
      ATU->TPPS &= (uint32_t)(~((uint32_t)ATU_TPPS_TPOE));
    }
}

/**
  * @brief  Configure TIOxA and TIOxB Remap to PWM_IOx Channels.
  * @param  PWMChannel: specifies the ATU PWM channel on the pins.
  *         This parameter can be one of the following values:
  *            @arg ATU_PWMIO_CHNANEL_0
  *            @arg ATU_PWMIO_CHNANEL_1
  *            @arg ATU_PWMIO_CHNANEL_2
  *            @arg ATU_PWMIO_CHNANEL_3
  *            @arg ATU_PWMIO_CHNANEL_4
  *            @arg ATU_PWMIO_CHNANEL_5
  * @param  PWMRemapSource: specifies the remap source from ATU TIOxA and TIOxB.
  *         This parameter can be one of the following values:
  *            @arg ATU_PWM_REMAP_SOURCE_TIO0A
  *            @arg ATU_PWM_REMAP_SOURCE_TIO0B
  *            @arg ATU_PWM_REMAP_SOURCE_TIO1A
  *            @arg ATU_PWM_REMAP_SOURCE_TIO1B
  *            @arg ATU_PWM_REMAP_SOURCE_TIO2A
  *            @arg ATU_PWM_REMAP_SOURCE_TIO2B
  * @retval None
  */
void ATU_PWMIORemap(uint16_t PWMChannel, uint16_t PWMRemapSource)
{
    /* Check the parameters */
    assert_param(IS_ATU_PWMIO_CHNANEL(PWMChannel));
    assert_param(IS_ATU_PWM_REMAP_SOURCE(PWMRemapSource));

    /* Set TIOxA and TIOxB Remap to PWM_IOx Channels */
    ATU->TIOMX &= (uint32_t)(~(uint32_t)(ATU_TIOMX_PWM_IO0_Mask << PWMChannel));
    ATU->TIOMX |= (uint32_t)PWMRemapSource << PWMChannel;
}

/**
  * @brief  Configure TIOxA and TIOxB Remap to PWM_IOx Channels.
  * @retval The Counter Direction state (DOWN_DIR or UP_DIR).
  */
ATU_CounterDirection ATU_GetCounterDirection(void)
{
    ATU_CounterDirection tmpdir = UP_DIR;

    if ((ATU->TSR & ATU_TSR_TCDF) != (uint32_t)RESET)
    {
      tmpdir = DOWN_DIR;
    }
    else
    {
      tmpdir = UP_DIR;
    }
    return tmpdir;
}

/**
  * @brief  Configure ATU Crest Reload Timing Point,Sets the ATU TRTC Register value.
  * @param  ReloadTimingPoint: specifies the ATU Crest Reload Timing Point.
  *         This parameter can be one of the following values:
  *            @arg ATU_CREST_RELOAD_EVERY_1_EVENT
  *            @arg ATU_CREST_RELOAD_EVERY_2_EVENT
  *            @arg ATU_CREST_RELOAD_EVERY_3_EVENT
  *            @arg ATU_CREST_RELOAD_EVERY_4_EVENT
  * @param  NewState: new state of the Crest Reload command.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ATU_CrestReloadTimingPoint(uint16_t ReloadTimingPoint, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ATU_CREST_RELOAD_TIMING_POINT(ReloadTimingPoint));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Config the ATU Crest Reload Timing Point */
    ATU->TRTC &= (uint8_t)(~(uint8_t)(ATU_TRTC_CRTS_Mask));
    ATU->TRTC |= (uint8_t)ReloadTimingPoint;

    if (NewState != DISABLE)
    {
      /* Enable the Crest Reload command */
      ATU->TRTC |= ATU_TRTC_CREN;
    }
    else
    {
      /* Disable the Crest Reload command */
      ATU->TRTC &= (uint16_t)(~((uint16_t)ATU_TRTC_CREN));
    }
}


/**
  * @brief  Configure ATU Trough Reload Timing Point,Sets the ATU TRTC Register value.
  * @param  ReloadTimingPoint: specifies the ATU Trough Reload Timing Point.
  *         This parameter can be one of the following values:
  *            @arg ATU_TROUGH_RELOAD_EVERY_1_EVENT
  *            @arg ATU_TROUGH_RELOAD_EVERY_2_EVENT
  *            @arg ATU_TROUGH_RELOAD_EVERY_3_EVENT
  *            @arg ATU_TROUGH_RELOAD_EVERY_4_EVENT
  * @param  NewState: new state of the Trough Reload command.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ATU_TroughReloadTimingPoint(uint16_t ReloadTimingPoint, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ATU_TROUGH_RELOAD_TIMING_POINT(ReloadTimingPoint));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Config the ATU Trough Reload Timing Point */
    ATU->TRTC &= (uint8_t)(~(uint8_t)(ATU_TRTC_TRTS_Mask));
    ATU->TRTC |= (uint8_t)ReloadTimingPoint;

    if (NewState != DISABLE)
    {
      /* Enable the Trough Reload command */
      ATU->TRTC |= ATU_TRTC_TREN;
    }
    else
    {
      /* Disable the Trough Reload command */
      ATU->TRTC &= (uint16_t)(~((uint16_t)ATU_TRTC_TREN));
    }
}

/**
  * @brief  Configure the TIO0A and TIO0B output when ProtectIF is active .
  * @note   It's used to configure the TIO0A and TIO0B output levels,when ProtectIF is active,
  *         Select the default level settings or the TPOC settings.
  * @param  DefaultLevelA: specifies the ATU TIO0A level when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO0A_DEFAULT_LEVEL_LOW 
  *           @arg ATU_PROTECT_TIO0A_DEFAULT_LEVEL_HIGH
  * @param  DefaultLevelB: specifies the ATU TIO0B level when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO0B_DEFAULT_LEVEL_LOW 
  *           @arg ATU_PROTECT_TIO0B_DEFAULT_LEVEL_HIGH
  * @param  ProtectOutputSelA: specifies the ATU TIO0A output Source when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO0A_OUTPUT_SELECT_DEFAULT_LEVLE 
  *           @arg ATU_PROTECT_TIO0A_OUTPUT_SELECT_TPOC_SETTING  
  * @param  ProtectOutputSelB: specifies the ATU TIO0B output Source when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO0B_OUTPUT_SELECT_DEFAULT_LEVLE 
  *           @arg ATU_PROTECT_TIO0B_OUTPUT_SELECT_TPOC_SETTING  
  * @retval None
  */
void ATU_ProtectOutputSetting0(uint32_t DefaultLevelA, uint32_t DefaultLevelB, \
                               uint32_t ProtectOutputSelA, uint32_t ProtectOutputSelB)
{
    /* Check the parameters */
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO0A(DefaultLevelA));
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO0B(DefaultLevelB));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO0A(ProtectOutputSelA));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO0A(ProtectOutputSelB));
    
    /*  Configure the TIO0A and TIO0B output when ProtectIF is active */
    ATU->TPPS &= (uint32_t)(~(uint32_t)(ATU_TPPS_TDO0A_Mask |\
                                        ATU_TPPS_TDO0B_Mask |\
                                        ATU_TPPS_TIOM0A_Mask |\
                                        ATU_TPPS_TIOM0B_Mask));
    ATU->TPPS |= (uint32_t)(DefaultLevelA | DefaultLevelB |\
                           ProtectOutputSelA | ProtectOutputSelB);
}

/**
  * @brief  Configure the TIO1A and TIO1B output when ProtectIF is active .
  * @note   It's used to configure the TIO1A and TIO1B output levels,when ProtectIF is active,
  *         Select the default level settings or the TPOC settings.
  * @param  DefaultLevelA: specifies the ATU TIO1A level when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO1A_DEFAULT_LEVEL_LOW 
  *           @arg ATU_PROTECT_TIO1A_DEFAULT_LEVEL_HIGH
  * @param  DefaultLevelB: specifies the ATU TIO1B level when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO1B_DEFAULT_LEVEL_LOW 
  *           @arg ATU_PROTECT_TIO1B_DEFAULT_LEVEL_HIGH
  * @param  ProtectOutputSelA: specifies the ATU TIO1A output Source when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO1A_OUTPUT_SELECT_DEFAULT_LEVLE 
  *           @arg ATU_PROTECT_TIO1A_OUTPUT_SELECT_TPOC_SETTING  
  * @param  ProtectOutputSelB: specifies the ATU TIO1B output Source when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO1B_OUTPUT_SELECT_DEFAULT_LEVLE 
  *           @arg ATU_PROTECT_TIO1B_OUTPUT_SELECT_TPOC_SETTING  
  * @retval None
  */
void ATU_ProtectOutputSetting1(uint32_t DefaultLevelA, uint32_t DefaultLevelB, \
                               uint32_t ProtectOutputSelA, uint32_t ProtectOutputSelB)
{
    /* Check the parameters */
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO1A(DefaultLevelA));
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO1B(DefaultLevelB));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO1A(ProtectOutputSelA));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO1A(ProtectOutputSelB));
    
    /*  Configure the TIO1A and TIO1B output when ProtectIF is active */
    ATU->TPPS &= (uint32_t)(~(uint32_t)(ATU_TPPS_TDO1A_Mask |\
                                        ATU_TPPS_TDO1B_Mask |\
                                        ATU_TPPS_TIOM1A_Mask |\
                                        ATU_TPPS_TIOM1B_Mask));
    ATU->TPPS |= (uint32_t)(DefaultLevelA | DefaultLevelB |\
                           ProtectOutputSelA | ProtectOutputSelB);
}

/**
  * @brief  Configure the TIO2A and TIO2B output when ProtectIF is active .
  * @note   It's used to configure the TIO2A and TIO2B output levels,when ProtectIF is active,
  *         Select the default level settings or the TPOC settings.
  * @param  DefaultLevelA: specifies the ATU TIO2A level when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO2A_DEFAULT_LEVEL_LOW 
  *           @arg ATU_PROTECT_TIO2A_DEFAULT_LEVEL_HIGH
  * @param  DefaultLevelB: specifies the ATU TIO2B level when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO2B_DEFAULT_LEVEL_LOW 
  *           @arg ATU_PROTECT_TIO2B_DEFAULT_LEVEL_HIGH
  * @param  ProtectOutputSelA: specifies the ATU TIO2A output Source when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO2A_OUTPUT_SELECT_DEFAULT_LEVLE 
  *           @arg ATU_PROTECT_TIO2A_OUTPUT_SELECT_TPOC_SETTING  
  * @param  ProtectOutputSelB: specifies the ATU TIO2B output Source when ProtectIF is active.
  *         This parameter can be one of the following values:
  *           @arg ATU_PROTECT_TIO2B_OUTPUT_SELECT_DEFAULT_LEVLE 
  *           @arg ATU_PROTECT_TIO2B_OUTPUT_SELECT_TPOC_SETTING  
  * @retval None
  */
void ATU_ProtectOutputSetting2(uint32_t DefaultLevelA, uint32_t DefaultLevelB, \
                               uint32_t ProtectOutputSelA, uint32_t ProtectOutputSelB)
{
    /* Check the parameters */
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO2A(DefaultLevelA));
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO2B(DefaultLevelB));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO2A(ProtectOutputSelA));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO2A(ProtectOutputSelB));
    
    /*  Configure the TIO2A and TIO2B output when ProtectIF is active */
    ATU->TPPS &= (uint32_t)(~(uint32_t)(ATU_TPPS_TDO2A_Mask |\
                                        ATU_TPPS_TDO2B_Mask |\
                                        ATU_TPPS_TIOM2A_Mask |\
                                        ATU_TPPS_TIOM2B_Mask));
    ATU->TPPS |= (uint32_t)(DefaultLevelA | DefaultLevelB |\
                           ProtectOutputSelA | ProtectOutputSelB);
}




/**
  * @brief  Configure ATU Protect output channels when ProtectIF is active 
  * @param  ATU_ProtectOutputInitStruct: pointer to a ATU_ProtectOutputInitTypeDef
  *         structure that contains the configuration information for the specified
  *         ATU Protect output channel.
  * @retval None
  */
void ATU_ProtectOutputConfig(ATU_ProtectOutputInitTypeDef* ATU_ProtectOutputInitStruct)
{
    #define ATU_PROTECT_OUTPUT_MASK   ((uint32_t)0x3F3F0000)
    /* Check the parameters */
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO0A(ATU_ProtectOutputInitStruct->ATU_ProtectOutput0DefaultLevelA));
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO0B(ATU_ProtectOutputInitStruct->ATU_ProtectOutput0DefaultLevelB));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO0A(ATU_ProtectOutputInitStruct->ATU_ProtectOutput0SelectA));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO0B(ATU_ProtectOutputInitStruct->ATU_ProtectOutput0SelectB));
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO1A(ATU_ProtectOutputInitStruct->ATU_ProtectOutput1DefaultLevelA));
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO1B(ATU_ProtectOutputInitStruct->ATU_ProtectOutput1DefaultLevelB));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO1A(ATU_ProtectOutputInitStruct->ATU_ProtectOutput1SelectA));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO1B(ATU_ProtectOutputInitStruct->ATU_ProtectOutput1SelectB));
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO2A(ATU_ProtectOutputInitStruct->ATU_ProtectOutput2DefaultLevelA));
    assert_param(IS_ATU_PROTECT_DEFAULT_LEVEL_TIO2B(ATU_ProtectOutputInitStruct->ATU_ProtectOutput2DefaultLevelB));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO2A(ATU_ProtectOutputInitStruct->ATU_ProtectOutput2SelectA));
    assert_param(IS_ATU_PROTECT_OUTPUT_SELECT_TIO2B(ATU_ProtectOutputInitStruct->ATU_ProtectOutput2SelectB));

    /*  Configure the ATU protect output when ProtectIF is active */
    ATU->TPPS &= (uint32_t)(~(uint32_t)ATU_PROTECT_OUTPUT_MASK);
    ATU->TPPS |= (uint32_t)(ATU_ProtectOutputInitStruct->ATU_ProtectOutput0DefaultLevelA |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput0DefaultLevelB |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput0SelectA       |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput0SelectB       |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput1DefaultLevelA |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput1DefaultLevelB |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput1SelectA       |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput1SelectB       |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput2DefaultLevelA |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput2DefaultLevelB |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput2SelectA       |\
                            ATU_ProtectOutputInitStruct->ATU_ProtectOutput2SelectB);
}

/**
  * @brief  Fills each ATU_ProtectOutputInitStruct member with its default value.
  * @param  ATU_ProtectOutputInitStruct: pointer to a ATU_ProtectOutputInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void ATU_ProtectOutputStructInit(ATU_ProtectOutputInitTypeDef* ATU_ProtectOutputInitStruct)
{
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput0DefaultLevelA = ATU_PROTECT_TIO0A_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput0DefaultLevelB = ATU_PROTECT_TIO0B_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput0SelectA = ATU_PROTECT_TIO0A_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput0SelectB = ATU_PROTECT_TIO0B_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput1DefaultLevelA = ATU_PROTECT_TIO1A_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput1DefaultLevelB = ATU_PROTECT_TIO1B_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput1SelectA = ATU_PROTECT_TIO1A_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput1SelectB = ATU_PROTECT_TIO1B_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput2DefaultLevelA = ATU_PROTECT_TIO2A_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput2DefaultLevelB = ATU_PROTECT_TIO2B_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput2SelectA = ATU_PROTECT_TIO2A_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputInitStruct->ATU_ProtectOutput2SelectB = ATU_PROTECT_TIO2B_OUTPUT_SELECT_DEFAULT_LEVLE;
}


/**
  * @brief  Configure the ATU PWM output status when CPU is halted in debug mode .
  * @param  DebugHaltPWMOutputSel: specifies the ATU PWM output status .
  *         This parameter can be one of the following values:
  *            @arg ATU_DEBUG_HALT_PWM_OUTPUT_DEFAULT_LEVLE
  *            @arg ATU_DEBUG_HALT_PWM_OUTPUT_NORMAL
  * @retval None
  */
void ATU_DebugHaltPWMOutput(uint32_t DebugHaltPWMOutputSel)
{
    /* Check the parameters */
    assert_param(IS_DEBUG_HALT_PWM_OUTPUT_SEL(DebugHaltPWMOutputSel));

    /* Configure the ATU PWM output status when CPU is halted in debug mode */
    ATU->TPPS &= (uint32_t)(~((uint32_t)ATU_TPPS_TPHMS_Mask));
    ATU->TPPS |= (uint32_t)DebugHaltPWMOutputSel;
}

/**
  * @brief  Enables or disables the specified ATU interrupts.
  * @param  ATU_IT: specifies the ATU interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *            @arg ATU_IT_CR0A  
  *            @arg ATU_IT_CR0B  
  *            @arg ATU_IT_CR1A  
  *            @arg ATU_IT_CR1B  
  *            @arg ATU_IT_CR2A  
  *            @arg ATU_IT_CR2B  
  *            @arg ATU_IT_CREST 
  *            @arg ATU_IT_TROUGH
  *            @arg ATU_IT_RELOAD
  *            @arg ATU_IT_TRG0  
  *            @arg ATU_IT_TRG1  
  *            @arg ATU_IT_TRGDB 
  *            @arg ATU_IT_TPP0  
  *            @arg ATU_IT_TPP1 
  * @param  NewState: new state of the ATU interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ATU_ITConfig(uint32_t ATU_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ATU_IT(ATU_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Interrupt sources */
        ATU->TIER  |= (uint16_t)ATU_IT;
        ATU->TPIER |= (uint8_t)(ATU_IT >> 16);
    }
    else
    {
        /* Disable the Interrupt sources */
        ATU->TIER  &= (uint16_t)~ATU_IT;
        ATU->TPIER &= (uint8_t)~(ATU_IT >> 16);
    }
}

/**
  * @brief  Checks whether the specified ATU flag is set or not.
  * @param  ATU_FLAG: specifies the flag to check.
  *         This parameter can be any combination of the following values:
  *            @arg ATU_FLAG_CR0A  
  *            @arg ATU_FLAG_CR0B  
  *            @arg ATU_FLAG_CR1A  
  *            @arg ATU_FLAG_CR1B  
  *            @arg ATU_FLAG_CR2A  
  *            @arg ATU_FLAG_CR2B  
  *            @arg ATU_FLAG_CREST 
  *            @arg ATU_FLAG_TROUGH
  *            @arg ATU_FLAG_RELOAD
  *            @arg ATU_FLAG_TRG0  
  *            @arg ATU_FLAG_TRG1  
  *            @arg ATU_FLAG_TRGDB 
  *            @arg ATU_FLAG_TPP0  
  *            @arg ATU_FLAG_TPP1 
  * @retval The new state of ATU_FLAG (SET or RESET).
  */
FlagStatus ATU_GetFlagStatus(uint32_t ATU_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_ATU_GET_FLAG(ATU_FLAG));

    if (((ATU->TIFR & (uint16_t)ATU_FLAG) != (uint16_t)RESET) || \
        ((ATU->TPIFR & (uint8_t)(ATU_FLAG >> 16)) != (uint8_t)RESET))
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
  * @brief  Clears the ATU pending flags.
  * @param  ATU_FLAG: specifies the flag to check.
  *         This parameter can be any combination of the following values:
  *            @arg ATU_FLAG_CR0A  
  *            @arg ATU_FLAG_CR0B  
  *            @arg ATU_FLAG_CR1A  
  *            @arg ATU_FLAG_CR1B  
  *            @arg ATU_FLAG_CR2A  
  *            @arg ATU_FLAG_CR2B  
  *            @arg ATU_FLAG_CREST 
  *            @arg ATU_FLAG_TROUGH
  *            @arg ATU_FLAG_RELOAD
  *            @arg ATU_FLAG_TRG0  
  *            @arg ATU_FLAG_TRG1  
  *            @arg ATU_FLAG_TRGDB 
  *            @arg ATU_FLAG_TPP0  
  *            @arg ATU_FLAG_TPP1 
  * @retval None
  */
void ATU_ClearFlag(uint32_t ATU_FLAG)
{
    /* Check the parameters */
    assert_param(IS_ATU_CLEAR_FLAG(ATU_FLAG));
    /* Clear the flags */
    ATU->TIFR  = (uint16_t)ATU_FLAG;
    ATU->TPIFR = (uint8_t)(ATU_FLAG >> 16);
}


/**
  * @brief  Checks whether the ATU interrupt has occurred or not.
  * @param  ATU_IT: specifies the ATU interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *            @arg ATU_IT_CR0A  
  *            @arg ATU_IT_CR0B  
  *            @arg ATU_IT_CR1A  
  *            @arg ATU_IT_CR1B  
  *            @arg ATU_IT_CR2A  
  *            @arg ATU_IT_CR2B  
  *            @arg ATU_IT_CREST 
  *            @arg ATU_IT_TROUGH
  *            @arg ATU_IT_RELOAD
  *            @arg ATU_IT_TRG0  
  *            @arg ATU_IT_TRG1  
  *            @arg ATU_IT_TRGDB 
  *            @arg ATU_IT_TPP0  
  *            @arg ATU_IT_TPP1 
  * @retval The new state of ATU_IT (SET or RESET).
  */
ITStatus ATU_GetITStatus(uint32_t ATU_IT)
{
    ITStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_ATU_GET_IT(ATU_IT));

    if ((((ATU->TIFR & (uint16_t)ATU_IT) != (uint16_t)RESET)  && \
         ((ATU->TIER & (uint16_t)ATU_IT) != (uint16_t)RESET)) || \
        (((ATU->TPIFR & (uint8_t)(ATU_IT >> 16)) != (uint8_t)RESET) && \
         ((ATU->TPIER & (uint8_t)(ATU_IT >> 16)) != (uint8_t)RESET)))
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
  * @brief  Clears the ATU interrupt pending bits.
  * @param  ATU_IT: specifies the ATU interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *            @arg ATU_IT_CR0A  
  *            @arg ATU_IT_CR0B  
  *            @arg ATU_IT_CR1A  
  *            @arg ATU_IT_CR1B  
  *            @arg ATU_IT_CR2A  
  *            @arg ATU_IT_CR2B  
  *            @arg ATU_IT_CREST 
  *            @arg ATU_IT_TROUGH
  *            @arg ATU_IT_RELOAD
  *            @arg ATU_IT_TRG0  
  *            @arg ATU_IT_TRG1  
  *            @arg ATU_IT_TRGDB 
  *            @arg ATU_IT_TPP0  
  *            @arg ATU_IT_TPP1 
  * @retval None
  */
void ATU_ClearITPendingBit(uint32_t ATU_IT)
{
    /* Check the parameters */
    assert_param(IS_ATU_IT(ATU_IT));
    /* Clear the IT pending Bit */
    ATU->TIFR  = (uint16_t)ATU_IT;
    ATU->TPIFR = (uint8_t)(ATU_IT >> 16);
}



/**
  * @brief  Enables or disables the ATU Registers protection.
  * @param  NewState: new state of the ATU Registers protection .
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ATU_RegisterProtectUnlockCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Disable ATU Registers protection, write the unlock key*/
        ATU->TRWPT  =  0xA5A5;
    }
    else
    {
        /* Enable ATU Registers protection, write lock key */
        ATU->TRWPT  =  0x0000;
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
/**
  * @}
  */
