/********************************************************************************
  * @Copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
  * @filename: hk32m07x_syscfg.c 
  * @brief: SYSCFG Initialization and Configuration functions
  * @Author: AE Team 
  * @version: V1.0.0/2022-05-24 
  * 1.³õÊ¼°æ±¾
*********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_syscfg.h"

/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup SYSCFG SYSCFG 
  * @brief SYSCFG driver modules
  * @{
	
  @verbatim
 ===============================================================================	
                     ##### How to use this driver #####
 ===============================================================================
     [..] 
      (#)To enable SYSCFG APB clock use:
         RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN, ENABLE).
				 
  @endverbatim
*/ 
 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup SYSCFG_Private_Functions SYSCFG_Private_Functions
  * @{
  */ 

/** @defgroup SYSCFG_Group1 SYSCFG Initialization and Configuration functions
 *  @brief   SYSCFG Initialization and Configuration functions 
 *
@verbatim
 ===============================================================================
        ##### SYSCFG Initialization and Configuration functions #####
 ===============================================================================
     [..] This section provides functions allowing to:
					(+)Configures the memory mapping at address 0x00000000
					(+)UART CTX_RTX Remap control
					(+)Enables or disables the DIV_VOUT_BUF
					(+)Enables or disables the SW_VREF12_PA9
					(+)Enables or disables the SW_VOLTAGE_DIV_PA5
					(+)Enables or disables the SW_VTEST_PA8
					(+)Enables or disables the SW_VIN_LVD_PA8
					(+)Enables or disables the VOLTAGE_DIV
					(+)Configures the VOLTAGE_DIV_DATA
					(+)Syscfg_CFGR3 Register Lockup
					(+)Configures the TIM3_CH4 remapping
					(+)Configure the I2C fast mode plus driving capability
					(+)Selects the GPIO pin used as EXTI Line
					(+)Connect the selected parameter to the break input of TIM1
		 
@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the SYSCFG registers to their default reset values.
  * @note   MEM_MODE bits are not affected by APB reset.
  * @note   MEM_MODE bits took the value from the user option bytes.
  * @retval None
  */
void SYSCFG_DeInit(void)
{
    /* Set SYSCFG_CFGR1 register to reset value without affecting MEM_MODE bits */
    SYSCFG->CFGR1 &= SYSCFG_CFGR1_MEM_MODE;
    
    /* Set EXTICRx registers to reset value */
    SYSCFG->EXTICR1 = 0;
    SYSCFG->EXTICR2 = 0;
    SYSCFG->EXTICR3 = 0;
    SYSCFG->EXTICR4 = 0;
}

/**
  * @brief  Configures the memory mapping at address 0x00000000.
  * @param  SYSCFG_MemoryRemap: selects the memory remapping.
  *          This parameter can be one of the following values:
  *            @arg SYSCFG_MemoryRemap_Flash: Main Flash memory mapped at 0x00000000  
  *            @arg SYSCFG_MemoryRemap_SRAM: Embedded SRAM mapped at 0x00000000
  * @retval None
  */
void SYSCFG_MemoryRemapConfig(uint32_t SYSCFG_MemoryRemap)
{
    uint32_t tmpctrl = 0;

    /* Check the parameter */
    assert_param(IS_SYSCFG_MEMORY_REMAP(SYSCFG_MemoryRemap));

    /* Get CFGR1 register value */
    tmpctrl = SYSCFG->CFGR1;

    /* Clear MEM_MODE bits */
    tmpctrl &= (uint32_t) (~SYSCFG_CFGR1_MEM_MODE);

    /* Set the new MEM_MODE bits value */
    tmpctrl |= (uint32_t) SYSCFG_MemoryRemap;

    /* Set CFGR1 register with the new memory remap configuration */
    SYSCFG->CFGR1 = tmpctrl;
}

/**
  * @brief  Configures the IO filter.
  * @param  SYSCFG_IO_FLT: SYSCFG_IO_FLT*LSIclk,SYSCFG_IO_FLT<255.
  * @retval None
  */
void SYSCFG_IOFLT_Config(uint32_t SYSCFG_IO_FLT)
{
    uint32_t tmpctrl = 0;

    /* The parameter limit */
    if(SYSCFG_IO_FLT > 255)
	{
		SYSCFG_IO_FLT = 255;
	}

    /* Get CFGR3 register value */
    tmpctrl = SYSCFG->CFGR3;

    /* Clear SYSCFG_CFGR3_IOFLT bit */
    tmpctrl &= (uint32_t) (~SYSCFG_CFGR3_IOFLT);

    /* Set the new SYSCFG_CFGR3_IOFLT bit value */
    tmpctrl |= (uint32_t) (SYSCFG_IO_FLT << 16);

    /* Set CFGR3 register with the SYSCFG_CFGR3_IOFLT configuration */
    SYSCFG->CFGR3 = tmpctrl;
}

/**
  * @brief  Syscfg_CFGR3 Register Lockup.The Syscfg_CFGR3 Register configuration is 
  *         locked and can be unlocked by REF_LOCK
  * @param  REF_LOCK: selects the Syscfg_CFGR3 Register configuration to be locked or to be unlocked
  *         This parameter can be any combination of the following values:
  *            @arg SYSCFG_REF_LOCK: Syscfg_CFGR3 Register is locked, read only.
  *            @arg SYSCFG_REF_UNLOCK: Syscfg_CFGR3 Register is unlock ,can be read or write.
  * @retval None
  */
void SYSCFG_REF_LOCK_Config(uint32_t REF_LOCK)
{
    uint32_t tmpctrl = 0;
    /* Check the parameter */
    assert_param(IS_SYSCFG_REF_LOCK_CONFIG(REF_LOCK));

    /* Get CFGR3 register value */
    tmpctrl = SYSCFG->CFGR3;

    /* Clear SYSCFG_REF_LOCK bit */
    tmpctrl &= (uint32_t) (~SYSCFG_REF_LOCK);

    /* Set the new SYSCFG_CFGR3_REF_LOCK bit value */
    tmpctrl |= (uint32_t) REF_LOCK;

    /* Set CFGR3 register with the SYSCFG_CFGR3_REF_LOCK configuration */
    SYSCFG->CFGR3 = tmpctrl;
}

/**
  * @brief  Enable BUF_OUT out put to BUF_OUT1.
  * @param  NewStaus: new state of BUF1_EN .
  *         This parameter can be:  ENABLE or DISABLE.
  * @retval None
  */
void SYSCFG_BUF1_Config(FunctionalState NewStaus)
{
	/* Check the parameter */
    assert_param(IS_FUNCTIONAL_STATE(NewStaus));
	
	/* Set the new SYSCFG_CFGR3_EN_BUF1 bit value */
	if(NewStaus == ENABLE)
    {
		
        SYSCFG->CFGR3 |= (uint32_t) SYSCFG_CFGR3_EN_BUF1;
    }
    else
    {
        SYSCFG->CFGR3 &= (uint32_t) (~SYSCFG_CFGR3_EN_BUF1);
    }	
}
/**
  * @brief  Enable BUF_OUT out put to BUF_OUT0.
  * @param  NewStaus: new state of BUF0_EN .
  *         This parameter can be:  ENABLE or DISABLE.
  * @retval None
  */
void SYSCFG_BUF0_Config(FunctionalState NewStaus)
{	
	/* Check the parameter */
    assert_param(IS_FUNCTIONAL_STATE(NewStaus));
	
	/* Set the new SYSCFG_CFGR3_EN_BUF0 bit value */
	if(NewStaus == ENABLE)
    {
		
        SYSCFG->CFGR3 |= (uint32_t) SYSCFG_CFGR3_EN_BUF0;
    }
    else
    {
        SYSCFG->CFGR3 &= (uint32_t) (~SYSCFG_CFGR3_EN_BUF0);
    }		
}
/**
  * @brief  Enable switch of VDIV out to BUF_OUTx.
  * @param  NewStaus: new state of BUF0_EN .
  *         This parameter can be:  ENABLE or DISABLE.
  * @retval None
  */
void SYSCFG_SW_VDIV_Config(FunctionalState NewStaus)
{	
	/* Check the parameter */
    assert_param(IS_FUNCTIONAL_STATE(NewStaus));
	
	/* Set the new SYSCFG_CFGR3_EN_SW_VDIV bit value */
	if(NewStaus == ENABLE)
    {
		
        SYSCFG->CFGR3 |= (uint32_t) SYSCFG_CFGR3_EN_SW_VDIV;
    }
    else
    {
        SYSCFG->CFGR3 &= (uint32_t) (~SYSCFG_CFGR3_EN_SW_VDIV);
    }	
}

/**
  * @brief  Enable switch of PGA1 out to BUF_OUTx.
  * @param  NewStaus: new state of BUF0_EN .
  *         This parameter can be:  ENABLE or DISABLE.
  * @retval None
  */
void SYSCFG_SW_PGA1_Config(FunctionalState NewStaus)
{	
	/* Check the parameter */
    assert_param(IS_FUNCTIONAL_STATE(NewStaus));
	
	/* Set the new SYSCFG_CFGR3_EN_SW_PGA1 bit value */
	if(NewStaus == ENABLE)
    {
		
        SYSCFG->CFGR3 |= (uint32_t) SYSCFG_CFGR3_EN_SW_PGA1;
    }
    else
    {
        SYSCFG->CFGR3 &= (uint32_t) (~SYSCFG_CFGR3_EN_SW_PGA1);
    }	
}

/**
  * @brief  Enable switch of PGA2 out to BUF_OUTx.
  * @param  NewStaus: new state of BUF0_EN .
  *         This parameter can be:  ENABLE or DISABLE.
  * @retval None
  */
void SYSCFG_SW_PGA2_Config(FunctionalState NewStaus)
{
    /* Check the parameter */
    assert_param(IS_FUNCTIONAL_STATE(NewStaus));
	
	/* Set the new SYSCFG_CFGR3_EN_SW_PGA2 bit value */
	if(NewStaus == ENABLE)
    {
		
        SYSCFG->CFGR3 |= (uint32_t) SYSCFG_CFGR3_EN_SW_PGA2;
    }
    else
    {
        SYSCFG->CFGR3 &= (uint32_t) (~SYSCFG_CFGR3_EN_SW_PGA2);
    }	
}


/**
  * @brief  Enable switch of PMU VTEST out to BUF_OUTx.
  * @param  NewStaus: new state of BUF0_EN .
  *         This parameter can be:  ENABLE or DISABLE.
  * @retval None
  */
void SYSCFG_SW_PMU_Config(FunctionalState NewStaus)
{
    /* Check the parameter */
    assert_param(IS_FUNCTIONAL_STATE(NewStaus));

    /* Set the new SYSCFG_CFGR3_EN_SW_PMU bit value */
	if(NewStaus == ENABLE)
    {
		
        SYSCFG->CFGR3 |= (uint32_t) SYSCFG_CFGR3_EN_SW_PMU;
    }
    else
    {
        SYSCFG->CFGR3 &= (uint32_t) (~SYSCFG_CFGR3_EN_SW_PMU);
    }
}
 

/**
  * @brief  Enable switch of IBIAS out,and Configures the GAIN of IBIAS ADJ .
  * @param  SYSCFG_IBIAS_ADJ_GAIN: This parameter can be :
  *            @arg SYSCFG_IBIAS_ADJ_GAIN_0: 2uA
  *            @arg SYSCFG_IBIAS_ADJ_GAIN_1: 4uA
  *            @arg SYSCFG_IBIAS_ADJ_GAIN_2: 6uA
  *            @arg SYSCFG_IBIAS_ADJ_GAIN_3: 8uA
  * @param  NewStaus: new state of BUF0_EN .
  *         This parameter can be:  ENABLE or DISABLE.
  * @retval None
  */
void SYSCFG_IBIAS_Config(uint32_t SYSCFG_IBIAS_ADJ_GAIN,FunctionalState NewStaus)
{
    uint32_t tmpctrl = 0;

    /* Check the parameter */    
    assert_param(IS_FUNCTIONAL_STATE(NewStaus));
    assert_param(IS_SYSCFG_IBIAS_ADJ_GAIN(SYSCFG_IBIAS_ADJ_GAIN));
    
	/* Set the new SYSCFG_CFGR3_EN_IBIAS bit value */
    if(NewStaus == ENABLE)
    {
		
        SYSCFG->CFGR3 |= (uint32_t)(SYSCFG_CFGR3_EN_IBIAS|SYSCFG_CFGR3_EN_SW_IBIAS);
    }
    else
    {
        SYSCFG->CFGR3 &= (uint32_t) (~(SYSCFG_CFGR3_EN_IBIAS|SYSCFG_CFGR3_EN_SW_IBIAS));
    }	
    
    /* Get CFGR3 register value */
    tmpctrl = SYSCFG->CFGR3;

    /* Clear SYSCFG_CFGR3_IBIAS_ADJ_GAIN bit */
    tmpctrl &= (uint32_t) (~SYSCFG_CFGR3_IBIAS_ADJ_GAIN);

    /* Set the new SYSCFG_CFGR3_IBIAS_ADJ_GAIN bit value */
    tmpctrl |= (uint32_t) SYSCFG_IBIAS_ADJ_GAIN;

    /* Set CFGR3 register with the SYSCFG_CFGR3_IBIAS_ADJ_GAIN configuration */
    SYSCFG->CFGR3 = tmpctrl;
}

/**
  * @brief  Selects the GPIO pin used as EXTI Line.
  * @note   This parameter can be EXTI_PinSourcex where x can be: 
  *         GPIOA, GPIOB.
  * @param  EXTI_PortSourceGPIOx: selects the GPIO port to be used as source 
  *                               for EXTI lines where x can be (A, B). 
  * @param  EXTI_PinSourcex: specifies the EXTI line to be configured.
  * @retval None
  */
void SYSCFG_EXTILineConfig(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex)
{
    uint32_t tmp = 0x00;

    /* Check the parameters */
    assert_param(IS_EXTI_PORT_SOURCE(EXTI_PortSourceGPIOx));
    assert_param(IS_EXTI_PIN_SOURCE(EXTI_PinSourcex));

    tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03));
    switch(EXTI_PinSourcex >> 0x02)
    {
        case (uint8_t)0x00:
            SYSCFG->EXTICR1 &= ~tmp;
            SYSCFG->EXTICR1 |= (((uint32_t)EXTI_PortSourceGPIOx) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03)));
            break;
        case (uint8_t)0x01:
            SYSCFG->EXTICR2 &= ~tmp;
            SYSCFG->EXTICR2 |= (((uint32_t)EXTI_PortSourceGPIOx) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03)));
            break;
        case (uint8_t)0x02:
            SYSCFG->EXTICR3 &= ~tmp;
            SYSCFG->EXTICR3 |= (((uint32_t)EXTI_PortSourceGPIOx) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03)));
            break;
        case (uint8_t)0x03:
            SYSCFG->EXTICR4 &= ~tmp;
            SYSCFG->EXTICR4 |= (((uint32_t)EXTI_PortSourceGPIOx) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03)));
            break;
        default:
            break;
    }
}

/**
  * @brief  Connect the Cortex-M0 LOCKUP to the break input of TIM1.
  * @note   The selected configuration is locked and can be unlocked by system reset
  * @param  NewStaus: new state of BreakConfig.
  *         This parameter can be:  ENABLE or DISABLE.
  * @retval None
  */
void SYSCFG_LOCKUP_LOCK_Break(FunctionalState NewStaus)
{
    /* Check the parameter */
    assert_param(IS_FUNCTIONAL_STATE(NewStaus));

    if(NewStaus == ENABLE)
    {
        SYSCFG->CFGR2 |= (uint32_t) SYSCFG_CFGR2_LOCKUP_LOCK;
    }
    else
    {
        SYSCFG->CFGR2 &= (uint32_t) (~SYSCFG_CFGR2_LOCKUP_LOCK);
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
