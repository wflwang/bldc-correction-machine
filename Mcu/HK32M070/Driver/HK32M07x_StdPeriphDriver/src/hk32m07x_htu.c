/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_htu.c
* @brief:     This file provides firmware functions to manage HTU moudle.
* @author:    AE Team
* @version:   V1.0.0/2024-12-19 
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_htu.h"


/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup HTU HTU
  * @brief HTU driver modules
  * @{
  @verbatim

  @endverbatim
*/

/**
  * @brief  Deinitializes the HTUx peripheral registers to their default reset values.
  * @retval None
  */
void HTU_DeInit(void)
{
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_HTU1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_HTU1, DISABLE);
}

/**
  * @brief  Initializes the HTUx Time Base Unit peripheral according to
  *         the specified parameters in the HTU_TimeBaseInitStruct.
  * @param  HTU_TimeBaseInitStruct: pointer to a HTU_TimeBaseInitTypeDef
  *         structure that contains the configuration information for
  *         the specified HTU peripheral.
  * @retval None
  */
void HTU_TimeBaseInit(HTU_TimeBaseInitTypeDef* HTU_TimeBaseInitStruct)
{
    uint32_t tmptcsr = 0;
    /* Check the parameters */
    assert_param(IS_HTU_ART(HTU_TimeBaseInitStruct->HTU_ART));
    assert_param(IS_HTU_WPA(HTU_TimeBaseInitStruct->HTU_WPA));
    assert_param(IS_HTU_CKD_DIV(HTU_TimeBaseInitStruct->HTU_ClockDivision));
    
    tmptcsr = HTU->HCSR;

    /* Select Clock division*/
    tmptcsr &= (uint32_t)(~((uint32_t)0xFFFF));
    tmptcsr |= ((HTU_TimeBaseInitStruct->HTU_NFCR)<<HTU_HCSR_NFCR_Pos) |\
               HTU_TimeBaseInitStruct->HTU_ART |\
               HTU_TimeBaseInitStruct->HTU_WPA |\
               HTU_TimeBaseInitStruct->HTU_ClockDivision;

    HTU->HCSR = tmptcsr;

    /* Set the Period value */
    HTU->HPR = HTU_TimeBaseInitStruct->HTU_Period;

}

/**
  * @brief  Fills each HTU_TimeBaseInitStruct member with its default value.
  * @param  HTU_TimeBaseInitStruct: pointer to a HTU_TimeBaseInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void HTU_TimeBaseStructInit(HTU_TimeBaseInitTypeDef* HTU_TimeBaseInitStruct)
{
    /* Set the default configuration */
    HTU_TimeBaseInitStruct->HTU_NFCR = 0x0;
    HTU_TimeBaseInitStruct->HTU_ART = HTU_ART_0;
    HTU_TimeBaseInitStruct->HTU_WPA = HTU_WPA_UNWARN;
    HTU_TimeBaseInitStruct->HTU_ClockDivision = HTU_CLK_DIV_1;
    HTU_TimeBaseInitStruct->HTU_Period = 0x0;
}

/**
  * @brief  Sets the HTU Counter Register value
  * @param  Counter: specifies the Counter register new value.
  * @retval None
  */
void HTU_SetCounter(uint16_t Counter)
{
    /* Set the Counter Register value */
    HTU->CNT = Counter;
}


/**
  * @brief  Sets the HTU TimePeriod Register value
  * @param  TimePeriod: specifies the TimePeriod register new value.
  * @retval None
  */
void HTU_SetTimePeriod(uint16_t TimePeriod)
{
    /* Set the TimePeriod Register value */
    HTU->HPR = TimePeriod;
}

/**
  * @brief  Gets the HTU Counter value.
  * @retval Counter Register value.
  */
uint32_t HTU_GetCounter(void)
{
    /* Get the Counter Register value */
    return HTU->CNT;
}

/**
  * @brief  Gets the HTU ClockDivision value.
  * @retval ClockDivision value.
  */
uint16_t HTU_GetClockDivision(void)
{
    /* Get the ClockDivision value */
    return HTU->HCSR & HTU_HCSR_CLK_DIV_Mask;
}


/**
  * @brief  Enables or disables the specified HTU interrupts.
  * @param  HTU_IT: specifies the HTU interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *            @arg HTU_IT_OVF  
  *            @arg HTU_IT_SW  
  *            @arg HTU_IT_ERR  
  * @param  NewState: new state of the HTU interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void HTU_ITConfig(uint32_t HTU_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_HTU_IT(HTU_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Interrupt sources */
        HTU->HIER  |= (uint32_t)HTU_IT;
    }
    else
    {
        /* Disable the Interrupt sources */
        HTU->HIER  &= (uint32_t)~HTU_IT;

    }
}

/**
  * @brief  Checks whether the specified HTU flag is set or not.
  * @param  HTU_FLAG: specifies the flag to check.
  *         This parameter can be any combination of the following values:
  *            @arg HTU_FLAG_OVF  
  *            @arg HTU_FLAG_SW  
  *            @arg HTU_FLAG_ERR  
  * @retval The new state of HTU_FLAG (SET or RESET).
  */
FlagStatus HTU_GetFlagStatus(uint32_t HTU_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_HTU_GET_FLAG(HTU_FLAG));

    if ((HTU->HIFR & (uint32_t)HTU_FLAG) != (uint32_t)RESET) 
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
  * @brief  Clears the HTU pending flags.
  * @param  HTU_FLAG: specifies the flag to check.
  *         This parameter can be any combination of the following values:
  *            @arg HTU_FLAG_OVF  
  *            @arg HTU_FLAG_SW  
  *            @arg HTU_FLAG_ERR  
  * @retval None
  */
void HTU_ClearFlag(uint32_t HTU_FLAG)
{
    /* Check the parameters */
    assert_param(IS_HTU_CLEAR_FLAG(HTU_FLAG));
    /* Clear the flags */
    HTU->HIFR  = (uint32_t)HTU_FLAG;   
}

/**
  * @brief  Checks whether the HTU interrupt has occurred or not.
  * @param  HTU_IT: specifies the HTU interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *            @arg HTU_IT_OVF  
  *            @arg HTU_IT_SW  
  *            @arg HTU_IT_ERR  
  * @retval The new state of HTU_IT (SET or RESET).
  */
ITStatus HTU_GetITStatus(uint32_t HTU_IT)
{
    ITStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_HTU_GET_IT(HTU_IT));

    if (((HTU->HIFR & (uint16_t)HTU_IT) != (uint16_t)RESET)  && \
         ((HTU->HIER & (uint16_t)HTU_IT) != (uint16_t)RESET)) 
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
  * @brief  Clears the HTU interrupt pending bits.
  * @param  HTU_IT: specifies the HTU interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *            @arg HTU_IT_OVF 
  *            @arg HTU_IT_SW  
  *            @arg HTU_IT_ERR  
  * @retval None
  */
void HTU_ClearITPendingBit(uint32_t HTU_IT)
{
    /* Check the parameters */
    assert_param(IS_HTU_IT(HTU_IT));
    /* Clear the IT pending Bit */
    HTU->HIFR  = (uint16_t)HTU_IT;
}


/**
  * @brief  Gets the hall raw sign value.
  * @retval SDR Register value.
  */
uint16_t HTU_GetHallRawSign(void)
{
    /* Get the SDR Register value */
    return ((HTU->HSDR)>>HTU_HSDR_RDO_Pos);
}
/**
  * @brief  Gets the hall filter sign value.
  * @retval SDR Register value.
  */
uint16_t HTU_GetHallFilterSign(void)
{
    /* Get the SDR Register value */
    return ((HTU->HSDR)& HTU_HSDR_FDO_Mask);
}
/**
  * @brief  Gets the WBRx value.
  * @param  HTU_WBRx: WBRx offset address.
  *         This parameter can be any combination of the following values:
  *            @arg HTU_WBR0 
  *            @arg HTU_WBR1  
  *            @arg HTU_WBR2  
  *            @arg HTU_WBR3
  *            @arg HTU_WBR4  
  *            @arg HTU_WBR5 
  *            @arg HTU_WBR6 
  *            @arg HTU_WBR7  
  * @retval WBRx Register value.
  */
uint32_t HTU_GetWBRValue(uint32_t HTU_WBRx)
{
  __IO uint32_t tmp = 0;
     /* Check the parameters */
    assert_param(IS_HTU_WBRx(HTU_WBRx));

  tmp = HTU_BASE + HTU_WBRx;
  
  /* Returns WBR value */
  return (*(__IO uint32_t*)  tmp);   

}
/**
  * @brief  Gets the average value.
  * @retval AVR Register value.
  */
uint32_t HTU_GetAverageValue(void)
{
    /* Get the AVR Register value */
    return HTU->HAVR;
}

/**
  * @brief  Configuration the compare.
  * @param  HCIS_CMPx: compare selection.
  * @param  TriggerEdge: compare trigger edge.
  * @retval None.
  */
void HTU_CompareConfig(uint32_t HCIS_CMPx , uint32_t TriggerEdge)
{
     /* Check the parameters */
    assert_param(IS_HTU_HCIS(HCIS_CMPx));
    assert_param(IS_HTU_HCTS(TriggerEdge));
  
    HTU->HCCT &= ~(uint32_t)(HTU_HCCT_HCTS_Mask|HTU_HCCT_HCIS_Mask);
    HTU->HCCT |=(HCIS_CMPx|TriggerEdge);
     
}
/**
  * @brief  The compare enable,compare or hall,only select one
  * @param  NewState: new state of the compare.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void HTU_CompareCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable HTU compare */
        HTU->HCCT |= HTU_HCCT_HCC_EN;
    }
    else
    {
        /* Disable HTU compare */
        HTU->HCCT &= ~(HTU_HCCT_HCC_EN);
    }
     
}
/**
  * @brief  The counter enable
  * @param  NewState: new state of the compare.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void HTU_CounterCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable counter*/
        HTU->HCSR |= HTU_HCSR_CNT_EN;
    }
    else
    {
        /* Disable counter */
        HTU->HCSR &= ~(HTU_HCSR_CNT_EN);
    }
     
}

/**
  * @}
  */
/**
  * @}
  */
