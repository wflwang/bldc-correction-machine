/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_adc.c
* @brief:     This file provides firmware functions to manage the following
*             functionalities of the Analog to Digital Convertor (ADC) peripheral:
*             + Initialization and Configuration
*             + Temperature Sensor, Vrefint (Internal Reference Voltage) management
*             + ADC Channels Configuration
*             + ADC Channels DMA Configuration
*             + Interrupts and flags management
*             + Quueue management
* @author:    AE Team
* @version:   V1.0.0/2024-12-18
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_adc.h"

/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup ADC ADC
  * @brief ADC driver modules
  * @{
  @verbatim
================================================================================
                      ##### How to use this driver #####
================================================================================
    [..]
    (#) Enable the ADC interface clock using
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_ADC, ENABLE);
    (#) ADC pins configuration
       (++) Enable the clock for the ADC GPIOs using the following function:
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOx, ENABLE);
       (++) Configure these ADC pins in analog mode using GPIO_Init();
    (#) Configure the ADC conversion resolution, using ADC_SETResolution function
                Configure external trigger and edge,  using ADC_ExtTrigConfig function
    (#) Activate the ADC peripheral using ADC_Cmd() function.

    *** ADC queue configuration ***
    ============================================
    [..]
    (+) Queue mode config, use ADC_QueModeConfig();
    (+) Queue priority config, use ADC_QuePriorityConfig();
    (+) queue element config, use ADC_QueElementConfig().
    (+) Enable/Disable the circulation mode, using ADC_QueCirCmdfunction().
    (+) Activate the Queue peripheral using ADC_QueCmd() function.

    *** DMA for ADC channels features configuration ***
    =============================================================
    [..]
    (+) To enable the DMA mode for ADC channels group, use the ADC_DMACmd() function.
   @endverbatim
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup ADC_Private_Functions ADC_Private_Functions
  * @{
  */

/** @defgroup ADC_Group1 Initialization and Configuration functions
 *  @brief   Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
          ##### Initialization and Configuration functions #####
 ===============================================================================
    [..] This section provides functions allowing to:
        (+) Initialize and configure the ADC Prescaler
        (+) ADC Conversion Resolution (12bit..6bit)
        (+) Enable or disable the ADC DMA

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes ADCx peripheral registers to their default reset values.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @retval None
  */
void ADC_DeInit(ADC_TypeDef* ADCx)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    /* Enable all ADCs reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_ADC, ENABLE);
    /* Release all ADCs from reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_ADC, DISABLE);
}

/**
  * @brief  Fills each ADC_InitStruct member with its default value.
  * @param  ADC_InitStruct: pointer to an ADC_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
    ADC_InitStruct->ADC_DateAlignment = ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct->ADC_ClkDiv = ADC_CLK_DIV_1;
    ADC_InitStruct->ADC_SampleCycle = ADC_SAMPLE_CYCLE_1_5;
    ADC_InitStruct->ADC_VrefSourse = ADC_VREF_INTERNAL;
    ADC_InitStruct->ADC_InternalVref = ADC_INTERNAL_VREF_2_4;
}

/**
  * @brief  Fills each Group_InitStruct member with its default value.
  * @param  Group_InitStruct:  pointer to an Group_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void ADC_GroupStructInit(Group_InitTypeDef* Group_InitStruct)
{
    Group_InitStruct->ADC_Group = GROUP_A;
    Group_InitStruct->ADC_GroupChNumber = 0;
    Group_InitStruct->ADC_GroupDoubleSampleCtrl = DISABLE;
    Group_InitStruct->ADC_GroupExtTrigCtrl = DISABLE;
    Group_InitStruct->ADC_GroupExtTrigSourse = ADC_EXT_TRIG_ATU_TRG0;
    Group_InitStruct->ADC_GroupSampleMode = ADC_SAMPLE_MODE_SINGLE;
    Group_InitStruct->ADC_GroupTrigSkip = ADC_TRIG_SKIP_NONE;

    for (uint8_t i = 0; i < GROUP_CHANNEL_MAX; i++)
    {
        Group_InitStruct->ADC_GroupChannel[i] = 0;
    }
}

/**
  * @brief  Initializes the ADCx peripheral according to the specified parameters
  * @note   This function is used to configure the global features of the ADC (
  *         Vref source, CLK division, Data Alignment).
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  ADC_InitStruct: pointer to an ADC_InitTypeDef structure that contains
  *         the configuration information for the specified ADC peripheral.
  * @retval None
  */
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    /* Vref source selection */
    ADC_VrefSourseSelect(ADCx, ADC_InitStruct->ADC_VrefSourse);
    /* Vref range selection */
    ADC_InternalVrefSelect(ADCx, ADC_InitStruct->ADC_InternalVref);
    /* ADC clk division */
    ADC_ClockDiv(ADCx, ADC_InitStruct->ADC_ClkDiv);
    /* ADC Register data align format */
    ADC_DataAlignment(ADCx, ADC_InitStruct->ADC_DateAlignment);
}


/**
  * @brief  Initializes the ADCx peripheral group according to the specified parameters
  * @note   This function is used to configure the global features of the Group (External
  *         trigger source, Trigger skipping times, Sampling mode, Sampling channel,
  *         sample number).
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Group_InitStruct: pointer to an Group_InitTypeDef structure that contains
  *         the configuration information for the specified ADC queue.
  * @retval None
  */
void ADC_GroupInit(ADC_TypeDef* ADCx, Group_InitTypeDef* Group_InitStruct)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    /* External trigger source selection */
    ADC_GroupExtTrigConfig(ADCx, Group_InitStruct->ADC_Group, \
                           Group_InitStruct->ADC_GroupExtTrigSourse, \
                           Group_InitStruct->ADC_GroupExtTrigCtrl);

    /* External trigger skipping times */
    ADC_GroupTrigSkipConfig(ADCx, Group_InitStruct->ADC_Group, \
                            Group_InitStruct->ADC_GroupTrigSkip);

    if (Group_InitStruct->ADC_Group == GROUP_B)
    {
        /* Sampling mode selection */
        ADC_GroupBSampleMode(ADCx, Group_InitStruct->ADC_GroupSampleMode);
        /* GroupB Dual sampling config */
        ADC_GroupBDoubleSampleCmd(ADCx, Group_InitStruct->ADC_GroupDoubleSampleCtrl);
    }

    /* Sampling channel signal source selection */
    for (uint16_t Cnt = 0; Cnt < Group_InitStruct->ADC_GroupChNumber; Cnt++)
    {
        ADC_GroupChannelSelect(ADCx, Group_InitStruct->ADC_Group, \
                               Cnt, \
                               Group_InitStruct->ADC_GroupChannel[Cnt]);
    }

    /* sample number config */
    ADC_GroupChannelNumber(ADCx, Group_InitStruct->ADC_Group, Group_InitStruct->ADC_GroupChNumber);
}

/**
  * @}
  */



/** @defgroup ADC_Group2 Interrupts and flags management functions
 *  @brief    Interrupts and flags management functions
 @verbatim
 ===============================================================================
            ##### Interrupts and flags management functions #####
 ===============================================================================

    [..] This section provides functions allowing to configure the ADC Interrupts
         and to get the status and clear flags and Interrupts pending bits.
@endverbatim

 * @{
 */


/**
  * @brief  Enables or disables the specified ADC interrupts.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  ADC_IT: specifies the ADC interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  *            @arg ADC_IT_GROUPA_FINISH: Group A sampling conversion completed interrupt mask
  *            @arg ADC_IT_GROUPB_FINISH: Group B sampling conversion completed interrupt mask
  *            @arg ADC_IT_GROUPB0_FINISH: Group A B0 sampling conversion completed interrupt mask
  *            @arg ADC_IT_GROUPB_INTERREPT: Group B sampling conversion Be interrupted interrupt mask
  *            @arg ADC_IT_BUSY_HW_TRIG: Hardware trigger occurs with the ADC on busy status interrupt mask
  *            @arg ADC_IT_BUSY_SW_TRIG: Software trigger occurs with the ADC on busy status interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE0: Window 0 compare protection interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE1: Window 1 compare protection interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE2: Window 2 compare protection interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE3: Window 3 compare protection interrupt mask
  * @param  NewState: new state of the specified ADC interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_ITConfig(ADC_TypeDef* ADCx, uint32_t ADC_IT, FunctionalState NewState)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_IT(ADC_IT));

    if (NewState != DISABLE)
    {
        ADCx->ADIER |= ADC_IT;
    }
    else
    {
        ADCx->ADIER &= ~(ADC_IT);
    }
}

/**
  * @brief  Checks whether the specified ADC interrupt has occurred or not.
  * @param  ADCx: where x to select the ADC peripheral
  * @param  ADC_IT: specifies the ADC interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  *            @arg ADC_IT_GROUPA_FINISH: Group A sampling conversion completed interrupt mask
  *            @arg ADC_IT_GROUPB_FINISH: Group B sampling conversion completed interrupt mask
  *            @arg ADC_IT_GROUPB0_FINISH: Group A B0 sampling conversion completed interrupt mask
  *            @arg ADC_IT_GROUPB_INTERREPT: Group B sampling conversion Be interrupted interrupt mask
  *            @arg ADC_IT_BUSY_HW_TRIG: Hardware trigger occurs with the ADC on busy status interrupt mask
  *            @arg ADC_IT_BUSY_SW_TRIG: Software trigger occurs with the ADC on busy status interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE0: Window 0 compare protection interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE1: Window 1 compare protection interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE2: Window 2 compare protection interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE3: Window 3 compare protection interrupt mask
  * @retval The new state of ADC_IT (SET or RESET).
  */
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint32_t ADC_IT)
{
    ITStatus ITFlag = RESET;
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_IT(ADC_IT));

    if ((ADCx->ADIFR & ADC_IT) != ADC_IT)
    {
        ITFlag = RESET;
    }
    else
    {
        ITFlag = SET;
    }

    return ITFlag;
}

/**
  * @brief  Clears the ADCx's interrupt pending bits.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  ADC_IT: specifies the ADC interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  *            @arg ADC_IT_GROUPA_FINISH: Group A sampling conversion completed interrupt mask
  *            @arg ADC_IT_GROUPB_FINISH: Group B sampling conversion completed interrupt mask
  *            @arg ADC_IT_GROUPB0_FINISH: Group A B0 sampling conversion completed interrupt mask
  *            @arg ADC_IT_GROUPB_INTERREPT: Group B sampling conversion Be interrupted interrupt mask
  *            @arg ADC_IT_BUSY_HW_TRIG: Hardware trigger occurs with the ADC on busy status interrupt mask
  *            @arg ADC_IT_BUSY_SW_TRIG: Software trigger occurs with the ADC on busy status interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE0: Window 0 compare protection interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE1: Window 1 compare protection interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE2: Window 2 compare protection interrupt mask
  *            @arg ADC_IT_WINDOW_COMPARE3: Window 3 compare protection interrupt mask
  * @retval None
  */
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint32_t ADC_IT)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_IT(ADC_IT));

    ADCx->ADIFR |= ADC_IT;
}

/**
  * @brief  Checks whether the specified ADC flag is set or not.
  * @param  ADCx: where x can be 1, 2 or 3 to select the ADC peripheral.
  * @param  ADC_Status: specifies the flag to check.
  *         This parameter can be a value of @ref ADC_flags_definition
  * @retval The new state of ADC_FLAG (SET or RESET).
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint32_t ADC_Status)
{
    FlagStatus Status;
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_STATUS(ADC_Status));

    if ((ADCx->ADSFR & ADC_Status) != ADC_Status)
    {
        Status = RESET;
    }
    else
    {
        Status = SET;
    }

    return Status;
}

/**
  * @}
  */

/** @defgroup ADC_Group3 Group configure functions
 *  @brief   Group configure functions
 * @{
 */

/**
  * @brief  GroupB sampling mode selection.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  SampleMode: specifies the groupB sample mode.
  *         This parameter can be one of the following values:
  *            @arg ADC_SAMPLE_MODE_SINGLE: single mode
  *            @arg ADC_SAMPLE_MODE_CONTINUE: continue mode
  * @retval None
  */
void ADC_GroupBSampleMode(ADC_TypeDef* ADCx, uint32_t SampleMode)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_SAMPLE_MODE(SampleMode));

    if (SampleMode != ADC_SAMPLE_MODE_CONTINUE)
    {
        ADCx->ADCSR &= ~(ADC_SAMPLE_MODE_CONTINUE << 21);
    }
    else
    {
        ADCx->ADCSR |= (ADC_SAMPLE_MODE_CONTINUE << 21);
    }

}


/**
  * @brief  GroupB dual sampling mode configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  NewState: new state of the specified GroupB dual sampling mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_GroupBDoubleSampleCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    if (NewState != DISABLE)
    {
        ADCx->ADCSR |= ADC_ADCSR_ATDEN_B;
    }
    else
    {
        ADCx->ADCSR &= ~ADC_ADCSR_ATDEN_B;
    }
}

/**
  * @brief  External triger configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Group: This parameter can be: GROUP_A or GROUP_B.
  * @param  ExtTrigSourse: This parameter can be a value of @ref ADC_group_trigger_sourse.
  * @param  NewState: new state of the external triger.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_GroupExtTrigConfig(ADC_TypeDef* ADCx, uint16_t Group, uint16_t ExtTrigSourse, FunctionalState NewState)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_GROUP_NAME(Group));
    assert_param(IS_ADC_EXT_TRIGGER(ExtTrigSourse));
    uint16_t regtmp = 0;

    regtmp = ADCx->ADTSS;
    regtmp &= ~0x00;
    regtmp = ((regtmp >> ADC_ADTSS_ATRGSB_Pos) | ((regtmp << ADC_ADTSS_ATRGSB_Pos) & ADC_ADTSS_ATRGSB));

    if (Group == GROUP_A)
    {
        regtmp &= ~ADC_ADTSS_ATRGSA;
        regtmp |= ExtTrigSourse;

        if (NewState != DISABLE)
        {
            /* Enble GroupA external triger */
            ADCx->ADCSR |= ADC_ADCSR_ATEN_A;
        }
        else
        {
            /* Disable GroupA external triger */
            ADCx->ADCSR &= ~ADC_ADCSR_ATEN_A;
        }
    }
    else
    {
        regtmp &= ~ADC_ADTSS_ATRGSB;
        regtmp |= (ExtTrigSourse << ADC_ADTSS_ATRGSB_Pos);

        if (NewState != DISABLE)
        {
            /* Enble GroupB external triger */
            ADCx->ADCSR |= ADC_ADCSR_ATEN_B;
        }
        else
        {
            /* Disable GroupB external triger */
            ADCx->ADCSR &= ~ADC_ADCSR_ATEN_B;
        }
    }

    ADCx->ADTSS = regtmp;
}


/**
  * @brief  Enables or disables the external configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Group: This parameter can be: GROUP_A or GROUP_B.
  * @param  NewState: new state of the external triger.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_GroupExtTrigCmd(ADC_TypeDef* ADCx, uint16_t Group, FunctionalState NewState)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_GROUP_NAME(Group));

    if (Group == GROUP_A)
    {
        if (NewState != DISABLE)
        {
            /* Enble GroupA external triger */
            ADCx->ADCSR |= ADC_ADCSR_ATEN_A;
        }
        else
        {
            /* Disable GroupA external triger */
            ADCx->ADCSR &= ~ADC_ADCSR_ATEN_A;
        }
    }
    else
    {
        if (NewState != DISABLE)
        {
            /* Enble GroupB external triger */
            ADCx->ADCSR |= ADC_ADCSR_ATEN_B;
        }
        else
        {
            /* Disable GroupB external triger */
            ADCx->ADCSR &= ~ADC_ADCSR_ATEN_B;
        }
    }
}

/**
  * @brief  Trigger Skipping Times configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Group: This parameter can be: GROUP_A or GROUP_B.
  * @param  TrigSkip: This parameter can be a value of @ref ADC_group_trigger_skip.
  * @retval None
  */
void ADC_GroupTrigSkipConfig(ADC_TypeDef* ADCx, uint16_t Group, uint16_t TrigSkip)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_GROUP_NAME(Group));
    assert_param(IS_ADC_TRIGGER_SKIP(TrigSkip));

    if (Group == GROUP_A)
    {
        ADCx->ADTST &= ~ADC_ADTST_ATSP_A;
        ADCx->ADTST |= TrigSkip;
    }
    else
    {
        ADCx->ADTST &= ~ADC_ADTST_ATSP_B;
        ADCx->ADTST |= (TrigSkip << ADC_ADTST_ATSP_B_Pos);
    }
}

/**
  * @brief  Enables or disables the Group software triger.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Group: This parameter can be: GROUP_A or GROUP_B.
  * @param  NewState: new state of the software triger.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_GroupSwTrig(ADC_TypeDef* ADCx, uint16_t Group, FunctionalState NewState)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_GROUP_NAME(Group));

    if (Group == GROUP_A)
    {
        if (NewState != DISABLE)
        {
            ADCx->ADCSR |= ADC_ADCSR_AEN_A;
        }
        else
        {
            ADCx->ADCSR &= ~ADC_ADCSR_AEN_A;
        }
    }
    else
    {
        if (NewState != DISABLE)
        {
            ADCx->ADCSR |= ADC_ADCSR_AEN_B;
        }
        else
        {
            ADCx->ADCSR &= ~ADC_ADCSR_AEN_B;
        }
    }
}

/**
  * @brief  Enables or disables the sampling register is automatically cleared after reading function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Group: This parameter can be: GROUP_A or GROUP_B.
  * @param  NewState: new state of the sampling register is automatically cleared after reading function.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_GroupDataReadClearCmd(ADC_TypeDef *ADCx, uint16_t Group, FunctionalState NewState)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_GROUP_NAME(Group));

    if (Group == GROUP_A)
    {
        if (NewState != DISABLE)
        {
            ADCx->ADCSR |= ADC_ADCSR_ACE_A;
        }
        else
        {
            ADCx->ADCSR &= ~ADC_ADCSR_ACE_A;
        }
    }
    else
    {
        if (NewState != DISABLE)
        {
            ADCx->ADCSR |= ADC_ADCSR_ACE_B;
        }
        else
        {
            ADCx->ADCSR &= ~ADC_ADCSR_ACE_B;
        }
    }
}

/**
  * @brief  Group sampling channel selection function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Group: This parameter can be: GROUP_A or GROUP_B.
  * @param  ASCS: Sampling conversion channel, this parameter can be a value of @ref ADC_group_ascs_select.
  * @param  ADC_Channel: ADC channle, This parameter can be a value of @ref ADC_channel.
  * @retval None
  */
void ADC_GroupChannelSelect(ADC_TypeDef* ADCx, uint16_t Group, uint16_t ASCS, uint16_t ADC_Channel)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_GROUP_NAME(Group));
    assert_param(IS_ADC_GROUP_ASCS(ASCS));
    assert_param(IS_ADC_CHANNEL(ADC_Channel));

    if (Group == GROUP_A)
    {
        /* GroupA only one sampling conversion channel  */
        ADCx->ADCCSA = ADC_Channel;
    }
    else
    {
        /* Judge the register to operate */
        if (ASCS <= ASCS_5)
        {
            if (ASCS <= ASCS_2)
            {
                ADCx->ADCCSB0 &= ~(0x1F << (5 * ASCS));
                ADCx->ADCCSB0 |= ADC_Channel << (5 * ASCS);
            }
            else
            {
                ADCx->ADCCSB0 &= ~(0x1F << (16 + 5 * (ASCS - ASCS_3)));
                ADCx->ADCCSB0 |= ADC_Channel << (16 + 5 * (ASCS - ASCS_3));
            }
        }
        else
        {
            if (ASCS <= ASCS_8)
            {
                ADCx->ADCCSB1 &= ~(0x1F << (5 * (ASCS - ASCS_6)));
                ADCx->ADCCSB1 |= ADC_Channel << (5 * (ASCS - ASCS_6));
            }
            else
            {
                ADCx->ADCCSB1 &= ~(0x1F << (16 + 5 * (ASCS - ASCS_9)));
                ADCx->ADCCSB1 |= ADC_Channel << (16 + 5 * (ASCS - ASCS_9));
            }
        }
    }
}

/**
  * @brief  Group sampling number configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Group: This parameter can be: GROUP_A or GROUP_B.
  * @param  ChNumber: sampling number, GroupA max 1, GroupB max 12.
  * @retval None
  */
void ADC_GroupChannelNumber(ADC_TypeDef* ADCx, uint16_t Group, uint16_t ChNumber)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_GROUP_NAME(Group));

    if (Group == GROUP_A)
    {
        assert_param(IS_ADC_GROUPA_CHANNEL_NUMBER(ChNumber));
        ADCx->ADSCN &= ~(ADC_ADSCN_AGSN_A);
        ADCx->ADSCN |= ChNumber;
    }
    else
    {
        assert_param(IS_ADC_GROUPB_CHANNEL_NUMBER(ChNumber));
        ADCx->ADSCN &= ~(ADC_ADSCN_AGSN_B);
        ADCx->ADSCN |= (ChNumber << ADC_ADSCN_AGSN_B_Pos);
    }
}

/**
  * @brief  Group sampling number configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  GroupBias: This parameter can be: GROUP_A or GROUP_B.
  * @param  BiasValue: sampling number, GroupA max 1, GroupB max 12.
  * @retval None
  */
void ADC_GroupBias(ADC_TypeDef* ADCx, uint16_t GroupBias, uint16_t BiasValue)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_GROUP_BIAS(GroupBias));
    assert_param(IS_ADC_GROUP_BIAS_VALUE(BiasValue));

    switch (GroupBias)
    {
        case GROUPA_ADBA0:
            ADCx->ADBA0 &= ~ADC_ADBA0_ADBA0;
            ADCx->ADBA0 |= BiasValue;
            break;

        case GROUPB_ADBB0:
            ADCx->ADBB0 &= ~ADC_ADBB0_ADBB0;
            ADCx->ADBB0 |= BiasValue;
            break;

        case GROUPB_ADBB1:
            ADCx->ADBB1 &= ~ADC_ADBB1_ADBB1;
            ADCx->ADBB1 |= BiasValue;
            break;

        default:
            break;
    }
}

/**
  * @}
  */

/** @defgroup ADC_Group4 ADC Base configure functions
 *  @brief   ADC ADC Base configure functions
 * @{
 */

/**
  * @brief  Enables or disables the ADC function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  NewState: new state of the ADC.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_Cmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    if (NewState != DISABLE)
    {
        /* Set the ADC_EN bit to Enable the ADC peripheral */
        ADCx->ADCSR |= ADC_ADCSR_ADC_EN;
    }
    else
    {
        /* Clear the ADC_EN to Disable the ADC peripheral */
        ADCx->ADCSR &= ~(ADC_ADCSR_ADC_EN);
    }
}

/**
  * @brief  ADC convert time selection function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  CycleSel: Specifies the vref selection.
  *         This parameter can be one of the following values:
  *            @arg ADC_CYCLE_SEL_12_5: adc cycle select 12.5 cycles
  *            @arg ADC_CYCLE_SEL_13_5: adc cycle select 13.5 cycles
  * @retval None
  */
void ADC_ADCCycleSel(ADC_TypeDef* ADCx, uint32_t CycleSel)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_CYCLE_SEL(CycleSel));

    ADCx->ADCSR &= ~(ADC_ADCSR_ADC_CYCLE_SEL);
    ADCx->ADCSR |= CycleSel;
}

/**
  * @brief  ADC clk division configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  ClkDiv: Specifies the ADC clk division.
  *         This parameter can be a value of @ref ADC_clk_div.
  * @retval None
  */
void ADC_ClockDiv(ADC_TypeDef *ADCx, uint32_t ClkDiv)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_CLK_DIV(ClkDiv));

    ADCx->ADCSR &= ~ADC_ADCSR_ADCLK_DIV;
    ADCx->ADCSR |= ClkDiv;
}

/**
  * @brief  Active the Calibration operation for the selected ADC.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @retval ADC Calibration factor
  */
uint32_t ADC_GetCalibrationFactor(ADC_TypeDef *ADCx)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    uint32_t CalOffset = 0, CalCount = 0, CalStatus = 0, RegTmp = 0;

    /* Get ADC clk div*/
    RegTmp = ADCx->ADCSR;

    ADC_ClockDiv(ADCx, ADC_CLK_DIV_4);

    /* Set the ADC calibartion */
    ADCx->ADCSR |= ADC_ADCSR_ADC_CAL;

    /* Wait until no ADC calibration is completed */
    do
    {
        CalStatus = (ADCx->ADCSR & ADC_ADCSR_ADC_CAL);
        CalCount++;
    } while ((CalCount != ADC_CAL_TIMEOUT) && (CalStatus != RESET));

    if ((ADCx->ADCSR & ADC_ADCSR_ADC_CAL) == RESET)
    {
        /* Get the calibration factor from the ADC data register */
        CalOffset = ADCx->ADCAL;
        CalOffset &= ADC_ADCAL_ADCAL;
        CalOffset = (CalOffset >> ADC_ADCAL_ADCAL_Pos);
        /* Make the offset take effect*/
        ADCx->ADCAL &= ~ADC_ADCAL_ADOFFSET;
        ADCx->ADCAL |= CalOffset;
    }
    else
    {
        /* Error factor */
        CalOffset = 0x00000000;
    }

    ADCx->ADCSR &= ~ADC_ADCSR_ADCLK_DIV;
    ADCx->ADCSR |= (RegTmp & ADC_ADCSR_ADCLK_DIV);

    /* ADC Gain compensate*/
    ADC_Gain(ADCx, DIRECTION_NEGATIVE, 3, ENABLE);

    return CalOffset;
}

/**
  * @brief  ADC Vref selection function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Vref: Specifies the vref selection.
  *         This parameter can be one of the following values:
  *            @arg ADC_VREF_EXTERNAL: vref select AVDD
  *            @arg ADC_VREF_INTERNAL: vref select internal vref
  * @retval None
  */
void ADC_VrefSourseSelect(ADC_TypeDef* ADCx, uint32_t Vref)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_VREF_SOURSE(Vref));

    ADCx->ADCSR &= ~(ADC_ADCSR_AREF);
    ADCx->ADCSR |= Vref;
}

/**
  * @brief  ADC internal vref selection function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Internal: Specifies the internal vref selection.
  *         This parameter can be one of the following values:
  *            @arg ADC_INTERNAL_VREF_2_4: Internal vref select 2.4V
  *            @arg ADC_INTERNAL_VREF_3_6: Internal vref select 3.6V
  * @retval None
  */
void ADC_InternalVrefSelect(ADC_TypeDef* ADCx, uint32_t Internal)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_INTERNAL_VREF(Internal));

    ADCx->ADCSR &= ~(ADC_ADCSR_ARNG);
    ADCx->ADCSR |= Internal;
}

/**
  * @brief  Enables or disables the over sampling function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  NewState: new state of the over sampling function.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_OverSamlpeCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    if (NewState != DISABLE)
    {
        ADCx->ADCSR |= ADC_ADCSR_AOSEN;
    }
    else
    {
        ADCx->ADCSR &= ~(ADC_ADCSR_AOSEN);
    }
}

/**
  * @brief  ADC alignment format configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Alignment: the alignment format .
  *         this parameter can be one of the following values:
  *            @arg ADC_DATA_ALIGN_RIGHT: Continuous oversampling mode
  *            @arg ADC_DATA_ALIGN_LEFT: Time sharing sampling mode
  * @retval None
  */
void ADC_DataAlignment(ADC_TypeDef* ADCx, uint32_t Alignment)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_DATA_ALIGNMENT(Alignment));

    ADCx->ADCSR &= ~(ADC_ADCSR_ARFMT);
    ADCx->ADCSR |= Alignment;
}

/**
  * @brief  ADC sampling time configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Group: This parameter can be: GROUP_A or GROUP_B.
  * @param  ASCS: Sampling conversion channel, this parameter can be a value of @ref ADC_group_ascs_select.
  * @param  SamlpeCycle: ADC sampling time, This parameter can be a value of @ref ADC_sample_cycle.
  * @retval None
  */
void ADC_SampleCycleConfig(ADC_TypeDef *ADCx, uint16_t Group, uint16_t ASCS, uint16_t SamlpeCycle)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_SAMPLE_CYCLE(SamlpeCycle));
    assert_param(IS_ADC_GROUP_ASCS(ASCS));
    assert_param(IS_GROUP_NAME(Group));

    if (Group == GROUP_A)
    {
        /* configure groupA sampling time */
        ADCx->ADSTA = SamlpeCycle;
    }
    else if (Group == GROUP_B)
    {
        if (ASCS < ASCS_8)
        {
            /* configure groupB ASCS_0 ~ ASCS_7 sampling time */
            ADCx->ADSTB0 &= ~(ADC_ADSTB0_ASMP0 << (ASCS * 4));
            ADCx->ADSTB0 |= (SamlpeCycle << (ASCS * 4));
        }
        else
        {
            /* configure groupB ASCS_8 ~ ASCS_11 sampling time */
            ADCx->ADSTB1 &= ~(ADC_ADSTB1_ASMP8 << ((ASCS - ASCS_8) * 4));
            ADCx->ADSTB1 |= (SamlpeCycle << ((ASCS - ASCS_8) * 4));
        }
    }
}

/**
  * @brief  ADC over sampling channel selection function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Channel: Specifies the over sampling channel.
  *         This parameter can be a value of @ref ADC_group_channel.
  * @retval None
  */
void ADC_OverSampleChannel(ADC_TypeDef* ADCx, uint16_t Channel)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_GROUP_CHANNEL(Channel));

    ADCx->ADOCR &= ~ADC_ADOCR_AOCCS;
    ADCx->ADOCR |= Channel;
}

/**
  * @brief  ADC average count of oversampling/Time-sharing configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  AvgeNumber: ADC average count of oversampling/Time-sharing,
  *         This parameter can be a value of @ref ADC_over_sample_avg_number.
  * @retval None
  */
void ADC_OverSampleAvgeNumber(ADC_TypeDef* ADCx, uint16_t AvgeNumber)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_OVER_SAMPLE_AVG_NUMBER(AvgeNumber));

    ADCx->ADOCR &= ~ADC_ADOCR_AOTS;
    ADCx->ADOCR |= AvgeNumber;
}

/**
  * @brief  ADC over sampling skip count configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  SkipNumber: Specifies the over sampling skip count.
  *         This parameter can be a value of @ref ADC_over_sample_trigger_skip.
  * @retval None
  */
void ADC_OverSampleTrigSkip(ADC_TypeDef* ADCx, uint16_t SkipNumber)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_OVSMP_TRIG_SKIP(SkipNumber));

    ADCx->ADOCR &= ~ADC_ADOCR_AOSKT;
    ADCx->ADOCR |= SkipNumber;
}

/**
  * @brief  The over sampling mode configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Mode: the over sampling mode.
  *         this parameter can be one of the following values:
  *            @arg ADC_OVSMP_CONT: Continuous oversampling mode
  *            @arg ADC_OVSMP_DISCONT: Time sharing sampling mode
  * @retval None
  */
void ADC_OverSampleMode(ADC_TypeDef* ADCx, uint16_t Mode)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_OVER_SAMPLE_MODE(Mode));

    ADCx->ADOCR &= ~(ADC_ADOCR_AOMS);
    ADCx->ADOCR |= Mode;
}


/**
  * @brief  Returns the last ADCx conversion result data for ADC channel.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Channel: The window comparator source channel.
  *         This parameter can be a value of @ref ADC_group_channel.
  * @retval The Data conversion value.
  */
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx, uint16_t Channel)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_GROUP_CHANNEL(Channel));
    uint16_t Result;

    if (Channel != CHANNEL_GROUPA_0)
    {
        Result = *((&(ADCx->ADDR0B)) + ((Channel - CHANNEL_GROUPB_0))) & ADC_ADDR0B_ADDRxR;
    }
    else
    {
        Result = ADCx->ADDR0A & ADC_ADDR0A_ADDR0R;
    }

    return Result;
}

/**
  * @brief  Returns the last ADCx buffer result data for ADC.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  buffer: The ADC buffer index.
  *         This parameter can be a value of @ref ADC_buffer.
  * @retval The ADC Data buffer value.
  */
uint16_t ADC_GetBufferValue(ADC_TypeDef* ADCx, uint16_t buffer)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_BUFFER(buffer));
    uint16_t Result;

    Result = *((&(ADCx->ADBUR0)) + (buffer * 0x04)) & ADC_ADBUR1_ADBURxR;

    return Result;
}

/**
  * @brief  The ADC user offset configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  UserOffset: the user offset value.
  *         This parameter can be a value of @ref ADC_user_offset.
  * @retval None
  */
void ADC_UserOffset(ADC_TypeDef* ADCx, uint16_t UserOffset)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_USER_OFFSET(UserOffset));

    ADCx->ADOFFSET &= ~(ADC_ADOFFSET_ADOFFSETUSR);
    ADCx->ADOFFSET |= UserOffset;
}


/**
  * @brief  The ADC gain fix configure function.
  * @param  ADCx: where x can be 1, 2 or 3 to select the ADC peripheral.
  * @param  GainDir: the gain fix direction.
  *         this parameter can be one of the following values:
  *            @arg DIRECTION_POSITIVE: gain error is negative value
  *            @arg DIRECTION_NEGATIVE: gain error is positive value
  * @param  GainFix: gain fix value, This parameter can be a value of @ref ADC_gain_fix.
  * @param  NewState: new state of the gain fix function.
  *         This parameter can be: ENABLE or DISABLE
  * @retval None
  */
void ADC_Gain(ADC_TypeDef* ADCx, uint8_t GainDir, uint32_t GainFix, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_GAIN_FIX(GainFix));
    assert_param(IS_ADC_GAIN_DIRECTION(GainDir));

    ADCx->ADGAIN &= ~ADC_ADGAIN_GAIN_FIX;

    if (GainDir == DIRECTION_NEGATIVE)
    {
        ADCx->ADGAIN |= (1 << ADC_GAIN_FIX_SIGN_POS);
    }

    if (NewState != DISABLE)
    {
        ADCx->ADGAIN |= (GainFix << ADC_ADGAIN_GAIN_FIX_Pos);
        ADCx->ADGAIN |= ADC_ADGAIN_GAIN_EN;
    }
    else
    {
        ADCx->ADGAIN &= ~(ADC_ADGAIN_GAIN_EN);
    }
}

/**
  * @}
  */

/** @defgroup ADC_Group5 Window comparator configure functions
 *  @brief   Window comparator configure functions
 * @{
 */

/**
  * @brief  The window comparator source channel selection.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  AW: window comparator, this parameter can be one of the following values:
  *            @arg AW_0: window comparator 0
  *            @arg AW_1: window comparator 1
  *            @arg AW_2: window comparator 2
  *            @arg AW_3: window comparator 3
  * @param  Channel: The window comparator source channel.
  *         This parameter can be a value of @ref ADC_group_channel.
  * @retval None
  */
void ADC_AnalogWindowChannel(ADC_TypeDef* ADCx, uint16_t AW, uint16_t Channel)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_AW_NAME(AW));
    assert_param(IS_ADC_GROUP_CHANNEL(Channel));

    switch (AW)
    {
        case AW_0:
            ADCx->ADWCR0 &= ~(ADC_ADWCR0_AWCS);
            ADCx->ADWCR0 |= Channel;
            break;

        case AW_1:
            ADCx->ADWCR1 &= ~(ADC_ADWCR1_AWCS);
            ADCx->ADWCR1 |= Channel;
            break;

        case AW_2:
            ADCx->ADWCR2 &= ~(ADC_ADWCR2_AWCS);
            ADCx->ADWCR2 |= Channel;
            break;

        case AW_3:
            ADCx->ADWCR3 &= ~(ADC_ADWCR3_AWCS);
            ADCx->ADWCR3 |= Channel;
            break;
    }
}

/**
  * @brief  The window comparator mode configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  AW: window comparator, this parameter can be one of the following values:
  *            @arg AW_0: window comparator 0
  *            @arg AW_1: window comparator 1
  *            @arg AW_2: window comparator 2
  *            @arg AW_3: window comparator 3
  * @param  Mode: The window comparator mode.
  *         this parameter can be one of the following values:
  *            @arg AW_MODE_0: window comparator mode 0
  *            @arg AW_MODE_1: window comparator mode 1
  * @retval None
  */
void ADC_AnalogWindowMode(ADC_TypeDef* ADCx, uint16_t AW, uint16_t Mode)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_AW_NAME(AW));
    assert_param(IS_ADC_AW_MODE(Mode));

    switch (AW)
    {
        case AW_0:
            ADCx->ADWCR0 &= ~(1 << 6);
            ADCx->ADWCR0 |= Mode;
            break;

        case AW_1:
            ADCx->ADWCR1 &= ~(1 << 6);
            ADCx->ADWCR1 |= Mode;
            break;

        case AW_2:
            ADCx->ADWCR2 &= ~(1 << 6);
            ADCx->ADWCR2 |= Mode;
            break;

        case AW_3:
            ADCx->ADWCR3 &= ~(1 << 6);
            ADCx->ADWCR3 |= Mode;
            break;
    }
}
/**
  * @brief  Enables or disables the window comparator.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  AW: window comparator, This parameter can be one of the following values:
  *            @arg AW_0: window comparator 0
  *            @arg AW_1: window comparator 1
  *            @arg AW_2: window comparator 2
  *            @arg AW_3: window comparator 3
  * @param  NewState: new state of the sampling register is automatically cleared after reading function.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_AnalogWindowCmd(ADC_TypeDef* ADCx, uint16_t AW, FunctionalState NewState)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_AW_NAME(AW));

    switch (AW)
    {
        case AW_0:
            ADCx->ADWCR0 &= ~ADC_ADWCR0_AWEN;
            ADCx->ADWCR0 |= (NewState << ADC_ADWCR0_AWEN_Pos);
            break;

        case AW_1:
            ADCx->ADWCR1 &= ~(ADC_ADWCR1_AWEN);
            ADCx->ADWCR1 |= (NewState << ADC_ADWCR1_AWEN_Pos);
            break;

        case AW_2:
            ADCx->ADWCR2 &= ~(ADC_ADWCR2_AWEN);
            ADCx->ADWCR2 |= (NewState << ADC_ADWCR2_AWEN_Pos);
            break;

        case AW_3:
            ADCx->ADWCR3 &= ~(ADC_ADWCR3_AWEN);
            ADCx->ADWCR3 |= (NewState << ADC_ADWCR3_AWEN_Pos);
            break;
    }
}

/**
  * @brief  The window comparator threshold configure function.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  AW: window comparator, this parameter can be one of the following values:
  *            @arg AW_0: window comparator 0
  *            @arg AW_1: window comparator 1
  *            @arg AW_2: window comparator 2
  *            @arg AW_3: window comparator 3
  * @param  HighThreshold: threshold high value, its can 0x000~0xfff.
  * @param  LowThreshold: threshold low value, its can 0x000~0xfff.
  * @retval None
  */
void ADC_AnalogWindowThreshold(ADC_TypeDef* ADCx, uint16_t AW, uint16_t HighThreshold, uint16_t LowThreshold)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_AW_NAME(AW));
    assert_param(IS_ADC_AW_THRESHOLD(HighThreshold));
    assert_param(IS_ADC_AW_THRESHOLD(LowThreshold));

    switch (AW)
    {
        case AW_0:
            ADCx->ADWH0 = HighThreshold;
            ADCx->ADWL0 = LowThreshold;
            break;

        case AW_1:
            ADCx->ADWH1 = HighThreshold;
            ADCx->ADWL1 = LowThreshold;
            break;

        case AW_2:
            ADCx->ADWH2 = HighThreshold;
            ADCx->ADWL2 = LowThreshold;
            break;

        case AW_3:
            ADCx->ADWH3 = HighThreshold;
            ADCx->ADWL3 = LowThreshold;
            break;
    }
}


/**
  * @}
  */

/** @defgroup ADC_Group6 ADC Extension functions
 *  @brief   ADC Extension functions
 * @{
 */

/**
  * @brief  The PGA2_OUT Extended selection.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  Select: The PGA2_OUT Extended selection.
  *         This parameter can be a value of @ref ADC_pga2_out_extended.
  * @retval None
  */
void ADC_PGA2OUTExtendedSelect(ADC_TypeDef* ADCx, uint16_t Select)
{
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_EXTENDED(Select));

    ADCx->ADEF &= ~(ADC_ADEF_AEO);
    ADCx->ADEF |= Select;
}

/**
  * @brief  Pin Remap of channel 13 configure function
  * @param  ADCx: where x to select the ADC peripheral.
  * @param  Remap: Pin remap value of Channel 13.
  *         This parameter can be a value of @ref ADC_ch13_pinremap.
  * @retval None
  */
void ADC_PinRemapCH13(ADC_TypeDef *ADCx, uint32_t Remap)
{
    uint16_t tmp_pinremap;

    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_PIN_REMAP_CH13(Remap));

    tmp_pinremap = ADCx->ADREMAP ;
    tmp_pinremap &= ~(ADC_ADREMAP_ADIN13_Mask);
    tmp_pinremap |= Remap;

    ADCx->ADREMAP = tmp_pinremap;
}

/**
  * @brief  Pin Remap of channel 14 configure function
  * @param  ADCx: where x to select the ADC peripheral.
  * @param  Remap: Pin remap value of Channel 14.
  *         This parameter can be a value of @ref ADC_ch14_pinremap.
  * @retval None
  */
void ADC_PinRemapCH14(ADC_TypeDef *ADCx, uint32_t Remap)
{
    uint16_t tmp_pinremap;

    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_PIN_REMAP_CH14(Remap));

    tmp_pinremap = ADCx->ADREMAP ;
    tmp_pinremap &= ~(ADC_ADREMAP_ADIN14_Mask);
    tmp_pinremap |= Remap;

    ADCx->ADREMAP = tmp_pinremap;
}

/**
  * @brief  Get Temperature at FT test (in 0.01 degree celsius).
  * @note   Get the temperature at FT test using the following formula
  *         temp_value (in 0.01 degree celsius);
  * @retval temp_value(in 0.01 degree celsius)
  */
int ADC_GetTemperatureAtFT(void)
{
    int  temp_value = 0;
    uint16_t tempData = 0;
    uint32_t read_data = 0;

    read_data = *(uint32_t*)TEMPS_CAL_ADDR;
    /*Temperature at FT test (in 0.01 degree celsius)*/
    tempData = read_data >> 16;

    if (tempData <= 0x7FF)
    {
        temp_value = (int)(6.25 * tempData);
    }
    else
    {
        temp_value = (int)(-6.25 * ((float)(0x1fff - tempData)));
    }

    return (temp_value);
}

/**
  * @brief  Get Temperature (in  degree celsius).
  * @note   Get the temperature using the following formula
  *         Temperature (in  degree celsius) = slope *(TS_DATA - TS_CAL) +25;
  *         TS_CAL is the temperature sensor calibration value acquired at 25 degree celsius(*TEMP25_CAL_FT_ADDR)
  *         TS_DATA is the actual temperature sensor output value converted by ADC(tempADbuff)
  * @param  tempADbuff is the temperature sensor ADC conversion value
  * @param  VDDA_mV is the current chip VDDA power supply voltage
  * @retval temperature(in  degree celsius)
  */
int ADC_GetTemperature(uint32_t tempADbuff, uint32_t VDDA_mV)
{
    int temperature = 0;
    float slope = 0;
    float measure1 = 0;
    float tempFT = 0;
    uint16_t intel_tempData = 0;
    intel_tempData = *(uint16_t*)TEMPS_CAL_ADDR;

    /*Get the Slope, from datasheet*/
    slope = 1.1762;
    measure1 = (tempADbuff * (float)VDDA_mV / 3000);

    tempFT = ADC_GetTemperatureAtFT();
    /*GET the temperature (in  degree celsius)*/
    temperature = (int)((slope * ((measure1 - (float)intel_tempData))) + (float)(tempFT / 100));
    return (temperature);
}

/**
  * @brief  Get real VDDA when the power supply voltage changes.
  * @param  VrefADbuff: Get the VDDA using the following formula
  *         VDDA = 3 V x VREFINT_CAL / VREFINT_DATA;
  *         VREFINT_CAL is the VREFINT calibration value acquired at VDD = 3 V(25 degree celsius)(*VREFINT_CAL_ADDR)
  *         VREFINT_DATA is the actual VREFINT output value converted by ADC(measure)
  * @retval VDDA(in mV)
  */
uint32_t ADC_GetVDDA(uint32_t VrefADbuff)
{
    uint32_t VDDA;
    VDDA = 5000 * ((*(uint32_t*)VREFINT_CAL_ADDR)) / VrefADbuff;
    return (VDDA);
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
