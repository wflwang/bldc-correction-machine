/**
  *****************************************************************************************************
  * @file    mc_tasks.c
  * @version V1.0.0
  * @date    2021-07-27
  *****************************************************************************************************
**/


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mc_type.h"
#include "mc_math.h"
#include "mc_config.h"
//#include "mc_interface.h"
#include "mc_tuning.h"
#include "state_machine.h"
#include "mc_tasks.h"
#include "parameters_conversion.h"
/* USER CODE BEGIN Includes */

int16_t AVrspeed = 0;

/* USER CODE END Includes */

/* USER CODE BEGIN Private define */
/* Private define ------------------------------------------------------------*/

#define CHARGE_BOOT_CAP_MS  10
#define OFFCALIBRWAIT_MS     0
#define STOPPERMANENCY_MS  400
#define CHARGE_BOOT_CAP_TICKS  (uint16_t)((SYS_TICK_FREQUENCY * CHARGE_BOOT_CAP_MS)/ 1000)
#define OFFCALIBRWAITTICKS     (uint16_t)((SYS_TICK_FREQUENCY * OFFCALIBRWAIT_MS)/ 1000)
#define STOPPERMANENCY_TICKS   (uint16_t)((SYS_TICK_FREQUENCY * STOPPERMANENCY_MS)/ 1000)

/* USER CODE END Private define */
#define VBUS_TEMP_ERR_MASK (MC_OVER_VOLT| MC_UNDER_VOLT| MC_OVER_TEMP)

/* Private variables----------------------------------------------------------*/
FOCVars_t FOCVars[NBR_OF_MOTORS];
MCI_Handle_t Mci[NBR_OF_MOTORS];
MCI_Handle_t * oMCInterface[NBR_OF_MOTORS];
MCT_Handle_t MCT[NBR_OF_MOTORS];
STM_Handle_t STM[NBR_OF_MOTORS];
SpeednTorqCtrl_Handle_t *pSTC[NBR_OF_MOTORS];
PID_Handle_t *pPIDSpeed[NBR_OF_MOTORS];
PID_Handle_t *pPIDIq[NBR_OF_MOTORS];
PID_Handle_t *pPIDId[NBR_OF_MOTORS];
RDivider_Handle_t *pBusSensorM1;
NTC_Handle_t *pTemperatureSensor[NBR_OF_MOTORS];
PWMC_Handle_t * pwmcHandle[NBR_OF_MOTORS];
PQD_MotorPowMeas_Handle_t *pMPM[NBR_OF_MOTORS];
CircleLimitation_Handle_t *pCLM[NBR_OF_MOTORS];
RampExtMngr_Handle_t *pREMNG[NBR_OF_MOTORS];   /*!< Ramp manager used to modify the Iq ref
                                                    during the start-up switch over.*/

static volatile uint16_t hMFTaskCounterM1 = 0;
static volatile uint16_t hBootCapDelayCounterM1 = 0;
static volatile uint16_t hStopPermanencyCounterM1 = 0;

uint8_t bMCBootCompleted = 0;

/* Private functions ---------------------------------------------------------*/
static void TSK_MediumFrequencyTaskM1(void);
static void FOC_Clear(uint8_t bMotor);
static void FOC_InitAdditionalMethods(uint8_t bMotor);
static void FOC_CalcCurrRef(uint8_t bMotor);
static uint16_t FOC_CurrController(uint8_t bMotor);
void TSK_SetChargeBootCapDelayM1(uint16_t hTickCount);
bool TSK_ChargeBootCapDelayHasElapsedM1(void);
static void TSK_SetStopPermanencyTimeM1(uint16_t hTickCount);
static bool TSK_StopPermanencyTimeHasElapsedM1(void);

void TSK_SafetyTask_PWMOFF(uint8_t motor);

void UI_Scheduler(void);


/**
  * @brief  It initializes the whole MC core according to user defined
  *         parameters.
  * @param  pMCIList pointer to the vector of MCInterface objects that will be
  *         created and initialized. The vector must have length equal to the
  *         number of motor drives.
  * @param  pMCTList pointer to the vector of MCTuning objects that will be
  *         created and initialized. The vector must have length equal to the
  *         number of motor drives.
  * @retval None
  */
void MCboot( MCI_Handle_t* pMCIList[NBR_OF_MOTORS], MCT_Handle_t* pMCTList[NBR_OF_MOTORS] )
{

    bMCBootCompleted = 0;
    pCLM[M1] = &CircleLimitationM1;

    /**********************************************************/
    /*    PWM and current sensing component initialization    */
    /**********************************************************/
    pwmcHandle[M1] = &PWM_Handle_M1._Super;
    R3F0XX_Init(&PWM_Handle_M1);

    /**************************************/
    /*    State machine initialization    */
    /**************************************/
    STM_Init(&STM[M1]);

    /******************************************************/
    /*   PID component initialization: speed regulation   */
    /******************************************************/
    PID_HandleInit(&PIDSpeedHandle_M1);

    /******************************************************/
    /*   Main speed sensor component initialization       */
    /******************************************************/
    pPIDSpeed[M1] = &PIDSpeedHandle_M1;
    pSTC[M1] = &SpeednTorqCtrlM1;

    HALL_Init (&HALL_M1);

    /******************************************************/
    /*   Speed & torque component initialization          */
    /******************************************************/
    STC_Init(pSTC[M1], pPIDSpeed[M1], &HALL_M1._Super);

    /********************************************************/
    /*   PID component initialization: current regulation   */
    /********************************************************/
    PID_HandleInit(&PIDIqHandle_M1);
    PID_HandleInit(&PIDIdHandle_M1);
    pPIDIq[M1] = &PIDIqHandle_M1;
    pPIDId[M1] = &PIDIdHandle_M1;

    /********************************************************/
    /*   Bus voltage sensor component initialization        */
    /********************************************************/
    pBusSensorM1 = &RealBusVoltageSensorParamsM1;
    RVBS_Init(pBusSensorM1);

    /*************************************************/
    /*   Power measurement component initialization  */
    /*************************************************/
    pMPM[M1] = &PQD_MotorPowMeasM1;
    pMPM[M1]->pVBS = &(pBusSensorM1->_Super);
    pMPM[M1]->pFOCVars = &FOCVars[M1];

    /*******************************************************/
    /*   Temperature measurement component initialization  */
    /*******************************************************/
    NTC_Init(&TempSensorParamsM1);
    pTemperatureSensor[M1] = &TempSensorParamsM1;

    pREMNG[M1] = &RampExtMngrHFParamsM1;
    REMNG_Init(pREMNG[M1]);

    FOC_Clear(M1);
    FOCVars[M1].bDriveInput = EXTERNAL;
    FOCVars[M1].Iqdref = STC_GetDefaultIqdref(pSTC[M1]);
    FOCVars[M1].UserIdref = STC_GetDefaultIqdref(pSTC[M1]).qI_Component2;
    oMCInterface[M1] = & Mci[M1];
    MCI_Init(oMCInterface[M1], &STM[M1], pSTC[M1], &FOCVars[M1]);
    MCI_ExecSpeedRamp(oMCInterface[M1],
                      STC_GetMecSpeedRef01HzDefault(pSTC[M1]), 0); /*First command to STC*/
    pMCIList[M1] = oMCInterface[M1];
    MCT[M1].pPIDSpeed = pPIDSpeed[M1];
    MCT[M1].pPIDIq = pPIDIq[M1];
    MCT[M1].pPIDId = pPIDId[M1];
    MCT[M1].pPIDFluxWeakening = MC_NULL; /* if M1 doesn't has FW */
    MCT[M1].pPWMnCurrFdbk = pwmcHandle[M1];
    MCT[M1].pRevupCtrl = MC_NULL;              /* only if M1 is sensorless*/
    MCT[M1].pSpeedSensorMain = (SpeednPosFdbk_Handle_t *) &HALL_M1;
    MCT[M1].pSpeedSensorAux = MC_NULL;
    MCT[M1].pSpeedSensorVirtual = MC_NULL;  /* only if M1 is sensorless*/
    MCT[M1].pSpeednTorqueCtrl = pSTC[M1];
    MCT[M1].pStateMachine = &STM[M1];
    MCT[M1].pTemperatureSensor = (NTC_Handle_t *) pTemperatureSensor[M1];
    MCT[M1].pBusVoltageSensor = &(pBusSensorM1->_Super);
    MCT[M1].pMPM =  (MotorPowMeas_Handle_t*)pMPM[M1];
    MCT[M1].pFW = MC_NULL;
    MCT[M1].pFF = MC_NULL;
    pMCTList[M1] = &MCT[M1];

    bMCBootCompleted = 1;
}

/**
 * @brief Runs all the Tasks of the Motor Control cockpit
 *
 * This function is to be called periodically at least at the Medium Frequency task
 * rate (It is typically called on the Systick interrupt). Exact invokation rate is
 * the Speed regulator execution rate set in the Motor Contorl Workbench.
 *
 * The following tasks are executed in this order:
 *
 * - Medium Frequency Tasks of each motors
 * - Safety Task
 * - Power Factor Correction Task (if enabled)
 * - User Interface task.
 */
void MC_RunMotorControlTasks(void)
{
    if ( bMCBootCompleted )
    {
        /* ** Medium Frequency Tasks ** */
        MC_Scheduler();

        /* Safety task is run after Medium Frequency task so that
        * it can overcome actions they initiated if needed. */
        if (ADC->ADIFR & 0x0004)//GroupB采样完成
        {
            TSK_SafetyTask();
            ADC->ADIFR |= 0x0004;//清除GroupB采样完成标志
        }
    }

}

/**
 * @brief  Executes the Medium Frequency Task functions for each drive instance.
 *
 * It is to be clocked at the Systick frequency.
 */
void MC_Scheduler(void)
{

    if (bMCBootCompleted == 1)
    {
        if (hMFTaskCounterM1 > 0u)
        {
            hMFTaskCounterM1--;
        }
        else
        {
            TSK_MediumFrequencyTaskM1();
            hMFTaskCounterM1 = MF_TASK_OCCURENCE_TICKS;
        }

        if (hBootCapDelayCounterM1 > 0u)
        {
            hBootCapDelayCounterM1--;
        }

        if (hStopPermanencyCounterM1 > 0u)
        {
            hStopPermanencyCounterM1--;
        }
    }
    else
    {
    }

}

/**
  * @brief Executes medium frequency periodic Motor Control tasks
  *
  * This function performs some of the control duties on Motor 1 according to the
  * present state of its state machine. In particular, duties requiring a periodic
  * execution at a medium frequency rate (such as the speed controller for instance)
  * are executed here.
  */
void TSK_MediumFrequencyTaskM1(void)
{
    State_t StateM1;
    int16_t wAux = 0;

    (void) HALL_CalcAvrgMecSpeed01Hz( &HALL_M1, &wAux );
    PQD_CalcElMotorPower( pMPM[M1] );
    AVrspeed = 6 * HALL_M1._Super.hAvrMecSpeed01Hz;

    StateM1 = STM_GetState( &STM[M1] );

    switch ( StateM1 )
    {
        case IDLE_START:
            R3F0XX_TurnOnLowSides( pwmcHandle[M1] );
            TSK_SetChargeBootCapDelayM1( CHARGE_BOOT_CAP_TICKS );
            STM_NextState( &STM[M1], CHARGE_BOOT_CAP );
            break;

        case CHARGE_BOOT_CAP:
            if ( TSK_ChargeBootCapDelayHasElapsedM1() )
            {
                PWMC_CurrentReadingCalibr( pwmcHandle[M1], CRC_START );

                STM_NextState(&STM[M1], OFFSET_CALIB);
            }

            break;

        case OFFSET_CALIB:
            if ( PWMC_CurrentReadingCalibr( pwmcHandle[M1], CRC_EXEC ) )
            {
                STM_NextState( &STM[M1], CLEAR );
            }

            break;

        case CLEAR:
            HALL_Clear( &HALL_M1 );

            if ( STM_NextState( &STM[M1], START ) == true )
            {
                FOC_Clear( M1 );

                R3F0XX_SwitchOnPWM( pwmcHandle[M1] );
            }

            break;

        case START:
        {
            STM_NextState( &STM[M1], START_RUN );
        }
        break;

        case START_RUN:
        {
            FOC_InitAdditionalMethods(M1);
            FOC_CalcCurrRef( M1 );
            STM_NextState( &STM[M1], RUN );
        }

            STC_ForceSpeedReferenceToCurrentSpeed( pSTC[M1] ); /* Init the reference speed to current speed */
            MCI_ExecBufferedCommands( oMCInterface[M1] ); /* Exec the speed ramp after changing of the speed sensor */

        break;

        case RUN:

            MCI_ExecBufferedCommands( oMCInterface[M1] );
            FOC_CalcCurrRef( M1 );


            /* USER CODE BEGIN MediumFrequencyTask M1 3 */

            break;

        case ANY_STOP:
            R3F0XX_SwitchOffPWM( pwmcHandle[M1] );
            FOC_Clear( M1 );
            MPM_Clear( (MotorPowMeas_Handle_t*) pMPM[M1] );
            TSK_SetStopPermanencyTimeM1( STOPPERMANENCY_TICKS );

            STM_NextState( &STM[M1], STOP );
            break;

        case STOP:
            if ( TSK_StopPermanencyTimeHasElapsedM1() )
            {
                STM_NextState( &STM[M1], STOP_IDLE );
            }

            break;

        case STOP_IDLE:
            /* USER CODE BEGIN MediumFrequencyTask M1 5 */

            STM_NextState( &STM[M1], IDLE );
            break;

        default:
            break;
    }


}

/**
  * @brief  It re-initializes the current and voltage variables. Moreover
  *         it clears qd currents PI controllers, voltage sensor and SpeednTorque
  *         controller. It must be called before each motor restart.
  *         It does not clear speed sensor.
  * @param  bMotor related motor it can be M1 or M2
  * @retval none
  */
void FOC_Clear(uint8_t bMotor)
{
    /* USER CODE BEGIN FOC_Clear 0 */

    /* USER CODE END FOC_Clear 0 */

    Curr_Components Inull = {(int16_t)0, (int16_t)0};
    Volt_Components Vnull = {(int16_t)0, (int16_t)0};

    FOCVars[bMotor].Iab = Inull;
    FOCVars[bMotor].Ialphabeta = Inull;
    FOCVars[bMotor].Iqd = Inull;
    FOCVars[bMotor].Iqdref = Inull;
    FOCVars[bMotor].hTeref = (int16_t)0;
    FOCVars[bMotor].Vqd = Vnull;
    FOCVars[bMotor].Valphabeta = Vnull;
    FOCVars[bMotor].hElAngle = (int16_t)0;
    FOCVars[bMotor].now_duty = 0;   //初始当前duty为0

    PID_SetIntegralTerm(pPIDIq[bMotor], (int32_t)0);
    PID_SetIntegralTerm(pPIDId[bMotor], (int32_t)0);

    STC_Clear(pSTC[bMotor]);

    PWMC_SwitchOffPWM(pwmcHandle[bMotor]);

    /* USER CODE BEGIN FOC_Clear 1 */

    /* USER CODE END FOC_Clear 1 */
}

/**
  * @brief  Use this method to initialize additional methods (if any) in
  *         START_TO_RUN state
  * @param  bMotor related motor it can be M1 or M2
  * @retval none
  */
void FOC_InitAdditionalMethods(uint8_t bMotor)
{
    /* USER CODE BEGIN FOC_InitAdditionalMethods 0 */

    /* USER CODE END FOC_InitAdditionalMethods 0 */
}

/**
  * @brief  It computes the new values of Iqdref (current references on qd
  *         reference frame) based on the required electrical torque information
  *         provided by oTSC object (internally clocked).
  *         If implemented in the derived class it executes flux weakening and/or
  *         MTPA algorithm(s). It must be called with the periodicity specified
  *         in oTSC parameters
  * @param  bMotor related motor it can be M1 or M2
  * @retval none
  */
void FOC_CalcCurrRef(uint8_t bMotor)
{
    if (FOCVars[bMotor].bDriveInput == INTERNAL)
    {
        FOCVars[bMotor].hTeref = STC_CalcTorqueReference(pSTC[bMotor]);
        FOCVars[bMotor].Iqdref.qI_Component1 = FOCVars[bMotor].hTeref;
    }
}

/**
  * @brief  It set a counter intended to be used for counting the delay required
  *         for drivers boot capacitors charging of motor 1
  * @param  hTickCount number of ticks to be counted
  * @retval void
  */
void TSK_SetChargeBootCapDelayM1(uint16_t hTickCount)
{
    hBootCapDelayCounterM1 = hTickCount;
}

/**
  * @brief  Use this function to know whether the time required to charge boot
  *         capacitors of motor 1 has elapsed
  * @param  none
  * @retval bool true if time has elapsed, false otherwise
  */
bool TSK_ChargeBootCapDelayHasElapsedM1(void)
{
    bool retVal = false;

    if (hBootCapDelayCounterM1 == 0)
    {
        retVal = true;
    }

    return (retVal);
}

/**
  * @brief  It set a counter intended to be used for counting the permanency
  *         time in STOP state of motor 1
  * @param  hTickCount number of ticks to be counted
  * @retval void
  */
void TSK_SetStopPermanencyTimeM1(uint16_t hTickCount)
{
    hStopPermanencyCounterM1 = hTickCount;
}

/**
  * @brief  Use this function to know whether the permanency time in STOP state
  *         of motor 1 has elapsed
  * @param  none
  * @retval bool true if time is elapsed, false otherwise
  */
bool TSK_StopPermanencyTimeHasElapsedM1(void)
{
    bool retVal = false;

    if (hStopPermanencyCounterM1 == 0)
    {
        retVal = true;
    }

    return (retVal);
}

#if defined (CCMRAM_ENABLED)
    #if defined (__ICCARM__)
        #pragma location = ".ccmram"
    #elif defined (__CC_ARM)
        __attribute__((section (".ccmram")))
    #endif
#endif
/**
  * @brief  Executes the Motor Control duties that require a high frequency rate and a precise timing
  *
  *  This is mainly the FOC current control loop. It is executed depending on the state of the Motor Control
  * subsystem (see the state machine(s)).
  *
  * @retval Number of the  motor instance which FOC loop was executed.
  */
uint8_t TSK_HighFrequencyTask(void)
{
    uint8_t bMotorNbr = 0;
    uint16_t hFOCreturn;
    //HALL_CalcElAngle (&HALL_M1);

    hFOCreturn = FOC_CurrController(M1);

    if (hFOCreturn == MC_FOC_DURATION)
    {
        STM_FaultProcessing(&STM[M1], MC_FOC_DURATION, 0);
    }
    else
    {
        /* USER CODE BEGIN HighFrequencyTask SINGLEDRIVE_3 */

        /* USER CODE END HighFrequencyTask SINGLEDRIVE_3 */
    }

    /* USER CODE BEGIN HighFrequencyTask 1 */

    /* USER CODE END HighFrequencyTask 1 */
    return bMotorNbr;
}

#if defined (CCMRAM)
    #if defined (__ICCARM__)
        #pragma location = ".ccmram"
    #elif defined (__CC_ARM) || defined(__GNUC__)
        __attribute__((section (".ccmram")))
    #endif
#endif



/**
  * @brief It executes the core of FOC drive that is the controllers for Iqd
  *        currents regulation. Reference frame transformations are carried out
  *        accordingly to the active speed sensor. It must be called periodically
  *        when new motor currents have been converted
  * @param this related object of class CFOC.
  * @retval int16_t It returns MC_NO_FAULTS if the FOC has been ended before
  *         next PWM Update event, MC_FOC_DURATION otherwise
  */
inline uint16_t FOC_CurrController(uint8_t bMotor)
{
    Curr_Components Iab;
    int16_t hElAngle;
    Volt_Components Valphabeta;
    Curr_Components Iqd, Ialphabeta;
    Volt_Components Vqd;
    uint16_t hCodeError;
    //duty 控制模式是 用 上次   sqrt(vq*vq+vd*vd)*2/sqrt(3)*sign(vq) = duty_now
    //角度由hall算出来
    if(motor->m_ang_hall_int_prev<65536){
        //hall 角度正常
    }else{
        //hall 失效 用观测器角度
    }
    //hElAngle = SPD_GetElAngle(STC_GetSpeedSensor(pSTC[bMotor]));
    PWMC_GetPhaseCurrents(pwmcHandle[bMotor], &Iab);
    
    Ialphabeta = MCM_Clarke(Iab);
    
    Iqd = MCM_Park(Ialphabeta, hElAngle);   
    
    Vqd.qV_Component1 = PI_Controller(pPIDIq[bMotor],
                                      (int32_t)(FOCVars[bMotor].Iqdref.qI_Component1) - Iqd.qI_Component1);

    Vqd.qV_Component2 = PI_Controller(pPIDId[bMotor],
                                      (int32_t)(FOCVars[bMotor].Iqdref.qI_Component2) - Iqd.qI_Component2);
                                         
    FOCVars[bMotor].Vqd = Vqd;
    Vqd = Circle_Limitation(pCLM[bMotor], Vqd);
    Valphabeta = MCM_Rev_Park(Vqd, hElAngle);
    hCodeError = PWMC_SetPhaseVoltage(pwmcHandle[bMotor], Valphabeta);

    int32_t vq2 = Vqd.qV_Component1*Vqd.qV_Component1;
    int32_t vd2 = Vqd.qV_Component2*Vqd.qV_Component2;
    FOCVars[bMotor].MaxDutyV = ((vq2+vd2)>>16);     //max is 0x2fff
    FOCVars[bMotor].Iab = Iab;
    FOCVars[bMotor].Ialphabeta = Ialphabeta;
    FOCVars[bMotor].Iqd = Iqd;
    FOCVars[bMotor].Valphabeta = Valphabeta;
    FOCVars[bMotor].hElAngle = hElAngle;
    return (hCodeError);
}

/**
  * @brief  Executes safety checks (e.g. bus voltage and temperature) for all drive instances.
  *
  * Faults flags are updated here.
  */
void TSK_SafetyTask(void)
{
    if (bMCBootCompleted == 1)
    {
        TSK_SafetyTask_PWMOFF(M1);
    }
}

/**
  * @brief  Safety task implementation if  MC.ON_OVER_VOLTAGE == TURN_OFF_PWM
  * @param  bMotor Motor reference number defined
  *         \link Motors_reference_number here \endlink
  * @retval None
  */
void TSK_SafetyTask_PWMOFF(uint8_t bMotor)
{
    uint16_t CodeReturn = MC_NO_ERROR;
    uint16_t errMask[NBR_OF_MOTORS] = {VBUS_TEMP_ERR_MASK};

    CodeReturn |= errMask[bMotor] & NTC_CalcAvTemp(pTemperatureSensor[bMotor]); /* check for fault if FW protection is activated. It returns MC_OVER_TEMP or MC_NO_ERROR */
    CodeReturn |= PWMC_CheckOverCurrent(pwmcHandle[bMotor]);                    /* check for fault. It return MC_BREAK_IN or MC_NO_FAULTS */

    if (bMotor == M1)
    {
        CodeReturn |=  errMask[bMotor] & RVBS_CalcAvVbus(pBusSensorM1);
    }

    STM_FaultProcessing(&STM[bMotor], CodeReturn, ~CodeReturn); /* Update the STM according error code */

    switch (STM_GetState(&STM[bMotor])) /* Acts on PWM outputs in case of faults */
    {
        case FAULT_NOW:
            PWMC_SwitchOffPWM(pwmcHandle[bMotor]);
            FOC_Clear(bMotor);
            MPM_Clear((MotorPowMeas_Handle_t*)pMPM[bMotor]);

            break;

        case FAULT_OVER:
            PWMC_SwitchOffPWM(pwmcHandle[bMotor]);

            break;

        default:
            break;
    }

}

/**
  * @brief  This function returns the reference of the MCInterface relative to
  *         the selected drive.
  * @param  bMotor Motor reference number defined
  *         \link Motors_reference_number here \endlink
  * @retval MCI_Handle_t * Reference to MCInterface relative to the selected drive.
  *         Note: it can be MC_NULL if MCInterface of selected drive is not
  *         allocated.
  */
MCI_Handle_t * GetMCI(uint8_t bMotor)
{
    MCI_Handle_t * retVal = MC_NULL;

    if ((oMCInterface != MC_NULL) && (bMotor < NBR_OF_MOTORS))
    {
        retVal = oMCInterface[bMotor];
    }

    return retVal;
}

/**
  * @brief  This function returns the reference of the MCTuning relative to
  *         the selected drive.
  * @param  bMotor Motor reference number defined
  *         \link Motors_reference_number here \endlink
  * @retval MCT_Handle_t motor control tuning handler for the selected drive.
  *         Note: it can be MC_NULL if MCInterface of selected drive is not
  *         allocated.
  */
MCT_Handle_t* GetMCT(uint8_t bMotor)
{
    MCT_Handle_t* retVal = MC_NULL;

    if (bMotor < NBR_OF_MOTORS)
    {
        retVal = &MCT[bMotor];
    }

    return retVal;
}

/**
  * @brief  Puts the Motor Control subsystem in in safety conditions on a Hard Fault
  *
  *  This function is to be executed when a general hardware failure has been detected
  * by the microcontroller and is used to put the system in safety condition.
  */
void TSK_HardwareFaultTask(void)
{
    /* USER CODE BEGIN TSK_HardwareFaultTask 0 */

    /* USER CODE END TSK_HardwareFaultTask 0 */

    R3F0XX_SwitchOffPWM(pwmcHandle[M1]);
    STM_FaultProcessing(&STM[M1], MC_SW_ERROR, 0);
    /* USER CODE BEGIN TSK_HardwareFaultTask 1 */

    /* USER CODE END TSK_HardwareFaultTask 1 */
}

/**
 * @brief  Locks GPIO pins used for Motor Control to prevent accidental reconfiguration
 */
void mc_lock_pins (void)
{
    GPIO_PinLockConfig(GPIOB, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

/* USER CODE BEGIN mc_task 0 */

/* USER CODE END mc_task 0 */

