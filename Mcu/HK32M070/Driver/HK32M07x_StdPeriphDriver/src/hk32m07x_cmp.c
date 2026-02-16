/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_cmp.c
* @brief:     API file of comp module
* @author:    AE Team
* @version:   V1.0.0/2024-12-19
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_cmp.h"

/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */


/** @defgroup CMP CMP
  * @brief CMP modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup CMP_Private_Functions CMP_Private_Functions
  * @{
  */

/** @defgroup CMP_Group1 Initialization and Configuration functions
 *  @brief   Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
          ##### Initialization and Configuration functions #####
 ===============================================================================
    [..] This section provides functions allowing to:
        (+) Initialize and configure the CMP peripheral

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the CMP peripheral registers to their default reset values.
  * @retval None.
  */
void CMP_DeInit(void)
{
    /* Enable all CMP reset state */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_COMP, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_COMP, DISABLE);
}

/**
  * @brief  Fills each CMP_InitStruct member with its default value.
  * @param  CMP_InitStruct: pointer to a CMP_InitStruct structure which will be initialized.
  * @retval None
  */
void CMP_StructInit(CMP_InitTypeDef *CMP_InitStruct)
{
    CMP_InitStruct->CMP_NInputSel = CMP_NEGATIVE_INPUT_N;
    CMP_InitStruct->CMP_PInputSel = CMP_POSITIVE_INPUT_P0;
    CMP_InitStruct->CMP_OutputPolarity = CMP_OUTPUT_POLARITY_NORMAL;
    CMP_InitStruct->CMP_HysteresisMode = CMP_NO_HYSTERESIS;
    CMP_InitStruct->CMP_Hysteresis = CMP_HYSTERESIS_DISABLE;
}

/**
  * @brief  Initialize some features of CMP instance.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @param  CMP_InitStruct: pointer to a CMP_InitTypeDef structure.
  * @retval An ErrorStatus enumeration value:
  *         - SUCCESS: CMP registers are initialized.
  *         - ERROR: CMP registers are not initialized.
  */
void CMP_Init(uint8_t CMPx, CMP_InitTypeDef *CMP_InitStruct)
{
    assert_param(IS_CMP_NAME(CMPx));
    assert_param(IS_CMP_HYSTERESIS_MODE(CMP_InitStruct->CMP_HysteresisMode));
    assert_param(IS_CMP_OUTPUT_POLARITY(CMP_InitStruct->CMP_OutputPolarity));
    assert_param(IS_CMP_HYSTERESIS(CMP_InitStruct->CMP_Hysteresis));
    assert_param(IS_CMP_POSITIVE_INPUT(CMP_InitStruct->CMP_PInputSel));
    assert_param(IS_CMP_NEGATIVE_INPUT(CMP_InitStruct->CMP_NInputSel));

    if (CMPx == CMP1)
    {
        CMP->CMCSR &= ~(CMP_CMCSR_CMHYM1 | CMP_CMCSR_CMPS1 | CMP_CMCSR_CMHYL1);
        CMP->CMISS &= ~(CMP_CMISS_CMIS1N | CMP_CMISS_CMIS1P);
        CMP->CMCSR |= (CMP_InitStruct->CMP_HysteresisMode
                       | CMP_InitStruct->CMP_OutputPolarity
                       | CMP_InitStruct->CMP_Hysteresis
                      );
        CMP->CMISS |= (CMP_InitStruct->CMP_PInputSel
                       | CMP_InitStruct->CMP_NInputSel
                      );
    }
    else if (CMPx == CMP2)
    {
        CMP->CMCSR &= ~(CMP_CMCSR_CMHYM2 | CMP_CMCSR_CMPS2 | CMP_CMCSR_CMHYL2);
        CMP->CMISS &= ~(CMP_CMISS_CMIS2N | CMP_CMISS_CMIS2P);
        CMP->CMCSR |= (CMP_InitStruct->CMP_HysteresisMode << 8
                       | CMP_InitStruct->CMP_OutputPolarity << 8
                       | CMP_InitStruct->CMP_Hysteresis << 8
                      );
        CMP->CMISS |= (CMP_InitStruct->CMP_PInputSel << 8
                       | CMP_InitStruct->CMP_NInputSel << 8
                      );
    }
}

/**
  * @}
  */


/** @defgroup CMP_Group2 CMP configure functions
 *  @brief   CMP configure functions
 * @{
 */

/**
  * @brief  Set comparators Positive input connection.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @param  PositiveInput: CMP positive signal input selection
  *         This parameter can be one of the following values:
  *            @arg CMP_POSITIVE_INPUT_P0.
  *            @arg CMP_POSITIVE_INPUT_P1.
  *            @arg CMP_POSITIVE_INPUT_P2.
  *            @arg CMP_POSITIVE_INPUT_P3.
  *            @arg CMP_POSITIVE_INPUT_PGA1OUT_A_B.
  *            @arg CMP_POSITIVE_INPUT_PGA2OUT.
  *            @arg CMP_POSITIVE_INPUT_PGA1_PA_PB.
  *            @arg CMP_POSITIVE_INPUT_PGA2_P.
  * @retval None.
  */
void CMP_SetPositiveInput(uint8_t CMPx, uint32_t PositiveInput)
{
    assert_param(IS_CMP_NAME(CMPx));
    assert_param(IS_CMP_POSITIVE_INPUT(PositiveInput));

    if (CMPx == CMP1)
    {
        CMP->CMISS &= ~(CMP_CMISS_CMIS1P);
        CMP->CMISS |= PositiveInput;
    }
    else if (CMPx == CMP2)
    {
        CMP->CMISS &= ~(CMP_CMISS_CMIS2P);
        CMP->CMISS |= (PositiveInput << 8);
    }
}

/**
  * @brief  Get comparators positive input connection.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @retval Comparators positive input connection.
  */
uint32_t CMP_GetPositiveInput(uint8_t CMPx)
{
    uint32_t Temp;
    assert_param(IS_CMP_NAME(CMPx));

    if (CMPx == CMP1)
    {
        Temp = (uint32_t)((CMP->CMISS & CMP_CMISS_CMIS1P));
    }
    else if (CMPx == CMP2)
    {
        Temp = (uint32_t)((CMP->CMISS & CMP_CMISS_CMIS2P)) >> CMP_CMISS_CMIS2P_Pos;
    }

    return Temp;
}

/**
  * @brief  Set comparators negative input connection.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @param  NegativeInput: CMP negative signal input selection
  *         This parameter can be one of the following values:
  *            @arg CMP_NEGATIVE_INPUT_N.
  *            @arg CMP_NEGATIVE_INPUT_BEMF.
  *            @arg CMP_NEGATIVE_INPUT_DAC_OUT.
  * @retval None.
  */
void CMP_SetNegativeInput(uint8_t CMPx, uint32_t NegativeInput)
{
    assert_param(IS_CMP_NAME(CMPx));
    assert_param(IS_CMP_NEGATIVE_INPUT(NegativeInput));

    if (CMPx == CMP1)
    {
        CMP->CMISS &= ~(CMP_CMISS_CMIS1N);
        CMP->CMISS |= NegativeInput;
    }
    else if (CMPx == CMP2)
    {
        CMP->CMISS &= ~(CMP_CMISS_CMIS2N);
        CMP->CMISS |= (NegativeInput << 8);
    }
}

/**
  * @brief  Get comparators negative input connection.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @retval Comparators negative input connection.
  */
uint32_t CMP_GetNegativeInput(uint8_t CMPx)
{
    uint32_t Temp;
    assert_param(IS_CMP_NAME(CMPx));

    if (CMPx == CMP1)
    {
        Temp = (uint32_t)((CMP->CMISS & CMP_CMISS_CMIS1N)) >> CMP_CMISS_CMIS1N_Pos;
    }
    else if (CMPx == CMP2)
    {
        Temp = (uint32_t)((CMP->CMISS & CMP_CMISS_CMIS2N)) >> CMP_CMISS_CMIS2N_Pos;
    }

    return Temp;
}

/**
  * @brief  Set comparator instance output polarity.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @param  OutputPolarity: CMP output polarity selection
  *         This parameter can be one of the following values:
  *            @arg CMP_OUTPUT_POLARITY_NORMAL.
  *            @arg CMP_OUTPUT_POLARITY_REVERSE.
  * @retval None.
  */
void CMP_SetOutputPolarity(uint8_t CMPx, uint32_t OutputPolarity)
{
    assert_param(IS_CMP_NAME(CMPx));
    assert_param(IS_CMP_OUTPUT_POLARITY(OutputPolarity));

    if (CMPx == CMP1)
    {
        CMP->CMCSR &= ~(CMP_CMCSR_CMPS1);
        CMP->CMCSR |= OutputPolarity;
    }
    else if (CMPx == CMP2)
    {
        CMP->CMCSR &= ~(CMP_CMCSR_CMPS2);
        CMP->CMCSR |= (OutputPolarity << 8);
    }
}

/**
  * @brief  Get comparator instance output polarity.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @retval Comparator instance output polarity.
  */
uint32_t CMP_GetOutputPolarity(uint8_t CMPx)
{
    uint32_t Temp;
    assert_param(IS_CMP_NAME(CMPx));

    if (CMPx == CMP1)
    {
        Temp = (uint32_t)((CMP->CMCSR & CMP_CMCSR_CMPS1)) >> CMP_CMCSR_CMPS1_Pos;
    }
    else if (CMPx == CMP2)
    {
        Temp = (uint32_t)((CMP->CMCSR & CMP_CMCSR_CMPS2)) >> CMP_CMCSR_CMPS2_Pos;
    }

    return Temp;
}

/**
  * @brief  Set comparator hysteresis mode.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @param  Mode: CMP hysteresis mode selection
  *         This parameter can be one of the following values:
  *            @arg CMP_NO_HYSTERESIS.
  *            @arg CMP_POSITIVE_HYSTERESIS.
  *            @arg CMP_NEGATIVE_HYSTERESIS.
  *            @arg CMP_NEGATIVE_POSITIVE_HYSTERESIS.
  * @retval None.
  */
void CMP_SetHysteresisMode(uint8_t CMPx, uint32_t Mode)
{
    assert_param(IS_CMP_NAME(CMPx));
    assert_param(IS_CMP_HYSTERESIS_MODE(Mode));

    if (CMPx == CMP1)
    {
        CMP->CMCSR &= ~(CMP_CMCSR_CMHYM1);
        CMP->CMCSR |= Mode;
    }
    else if (CMPx == CMP2)
    {
        CMP->CMCSR &= ~(CMP_CMCSR_CMHYM2);
        CMP->CMCSR |= (Mode << 8);
    }
}

/**
  * @brief  Get comparator hysteresis Mode.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @retval Comparator hysteresis Mode.
  */
uint32_t CMP_GetHysteresisMode(uint8_t CMPx)
{
    uint32_t Temp;
    assert_param(IS_CMP_NAME(CMPx));

    if (CMPx == CMP1)
    {
        Temp = (uint32_t)((CMP->CMCSR & CMP_CMCSR_CMHYM1)) >> CMP_CMCSR_CMHYM1_Pos;
    }
    else if (CMPx == CMP2)
    {
        Temp = (uint32_t)((CMP->CMCSR & CMP_CMCSR_CMHYM2)) >> CMP_CMCSR_CMHYM2_Pos;
    }

    return Temp;
}

/**
  * @brief  Set comparator hysteresis.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @param  Hysteresis: CMP hysteresis level selection
  *         This parameter can be one of the following values:
  *            @arg CMP_HYSTERESIS_DISABLE.
  *            @arg CMP_HYSTERESIS_LEVEL1.
  *            @arg CMP_HYSTERESIS_LEVEL2.
  *            @arg CMP_HYSTERESIS_LEVEL3.
  * @retval None.
  */
void CMP_SetHysteresis(uint8_t CMPx, uint32_t Hysteresis)
{
    assert_param(IS_CMP_NAME(CMPx));
    assert_param(IS_CMP_HYSTERESIS(Hysteresis));

    if (CMPx == CMP1)
    {
        CMP->CMCSR &= ~(CMP_CMCSR_CMHYL1);
        CMP->CMCSR |= Hysteresis;
    }
    else if (CMPx == CMP2)
    {
        CMP->CMCSR &= ~(CMP_CMCSR_CMHYL2);
        CMP->CMCSR |= (Hysteresis << 8);
    }
}

/**
  * @brief  Get comparator hysteresis.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @retval Comparator hysteresis.
  */
uint32_t CMP_GetHysteresis(uint8_t CMPx)
{
    uint32_t Temp;
    assert_param(IS_CMP_NAME(CMPx));

    if (CMPx == CMP1)
    {
        Temp = (uint32_t)((CMP->CMCSR & CMP_CMCSR_CMHYL1)) >> CMP_CMCSR_CMHYL1_Pos;
    }
    else if (CMPx == CMP2)
    {
        Temp = (uint32_t)((CMP->CMCSR & CMP_CMCSR_CMHYL2)) >> CMP_CMCSR_CMHYL2_Pos;
    }

    return Temp;
}

/**
  * @brief  Enable comparator.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @param  NewState: new state of the CMP.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void CMP_Cmd(uint8_t CMPx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_CMP_NAME(CMPx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (CMPx == CMP1)
    {
        if (NewState != DISABLE)
        {
            CMP->CMCSR |= CMP_CMCSR_CMEN1;
        }
        else
        {
            CMP->CMCSR &= ~(CMP_CMCSR_CMEN1);
        }
    }
    else if (CMPx == CMP2)
    {
        if (NewState != DISABLE)
        {
            CMP->CMCSR |= CMP_CMCSR_CMEN2;
        }
        else
        {
            CMP->CMCSR &= ~(CMP_CMCSR_CMEN2);
        }
    }
}

/**
  * @brief  Get comparator enable state.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @retval Comparator enable state.
  */
uint32_t CMP_IsEnabled(uint8_t CMPx)
{
    uint32_t Temp;
    assert_param(IS_CMP_NAME(CMPx));

    if (CMPx == CMP1)
    {
        Temp = (uint32_t)((CMP->CMCSR & CMP_CMCSR_CMEN1));
    }
    else if (CMPx == CMP2)
    {
        Temp = (uint32_t)((CMP->CMCSR & CMP_CMCSR_CMEN2)) >> CMP_CMCSR_CMEN2_Pos;
    }

    return Temp;
}

/**
  * @brief  Select ATU PWM Channel generate blank comparison intervals.
  * @param  Blank_Channel: pwm channel of produce blanking area selection..
  *         This parameter can be one of the following values:
  *            @arg CMP_Blank_Channel0
  *            @arg CMP_Blank_Channel1
  *            @arg CMP_Blank_Channel2
  * @retval None.
  */
void CMP_BlankChannelSel(uint8_t Blank_Channel)
{
    assert_param(IS_CMP_BLANK_CHANNEL(Blank_Channel));
    
    CMP->CMBSR &= ~(CMP_CMBSR_CMBE0A | CMP_CMBSR_CMBE1A | CMP_CMBSR_CMBE2A);
    CMP->CMBSR |= Blank_Channel;        
}

/**
  * @brief  Blanking CMP select.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @retval None.
  */
void CMP_BlankConfig(uint8_t CMPx)
{
    assert_param(IS_CMP_NAME(CMPx));
    
    
    if (CMPx == CMP1)
    {   
        CMP->CMBSR &= ~(CMP_CMBSR_CMBCA);    
    }
    else
    {
        CMP->CMBSR &= ~(CMP_CMBSR_CMBCA);
        CMP->CMBSR |= CMP_CMBSR_CMBCA_0;         
    }
}

/**
  * @brief  CMP Blanking polarity select.
  * @param  Polar: CMP Blanking polarity selection. 
  *         This parameter can be one of the following values:
  *            @arg CMP_Blank_Polarity_LOW
  *            @arg CMP_Blank_Polarity_High
  * @retval None.
  */
void CMP_BlankPolaritySel(uint32_t Polar)
{
    assert_param(IS_CMP_BLANK_POLAR(Polar));    
    
    CMP->CMBSR &= ~(CMP_CMBSR_CMBPA);
    CMP->CMBSR |= Polar;         
}

/**
  * @brief  Set CMP filter Clock prescale.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @param  Prescaler:
  *         This parameter can be one of the following values:
  *            @arg CMP_Filter_Prescaler_1
  *            @arg CMP_Filter_Prescaler_2
  *            @arg CMP_Filter_Prescaler_4
  *            @arg CMP_Filter_Prescaler_8
  *            @arg CMP_Filter_Prescaler_16
  *            @arg CMP_Filter_Prescaler_32
  *            @arg CMP_Filter_Prescaler_64
  *            @arg CMP_Filter_Prescaler_128
  * @retval None.
  */
void CMP_FilterPrescaler(uint8_t CMPx, uint32_t Prescaler)
{
    assert_param(IS_CMP_NAME(CMPx));
    assert_param(IS_CMP_FILTER_PRESCALER(Prescaler));

    if (CMPx == CMP1)
    {
        CMP->NFCLK  &= ~(CMP_NFCLK_CLK_DIV1);
        CMP->NFCLK  |= (Prescaler);
    }
    else if (CMPx == CMP2)
    {
        CMP->NFCLK  &= ~(CMP_NFCLK_CLK_DIV2);
        CMP->NFCLK  |= (Prescaler << 4);
    }
}

/**
  * @brief  Set CMP filer Clock Count.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @param  FilterCLKCnt: This parameter can be 0x0~0xF
  * @retval None.
  */
void CMP_SetFilterCnt(uint8_t CMPx, uint32_t FilterCLKCnt)
{
    assert_param(IS_CMP_NAME(CMPx));
    assert_param(IS_CMP_FILTER_COUNT(FilterCLKCnt));

    if (CMPx == CMP1)
    {
        CMP->NFCR  &= ~(CMP_NFCR_CMPT1_FLT);
        CMP->NFCR  |= (FilterCLKCnt);
    }
    else if (CMPx == CMP2)
    {
        CMP->NFCR  &= ~(CMP_NFCR_CMPT2_FLT);
        CMP->NFCR  |= (FilterCLKCnt << 4);
    }
}


/**
  * @brief  Set CMP filer Enable or Disable.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @param  NewState: new state of the CMP.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void CMP_FilterCmd(uint8_t CMPx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_CMP_NAME(CMPx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (CMPx == CMP1)
    {
        if (NewState != DISABLE)
        {
            CMP->NFCLK |= CMP_NFCLK_CLK_EN1;
        }
        else
        {
            CMP->NFCLK &= ~(CMP_NFCLK_CLK_EN1);
        }
    }
    else if (CMPx == CMP2)
    {
        if (NewState != DISABLE)
        {
            CMP->NFCLK |= CMP_NFCLK_CLK_EN2;
        }
        else
        {
            CMP->NFCLK &= ~(CMP_NFCLK_CLK_EN2);
        }
    }
}

/**
  * @brief  Get CMP origin output status.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @retval CMP origin output status.
  */
uint32_t CMP_GetOriginOutput(uint8_t CMPx)
{
    uint32_t Temp;
    assert_param(IS_CMP_NAME(CMPx));

    if (CMPx == CMP1)
    {
        Temp = (uint32_t)(CMP->CMSFR & CMP_CMSFR_CMO1_RA) ;
    }
    else if (CMPx == CMP2)
    {
        Temp = (uint32_t)(CMP->CMSFR & CMP_CMSFR_CMO2_RA) >> CMP_CMSFR_CMO2_RA_Pos;
    }

    return Temp;
}

/**
  * @brief  Get CMP filter output status.
  * @param  CMPx: where x can be 1, or 2 to select the CMP peripheral.
  * @retval CMP filter output status.
  */
uint32_t CMP_GetFilterOutput(uint8_t CMPx)
{
    uint32_t Temp;
    assert_param(IS_CMP_NAME(CMPx));

    if (CMPx == CMP1)
    {
        Temp = (uint32_t)(CMP->CMSFR & CMP_CMSFR_CMO1_FL) >> CMP_CMSFR_CMO1_FL_Pos;
    }
    else if (CMPx == CMP2)
    {
        Temp = (uint32_t)(CMP->CMSFR & CMP_CMSFR_CMO2_FL) >> CMP_CMSFR_CMO2_FL_Pos;
    }

    return Temp;
}

/**
  * @brief  Select CMP Internal BEMF input.
  * @param  BEMFInput: Input of internal BEMF.
  *         This parameter can be one of the following values:
  *            @arg CMP_BEMF_INPUT_HIZ
  *            @arg CMP_BEMF_INPUT_CMP1
  *            @arg CMP_BEMF_INPUT_CMP2
  * @retval None.
  */
void CMP_BEMFSel(uint32_t BEMFInput)
{
    /* Check the parameters */
    assert_param(IS_CMP_BEMF_INPUT_SEL(BEMFInput));
  
    /* Clear CMBMF bit field */
    CMP->CMISS &= (uint32_t)~CMP_CMISS_CMBMF;
  
    /* Set CMBMF bit field */
    CMP->CMISS |= (uint32_t)BEMFInput;
}

/**
  * @}
  */

/** @defgroup CMP_Group3 Interrupts and flags management functions
 *  @brief    Interrupts and flags management functions
 @verbatim
 ===============================================================================
            ##### Interrupts and flags management functions #####
 ===============================================================================

    [..] This section provides functions allowing to configure the CMP Interrupts
         and to get the status and clear flags and Interrupts pending bits.
@endverbatim

 * @{
 */

/**
  * @brief  Enables or disables the specified CMP interrupts.
  * @param  CMP_IT: specifies the CMP interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  *            @arg CMP_CMP1_Rising_Flag: CMP1 rising interrupt flag
  *            @arg CMP_CMP1_Falling_Flag: CMP1 falling interrupt flag
  *            @arg CMP_CMP2_Rising_Flag: CMP2 rising interrupt flag
  *            @arg CMP_CMP2_Falling_Flag: CMP2 falling interrupt flag
  * @param  NewState: new state of the specified CMP interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CMP_ITConfig(uint32_t CMP_IT, FunctionalState NewState)
{
    assert_param(IS_CMP_IT(CMP_IT));

    if (NewState != DISABLE)
    {
        CMP->CMIER |= CMP_IT;
    }
    else
    {
        CMP->CMIER &= ~(CMP_IT);
    }
}

/**
  * @brief  Checks whether the specified CMP interrupt has occurred or not.
  * @param  FLAG: specifies the CMP interrupt source to check.
  *         This parameter can be one of the following values:
  *            @arg CMP_CMP1_Rising_Flag: CMP1 rising interrupt flag
  *            @arg CMP_CMP1_Falling_Flag: CMP1 falling interrupt flag
  *            @arg CMP_CMP2_Rising_Flag: CMP2 rising interrupt flag
  *            @arg CMP_CMP2_Falling_Flag: CMP2 falling interrupt flag
  * @retval The new state of CMP interrupt FLAG (SET or RESET).
  */
ITStatus CMP_GetITStatus(uint32_t FLAG)
{
    ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_CMP_IT(FLAG));

    /* Check the status of the specified RCC interrupt */
    if ((CMP->CMIFR & FLAG) != (uint32_t)RESET)
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
  * @brief  Clears the CMP's interrupt pending bits.
  * @param  CMP_IT: specifies the interrupt pending bit to clear.
  *         This parameter can be any combination of the following values:
  *            @arg CMP_CMP1_Rising_Flag: CMP1 rising interrupt flag
  *            @arg CMP_CMP1_Falling_Flag: CMP1 falling interrupt flag
  *            @arg CMP_CMP2_Rising_Flag: CMP2 rising interrupt flag
  *            @arg CMP_CMP2_Falling_Flag: CMP2 falling interrupt flag
  * @retval None.
  */
void CMP_ClearITPendingBit(uint32_t CMP_IT)
{
    /* Check the parameters */
    assert_param(IS_CMP_IT(CMP_IT));

    /* Clear the selected interrupt pending bits */
    CMP->CMIFR = (uint32_t)CMP_IT ;
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
