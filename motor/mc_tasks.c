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
#include <stdint.h>
#include "foc_hall.h"
#include "hw_correct.h"
/* USER CODE BEGIN Includes */

int16_t AVrspeed = 0;
static int16_t NowVBusx100 = 0;   //当前母线电压*100
static int16_t NowTempMotorx100 = 0;   //当前马达电压*100
static int16_t NowTempPCBx100 = 0;   //当前PCB电压*100

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

    M_Hall_Init(&FOCVars[M1].foc_hall);
    //HALL_Init (&HALL_M1);
    //开启一次AD转换，触发一次中断，读取一次电压，给FOC一个初始的v_bus值
    //ADC_StartConversion(ADC1, ADC_CHANNEL_0); // 触发一次AD
    //读取上电时候电压 根据初始顶呀KV值算出 最大转速
    //电机停止2s比较电压最大转速 超过最大电压20%目标转速

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
    //NTC_Init(&TempSensorParamsM1);
    //pTemperatureSensor[M1] = &TempSensorParamsM1;

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
    //MCT[M1].pTemperatureSensor = (NTC_Handle_t *) pTemperatureSensor[M1];
    //MCT[M1].pBusVoltageSensor = &(pBusSensorM1->_Super);
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
            TSK_SafetyTask();       //500us执行一次安全任务，检查过压欠压等
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
    //计算hall的平均机械速度和电机功率
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
            //这里要先检测一下 有没有校准hall 和 学习FOC参数 没有的话就先校准一下
            if(FOCVars[bMotor].status == ready_RUN){
                //待机模式才可以初始化hall 并开始校准
                GetHallState(&HALL_M1);  //获取hall状态
                STM_NextState( &STM[M1], START_RUN );
            }
        }
        break;

        case START_RUN:
        {
            //FOC_InitAdditionalMethods(M1);
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
    PWMC_GetPhaseCurrents(pwmcHandle[bMotor], &Iab);    //获取电流
    if(FOCVars[bMotor].foc_hall.hallState==0){
        //正常控制模式
        #ifdef duty_ControlMode //duty控制模式
	    int16_t duty_set = FOCVars[bMotor].m_duty_cycle_set;	//当前设置的目标duty
		int16_t duty_now =  FOCVars[bMotor].now_duty;	//上次算出的duty大小
		int16_t duty_abs = fabsf(duty_now);
		// Duty cycle control
		if ((fabsf(duty_set) < duty_abs) &&
				((!FOCVars[bMotor].duty_was_pi) || (SIGN_int16(FOCVars[bMotor].duty_pi_duty_last) == SIGN_int16(duty_now)))) {
			// Truncating the duty cycle here would be dangerous, so run a PI controller.
			FOCVars[bMotor].duty_pi_duty_last = duty_now;
			FOCVars[bMotor].duty_was_pi = true;	//上次控制是PI控制输出
			// Reset the integrator in duty mode to not increase the duty if the load suddenly changes. In braking
			// mode this would cause a discontinuity, so there we want to keep the value of the integrator.
			//if (motor_now->m_control_mode == CONTROL_MODE_DUTY) {
				if (duty_now > 0) {
					if (FOCVars[bMotor].m_duty_i_term > 0) {
						FOCVars[bMotor].m_duty_i_term = 0;
					}
				} else {
					if (FOCVars[bMotor].m_duty_i_term < 0) {
						FOCVars[bMotor].m_duty_i_term = 0;
					}
				}
			//}
			// Compute error
			int32_t error = (int32_t)duty_set - (int32_t)duty_now;
			//带上v_bus 的意义是 不同电压下 做一个比例参考 让电压一致
			// Compute parameters
			//float scale = 1.0 / state_now->v_bus;
			int32_t p_term = (error * FOCVars[bMotor].foc_duty_dowmramp_kp<<15) / FOCVars[bMotor].vBus;
            //*dt 是不同中断时间 ki的影响不大 FOCVars[bMotor].foc_hall.intTime
			// 先计算增量
            int32_t i_inc = (error * (FOCVars[bMotor].foc_duty_dowmramp_ki << 15)) / FOCVars[bMotor].vBus;

            // 【安全写法】防止累加后溢出
            if ((i_inc > 0) && (FOCVars[bMotor].m_duty_i_term > (INT32_MAX - i_inc))) {
                FOCVars[bMotor].m_duty_i_term = INT32_MAX;
            } else if ((i_inc < 0) && (FOCVars[bMotor].m_duty_i_term < (-INT32_MAX - i_inc))) {
                FOCVars[bMotor].m_duty_i_term = -INT32_MAX;
            } else {
                FOCVars[bMotor].m_duty_i_term += i_inc;
            }
			//utils_truncate_number((float*)&FOCVars[bMotor].m_duty_i_term, -1.0, 1.0);
			// Calculate output
            int32_t output;
            if ((p_term > 0) && (FOCVars[bMotor].m_duty_i_term > (INT32_MAX - p_term))) {
                output = INT32_MAX;
            } else if ((p_term < 0) && (FOCVars[bMotor].m_duty_i_term < (-INT32_MAX - p_term))) {
                output = -INT32_MAX;
            } else {
                output = p_term + FOCVars[bMotor].m_duty_i_term;
            }
			//utils_truncate_number(&output, -1.0, 1.0);
			FOCVars[bMotor].Iqdref.qI_Component1  = ((int16_t)(output>>15) * current_max_for_duty)>>15;
		} else {
			// If the duty cycle is less than or equal to the set duty cycle just limit
			// the modulation and use the maximum allowed current.
            //目的是给一个积分,让下次电流减小时候先锁定在这个电流值 不要直接锁定在duty上 这样可以让电流变化更平滑
			FOCVars[bMotor].m_duty_i_term = (FOCVars[bMotor].Iqd.qI_Component1<<15) / current_max_for_duty;   //当前iq/max
			//state_now->max_duty = duty_set; //猛加速时让D轴变化慢一点  转速不高不考虑
			if (duty_set > 0) {
				FOCVars[bMotor].Iqdref.qI_Component1 = current_max_for_duty;	//如果设置的目标duty是正的 就给正的最大电流
			} else {
				FOCVars[bMotor].Iqdref.qI_Component1 = -current_max_for_duty;
			}
			FOCVars[bMotor].duty_was_pi = false;
		}
        FOCVars[bMotor].Iqdref.qI_Component2 = 0; // 初始化id_set_tmp
        #endif
        //duty 控制模式是 用 上次   sqrt(vq*vq+vd*vd)*2/sqrt(3)*sign(vq) = duty_now
        //角度由hall算出来
        if(FOCVars[bMotor].foc_hall.angUpdate==true){ //有换相更新
            FOCVars[bMotor].foc_hall.real_phase = FOCVars[bMotor].foc_hall.m_ang_hall_int_prev;    //真实相位
            FOCVars[bMotor].foc_hall.real_phase_Next = FOCVars[bMotor].foc_hall.m_ang_hall_int_Next; //下次相位
            FOCVars[bMotor].foc_hall.angUpdate = false;   //换相更新完成
        }else{
            int16_t minDec = FOCVars[bMotor].foc_hall.anginc >>3; 
            if(minDec==0){
                minDec = 1; //最小的幅度
            }
            FOCVars[bMotor].foc_hall.real_phase =  FOCVars[bMotor].foc_hall.real_phase + FOCVars[bMotor].foc_hall.anginc;    //真实相位
            if(FOCVars[bMotor].foc_hall.anginc > 0)  // 正转
            {
                if(FOCVars[bMotor].foc_hall.real_phase > FOCVars[bMotor].foc_hall.real_phase_Next)
                {
                    // 不让角度超过目标
                    FOCVars[bMotor].foc_hall.real_phase = FOCVars[bMotor].foc_hall.real_phase_Next;  
                    // 速度自动变小（衰减），防止超前太多
                    // 下次以更小步距走，等待真实霍尔跳变
                    FOCVars[bMotor].foc_hall.anginc -= minDec;  
                    // 防止速度变成0，保持极小蠕动
                    if(FOCVars[bMotor].foc_hall.anginc < 1){
                        //达到下次换相点但是不换相,角度变化很慢了 此时要限制duty_set
                        duty_set = (duty_set *7) >>3;   //87.5%
                        if(duty_set < 10) duty_set = 10; // 最低限幅
                        FOCVars[bMotor].foc_hall.anginc = 1;
                    }
                }
            }else if(FOCVars[bMotor].foc_hall.anginc < 0){
                if(FOCVars[bMotor].foc_hall.real_phase < FOCVars[bMotor].foc_hall.real_phase_Next)
                {
                    FOCVars[bMotor].foc_hall.real_phase = FOCVars[bMotor].foc_hall.real_phase_Next;
                    // 反向速度衰减
                    FOCVars[bMotor].foc_hall.anginc += minDec;
                    if(FOCVars[bMotor].foc_hall.anginc > -1){
                        duty_set = (duty_set *7) >>3;   //87.5%
                        if(duty_set > -10) duty_set = -10;
                        FOCVars[bMotor].foc_hall.anginc = -1;
                    }
                }
            }else{
                //没有角度增量 是停止了 此时duty要 = 0;
                duty_set = 0;
            }
        }
    }   //其他模式角度由外部提供
    hElAngle = FOCVars[bMotor].foc_hall.real_phase;
    //hElAngle = SPD_GetElAngle(STC_GetSpeedSensor(pSTC[bMotor]));
    
    Ialphabeta = MCM_Clarke(Iab);
    
    Iqd = MCM_Park(Ialphabeta, hElAngle);   
    
    Vqd.qV_Component1 = PI_Controller(pPIDIq[bMotor],
                                      (int32_t)(FOCVars[bMotor].Iqdref.qI_Component1) - Iqd.qI_Component1);

    Vqd.qV_Component2 = PI_Controller(pPIDId[bMotor],
                                      (int32_t)(FOCVars[bMotor].Iqdref.qI_Component2) - Iqd.qI_Component2);
     
    #ifdef decodedqEm            //         解耦             
    //=====================================================================
    //  VESC 标准 d/q 交叉解耦（MXlemming 高速弱磁必加，角度更稳）
    //=====================================================================
    #define ELEC_SPEED   hElSpeed         // 你的电角速度 Q15 或 int32
    #define FLUX        pMC[bMotor]->Flux // 磁链
    #define Lq_Ld       (pMC[bMotor]->Lq - pMC[bMotor]->Ld)

    Vqd.qV_Component1 += (int32_t)ELEC_SPEED * FLUX / 32768;       // 反电动势前馈
    Vqd.qV_Component2 -= (int32_t)ELEC_SPEED * Lq_Ld * Iqd.qI_Component1 / 32768; // 解耦
    //FOCVars[bMotor].Vqd = Vqd;
    //Vqd = Circle_Limitation(pCLM[bMotor], Vqd);
    #endif
    //=====================================================================
    //  VESC 风格：内切圆 → 六边形 平滑渐进过调制（适配你的代码）
    //=====================================================================
    Volt_Components Vqd_circle;
    int32_t mag_sq;
    int32_t circle_max_sq;
    int32_t hex_max_sq;
    int32_t diff;
    int32_t ratio;
    int32_t one_minus_ratio;

    // 1. 先做你原来的圆形限制
    Vqd_circle = Circle_Limitation(pCLM[bMotor], Vqd);

    // 2. 计算电压模长平方（不用开方！）
    mag_sq = (int32_t)Vqd.qV_Component1 * Vqd.qV_Component1
           + (int32_t)Vqd.qV_Component2 * Vqd.qV_Component2;

    // 3. 你内切圆的最大幅值平方
    circle_max_sq = (int32_t)pCLM[bMotor]->MaxModule * pCLM[bMotor]->MaxModule;

    // 4. 六边形最大幅值平方（比圆大 15% 左右，VESC标准）
    //hex_max_sq = (circle_max_sq * 130) / 100;  // 放大1.3倍，最安全
    hex_max_sq = ((circle_max_sq * 334) >> 8);  // 放大1.3倍，最安全

    Volt_Components Vqd_final;

    if (mag_sq <= circle_max_sq)
    {
        // 完全在线性区 → 只用圆形限制
        Vqd_final = Vqd_circle;
    }
    else if (mag_sq >= hex_max_sq)
    {
        // 完全进入过调制 → 不用限制
        Vqd_final = Vqd;
    }
    else
    {
        // ==============================
        // VESC 核心：平滑过渡（全定点）
        // ==============================
        diff = mag_sq - circle_max_sq;
        //ratio = (diff * 32767) / (hex_max_sq - circle_max_sq); // 0~32767
        ratio = (diff << 15) / (hex_max_sq - circle_max_sq); // 0~32767
        //one_minus_ratio = 32767 - ratio;
        one_minus_ratio = 32768 - ratio;

        // 平滑混合：圆形限制 + 原始电压
        //Vqd_final.qV_Component1 = (Vqd_circle.qV_Component1 * one_minus_ratio
        //                         + Vqd.qV_Component1 * ratio) / 32767;
        Vqd_final.qV_Component1 = (Vqd_circle.qV_Component1 * one_minus_ratio
                                 + Vqd.qV_Component1 * ratio) >> 15;

        //Vqd_final.qV_Component2 = (Vqd_circle.qV_Component2 * one_minus_ratio
        //                         + Vqd.qV_Component2 * ratio) / 32767;
        Vqd_final.qV_Component2 = (Vqd_circle.qV_Component2 * one_minus_ratio
                                 + Vqd.qV_Component2 * ratio) >>15;
    }

    // 最终输出
    Vqd = Vqd_final;

    Valphabeta = MCM_Rev_Park(Vqd, hElAngle);
    hCodeError = PWMC_SetPhaseVoltage(pwmcHandle[bMotor], Valphabeta);

    int32_t vq2 = Vqd.qV_Component1*Vqd.qV_Component1;
    int32_t vd2 = Vqd.qV_Component2*Vqd.qV_Component2;
    //motor_now->m_motor_state.duty_now = ((vq2+vd2)>>16);    //当前算出的duty大小
    if(Vqd.qV_Component1<0)
        FOCVars[bMotor].now_duty = -((vq2+vd2)>>16);
    else
        FOCVars[bMotor].now_duty = ((vq2+vd2)>>16);     //max is 0x2fff
    FOCVars[bMotor].Vqd = Vqd;
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
  * 500us 执行一次安全任务，检查过压欠压等
  */
void TSK_SafetyTask_PWMOFF(uint8_t bMotor)
{
    uint16_t vBusx100;   //母线电压*100
    uint16_t vTempMotorx100;   //马达温度*100
    uint16_t vTempPCBx100;   //PCB温度*100
    static int countVolUnder = 0;   //连续过压欠压的次数
    static int countVolOver = 0;   //连续过压欠压的次数
    static int countTempMotorOver = 0;   //连续过压欠压的次数
    static int countTempMosOver = 0;   //连续过压欠压的次数
    //uint16_t CodeReturn = MC_NO_ERROR;
    //uint16_t errMask[NBR_OF_MOTORS] = {VBUS_TEMP_ERR_MASK};
    if(FOCVars[bMotor].status == not_ready){
        //电机准备中
        vBusx100 = GET_INPUT_VOLTAGE();     //获取VDD 的AD值
        vTempMotorx100 = NTC_TEMP_MOTOR(TempBeta);
        vTempPCBx100 = NTC_TEMP_PCB(TempBeta);
        NowVBusx100 = vBusx100;   //更新当前电压值
        NowTempMotorx100 = vTempMotorx100;
        NowTempPCBx100 = vTempPCBx100;
        if(vBusx100 < vMinBus){   //
            //电机过低 不工作
            FOCVars[bMotor].status = mc_under_voltage;   //欠压了
        }else if(vBusx100 > vMaxBus){
            FOCVars[bMotor].status = mc_over_voltage;   //过压了
        }else{
            FOCVars[bMotor].mc_MaxSpeed = vBusx100*FOCVars[bMotor].mc_KV/100;   //算出最大转速
            //初始化电压 根据上电电压算出最大转速
            FOCVars[bMotor].status = ready_RUN;   //准备好了 可以运行了
        }
        return; //如果电机还没有开始运行 就不检查安全了
    }else{
        if(FOCVars[bMotor].status != ready_RUN)
            return;     //没有正常运行
        vBusx100 = GET_INPUT_VOLTAGE();     //获取VDD 的AD值
        UTILS_LPInt16_FAST(NowVBusx100, vBusx100, VBusFilterConstant);   //更新当前电压值 
        UTILS_LPInt16_FAST(NowTempMotorx100, vTempMotorx100, VTempMotorFilterConstant);   //更新Motor温度
        UTILS_LPInt16_FAST(NowTempPCBx100, vTempPCBx100, VTempPCBFilterConstant);   //更新PCB温度
        if(NowVBusx100 < vMinBus){   //
            //电机过低 不工作
            countVolUnder++;
            if(countVolUnder > 2000){    //1s
                FOCVars[bMotor].status = mc_under_voltage;   //欠压了
            }
        }else
            countVolUnder = 0;
        if(NowVBusx100 > vMaxBus){
            countVolOver++;
            if(countVolOver > 2000){    //1s
                FOCVars[bMotor].status = mc_over_voltage;   //过压了
            }
        }else   
            countVolOver = 0;
        #ifdef MotorTempEn
        if(NowTempMotorx100>vMaxMotorTemp){   //电机温度超过
            countTempMotorOver++;
            if(countTempMotorOver>6000){
                //3s
                FOCVars[bMotor].status = mc_over_MotorTemp;   //motor 过温
            }
        }else
            countTempMotorOver = 0;
        #endif
        #ifdef MosTempEn
        if(NowTempPCBx100>vMaxPCBTemp){   //MOS 温度超过
            countTempMosOver++;
            if(countTempMosOver>6000){
                //3s
                FOCVars[bMotor].status = mc_over_MosTemp;   //mos 过温
            }
        }else   
            countTempMosOver = 0;
        #endif
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

