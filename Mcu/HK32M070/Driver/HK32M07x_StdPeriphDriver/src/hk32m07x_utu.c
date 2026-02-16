/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_utu.c
* @brief:     This file provides firmware functions to manage the following
* @author:    AE Team
* @version:   V1.0.0/2024-12-16
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_utu.h"


/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup UTU UTU
  * @brief UTU driver modules
  * @{
  @verbatim

   ===============================================================================
                    ##### How to use this driver #####
   ===============================================================================
      [..] This driver provides functions to configure and program the UTU
           of all HK32F0xxA devices These functions are split in 8 groups:
           (#) UTU TimeBase management: this group includes all needed functions
               to configure the TM Timebase unit:
               (++) Set/Get Prescaler.
               (++) Set/Get TimePriod(register:UPR).
               (++) Select the One Pulse mode.
               (++) Update Request Configuration.
               (++) Update Disable Configuration.
               (++) Auto-Preload Configuration.
               (++) Enable/Disable the counter.
               (++) Set Triger.

           (#) UTU Output Compare management: this group includes all needed
               functions to configure the Capture/Compare unit used in Output
               compare mode:
               (++) Configure each channel, independently, in Output Compare mode.
               (++) Select the output compare modes.
               (++) Select the Polarities of each channel.
               (++) Set/Get the Capture/Compare register values.
               (++) Select the Output Compare Fast mode.
               (++) Select the Output Compare Forced mode.
               (++) Enable/Disable the Capture/Compare Channels.

           (#) UTU Input Capture management: this group includes all needed
               functions to configure the Capture/Compare unit used in
               Input Capture mode:
               (++) Configure each channel in input capture mode.
               (++) Configure Channel1/2 in PWM Input mode.
               (++) Get the Capture/Compare values.

          (#) Advanced-control specific features
              (++) Configures the Break input, dead time and Lock level
              (++) Enable/Disable the UTU peripheral Main Outputs

           (#) UTU interrupts, flags management.
               (++) Enable/Disable interrupt sources.
               (++) Get flags status.
               (++) Clear flags/ Pending bits.

           (#) UTU synchronization management: this group includes all needed.
               functions to configure the Synchronization unit:
               (++) Select Input Trigger.
               (++) Select Output Trigger.
  @endverbatim
    *
    ******************************************************************************
    */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup UTU_Private_Functions UTU_Private_Functions
  * @{
  */


/**
  * @}
  */
/** @defgroup UTU_Group1 TimeBase management functions
 *  @brief   TimeBase management functions
 *
@verbatim
 ===============================================================================
               ##### TimeBase management functions #####
 ===============================================================================

        *** UTU Driver: how to use it in Timing(Time base) Mode ***
 ===============================================================================
    [..] To use the Timer in Timing(Time base) mode, the following steps are
         mandatory:
         (#) Enable UTU clock using
             RCC_APBxPeriphClockCmd(RCC_APBxPeriph_UTUx, ENABLE) function.
         (#) Fill the UTU_TimeBaseInitStruct with the desired parameters.
         (#) Call UTU_TimeBaseInit(UTUx, &UTU_TimeBaseInitStruct) to configure
             the Time Base unit with the corresponding configuration.
         (#) Enable the NVIC if you need to generate the update interrupt.
         (#) Enable the corresponding interrupt using the function
             UTU_ITConfig(UTUx, UTU_IT_Update).
         (#) Call the UTU_Cmd(ENABLE) function to enable the UTU counter.
    [..]
        (@) All other functions can be used seperatly to modify, if needed,
            a specific feature of the Timer.

@endverbatim
  * @{
  */


/**
  * @brief  Deinitializes the UTUx peripheral registers to their default reset values.
  * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
  * @note   UTU1 counter is 32bits.
  * @note   UTU2 counter is 16bits.
  * @retval None
  */
void UTU_DeInit(UTU_TypeDef* UTUx)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    if (UTUx == UTU1)
    {
        RCC_APB2PeriphResetCmd(RCC_APB1Periph_UTU1, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB1Periph_UTU1, DISABLE);
    }
    else if (UTUx == UTU2)
    {
        RCC_APB1PeriphResetCmd(RCC_APB2Periph_UTU2, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB2Periph_UTU2, DISABLE);
    }

}

/**
  * @brief  Initializes the UTUx Time Base Unit peripheral according to
  *         the specified parameters in the UTU_TimeBaseInitStruct.
  * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  UTU_TimeBaseInitStruct: pointer to a UTU_TimeBaseInitTypeDef
  *         structure that contains the configuration information for
  *         the specified UTU peripheral.
  * @retval None
  */
void UTU_TimeBaseInit(UTU_TypeDef* UTUx, UTU_TimeBaseInitTypeDef* UTU_TimeBaseInitStruct)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_CKD_DIV(UTU_TimeBaseInitStruct->UTU_Prescaler));

    /* set UTU timer period and Triger counter */
    UTUx->UPR = UTU_TimeBaseInitStruct->UTU_Period;
    //UTUx->TRG0 = UTU_TimeBaseInitStruct->UTU_TrigCnt;

    /* set UTU precaler & repetion */
    tmpreg = UTUx->UCSR;
    tmpreg &= (~(UTU_UCSR_CLK_DIV_Mask | UTU_UCSR_SCWM_Mask));
    tmpreg |= (UTU_TimeBaseInitStruct->UTU_Prescaler | UTU_TimeBaseInitStruct->UTU_PeriodMode);

    UTUx->UCSR = tmpreg;
}

/**
  * @brief  Fills each UTU_TimeBaseInitStruct member with its default value.
  * @param  UTU_TimeBaseInitStruct: pointer to a UTU_TimeBaseInitTypeDef structure
  *         which will be initialized.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @retval None
  */
void UTU_TimeBaseStructInit(UTU_TimeBaseInitTypeDef* UTU_TimeBaseInitStruct)
{
    /* Set the default configuration */
    UTU_TimeBaseInitStruct->UTU_Period = 0xFFFFFFFF;
    UTU_TimeBaseInitStruct->UTU_Prescaler = UTU_CKD_DIV1;
    //UTU_TimeBaseInitStruct->UTU_TrigCnt = 0x0;

    /* continue output mode as default */
    UTU_TimeBaseInitStruct->UTU_PeriodMode = UTU_PRD_REPEAT;
}

/**
  * @brief  Configures the UTUx Prescaler.
  * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  Prescaler: specifies the Prescaler Register value
  * @retval None
  */
void UTU_PrescalerConfig(UTU_TypeDef* UTUx, uint16_t Prescaler)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    /* set UTU precaler & repetion */
    tmpreg = UTUx->UCSR;
    tmpreg &= (~UTU_UCSR_CLK_DIV_Mask );
    tmpreg |= Prescaler;

    UTUx->UCSR = tmpreg;
}

/**
    * @brief  Gets the UTUx Prescaler value.
    * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
    * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
    * @retval the Prescaler Register value
  */
uint32_t UTU_GetPrescaler(UTU_TypeDef* UTUx)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    /* Get the Prescaler Register value */
    tmpreg = UTUx->UCSR;
    tmpreg &= UTU_UCSR_CLK_DIV_Mask;

    return tmpreg;
}


/**
   * @brief  Sets the UTUx Counter Register value
   * @param  UTUx: where x can be 1, 2 to select the UTU
   *         peripheral.
   * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
   * @param  Counter: specifies the Counter register new value.
   * @retval None
  */
void UTU_SetCounter(UTU_TypeDef* UTUx, uint32_t Counter)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    /* Set the Counter Register value */
    UTUx->CNT = Counter;
}

/**
    * @brief  Gets the UTUx Counter value.
    * @param  UTUx: where x can be 1, 2 to select the UTU
    *         peripheral.
    * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
    * @retval the UTUx Counter value
  */
uint32_t UTU_GetCounter(UTU_TypeDef* UTUx)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    /* Get the Counter Register value */
    return UTUx->CNT;
}

/**
  * @brief  Gets the UTUx Counter Period value.
  * @param  UTUx: where x can be 1, 2 to select the UTU
  *         peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @retval Counter Register Period value.
  */
uint32_t UTU_GetPeriod(UTU_TypeDef* UTUx)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    /* Get the Period Register value */
    return UTUx->UPR;
}

/**
  * @brief  Sets the UTUx Period Register value
  * @param  UTUx: where x can be 1, 2 to select the UTU
  *         peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  Counter: specifies the Counter register new value.
  * @retval None
  */
void UTU_SetPeriod(UTU_TypeDef* UTUx, uint32_t Counter)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    /* Set the Period Register value */
    UTUx->UPR = Counter;
}

/**
  * @brief  Gets the UTUx Period update mode.
  * @param  UTUx: where x can be 1, 2 to select the UTU
  *         peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @retval the UTUx Period update mode
  */
uint32_t UTU_GetPeriodUpdateMode(UTU_TypeDef* UTUx)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    /* Get the Period Register value */
    return (UTUx->UCSR & UTU_UCSR_ARPR_Mask);
}

/**
  * @brief  Sets the UTUx Period update mode
  * @param  UTUx: where x can be 1, 2 to select the UTU
  *         peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  NewMode: specifies the period update new mode.
  * @retval None
  */
void UTU_SetPeriodUpdateMode(UTU_TypeDef* UTUx, uint32_t NewMode)
{
    uint32_t Temp;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_PERIOD_UP_MODE(NewMode));

    /* Set the UCSR Register value */
    Temp = UTUx->UCSR;

    Temp &= (~UTU_UCSR_ARPR_Mask);
    Temp |= NewMode;
    UTUx->UCSR = Temp;
}

/**
  * @brief  Enables or disables the specified UTU peripheral.
  * @param  UTUx: where x can be 1, 2 to select the UTUx
  *         peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  NewState: new state of the UTUx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UTU_Cmd(UTU_TypeDef* UTUx, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* get UCSR  */
    tmpreg = UTUx->UCSR;

    if (NewState != DISABLE)
    {
        /* Enable the UTU Counter */
        tmpreg |= UTU_UCSR_CNT_EN;
    }
    else
    {
        /* Disable the UTU Counter */
        tmpreg &= ~UTU_UCSR_CNT_EN;
    }

    UTUx->UCSR = tmpreg;

}

/**
     * @brief  UTU Set Output Triger  Register.
     * @param  UTUx: where x can be 1, 2 to select the UTUx
     *         peripheral.
     * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
     * @param  TrigerCnt:  the triger register counter.
     *         This parameter can be one of the value @ref UTU_Triger_Source.
     * @retval None
  */
void UTU_SetOutputTrigerReg(UTU_TypeDef* UTUx, uint32_t TrigerCnt)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    /* set input triger soucre */
    UTUx->TRG0 = TrigerCnt;

}


/**
  * @brief  UTU External peripheral Triger source select.
  * @param  UTUx: where x can be 1, 2 to select the UTUx
  *         peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  TrigerSource:  the triger input source.
  *         This parameter can be one of the value @ref UTU_Triger_Source.
  * @retval None
  */
void UTU_InputTrigerConfig(UTU_TypeDef* UTUx, uint32_t TrigerSource)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_TRG_S(TrigerSource));

    /* set input triger soucre */
    tmpreg = UTUx->UTSS;

    tmpreg &= ~(UTU_UTSS_UTRGS_Mask);

    tmpreg |= TrigerSource;

    UTUx->UTSS = tmpreg;

}


/**
  * @brief  UTU peripheral Triger Enable or Disable.
  * @param  UTUx: where x can be 1, 2 to select the UTUx
  *         peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  NewState: new state of the UTUx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UTU_InputTrigerCmd(UTU_TypeDef* UTUx, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* set UTU precaler & repetion */
    tmpreg = UTUx->UTSS;

    tmpreg &= ~(UTU_UTSS_UTEN_Mask);

    if (NewState != DISABLE)
    {
        /* Enable the TIM Counter */
        tmpreg |= UTU_UTSS_UTEN;
    }
    else
    {
        /* Disable the TIM Counter */
        tmpreg &= ~UTU_UTSS_UTEN;
    }

    UTUx->UTSS = tmpreg;

}

/**
  * @}
  */

/** @defgroup UTU_Group2 OC guard specific features
 *  @brief   UTU compare output guard specific features
 *
@verbatim
 ===============================================================================
    ##### Advanced-control timers (UTU1) specific features #####
 ===============================================================================

       ===================================================================
              *** UTU Driver: how to use the Break feature ***
       ===================================================================
       [..] After configuring the Timer channel(s) in the appropriate Output Compare mode:

           (#) Fill the OCGuardStructInit with the desired parameters for the Timer
               guard Polarity, dead time, Lock level, and the
               AOE(automatic output enable).

           (#) Call UTU_OCGuardConfig(UTUx, &UTU_BDTRInitStruct) to configure the Timer

           (#) Enable the guard feature using UTU_OCGuardCmd(UTU1, ENABLE)

           (#) Once the guard even occurs, the Timer's output signals are put in reset
               state or in a known state (according to the configuration made in
               UTU_OCGuardConfig() function).

@endverbatim
  * @{
  */
/**
  * @brief  Configures the: Break feature, dead time, Lock level, OSSI/OSSR State
  *         and the AOE(automatic output enable).
  * @param  UTUx: where x can be  1 , 2 to select the UTU
  * @param  UTU_BDTRInitStruct: pointer to a UTU_BDTRInitTypeDef structure that
  *         contains the BDTR Register configuration  information for the UTU peripheral.
  * @retval None
  */
void UTU_OCGuardConfig(UTU_TypeDef* UTUx, UTU_BDTRInitTypeDef *UTU_BDTRInitStruct)
{

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_CHNL_GUARD_POLARITY(UTU_BDTRInitStruct->UTU_ChnlBGuardLevel));
    assert_param(IS_UTU_CHNL_GUARD_POLARITY(UTU_BDTRInitStruct->UTU_ChnlAGuardLevel));
    assert_param(IS_UTU_AUTOMATIC_OUTPUT_STATE(UTU_BDTRInitStruct->UTU_GuardAutoCancle));
    assert_param(IS_UTU_GUARD_EN_POLARITY(UTU_BDTRInitStruct->UTU_GuardEnPolarity));
    assert_param(IS_UTU_GUART_SOURCE(UTU_BDTRInitStruct->UTU_GuardSource));
    assert_param(IS_UTU_HALT_GUARD_STATE(UTU_BDTRInitStruct->UTU_HaltGuardEn));
    assert_param(IS_UTU_GUARD_STATE(UTU_BDTRInitStruct->UTU_GuardEn));

    /* Set the Lock level, the Break enable Bit and the Ploarity, the OSSR State,
       the OSSI State, the dead time value and the Automatic Output Enable Bit */
    UTUx->UPPS = (uint32_t)UTU_BDTRInitStruct->UTU_ChnlBGuardLevel | UTU_BDTRInitStruct->UTU_ChnlAGuardLevel |
                 UTU_BDTRInitStruct->UTU_GuardAutoCancle | UTU_BDTRInitStruct->UTU_GuardEnPolarity |
                 UTU_BDTRInitStruct->UTU_GuardSource | UTU_BDTRInitStruct->UTU_HaltGuardEn |
                 UTU_BDTRInitStruct->UTU_GuardEn;

    /* set filter param */
    {
        uint32_t Temp = UTUx->NFCR;

        Temp &= (~UTU_NFCR_UPI_FLT_Mask);

        Temp |= (UTU_BDTRInitStruct->UTU_GuardFilter << UTU_NFCR_UPI_FLT_Pos);

        UTUx->NFCR = Temp;
    }
}

/**
  * @brief  Fills each UTU_BDTRInitStruct member with its default value.
  * @param  UTU_BDTRInitStruct: pointer to a UTU_BDTRInitTypeDef structure which
  *         will be initialized.
  * @retval None
  */
void UTU_OCGuardStructInit(UTU_BDTRInitTypeDef* UTU_BDTRInitStruct)
{
    /* Set the default configuration */
    UTU_BDTRInitStruct->UTU_ChnlBGuardLevel = UTU_CHNL_A_B_GuardPolarity_Low;
    UTU_BDTRInitStruct->UTU_ChnlAGuardLevel = UTU_CHNL_A_B_GuardPolarity_Low;
    UTU_BDTRInitStruct->UTU_GuardAutoCancle = UTU_AutomaticOutput_Disable;
    UTU_BDTRInitStruct->UTU_GuardEnPolarity = UTU_GuardPolarity_High;
    UTU_BDTRInitStruct->UTU_GuardSource = UTU_Guard_S_BKIN;
    //UTU_BDTRInitStruct->UTU_HaltGuardEn = UTU_Halt_Guard_Enable;
    UTU_BDTRInitStruct->UTU_HaltGuardEn = UTU_Halt_Guard_Disable;
    UTU_BDTRInitStruct->UTU_GuardEn = UTU_Guard_Disable;
    UTU_BDTRInitStruct->UTU_GuardFilter = 0;
}

/**
  * @brief  Enables or disables the UTU Guard.
  * @param  UTUx: where x can be 1, 2 to select the UTUx peripheral.
  * @param  NewState: new state of the UTU peripheral Main Outputs.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UTU_OCGuardCmd(UTU_TypeDef* UTUx, FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Output guard */
        UTUx->UPPS |= UTU_UPPS_UPE;
    }
    else
    {
        /* Disable the UTU Main Output */
        UTUx->UPPS &= (~((uint32_t)UTU_UPPS_UPE));
    }
}

/**
  * @}
  */

/** @defgroup UTU_Group3 Output Compare management functions
 *  @brief    Output Compare management functions
 *
@verbatim
 ===============================================================================
              ##### Output Compare management functions #####
 ===============================================================================
        *** UTU Driver: how to use it in Output Compare Mode ***
 ===============================================================================
    [..] To use the Timer in Output Compare mode, the following steps are mandatory:
         (#) Enable UTU clock using
             RCC_APBxPeriphClockCmd(RCC_APBxPeriph_UTUx, ENABLE) function.
         (#) Configure the UTU pins by configuring the corresponding GPIO pins
         (#) Configure the Time base unit as described in the first part of this
             driver.
         (#) Fill the UTU_OCInitStruct with the desired parameters including:
             (++) The UTU Output Compare mode: UTU_OCMode.
             (++) UTU Output State: UTU_OutputState.
             (++) UTU Pulse value: UTU_Pulse.
             (++) UTU Output Compare Polarity : UTU_OCPolarity.
         (#) Call UTU_OCxInit(UTUx, &UTU_OCInitStruct) to configure the desired
             channel with the corresponding configuration.
         (#) Call the UTU_Cmd(ENABLE) function to enable the UTU counter by software
             or Call UTU_TrigerConfig and UTU_TrigerCmd by Triger mode.
    [..]
        (@) All other functions can be used separately to modify, if needed,
          a specific feature of the Timer.
        (@) In case of PWM mode, this function is mandatory:
            UTU_OCxPreloadConfig(UTUx, UTU_OCPreload_ENABLE).
        (@) If the corresponding interrupt request are needed, the user should:
            (#@) Enable the NVIC to use the UTU interrupts.
            (#@) Enable the corresponding interrupt using the function
                 UTU_ITConfig(UTUx, UTU_IT_CCx).

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the UTUx Channel1 according to the specified
  *         parameters in the UTU_OCInitStruct.
  * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  UTU_OCInitStruct: pointer to a UTU_OCInitTypeDef structure
  *         that contains the configuration information for the specified UTU
  *         peripheral.
  * @retval None
  */
void UTU_OCInit(UTU_TypeDef* UTUx, UTU_OCInitTypeDef* UTU_OCInitStruct)
{
    uint32_t TmpReg;

    __IO uint32_t * p_RegCtrl;
    __IO uint32_t * p_RegCmp;
    //__IO uint32_t * p_RegDBT;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_OC_MODE(UTU_OCInitStruct->UTU_OCMode));
    assert_param(IS_UTU_OC_POLARITY(UTU_OCInitStruct->UTU_OCPolarity));
    assert_param(IS_UTU_CHANNEL(UTU_OCInitStruct->UTU_Chnl));

    if (UTU_OCInitStruct->UTU_Chnl == UTU_CHANNEL_A)
    {
        p_RegCtrl = &UTUx->UCCSA;
        p_RegCmp = &UTUx->UCCRA;
        //p_RegDBT = &UTUx->DBTA;
    }
    else
    {
        p_RegCtrl = &(UTUx->UCCSB);
        p_RegCmp = &(UTUx->UCCRB);
        //p_RegDBT = &(UTUx->DBTB);
    }

    /* Disable the Channel 1: Reset the Enable Bit */
    *p_RegCtrl &= (~UTU_UCCSA_CCL_EN);

    /* Set the OC mode */
    TmpReg = UTUx->UCSR;
    TmpReg &= (~UTU_UCSR_UTMS_Mask);
    TmpReg |= UTU_OCInitStruct->UTU_OCMode;
    UTUx->UCSR = TmpReg;

    /* Set the compare value */
    *p_RegCmp =  UTU_OCInitStruct->UTU_CMP_Value;

    /* Set the OC polarity */
    TmpReg = *p_RegCtrl;
    TmpReg &= (~UTU_UCCSA_POL_Mask);
    TmpReg |= UTU_OCInitStruct->UTU_OCPolarity;
    *p_RegCtrl = TmpReg;

    /* Set the DBT */
    //*p_RegDBT = UTU_OCInitStruct->UTU_DBT;
}

/**
   * @brief  Fills each UTU_OCInitStruct member with its default value.
   * @param  UTU_OCInitStruct: pointer to a UTU_OCInitTypeDef structure which will
   *         be initialized.
   * @retval None
  */
void UTU_OCStructInit(UTU_OCInitTypeDef* UTU_OCInitStruct)
{
    /* Set the default configuration */
    UTU_OCInitStruct->UTU_OCMode = UTU_OCMode_NORMAL;
    UTU_OCInitStruct->UTU_OCPolarity = UTU_OCPolarity_Normal;
    UTU_OCInitStruct->UTU_CMP_Value = 0xff;
    //UTU_OCInitStruct->UTU_DBT = 0;
    UTU_OCInitStruct->UTU_Chnl = UTU_CHANNEL_A;
}

/**
  * @brief  Sets the UTUx Capture Compare1 Register value
  * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  Chnl: specifies which channel will be used.
  * @param  Compare1: specifies the Capture Compare1 register new value.
  * @retval None
  */
void UTU_OCSetCompare(UTU_TypeDef* UTUx, uint32_t Chnl, uint32_t Compare1)
{
    __IO uint32_t * p_RegCmp;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_CHANNEL(Chnl));

    if (Chnl == UTU_CHANNEL_A)
    {
        p_RegCmp = &(UTUx->UCCRA);
    }
    else
    {
        p_RegCmp = &(UTUx->UCCRB);
    }

    /* Set the compare value */
    *p_RegCmp =  Compare1;
}


/**
    * @brief  Sets the UTUx Dead Bank Time  Register value
    * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
    * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
    * @param  Chnl: specifies the output channel to be used.
    * @param  Value: the dead bank time to be set.
    * @retval None
  */
void UTU_OCSetDBT(UTU_TypeDef* UTUx, uint32_t Chnl, uint32_t Value)
{
    __IO uint32_t * p_RegCmp;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_CHANNEL(Chnl));
    assert_param(IS_UTU_DBT(Value));

    if (Chnl == UTU_CHANNEL_A)
    {
        p_RegCmp = &(UTUx->DBTA);
    }
    else
    {
        p_RegCmp = &(UTUx->DBTB);
    }

    /* Set the compare value */
    *p_RegCmp =  Value;
}

/**
    * @brief  Sets the UTUx Capture Compare1 Register update mode
    * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
    * @param  mode: whether update CC register immediately,  can be one of @ref UTU_CMP_Update_Mode.
    * @retval None
  */
void UTU_OCCmpUpModeConfig(UTU_TypeDef* UTUx, uint32_t mode)
{
    uint32_t tempReg;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_CMP_Update_MODE(mode));

    tempReg = UTUx->UCSR;
    tempReg &= (~UTU_UCSR_ARCR_Mask);
    tempReg |= mode;

    /* Set the Register */
    UTUx->UCSR =  tempReg;
}


/**
  * @brief  Enables or disables the UTU Capture Compare.
  * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  NewState: new state of the UTUx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UTU_OCCmd(UTU_TypeDef* UTUx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the UTU Main Output */
        UTUx->UPPS |= UTU_UPPS_UPOE;
    }
    else
    {
        /* Disable the UTU Main Output */
        UTUx->UPPS &= (~((uint32_t)UTU_UPPS_UPOE));
    }
}

/**
  * @}
  */

/** @defgroup UTU_Group4 Input Capture management functions
 *  @brief    Input Capture management functions
 *
@verbatim
 ===============================================================================
             ##### Input Capture management functions #####
 ===============================================================================

          *** UTU Driver: how to use it in Input Capture Mode ***
 ===============================================================================
    [..] To use the Timer in Input Capture mode, the following steps are mandatory:
         (#) Enable UTU clock using RCC_APBxPeriphClockCmd(RCC_APBxPeriph_UTUx, ENABLE)
             function.
         (#) Configure the UTU pins by configuring the corresponding GPIO pins.
         (#) Configure the Time base unit as described in the first part of this
             driver, if needed, else the Timer will run with the default configuration:
             (++) Autoreload value = 0xFFFF.
             (++) Prescaler value = 0x0000.
             (++) Counter mode = Up counting.
             (++) Clock Division = UTU_CKD_DIV1.
         (#) Fill the UTU_ICInitStruct with the desired parameters including:
             (++) UTU Channel: UTU_Channel.
             (++) UTU Input Capture polarity: UTU_ICPolarity.
             (++) UTU Input Capture selection: UTU_ICSelection.
             (++) UTU Input Capture Prescaler: UTU_ICPrescaler.
             (++) UTU Input CApture filter value: UTU_ICFilter.
         (#) Call UTU_ICInit(UTUx, &UTU_ICInitStruct) to configure the desired
             channel with the corresponding configuration and to measure only
             frequency or duty cycle of the input signal,or, Call
             UTU_PWMIConfig(UTUx, &UTU_ICInitStruct) to configure the desired
             channels with the corresponding configuration and to measure the
             frequency and the duty cycle of the input signal.
         (#) Enable the NVIC or the DMA to read the measured frequency.
         (#) Enable the corresponding interrupt (or DMA request) to read
             the Captured value, using the function UTU_ITConfig(UTUx, UTU_IT_CCx)
             (or UTU_DMA_Cmd(UTUx, UTU_DMA_CCx)).
         (#) Call the UTU_Cmd(ENABLE) function to enable the UTU counter.
         (#) Use UTU_GetCapturex(UTUx); to read the captured value.
    [..]
        (@) All other functions can be used separately to modify, if needed,
            a specific feature of the Timer.

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the UTU peripheral according to the specified
  *         parameters in the UTU_ICInitStruct.
  * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
  * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
  * @param  UTU_ICInitStruct: pointer to a UTU_ICInitTypeDef structure
  *         that contains the configuration information for the specified UTU
  *         peripheral.
  * @retval None
  */
void UTU_ICInit(UTU_TypeDef* UTUx, UTU_ICInitTypeDef* UTU_ICInitStruct)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_CHANNEL(UTU_ICInitStruct->UTU_Channel));
    assert_param(IS_UTU_IC_SELECTION(UTU_ICInitStruct->UTU_ICSelection));
    assert_param(IS_UTU_IC_POLARITY(UTU_ICInitStruct->UTU_ICPolarity));
    assert_param(IS_UTU_IC_CNT_CLR_SET(UTU_ICInitStruct->UTU_ClrCnt));

    /* config channel */
    temp = ( UTU_ICInitStruct->UTU_ClrCnt | UTU_ICInitStruct->UTU_ICSelection
             | UTU_ICInitStruct->UTU_ICPolarity);

    if (UTU_ICInitStruct->UTU_Channel == UTU_CHANNEL_A)
    {
        /* ICA Configuration */
        UTUx->UCCSA = temp;
    }
    else if (UTU_ICInitStruct->UTU_Channel == UTU_CHANNEL_B)
    {
        /* ICB Configuration */
        UTUx->UCCSB = temp;
    }

    /* config input capture filter */
    temp = UTUx->NFCR;

    temp &= (~UTU_NFCR_UIO_FLT_Mask);
    temp |= (UTU_ICInitStruct->UTU_ICFilter & UTU_NFCR_UIO_FLT_Mask);

    UTUx->NFCR = temp;

    /* config timer work mode */
    temp = UTUx->UCSR;
    temp &= (~UTU_UCSR_UTMS_Mask);
    temp |= ((uint32_t)0x02) << UTU_UCSR_UTMS_Pos;
    UTUx->UCSR = temp;

}

/**
  * @brief  Fills each UTU_ICInitStruct member with its default value.
  * @param  UTU_ICInitStruct: pointer to a UTU_ICInitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void UTU_ICStructInit(UTU_ICInitTypeDef* UTU_ICInitStruct)
{
    /* Set the default configuration */
    UTU_ICInitStruct->UTU_Channel = UTU_CHANNEL_A;
    UTU_ICInitStruct->UTU_ICPolarity = UTU_ICPolarity_Rising;
    UTU_ICInitStruct->UTU_ICSelection = UTU_ICSelection_IO_SELF;
    UTU_ICInitStruct->UTU_ICFilter = 0x00;
    UTU_ICInitStruct->UTU_ClrCnt = UTU_IC_CNT_Not_Clr;
}

/**
   * @brief  Gets the UTUx Input Capture value.
   * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
   * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
   * @param  Chnl: which channel used for capture.
   * @retval Input Capture value
  */
uint32_t UTU_GetCapture(UTU_TypeDef* UTUx, uint32_t Chnl)
{
    uint32_t Temp;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    if (Chnl == UTU_ICSelection_IO_SELF)
    {
        Temp = UTUx->UCCRA;
    }
    else
    {
        Temp = UTUx->UCCRB;
    }

    return Temp;
}

/**
    * @brief  Gets the UTUx Input Capture Status value.
    * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
    * @note   UTU1 counter is 32 bits while UTU2 counter is 16 bits.
    * @retval Input Capture Status
  */
uint32_t UTU_GetCaptureStatus(UTU_TypeDef* UTUx)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));

    return UTUx->USR;
}


/**
   * @}
   */

/** @defgroup UTU_Group5 Interrupts and flags management functions
 *  @brief    Interrupts and flags management functions
 *
@verbatim
 ===============================================================================
        ##### Interrupts and flags management functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the specified UTU interrupts.
  * @param  UTUx: where x can be 1, 2 to select the UTUx peripheral.
  * @param  UTU_IT: specifies the UTU interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *            @arg UTU_IT_UCRA: UTU Channel A rising Edge interrupt
  *            @arg UTU_IT_UCFA: UTU Channel A falling Edge interrupt
  *            @arg UTU_IT_UCRB: UTU Channel B rising Edge interrupt
  *            @arg UTU_IT_UCFB: UTU Channel B falling Edge interrupt
  *            @arg UTU_IT_OVF: UTU over flow interrupt
  *            @arg UTU_IT_UPP: UTU Guard interrupt
  * @param  NewState: new state of the UTU interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UTU_ITConfig(UTU_TypeDef* UTUx, uint16_t UTU_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_IT(UTU_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Interrupt sources */
        UTUx->UIER |= UTU_IT;
    }
    else
    {
        /* Disable the Interrupt sources */
        UTUx->UIER &= (uint16_t)~UTU_IT;
    }
}

/**
  * @brief  Checks whether the specified UTU flag is set or not.
  * @param  UTUx: where x can be 1, 2 to select the UTU peripheral.
  * @param  UTU_FLAG: specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg UTU_FLAG_UCRA: UTU update Flag
  *            @arg UTU_FLAG_UCFA: UTU Capture Compare 1 Flag
  *            @arg UTU_FLAG_UCRB: UTU Capture Compare 2 Flag
  *            @arg UTU_FLAG_UCFB: UTU Capture Compare 3 Flag
  *            @arg UTU_FLAG_OVF: UTU Capture Compare 4 Flag
  *            @arg UTU_FLAG_UPP: UTU Commutation Flag
  * @note   UTU_FLAG_COM is used only with UTU1 UTU15, UTU16 and UTU17.
  * @retval The new state of UTU_FLAG (SET or RESET).
  */
FlagStatus UTU_GetFlagStatus(UTU_TypeDef* UTUx, uint16_t UTU_FLAG)
{
    ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_GET_FLAG(UTU_FLAG));

    if ((UTUx->UIFR & UTU_FLAG) != (uint16_t)RESET)
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
  * @brief  Clears the UTUx's pending flags.
  * @param  UTUx: where x can be 1 or 2 to select the UTU peripheral.
  * @param  UTU_FLAG: specifies the flag bit to clear.
  *         This parameter can be any combination of the following values:
  *            @arg UTU_FLAG_UCRA: CHNL_A Rising Edge Flag.
  *            @arg UTU_FLAG_UCFA: CHNL_A Falling Edge Flag
  *            @arg UTU_FLAG_UCRB: CHNL_B Rising Edge Flag.
  *            @arg UTU_FLAG_UCFB: CHNL_B Falling Edge Flag
  *            @arg UTU_FLAG_OVF: UTU conter overflow Flag.
  *            @arg UTU_FLAG_UPP: UTU Protection Flag.
  * @retval None
  */
void UTU_ClearFlag(UTU_TypeDef* UTUx, uint16_t UTU_FLAG)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_CLEAR_FLAG(UTU_FLAG));

    /* Clear the flags */
    UTUx->UIFR |= UTU_FLAG;
}

/**
  * @brief  Checks whether the UTU interrupt has occurred or not.
  * @param  UTUx: where x can be 1 or 2 to select the UTU peripheral.
  * @param  UTU_IT: specifies the UTU interrupt source to check.
  *         This parameter can be one of the following values:
  *            @arg UTU_FLAG_UCRA: CHNL_A Rising Edge Flag.
  *            @arg UTU_FLAG_UCFA: CHNL_A Falling Edge Flag
  *            @arg UTU_FLAG_UCRB: CHNL_B Rising Edge Flag.
  *            @arg UTU_FLAG_UCFB: CHNL_B Falling Edge Flag
  *            @arg UTU_FLAG_OVF: UTU conter overflow Flag.
  *            @arg UTU_FLAG_UPP: UTU Protection Flag.
  * @retval The new state of the UTU_IT(SET or RESET).
  */
ITStatus UTU_GetITStatus(UTU_TypeDef* UTUx, uint16_t UTU_IT)
{
    ITStatus bitstatus = RESET;
    uint16_t itstatus = 0x0, itenable = 0x0;

    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_GET_IT(UTU_IT));

    itstatus = UTUx->UIFR & UTU_IT;

    itenable = UTUx->UIER & UTU_IT;

    if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
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
  * @brief  Clears the UTUx's interrupt pending bits.
  * @param  UTUx: where x can be 1 or 2 to select the UTU peripheral.
  * @param  UTU_IT: specifies the UTU interrupt source to check.
  *         This parameter can be one of the following values:
  *            @arg UTU_FLAG_UCRA: CHNL_A Rising Edge Flag.
  *            @arg UTU_FLAG_UCFA: CHNL_A Falling Edge Flag
  *            @arg UTU_FLAG_UCRB: CHNL_B Rising Edge Flag.
  *            @arg UTU_FLAG_UCFB: CHNL_B Falling Edge Flag
  *            @arg UTU_FLAG_OVF: UTU conter overflow Flag.
  *            @arg UTU_FLAG_UPP: UTU Protection Flag.
  * @retval None
  */
void UTU_ClearITPendingBit(UTU_TypeDef* UTUx, uint16_t UTU_IT)
{
    /* Check the parameters */
    assert_param(IS_UTU_ALL_PERIPH(UTUx));
    assert_param(IS_UTU_IT(UTU_IT));

    /* Clear the IT pending Bit */
    UTUx->UIFR |= UTU_IT;
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

  @endverbatim
*/
/**
  * @}
  */
/**
  * @}
  */
