/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_mc_it.c
* @brief:     This file provides the hk32f4xx interrupt source file.
* @author:    AE Team
* @version:   V1.0.0/2025-2-24
*             1.Initial version
* @log:
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
#include "mc_config.h"
#include "mc_tasks.h"
#include "parameters_conversion.h"
#include "motorcontrol.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup hk32m07x_mc_Handlers hk32m07x mc Handlers
  * @{
  */

/* USER CODE BEGIN PRIVATE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SYSTICK_DIVIDER (SYS_TICK_FREQUENCY/1000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USER CODE END PRIVATE */

/* Public prototypes of IRQ handlers called from assembly code ---------------*/
void CURRENT_REGULATION_IRQHandler(void);

void SPD_TIM_M1_IRQHandler(void);
void USART_IRQHandler(void);
void HardFault_Handler(void);
void SysTick_Handler(void);
void EXTI4_15_IRQHandler (void);

void CURRENT_REGULATION_IRQHandler(void)
{
    if ((ADC->ADIFR & ADC_IT_GROUPB0_FINISH) != RESET)
    {
        /* Clears the ADCx's interrupt pending bits */
        ADC->ADIFR |= ADC_IT_GROUPB0_FINISH;
        TSK_HighFrequencyTask();
    }
}


/**
  * @brief  This function handles HTU global interrupt request for M1 Speed Sensor.
  * @param  None
  * @retval None
  */
void HTU_IRQHandler(void)
{
    /* HALL Timer Update IT always enabled, no need to check enable UPDATE state */
    if (((HTU->HIFR & (uint16_t)HTU_IT_OVF) != (uint16_t)RESET)) 
    {
        HTU->HIFR  = (uint16_t)HTU_IT_OVF;
        HALL_TIMx_UP_IRQHandler(&HALL_M1);
    }

    /* HALL Timer CC1 IT always enabled, no need to check enable CC1 state */
    if (((HTU->HIFR & (uint16_t)HTU_IT_SW) != (uint16_t)RESET))  
    {
        HTU->HIFR  = (uint16_t)HTU_IT_SW;
        HALL_TIMx_CC_IRQHandler(&HALL_M1);
    }
}


/**
  * @brief  This function handles ATU Update, Break-in interrupt request.
  * @param  None
  * @retval None
  */
void ATU_IRQHandler(void)
{
    if (ATU->TIFR & 0x0300)//²¨¹È²¨·åÖÐ¶Ï
    {
        ATU->TIFR |= 0x0300;
    }

}

void ATU_BKP_IRQHandler(void)
{
    if (ATU->TPIFR & 0x0001)
    {
        ATU->TPIFR |= 0x0001;//Çå³ýBreak Flg
        F0XX_BRK_IRQHandler(&PWM_Handle_M1);
    }

}

/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{

}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */
    /* USER CODE END HardFault_IRQn 0 */
    TSK_HardwareFaultTask();

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {

    }

    /* USER CODE BEGIN HardFault_IRQn 1 */
    /* USER CODE END HardFault_IRQn 1 */
}
__IO uint32_t msTick = 0;
void Delay_ms(__IO uint32_t Delay)
{
    uint32_t tickstart = msTick;
    uint32_t wait = Delay;

    while ((msTick - tickstart) < wait)
    {
    }
}

/**
  * @brief  This function handles SysTick exception,Run MotorControl Tasks.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    msTick++;
    MC_RunMotorControlTasks();
}

/*This section is present only if start/stop button is enabled*/
/**
  * @brief  This function handles Button IRQ on PIN PA11.
  */
void EXTI4_15_IRQHandler (void)
{
    WRITE_REG(EXTI->PR, EXTI_IMR_IM11);

    if (MC_GetSTMStateMotor1() == IDLE)
    {
        /* Ramp parameters should be tuned for the actual motor */
        MC_StartMotor1();
    }
    else
    {
        MC_StopMotor1();
    }
}

/**
  * @}
  */

/**
  * @}
  */

