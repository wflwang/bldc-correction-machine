/**
  ******************************************************************************
  * @file    r3_f0xx_pwm_curr_fdbk.c
  * @author  Motor Control SDK Team, HK Microchip
  * @brief   This file provides firmware functions that implement current sensor
  *          class to be stantiated when the three shunts current sensing
  *          topology is used. It is specifically designed for HK32M07x
  *          microcontrollers and implements the successive sampling of two motor
  *          current using only one ADC.
  *           + MCU peripheral and handle initialization fucntion
  *           + three shunt current sesnsing
  *           + space vector modulation function
  *           + ADC sampling function
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pwm_curr_fdbk.h"
#include "r3_f0xx_pwm_curr_fdbk.h"
#include "mc_type.h"
uint8_t Cal_FOC = 0;
/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup pwm_curr_fdbk
  * @{
  */

/**
 * @defgroup r3_f0XX_pwm_curr_fdbk R3 F0xx PWM & Current Feedback
 *
 * @brief HK32M07x, 3-Shunt PWM & Current Feedback implementation
 *
 * This component is used in applications based on an HK32M07x MCU
 * and using a three shunt resistors current sensing topology.
 *
 * @todo: TODO: complete documentation.
 * @{
 */

/* Private defines -----------------------------------------------------------*/

#define NB_CONVERSIONS 16u

#define ADC_CLEAR_TRIG_EDGE_Mask  (~ADC_REG_TRIG_EXT_RISINGFALLING) /* 32 bit Mask */
#define CURRENT_READING_ON   0x0002u /*!< Flag to indicate the active states
                                          conditions (as START and RUN states) when
                                          the currents reading is executed, to avoid the
                                          the regular conversions during these states. */

#define CALIB 0x0010u                /*!< This flag is used to indicate the ADC
                                          calibration phase in order to avoid
                                          concurrent regular conversions*/

/* Private function prototypes -----------------------------------------------*/
static void R3F0XX_ATUxInit( PWMC_Handle_t * pHdl );
static void R3F0XX_HFCurrentsCalibration( PWMC_Handle_t * pHdl, Curr_Components * pStator_Currents );
static uint16_t R3F0XX_WriteTIMRegisters( PWMC_Handle_t * pHdl, uint16_t CCR4Reg );

/* Private functions ---------------------------------------------------------*/



/**
  * @brief  It initializes ATU, ADC1, GPIO and NVIC for three shunt current
  *         reading configuration using HK32M07x.
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
void R3F0XX_Init( PWMC_R3_F0_Handle_t * pHandle )
{
    if ( ( uint32_t )pHandle == ( uint32_t )&pHandle->_Super )
    {
        /* disable IT and flags  */
        ADC->ADIER =0x0000;//关闭双Group模式转换完成中断
        ADC->ADIFR =0x0008;//清一次双Group，GroupB0转换完成状态
        R3F0XX_ATUxInit( &pHandle->_Super );

        /* Clear ATU break flag. */
        ATU->TPIFR |= 0x0001;//清保护中断0
        ATU->TPPS =0x00000005;//选择CMP1做保护刹车，使能高电平保护，关闭保护2，配置Halt模式输出保护默认值，关闭PWM输出恢复，保护默认值输出为低电平
        /* Enables the ADC peripheral */
        ADC->ADCSR |=0x0001;

        /* Clear the flags */
        pHandle->OverVoltageFlag = false;
        pHandle->OverCurrentFlag = false;

        /* We allow ADC usage for regular conversion on Systick*/
        pHandle->ADCRegularLocked=false; 

        pHandle->_Super.DTTest = 0u;
        pHandle->_Super.DTCompCnt = pHandle->_Super.hDTCompCnt;//死区时间的一半
    }
}

/**
* @brief  It initializes ATU peripheral for PWM generation
* @param  TIMx Timer to be initialized
* @param  pHdl: handler of the current instance of the PWM component
* @retval none
*/
static void R3F0XX_ATUxInit( PWMC_Handle_t * pHdl )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;
    
    ATU->TCSR |=0x00A0;//使能CNT计数
//    ATU->TCSR &=0x007F;//关闭CNT计数
    ATU->TPPS =0x00004005;
    ATU->TPOC =0x00404040;//选择通道2，1,0为互补PWM输出模式，正常极性输出，PWM信号来自内部比较
    ATU->TPIFR |= 0x0001;//清除PWM保护0Break中断标志
    ATU->TPIER |= 0x0001;//使能PWM保护0Break中断
    ATU->TPR =( uint32_t )( pHandle->Half_PWMPeriod ) - 1u;//配置计数顶点
    pHandle->_Super.DTTest = 0u;
    pHandle->_Super.DTCompCnt = pHandle->_Super.hDTCompCnt;//死区时间的一半
}

/**
  * @brief  It stores into the handler the voltage present on the
  *         current feedback analog channel when no current is flowin into the
  *         motor
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
void R3F0XX_CurrentReadingCalibration( PWMC_Handle_t * pHdl )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;
    uint16_t hCalibrationPeriodCounter;
    uint16_t hMaxPeriodsNumber;

    /* Set the CALIB flags to indicate the ADC calibartion phase*/
    pHandle->hFlags |= CALIB;
  
    pHandle-> wPhaseAOffset = 0u;
    pHandle-> wPhaseBOffset = 0u;
    pHandle-> wPhaseCOffset = 0u;

    pHandle->bIndex = 0u;

    /* ADC sequence conversion detection to a correct DMA Buffer reading */
    if ( pHandle->pParams_str->bIaChannel < pHandle->pParams_str->bIbChannel )
    {
        if ( pHandle->pParams_str->bIbChannel < pHandle->pParams_str->bIcChannel )
        {
            pHandle->bCalib_A_index = 0u;
            pHandle->bCalib_B_index = 1u;
            pHandle->bCalib_C_index = 2u;
        }
        else
        {
            if ( pHandle->pParams_str->bIaChannel < pHandle->pParams_str->bIcChannel )
            {
                pHandle->bCalib_A_index = 0u;
                pHandle->bCalib_B_index = 2u;
                pHandle->bCalib_C_index = 1u;
            }
            else
            {
                pHandle->bCalib_A_index = 1u;
                pHandle->bCalib_B_index = 2u;
                pHandle->bCalib_C_index = 0u;
            }
        }
    }
    else
    {
        if ( pHandle->pParams_str->bIaChannel < pHandle->pParams_str->bIcChannel )
        {
            pHandle->bCalib_A_index = 1u;
            pHandle->bCalib_B_index = 0u;
            pHandle->bCalib_C_index = 2u;
        }
        else
        {
            if ( pHandle->pParams_str->bIbChannel < pHandle->pParams_str->bIcChannel )
            {
                pHandle->bCalib_A_index = 2u;
                pHandle->bCalib_B_index = 0u;
                pHandle->bCalib_C_index = 1u;
            }
            else
            {
                pHandle->bCalib_A_index = 2u;
                pHandle->bCalib_B_index = 1u;
                pHandle->bCalib_C_index = 0u;
            }
        }
    }
    ATU->TCSR &=0x007F;//关闭CNT计数
    ADC->ADBA0 =0;
    ADC->ADBB0 =0;    
    /* Offset calibration for A B c phases */
    /* Change function to be executed in ADCx_ISR */
    pHandle->_Super.pFctGetPhaseCurrents     = &R3F0XX_HFCurrentsCalibration;
    pHandle->_Super.pFctSetADCSampPointSect1 = &R3F0XX_SetADCSampPointCalibration;
    pHandle->_Super.pFctSetADCSampPointSect2 = &R3F0XX_SetADCSampPointCalibration;
    pHandle->_Super.pFctSetADCSampPointSect3 = &R3F0XX_SetADCSampPointCalibration;
    pHandle->_Super.pFctSetADCSampPointSect4 = &R3F0XX_SetADCSampPointCalibration;
    pHandle->_Super.pFctSetADCSampPointSect5 = &R3F0XX_SetADCSampPointCalibration;
    pHandle->_Super.pFctSetADCSampPointSect6 = &R3F0XX_SetADCSampPointCalibration;

    R3F0XX_SwitchOnPWM( &pHandle->_Super );

    /* Wait for NB_CONVERSIONS to be executed */
    hMaxPeriodsNumber = ( NB_CONVERSIONS + 1u ) * ( ( ( uint16_t )( pHandle->pParams_str->bRepetitionCounter ) + 1u ) >>
                      1 );
    ATU->TIFR |=0x0100;//清除波峰状态

    hCalibrationPeriodCounter = 0u;
    while ( pHandle->bIndex < NB_CONVERSIONS )
    {
        if ( ATU->TIFR & 0x0100)
        {
            ATU->TIFR |=0x0100;//清除波峰状态
            hCalibrationPeriodCounter++;
            if ( hCalibrationPeriodCounter >= hMaxPeriodsNumber )
            {
                if ( pHandle->bIndex < NB_CONVERSIONS )
            {
                pHandle->_Super.SWerror = 1u;
                break;
            }
            }
        }
        else
        {

        }
    }

    R3F0XX_SwitchOffPWM( &pHandle->_Super );

    /* Mean Value of PhaseCurrents Offset calculation by 4bit shifting operation
    *  instead division by NB_CONVERSIONS value fixed to 16. */

    pHandle->wPhaseAOffset = pHandle->wPhaseAOffset / 16;
    pHandle->wPhaseBOffset = pHandle->wPhaseBOffset / 16;
    pHandle->wPhaseCOffset = pHandle->wPhaseCOffset / 16;
    ADC->ADBA0 =(pHandle->wPhaseAOffset)>>4;
    ADC->ADBB0 =(pHandle->wPhaseBOffset)>>4;
    /* Change back function to be executed in ADCx_ISR */
    pHandle->_Super.pFctGetPhaseCurrents     = &R3F0XX_GetPhaseCurrents;
    pHandle->_Super.pFctSetADCSampPointSect1 = &R3F0XX_SetADCSampPointSect1;
    pHandle->_Super.pFctSetADCSampPointSect2 = &R3F0XX_SetADCSampPointSect2;
    pHandle->_Super.pFctSetADCSampPointSect3 = &R3F0XX_SetADCSampPointSect3;
    pHandle->_Super.pFctSetADCSampPointSect4 = &R3F0XX_SetADCSampPointSect4;
    pHandle->_Super.pFctSetADCSampPointSect5 = &R3F0XX_SetADCSampPointSect5;
    pHandle->_Super.pFctSetADCSampPointSect6 = &R3F0XX_SetADCSampPointSect6;

    /* It over write TIMx CCRy wrongly written by FOC during calibration so as to
     force 50% duty cycle on the three inverer legs */
    /* Disable TIMx preload */

    /* Set all duty to 50% */
    ATU->CR0A = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR0B = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR1A = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR1B = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR2A = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR2B = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1; 

    pHandle->BrakeActionLock = false;

    /* Reset the CALIB flags to indicate the end of ADC calibartion phase*/
    pHandle->hFlags &= ( ~CALIB );

}

/**
 * @brief  It computes and return latest converted motor phase currents
 * @param  pHdl: handler of the current instance of the PWM component
 * @retval Curr_Components Ia and Ib current in Curr_Components format
 */
void R3F0XX_GetPhaseCurrents( PWMC_Handle_t * pHdl, Curr_Components * pStator_Currents )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;

    /* Clear the flag to indicate the start of FOC algorithm*/
    ATU->TIFR |=0x0300;//清除波峰波谷中断标志
    /* Ia = PhaseAOffset - ADC converted value) ------------------------------*/
    /* Ib = PhaseBOffset - ADC converted value) ------------------------------*/
//    pStator_Currents->qI_Component1 = ( int32_t )( pHandle->wPhaseAOffset ) - ( int32_t )(ADC->ADDR0A);
//    pStator_Currents->qI_Component2 = ( int32_t )( pHandle->wPhaseBOffset ) - ( int32_t )(ADC->ADDR0B); 
    pStator_Currents->qI_Component1 = - ADC->ADDR0A;
    pStator_Currents->qI_Component2 = - ADC->ADDR0B;     
    pHandle->_Super.hIa = pStator_Currents->qI_Component1;
    pHandle->_Super.hIb = pStator_Currents->qI_Component2;
//    pHandle->_Super.hIc = -pStator_Currents->qI_Component1 - pStator_Currents->qI_Component2;
}

/**
  * @brief  Configure the ADC for the current sampling during calibration.
  *         It means set the sampling point via TIMx_Ch4 value and polarity
  *         ADC sequence length and channels.
  *         And call the WriteTIMRegisters method.
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
uint16_t R3F0XX_SetADCSampPointCalibration( PWMC_Handle_t * pHdl )
{
  PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;
 /* comparator 1 is used to count the number of PWM cycles, so better to keep it at
  *  a known value */
  pHandle->_Super.hCntPhA = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
  return R3F0XX_WriteTIMRegisters( pHdl,  1);
}

/**
  * @brief  Configure the ADC for the current sampling related to sector 1.
  *         It means set the sampling point via TIM1_Ch4 value, the ADC sequence
  *         and channels.
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
uint16_t R3F0XX_SetADCSampPointSect1( PWMC_Handle_t * pHdl )
{

    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;	
  	return R3F0XX_WriteTIMRegisters( &pHandle->_Super, 1 );
}

/**
* @brief  Configure the ADC for the current sampling related to sector 2.
*         It means set the sampling point via TIM1_Ch4 value and polarity
*         ADC sequence length and channels.
* @param  pHdl: handler of the current instance of the PWM component
* @retval none
*/
uint16_t R3F0XX_SetADCSampPointSect2( PWMC_Handle_t * pHdl )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;	
    return R3F0XX_WriteTIMRegisters( &pHandle->_Super, 1 );
}

/**
* @brief  Configure the ADC for the current sampling related to sector 3.
*         It means set the sampling point via TIM1_Ch4 value and polarity
*         ADC sequence length and channels.
*         And call the WriteTIMRegisters method.
* @param  pHdl: handler of the current instance of the PWM component
* @retval none
*/
uint16_t R3F0XX_SetADCSampPointSect3( PWMC_Handle_t * pHdl )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;		
    return R3F0XX_WriteTIMRegisters( &pHandle->_Super, 1 );
}

/**
* @brief  Configure the ADC for the current sampling related to sector 4.
*         It means set the sampling point via TIM1_Ch4 value and polarity
*         ADC sequence length and channels.
*         And call the WriteTIMRegisters method.
* @param  pHdl: handler of the current instance of the PWM component
* @retval none
*/
uint16_t R3F0XX_SetADCSampPointSect4( PWMC_Handle_t * pHdl )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;		
    return R3F0XX_WriteTIMRegisters( &pHandle->_Super, 1 );
}

/**
* @brief  Configure the ADC for the current sampling related to sector 5.
*         It means set the sampling point via TIM1_Ch4 value and polarity
*         ADC sequence length and channels.
*         And call the WriteTIMRegisters method.
* @param  pHdl: handler of the current instance of the PWM component
* @retval none
*/
uint16_t R3F0XX_SetADCSampPointSect5( PWMC_Handle_t * pHdl )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;	 
    return R3F0XX_WriteTIMRegisters( &pHandle->_Super, 1 );
}

/**
* @brief  Configure the ADC for the current sampling related to sector 6.
*         It means set the sampling point via TIM1_Ch4 value and polarity
*         ADC sequence length and channels.
*         And call the WriteTIMRegisters method.
* @param  pHdl: handler of the current instance of the PWM component
* @retval none
*/
uint16_t R3F0XX_SetADCSampPointSect6( PWMC_Handle_t * pHdl )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;	
    return R3F0XX_WriteTIMRegisters( &pHandle->_Super, 1 );
}

/**
* @brief  Write the duty cycle into timer registers and check for FOC duration.
* @param  pHandle Pointer on the target component instance.
* @param  hCCR4Reg capture/compare register value.
* @retval none
*/
static uint16_t R3F0XX_WriteTIMRegisters( PWMC_Handle_t * pHdl,  uint16_t hCCR4Reg)
{
    uint16_t hAux;
    uint16_t duty0;
    uint16_t duty1;
    uint16_t duty2;
        
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;
    
    duty0 = pHandle->Half_PWMPeriod - pHandle->_Super.hCntPhA;
    duty1 = pHandle->Half_PWMPeriod - pHandle->_Super.hCntPhB;
    duty2 = pHandle->Half_PWMPeriod - pHandle->_Super.hCntPhC;
    //配置三相上下沿占空比
    ATU->CR0B = duty0;
    ATU->CR0A = duty0;      
    ATU->CR1B = duty1;
    ATU->CR1A = duty1;
    ATU->CR2B = duty2;
    ATU->CR2A = duty2;    
    
    ATU->TRG0 = 1;//配置TRGO先触发采样U相（也可以配置同时刻触发）
    ATU->TRG1 = 1;//配置TRG1后触发采样V相（也可以配置同时刻触发）     
    /* Re-configuration of CCR4 must be done before the timer update to be taken
        into account at the next PWM cycle. Otherwise we are too late, we flag a
        FOC_DURATION error */
    if ( ATU->TIFR & 0x0100 )//波峰中断Flag发生
    {
        hAux = MC_FOC_DURATION;
    }
    else
    {
        hAux = MC_NO_ERROR;
    }
    if ( pHandle->_Super.SWerror == 1u )
    {
        hAux = MC_FOC_DURATION;
        pHandle->_Super.SWerror = 0u;
    }
  
  	return hAux;
}

/**
* @brief  Implementation of PWMC_GetPhaseCurrents to be performed during
*         calibration. It sum up ADC conversion data into wPhaseAOffset and
*         wPhaseBOffset to compute the offset introduced in the current feedback
*         network. It is required to proper configure ADC inputs before to enable
*         the offset computation.
* @param  pHandle Pointer on the target component instance.
* @retval It always returns {0,0} in Curr_Components format
*/
static void R3F0XX_HFCurrentsCalibration( PWMC_Handle_t * pHdl, Curr_Components * pStator_Currents )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;
    static uint16_t hAux1 = 0;
    static uint16_t hAux2 = 0;
    static uint16_t hAux3 = 0;
    /* Clear the flag to indicate the start of FOC algorithm*/
    ATU->TIFR |=0x0300;//清除波峰波谷中断
    if ( pHandle->bIndex < NB_CONVERSIONS )
    {
        hAux1 = ADC->ADDR0A;//GroupA采样U相电流 ;
        hAux2 = ADC->ADDR0B;//GroupB采样V相电流;
        hAux3 = ADC->ADDR0A;//采样W相电流 ;
        pHandle->wPhaseAOffset += hAux1;
        pHandle->wPhaseBOffset += hAux2;
        pHandle->wPhaseCOffset += hAux3;
        pHandle->bIndex++;
    }

    /* during offset calibration no current is flowing in the phases */
    pStator_Currents->qI_Component1 = 0;
    pStator_Currents->qI_Component2 = 0;
}


/**
  * @brief  It turns on low sides switches. This function is intended to be
  *         used for charging boot capacitors of driving section. It has to be
  *         called each motor start-up when using high voltage drivers
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
void R3F0XX_TurnOnLowSides( PWMC_Handle_t * pHdl )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;
    pHandle->_Super.bTurnOnLowSidesAction = true;
    //用PWM控制或者用IO控制
    ATU->CR0A =ATU->TPR;//TIO0A驱动关闭
    ATU->CR1A =ATU->TPR;//TIO1A驱动关闭
    ATU->CR2A =ATU->TPR;//TIO2A驱动关闭
    ATU->CR0B =ATU->TPR;//TIO0B驱动关闭
    ATU->CR1B =ATU->TPR;//TIO1B驱动关闭
    ATU->CR2B =ATU->TPR;//TIO1B驱动关闭
    
    //清除更新状态波峰和波谷
    ATU->TIFR |=0x0300;
    while ( ATU->TIFR & 0x0300)//清除完毕
    {}
    ATU->TCSR |=0x0A0;//使能ATU计数器
    return;
}

/**
  * @brief  This function enables the PWM outputs
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
void R3F0XX_SwitchOnPWM( PWMC_Handle_t * pHdl )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;

    pHandle->_Super.bTurnOnLowSidesAction = false;

    /* We forbid ADC usage for regular conversion on Systick*/
    pHandle->ADCRegularLocked=true; 

    /* Wait for a new PWM period */
    /* Clear Update Flag */    
    //清除更新状态波峰和波谷,开启波峰可波谷中断
    ATU->TIFR |=0x0300;

    while ( ATU->TIFR & 0x0300)//清除完毕
    {}

    //清除更新状态波峰和波谷,开启波峰可波谷中断
    ATU->TIFR |=0x0300;

    /* Set all duty to 50% */
    ATU->CR0A = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR0B = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR1A = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR1B = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR2A = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR2B = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->TRG0 = 0x0001;//配置TRGO先触发采样U相（也可以配置同时刻触发）
    ATU->TRG1 = 0x0001;//配置TRG1后触发采样V相（也可以配置同时刻触发）    

    while ( ATU->TIFR & 0x0300)//清除完毕
    {}

//    NVIC_EnableIRQ(ADC_IRQn);//开ADC中断
    ADC->ADIFR = 0x0008;//清除中断
    ADC->ADIER =0x0008;//GroupB第一通道采样完成中断使能    
    ADC_Cmd(ADC, ENABLE);
     /* Main PWM Output Enable */
    ATU->TCSR =0xA0;//开启ATU计数器
    ATU->TPPS =0x00004005;       
    /* If Calibration is true */
    if ( ( pHandle->hFlags & CALIB ) != 0u )  
    {
        Cal_FOC = 0;
    }
    else
    {
        Cal_FOC = 1;
    }
    /* Set the RUN flag to indicate the NOT IDLE condition */
    pHandle->hFlags |= CURRENT_READING_ON;

    return;
}


/**
  * @brief  It disables PWM generation on the proper Timer peripheral acting on
  *         MOE bit and reset the TIM status
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
void R3F0XX_SwitchOffPWM( PWMC_Handle_t * pHdl )
{
    PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;
    pHandle->_Super.bTurnOnLowSidesAction = false;

    /* Main PWM Output Disable */
/* Main PWM Output Disable */
    ATU->TCSR =0x00;//关闭ATU计数器
    ATU->TPPS =0x00000005;
    ADC->ADIER &=0xFFF7;//关GroupB第一通道采样完成中断
//    NVIC_DisableIRQ(ADC_IRQn);//关闭ADC中断 

    /* We allow ADC usage for regular conversion on Systick*/
    pHandle->ADCRegularLocked=false; 

    return;
}

/**
 * @brief  It contains the Break event interrupt
 * @param  pHandle: handler of the current instance of the PWM component
 * @retval none
 */
void * F0XX_BRK_IRQHandler( PWMC_R3_F0_Handle_t * pHandle )
{
    pHandle->OverCurrentFlag = true;
    return 0;
}

/**
  * @brief  It is used to check if an overcurrent occurred since last call.
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval uint16_t It returns MC_BREAK_IN whether an overcurrent has been
  *                  detected since last method call, MC_NO_FAULTS otherwise.
  */
uint16_t R3F0XX_IsOverCurrentOccurred( PWMC_Handle_t * pHdl )
{
  uint16_t retVal = MC_NO_FAULTS;

  PWMC_R3_F0_Handle_t * pHandle = ( PWMC_R3_F0_Handle_t * )pHdl;

  if ( pHandle->OverVoltageFlag == true )
  {
    retVal = MC_OVER_VOLT;
    pHandle->OverVoltageFlag = false;
  }

  if ( pHandle->OverCurrentFlag == true )
  {
    retVal |= MC_BREAK_IN;
    pHandle->OverCurrentFlag = false;
  }
  return retVal;
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

