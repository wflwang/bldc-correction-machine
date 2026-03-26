/**
  ******************************************************************************
  * @file    r1_f0xx_pwm_curr_fdbk_shift.c
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
#include "r1_f0xx_pwm_curr_fdbk_shift.h"
#include "mc_type.h"

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup pwm_curr_fdbk
  * @{
  */

/**
 * @defgroup r1_f0XX_pwm_curr_fdbk R1 F0xx PWM & Current Feedback
 *
 * @brief hk32M070, 1-Shunt PWM & Current Feedback implementation
 *
 * This component is used in applications based on an hk32M07x MCU
 * and using a single shunt resistor current sensing topology.
 *
 * @todo: TODO: complete documentation.
 * @{
 */

/* Private Defines -----------------------------------------------------------*/
#define NB_CONVERSIONS 16u

#define SAMP_NO 0u
#define SAMP_IA 1u
#define SAMP_IB 2u
#define SAMP_IC 3u
#define SAMP_NIA 4u
#define SAMP_NIB 5u
#define SAMP_NIC 6u
#define SAMP_OLDA 7u
#define SAMP_OLDB 8u
#define SAMP_OLDC 9u

/* Constant values -----------------------------------------------------------*/
static const uint8_t REGULAR_SAMP_CUR1[6] = {SAMP_NIC, SAMP_NIC, SAMP_NIA, SAMP_NIA, SAMP_NIB, SAMP_NIB};
static const uint8_t REGULAR_SAMP_CUR2[6] = {SAMP_IA, SAMP_IB, SAMP_IB, SAMP_IC, SAMP_IC, SAMP_IA};

/* Private function prototypes -----------------------------------------------*/
static void R1F0XX_1ShuntMotorVarsInit( PWMC_Handle_t * pHdl );
static void R1F0XX_ATUxInit(PWMC_Handle_t * pHdl );

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  It initializes ATU, ADC, GPIO, NVIC for single shunt current
  *         reading configuration using hk32aspin06x family.
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
void R1F0XX_Init( PWMC_R1_F0_Handle_t * pHandle )
{
    if ( ( uint32_t )pHandle == ( uint32_t )&pHandle->_Super )
    {
        /* disable ADC IT and flags  */
        ADC_ITConfig(ADC, ADC_IT_GROUPA_FINISH, DISABLE);
        ADC_ITConfig(ADC, ADC_IT_GROUPB_FINISH, DISABLE);        
		ADC_ClearITPendingBit(ADC, ADC_IT_GROUPA_FINISH);
		ADC_ClearITPendingBit(ADC, ADC_IT_GROUPB_FINISH);        

        R1F0XX_1ShuntMotorVarsInit(&pHandle->_Super );

        R1F0XX_ATUxInit(&pHandle->_Super);
        
        /* Enable DBG */
        ATU_DebugHaltPWMOutput(ATU_DEBUG_HALT_PWM_OUTPUT_DEFAULT_LEVLE);

        /* Enable ADC */
		ADC_Cmd(ADC, ENABLE);

        ATU_CounterCMD(ENABLE);
        
        pHandle->ADCRegularLocked = false; /* We allow ADC usage for regular conversion on Systick*/
        pHandle->_Super.DTTest = 0u;
        pHandle->_Super.DTCompCnt = pHandle->_Super.hDTCompCnt;

    }
}

/**
  * @brief  It initializes ATU peripheral for PWM generation,
  *          active vector insertion and adc triggering.
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
static void R1F0XX_ATUxInit(PWMC_Handle_t * pHdl )
{
//    PWMC_R1_F0_Handle_t * pHandle = ( PWMC_R1_F0_Handle_t * )pHdl;
    
    //使能CNT计数
    ATU_CounterCMD(ENABLE);

    ATU_ConfigOutputLeveLChannel0(ATU_OUTPUT_LEVEL_SETA_LOW, ATU_OUTPUT_LEVEL_SETB_LOW);
    ATU_ConfigOutputLeveLChannel1(ATU_OUTPUT_LEVEL_SETA_LOW, ATU_OUTPUT_LEVEL_SETB_LOW);
    ATU_ConfigOutputLeveLChannel2(ATU_OUTPUT_LEVEL_SETA_LOW, ATU_OUTPUT_LEVEL_SETB_LOW);
    ATU_ConfigOutputSourceChannel0(ATU_OUTPUT_SOURCEA_IO_SET, ATU_OUTPUT_SOURCEB_IO_SET);
    ATU_ConfigOutputSourceChannel1(ATU_OUTPUT_SOURCEA_IO_SET, ATU_OUTPUT_SOURCEB_IO_SET);
    ATU_ConfigOutputSourceChannel2(ATU_OUTPUT_SOURCEA_IO_SET, ATU_OUTPUT_SOURCEB_IO_SET);
    
    ATU_PWMOutputCmd(ENABLE);
//    ATU_SetDeadBandTime( ( pHandle->pParams_str->hDeadTime ) / 2u, ( pHandle->pParams_str->hDeadTime ) / 2u );

}

/**
  * @brief  It stores into handler the voltage present on the
  *         current feedback analog channel when no current is flowin into the
  *         motor
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
void R1F0XX_CurrentReadingCalibration( PWMC_Handle_t * pHdl )
{
    uint8_t bIndex = 0u;
    uint32_t wPhaseOffset = 0u;

    PWMC_R1_F0_Handle_t * pHandle = ( PWMC_R1_F0_Handle_t * )pHdl;

    /* Set the CALIB flags to indicate the ADC calibartion phase*/
    pHandle->hFlags |= CALIB;

  /* We forbid ADC usage for regular conversion on Systick*/
    pHandle->ADCRegularLocked=true; 
  /* ADC Channel and sampling time config for current reading */
    ADC_Cmd(ADC, DISABLE);
    ADC_GroupBias(ADC, GROUPA_ADBA0, 0);
    ADC_GroupBias(ADC, GROUPB_ADBB0, 0);
    ADC_GroupExtTrigConfig(ADC, GROUP_A, ADC_EXT_TRIG_ATU_TRG1, DISABLE);    
	ADC_ClearITPendingBit(ADC, ADC_IT_GROUPA_FINISH); 
    ADC_Cmd(ADC, ENABLE);    
    /* ADC Channel used for current reading are read
    in order to get zero currents ADC values*/
    while ( bIndex < NB_CONVERSIONS )
    {
        /* Software start of conversion */		
        ADC_GroupSwTrig(ADC, GROUP_A, ENABLE);
			
		/* Wait until end of regular conversion */
        while ( ADC_GetITStatus(ADC, ADC_IT_GROUPA_FINISH) != SET)
        {}
 
		wPhaseOffset += ADC_GetConversionValue(ADC, CHANNEL_GROUPA_0);
        bIndex++;
    }
	
    pHandle->hPhaseOffset = ( uint16_t )( wPhaseOffset / NB_CONVERSIONS );
    ADC_GroupBias(ADC, GROUPA_ADBA0, ((pHandle->hPhaseOffset)>>4));
    ADC_GroupBias(ADC, GROUPB_ADBB0, ((pHandle->hPhaseOffset)>>4));
    /* Reset the CALIB flags to indicate the end of ADC calibartion phase*/
    pHandle->hFlags &= ( ~CALIB );
    
    ADC_Cmd(ADC, DISABLE);
	ADC_ClearITPendingBit(ADC, ADC_IT_GROUPA_FINISH); 
    ADC_GroupExtTrigConfig(ADC, GROUP_A, ADC_EXT_TRIG_ATU_TRG1, ENABLE);  
    ADC_Cmd(ADC, ENABLE); 
}

/**
  * @brief  First initialization of class members
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
static void R1F0XX_1ShuntMotorVarsInit( PWMC_Handle_t * pHdl )
{
    PWMC_R1_F0_Handle_t * pHandle = ( PWMC_R1_F0_Handle_t * )pHdl;

    /* Init motor vars */
    pHandle->hPhaseOffset = 0u;

    pHandle->hFlags &= ( ~STBD3 );
    pHandle->hFlags &= ( ~DSTEN );

    /* After reset value of dvDutyValues */
    pHandle->_Super.hCntPhA = pHandle->Half_PWMPeriod >> 1;
    pHandle->_Super.hCntPhB = pHandle->Half_PWMPeriod >> 1;
    pHandle->_Super.hCntPhC = pHandle->Half_PWMPeriod >> 1;

    /* Default value of DutyValues */
    pHandle->hCntSmp1 = ( pHandle->Half_PWMPeriod >> 1 ) - pHandle->pParams_str->hTbefore;/* First point */
    pHandle->hCntSmp2 = ( pHandle->Half_PWMPeriod >> 1 ) + pHandle->pParams_str->hTafter;/* Second point */

}


/**
  * @brief  It computes and return latest converted motor phase currents motor
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval Curr_Components Ia and Ib current in Curr_Components format
  */
void R1F0XX_GetPhaseCurrents( PWMC_Handle_t * pHdl, Curr_Components * pStator_Currents )
{
    int32_t wAux;
    int16_t hCurrA = 0;
    int16_t hCurrB = 0;
    int16_t hCurrC = 0;
    uint8_t bCurrASamp = 0u;
    uint8_t bCurrBSamp = 0u;
    uint8_t bCurrCSamp = 0u;

    PWMC_R1_F0_Handle_t * pHandle = ( PWMC_R1_F0_Handle_t * )pHdl;
	
    /* Reset the bSOFOC flags to indicate the start of FOC algorithm*/
    pHandle->hFlags &= ( ~SOFOC );

    /* First sampling point */
    wAux = ADC->ADDR0B;

    switch ( pHandle->sampCur1 )
    {
        case SAMP_IA:
            hCurrA = ( int16_t )( wAux );
            bCurrASamp = 1u;
            break;
        case SAMP_IB:
            hCurrB = ( int16_t )( wAux );
            bCurrBSamp = 1u;
            break;
        case SAMP_IC:
            hCurrC = ( int16_t )( wAux );
            bCurrCSamp = 1u;
            break;
        case SAMP_NIA:
            wAux = -wAux;
            hCurrA = ( int16_t )( wAux );
            bCurrASamp = 1u;
        break;
        case SAMP_NIB:
            wAux = -wAux;
            hCurrB = ( int16_t )( wAux );
            bCurrBSamp = 1u;
        break;
        case SAMP_NIC:
            wAux = -wAux;
            hCurrC = ( int16_t )( wAux );
            bCurrCSamp = 1u;
            break;
        case SAMP_OLDA:
            hCurrA = pHandle->hCurrAOld;
            bCurrASamp = 1u;
            break;
        case SAMP_OLDB:
            hCurrB = pHandle->hCurrBOld;
            bCurrBSamp = 1u;
            break;
        default:
            break;
    }

  /* Second sampling point */
    wAux = ADC->ADDR0A;
    
    switch ( pHandle->sampCur2 )
    {
        case SAMP_IA:
            hCurrA = ( int16_t )( wAux );
            bCurrASamp = 1u;
            break;
        case SAMP_IB:
            hCurrB = ( int16_t )( wAux );
            bCurrBSamp = 1u;
            break;
        case SAMP_IC:
            hCurrC = ( int16_t )( wAux );
            bCurrCSamp = 1u;
            break;
        case SAMP_NIA:
            wAux = -wAux;
            hCurrA = ( int16_t )( wAux );
            bCurrASamp = 1u;
            break;
        case SAMP_NIB:
            wAux = -wAux;
            hCurrB = ( int16_t )( wAux );
            bCurrBSamp = 1u;
            break;
        case SAMP_NIC:
            wAux = -wAux;
            hCurrC = ( int16_t )( wAux );
            bCurrCSamp = 1u;
            break;
        default:
            break;
    }

    /* Computation of the third value */
    if ( bCurrASamp == 0u )
    {
        wAux = -( ( int32_t )( hCurrB ) ) - ( ( int32_t )( hCurrC ) );

        /* Check saturation */
        if ( wAux > -INT16_MAX )
        {
            if ( wAux < INT16_MAX )
            {
            }
            else
            {
                wAux = INT16_MAX;
            }
        }
        else
        {
            wAux = -INT16_MAX;
        }

        hCurrA = ( int16_t )wAux;
    }
    
    if ( bCurrBSamp == 0u )
    {
        wAux = -( ( int32_t )( hCurrA ) ) - ( ( int32_t )( hCurrC ) );

        /* Check saturation */
        if ( wAux > -INT16_MAX )
        {
            if ( wAux < INT16_MAX )
            {
            }
            else
            {
                wAux = INT16_MAX;
            }
        }
        else
        {
            wAux = -INT16_MAX;
        }

        hCurrB = ( int16_t )wAux;
    }

    if ( bCurrCSamp == 0u )
    {
        wAux = -( ( int32_t )( hCurrA ) ) - ( ( int32_t )( hCurrB ) );

        /* Check saturation */
        if ( wAux > -INT16_MAX )
        {
            if ( wAux < INT16_MAX )
            {
            }
            else
            {
                wAux = INT16_MAX;
            }
        }
        else
        {
            wAux = -INT16_MAX;
        }

        hCurrC = ( int16_t )wAux;
    }

    /* hCurrA, hCurrB, hCurrC values are the sampled values */
    pHandle->hCurrAOld = hCurrA;
    pHandle->hCurrBOld = hCurrB;
    pHandle->hCurrCOld = hCurrC;

    pStator_Currents->qI_Component1 = hCurrA;
    pStator_Currents->qI_Component2 = hCurrB;

    pHandle->_Super.hIa = pStator_Currents->qI_Component1;
    pHandle->_Super.hIb = pStator_Currents->qI_Component2;
    pHandle->_Super.hIc = -pStator_Currents->qI_Component1 - pStator_Currents->qI_Component2;

}

/**
  * @brief  It turns on low sides switches. This function is intended to be
  *         used for charging boot capacitors of driving section. It has to be
  *         called each motor start-up when using high voltage drivers
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
void R1F0XX_TurnOnLowSides( PWMC_Handle_t * pHdl )
{
    PWMC_R1_F0_Handle_t * pHandle = ( PWMC_R1_F0_Handle_t * )pHdl;

    pHandle->_Super.bTurnOnLowSidesAction = true;

    ATU_ConfigOutputLeveLChannel0(ATU_OUTPUT_LEVEL_SETA_LOW, ATU_OUTPUT_LEVEL_SETB_HIGH);
    ATU_ConfigOutputSourceChannel0(ATU_OUTPUT_SOURCEA_IO_SET, ATU_OUTPUT_SOURCEB_IO_SET);

    ATU_ConfigOutputLeveLChannel1(ATU_OUTPUT_LEVEL_SETA_LOW, ATU_OUTPUT_LEVEL_SETB_HIGH);
    ATU_ConfigOutputSourceChannel1(ATU_OUTPUT_SOURCEA_IO_SET, ATU_OUTPUT_SOURCEB_IO_SET);

    ATU_ConfigOutputLeveLChannel2(ATU_OUTPUT_LEVEL_SETA_LOW, ATU_OUTPUT_LEVEL_SETB_HIGH);
    ATU_ConfigOutputSourceChannel2(ATU_OUTPUT_SOURCEA_IO_SET, ATU_OUTPUT_SOURCEB_IO_SET);    
    
    ATU_ClearFlag(ATU_FLAG_TROUGH);

    /* Main PWM Output Enable */
    ATU_CounterCMD(ENABLE);
    ATU_PWMOutputCmd(ENABLE);

    return;
}

/**
  * @brief  It enables PWM generation on the proper Timer peripheral acting on
  *         MOE bit, enaables the single shunt distortion and reset the TIM status
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
void R1F0XX_SwitchOnPWM( PWMC_Handle_t * pHdl )
{
    PWMC_R1_F0_Handle_t * pHandle = ( PWMC_R1_F0_Handle_t * )pHdl;

    pHandle->_Super.bTurnOnLowSidesAction = false;

    //清除更新状态波峰和波谷,开启波谷中断
    ATU_ClearFlag(ATU_FLAG_TROUGH);
    
    /* Set all duty to 50% */
    ATU->CR0A = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR0B = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR1A = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR1B = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR2A = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;
    ATU->CR2B = ( uint32_t )( pHandle->Half_PWMPeriod ) >> 1;

    ATU_ConfigOutputLeveLChannel0(ATU_OUTPUT_LEVEL_SETA_LOW, ATU_OUTPUT_LEVEL_SETB_LOW);
    ATU_ConfigOutputSourceChannel0(ATU_OUTPUT_SOURCEA_COUNTER, ATU_OUTPUT_SOURCEB_COUNTER);
    ATU_ConfigOutputLeveLChannel1(ATU_OUTPUT_LEVEL_SETA_LOW, ATU_OUTPUT_LEVEL_SETB_LOW);
    ATU_ConfigOutputSourceChannel1(ATU_OUTPUT_SOURCEA_COUNTER, ATU_OUTPUT_SOURCEB_COUNTER);
    ATU_ConfigOutputLeveLChannel2(ATU_OUTPUT_LEVEL_SETA_LOW, ATU_OUTPUT_LEVEL_SETB_LOW);
    ATU_ConfigOutputSourceChannel2(ATU_OUTPUT_SOURCEA_COUNTER, ATU_OUTPUT_SOURCEB_COUNTER);    
    
    /* Enable UPDATE ISR */
    ATU_ITConfig(ATU_IT_TROUGH, ENABLE);
	
    NVIC_EnableIRQ(ADC_IRQn);
    ATU_PWMOutputCmd(ENABLE);
    /* Enabling distortion for single shunt */
    pHandle->hFlags |= DSTEN;
    /* We forbid ADC usage for regular conversion on Systick*/
    pHandle->ADCRegularLocked=true; 
    return;
}

/**
  * @brief  It disables PWM generation on the proper Timer peripheral acting on
  *         MOE bit, disables the single shunt distortion and reset the TIM status
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
void R1F0XX_SwitchOffPWM( PWMC_Handle_t * pHdl )
{
    PWMC_R1_F0_Handle_t * pHandle = ( PWMC_R1_F0_Handle_t * )pHdl;

    pHandle->_Super.bTurnOnLowSidesAction = false;

    /* Main PWM Output Disable */
    ATU_PWMOutputCmd(DISABLE);

    /* Disable UPDATE ISR */
    ATU_ITConfig(ATU_IT_TROUGH, DISABLE);
    ATU_ClearFlag(ATU_FLAG_TROUGH);

    /* Disabling distortion for single */
    pHandle->hFlags &= ( ~DSTEN );

    /* We allow ADC usage for regular conversion on Systick*/
    pHandle->ADCRegularLocked=false; 
    
    NVIC_DisableIRQ(ADC_IRQn);
    return;
}


/**
  * @brief  Implementation of the single shunt algorithm to setup the
  *         ATU register values for the next PWM period.
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval uint16_t It returns MC_FOC_DURATION if the TIMx update occurs
  *          before the end of FOC algorithm else returns MC_NO_ERROR
  */
uint16_t R1F0XX_CalcDutyCycles( PWMC_Handle_t * pHdl )
{
    int16_t hDeltaDuty_0;
    int16_t hDeltaDuty_1;
    uint16_t hDutyV_0 = 0u;
    uint16_t hDutyV_1 = 0u;
    uint16_t hDutyV_2 = 0u;
    uint8_t bSector;
    int16_t Tup_0,Tdown_0,Tup_1,Tdown_1,Tup_2,Tdown_2;
    uint16_t hAux;
    int16_t cnt_shift_0=0;//right +; left -
    int16_t cnt_shift_1=0;
    int16_t cnt_shift_2=0;
    
    PWMC_R1_F0_Handle_t * pHandle = ( PWMC_R1_F0_Handle_t * )pHdl;

    bSector = ( uint8_t )pHandle->_Super.hSector;

    if ( ( pHandle->hFlags & DSTEN ) != 0u )
    {
        switch ( bSector )
        {
            case SECTOR_1:
                hDutyV_2 = pHandle->_Super.hCntPhA;
                hDutyV_1 = pHandle->_Super.hCntPhB;
                hDutyV_0 = pHandle->_Super.hCntPhC;
                break;
            case SECTOR_2:
                hDutyV_2 = pHandle->_Super.hCntPhB;
                hDutyV_1 = pHandle->_Super.hCntPhA;
                hDutyV_0 = pHandle->_Super.hCntPhC;
                break;
            case SECTOR_3:
                hDutyV_2 = pHandle->_Super.hCntPhB;
                hDutyV_1 = pHandle->_Super.hCntPhC;
                hDutyV_0 = pHandle->_Super.hCntPhA;
                break;
            case SECTOR_4:
                hDutyV_2 = pHandle->_Super.hCntPhC;
                hDutyV_1 = pHandle->_Super.hCntPhB;
                hDutyV_0 = pHandle->_Super.hCntPhA;
                break;
            case SECTOR_5:
                hDutyV_2 = pHandle->_Super.hCntPhC;
                hDutyV_1 = pHandle->_Super.hCntPhA;
                hDutyV_0 = pHandle->_Super.hCntPhB;
                break;
            case SECTOR_6:
                hDutyV_2 = pHandle->_Super.hCntPhA;
                hDutyV_1 = pHandle->_Super.hCntPhC;
                hDutyV_0 = pHandle->_Super.hCntPhB;
                break;
            default:
                break;
        }
        
        Tup_0 = hDutyV_0;
        Tdown_0 = hDutyV_0;  
        Tup_1 = hDutyV_1;
        Tdown_1 = hDutyV_1; 
        Tup_2 = hDutyV_2;
        Tdown_2 = hDutyV_2; 
        
        hDeltaDuty_0 = ( int16_t )( hDutyV_1 ) - ( int16_t )( hDutyV_0 );
        if(hDeltaDuty_0 < pHandle->pParams_str->hTMin)
        {
            if((hDutyV_0+hDeltaDuty_0) > pHandle->pParams_str->hTMin)
            {
                cnt_shift_0 = hDeltaDuty_0 - pHandle->pParams_str->hTMin;//left
                Tup_0 = hDutyV_0 + cnt_shift_0;
                Tdown_0 = hDutyV_0 - cnt_shift_0; 
                if(Tdown_0 > pHandle->Half_PWMPeriod)
                    Tdown_0 = pHandle->Half_PWMPeriod;
            }
            else
            {
                cnt_shift_1 = pHandle->pParams_str->hTMin - hDeltaDuty_0;//right   
                Tup_1 = hDutyV_1 + cnt_shift_1;
                Tdown_1 = hDutyV_1 - cnt_shift_1;
                if(Tdown_0 < 0)
                    Tdown_0 = 0;                
            }                
        }
        
        hDeltaDuty_1 = ( int16_t )( hDutyV_2 ) - ( int16_t )( hDutyV_1 ) - cnt_shift_1;
        
        if(hDeltaDuty_1 < pHandle->pParams_str->hTMin)
        {
            if((pHandle->Half_PWMPeriod - hDutyV_2+hDeltaDuty_1) > pHandle->pParams_str->hTMin)
            {
                cnt_shift_2 = pHandle->pParams_str->hTMin - hDeltaDuty_1;//right
                Tup_2 = hDutyV_2 + cnt_shift_2;
                Tdown_2 = hDutyV_2 - cnt_shift_2;
                if(Tdown_2 < 0)
                    Tdown_2 = 0;                 
            }
            else
            {
                cnt_shift_1 = hDeltaDuty_1 - pHandle->pParams_str->hTMin;//left  
                Tup_1 = hDutyV_1 + cnt_shift_1;
                Tdown_1 = hDutyV_1 - cnt_shift_1; 
                if(Tdown_1 > pHandle->Half_PWMPeriod)
                    Tdown_1 = pHandle->Half_PWMPeriod;        
            }                
        }        
        
        /* First point */
//        if ( ( Tup_1 - Tup_0 - pHandle->pParams_str->hDeadTime ) > pHandle->pParams_str->hMaxTrTs )
//        {
//            pHandle->hCntSmp1 = Tup_0 + Tup_1 + pHandle->pParams_str->hDeadTime;
//            pHandle->hCntSmp1 >>= 1;
//        }
//        else
        {
            pHandle->hCntSmp1 = Tup_1 - pHandle->pParams_str->hTbefore;
        }
        /* Second point */
//        if ( ( Tup_2 - Tup_1 - pHandle->pParams_str->hDeadTime ) > pHandle->pParams_str->hMaxTrTs )
//        {
//            pHandle->hCntSmp2 = Tup_1 + Tup_2 + pHandle->pParams_str->hDeadTime;
//            pHandle->hCntSmp2 >>= 1;
//        }
//        else
        {
            pHandle->hCntSmp2 = Tup_2 - pHandle->pParams_str->hTbefore;
        }               
   
    }

    /* Update Timer Ch 1,2,3 (These value are required before update event) */
    pHandle->hFlags |= EOFOC;

//    if ( pHandle->bDMACur == 0u )
    {
        switch ( bSector )
        {
            case SECTOR_1: 
                ATU->CR0A = Tup_2;
                ATU->CR0B = Tdown_2;
                ATU->CR1A = Tup_1;
                ATU->CR1B = Tdown_1;
                ATU->CR2A = Tup_0;
                ATU->CR2B = Tdown_0;  
            
                break;
            case SECTOR_2:  
                ATU->CR1A = Tup_2;
                ATU->CR1B = Tdown_2;
                ATU->CR0A = Tup_1;
                ATU->CR0B = Tdown_1;
                ATU->CR2A = Tup_0;
                ATU->CR2B = Tdown_0;           
                break;
            
            case SECTOR_3:
                ATU->CR1A = Tup_2;
                ATU->CR1B = Tdown_2;
                ATU->CR2A = Tup_1;
                ATU->CR2B = Tdown_1;
                ATU->CR0A = Tup_0;
                ATU->CR0B = Tdown_0;           
                break;
            
            case SECTOR_4:
                ATU->CR2A = Tup_2;
                ATU->CR2B = Tdown_2;
                ATU->CR1A = Tup_1;
                ATU->CR1B = Tdown_1;
                ATU->CR0A = Tup_0;
                ATU->CR0B = Tdown_0;            
                break;
            
            case SECTOR_5:
                ATU->CR2A = Tup_2;
                ATU->CR2B = Tdown_2;
                ATU->CR0A = Tup_1;
                ATU->CR0B = Tdown_1;
                ATU->CR1A = Tup_0;
                ATU->CR1B = Tdown_0;    
                break;
            
            case SECTOR_6:       
                ATU->CR0A = Tup_2;
                ATU->CR0B = Tdown_2;
                ATU->CR2A = Tup_1;
                ATU->CR2B = Tdown_1;
                ATU->CR1A = Tup_0;
                ATU->CR1B = Tdown_0;     
                break;         
            
            default:
                break;
        }
        

    }

    /* Check the status of bSOFOC flags if is set the next update event has been
    occurred so an error will be reported*/
    if ( ( pHandle->hFlags & SOFOC ) != 0u )
    {
        hAux = MC_FOC_DURATION;
    }
    else
    {
        hAux = MC_NO_ERROR;
    }

    /* The following instruction can be executed after Update handler
        before the get phase current (Second EOC) */

    /* Set the current sampled */
    pHandle->sampCur1 = REGULAR_SAMP_CUR1[bSector];
    pHandle->sampCur2 = REGULAR_SAMP_CUR2[bSector];

    return ( hAux );
}

/**
 * @brief  It contains the TIMx Update event interrupt
 * @param  pHandle: handler of the current instance of the PWM component
 * @retval none
 */
void * R1F0XX_TIMx_UP_IRQHandler( PWMC_R1_F0_Handle_t * pHandle )
{
    /* Critical point start */
    /* Enabling the External triggering for ADCx*/
    ADC_ITConfig(ADC, ADC_IT_GROUPA_FINISH, ENABLE);

	ATU->TRG0=pHandle->hCntSmp1;	//first point
	ATU->TRG1=pHandle->hCntSmp2;	//second point

    /* Critical point stop */    
    return MC_NULL;

}

/**
 * @brief  It contains the Break event interrupt
 * @param  pHandle: handler of the current instance of the PWM component
 * @retval none
 */
void * F0XX_BRK_IRQHandler( PWMC_R1_F0_Handle_t * pHandle )
{
    pHandle->OverCurrentFlag = true;

    return MC_NULL;
}



/**
  * @brief  It is used to check if an overcurrent occurred since last call.
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval uint16_t It returns MC_BREAK_IN whether an overcurrent has been
  *                  detected since last method call, MC_NO_FAULTS otherwise.
  */
uint16_t R1F0XX_IsOverCurrentOccurred( PWMC_Handle_t * pHdl )
{
    uint16_t retVal = MC_NO_FAULTS;

    PWMC_R1_F0_Handle_t * pHandle = ( PWMC_R1_F0_Handle_t * )pHdl;

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

