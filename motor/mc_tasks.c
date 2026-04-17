/**
  *****************************************************************************************************
  * @file    mc_tasks.c
  * @version V1.0.0
  * @date    2021-07-27
  *****************************************************************************************************
**/


/* Includes ------------------------------------------------------------------*/
#include "MCLib/Any/Inc/mc_type.h"
#include "main.h"
#include "mc_type.h"
#include "mc_math.h"
#include "mc_config.h"
#include "mc_interface.h"
#include "mc_tuning.h"
#include "state_machine.h"
#include "mc_api.h"
#include "mc_tasks.h"
#include "parameters_conversion.h"
#include <stdint.h>
#include "foc_hall.h"
#include "hw_correct.h"
#include "EEPROM.h"
#include "app.h"
#include "crc.h"
#include <math.h>
#include "utils_math.h"
#include "peripherals.h"
/* USER CODE BEGIN Includes */

int16_t AVrspeed = 0;
static int16_t NowVBusAD = 0;   //��ǰĸ�ߵ�ѹAD
static int16_t NowTempMotorx100 = 0;   //��ǰ������?*100
static int16_t NowTempPCBx100 = 0;   //��ǰPCB��ѹ*100
static int IsSpeech = 0;  //�Ƿ���
#ifndef cTestSVPWM
static int IScount = 0; //�������ٶȻ��л�ʱ��
#endif
#define SYSTICK_DIVIDER (SYS_TICK_FREQUENCY/1000)

static int32_t Maxspeed = 1000;     //��������?�� 
//int nowSpeed=0;

int32_t Count1ms = 0;   //1ms count

mc_config_t mcconf;     //��ʼ�� ��������
app_config_t appconf;   //app config

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
//static void FOC_InitAdditionalMethods(uint8_t bMotor);
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
    //GetMCConfig();  //��flash�ж�ȡ�������?
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

    M_Hall_Init(&HALL_M1,&mcconf);
    //HALL_Init (&HALL_M1);
    //����һ��ADת��������һ���жϣ���ȡһ�ε�ѹ����FOCһ����ʼ��v_busֵ
    //ADC_StartConversion(ADC1, ADC_CHANNEL_0); // ����һ��AD
    //��ȡ�ϵ�ʱ����? ���ݳ�ʼ��ѽKVֵ���? ����?��
    //���ͣ�?2s�Ƚϵ�ѹ����?�� ��������ѹ20%Ŀ��ת��

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
    //pBusSensorM1 = &RealBusVoltageSensorParamsM1;
    //RVBS_Init(pBusSensorM1);

    /*************************************************/
    /*   Power measurement component initialization  */
    /*************************************************/
    //pMPM[M1] = &PQD_MotorPowMeasM1;
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
    //
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
        if (ADC->ADIFR & 0x0004)//GroupB�������?
        {
            TSK_SafetyTask();       //500usִ��һ�ΰ�ȫ���񣬼���ѹǷѹ��
            ADC->ADIFR |= 0x0004;//���GroupB������ɱ��?
        }
    }

}
//��ȡ�Ƿ���״̬
int GetMSpeechEN(void){
  return IsSpeech;
}
//���÷���״̬
void SetMSpeechEN(int state){
  IsSpeech = state;
}
bool GetISChangeState(void){
  return HALL_M1.I_feed;
}
/**
 * @brief get now VBus VDD*100
 * 
 */
int16_t GetNowVBusAD(void){
    return NowVBusAD;
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
 * @brief Get now speed
 * �����ٶȻ����ٶ� Ҫ�ȶ�,ͬʱ������̫���ӳ�
 * 
 */
void Hall_CalcAvrgMecSpeed01Hz( foc_hall_t * pHandle)
{
  //->�����ٶȲ�
  /*Stores average mechanical speed [01Hz]*/
  if(pHandle->_Super.bElToMecRatio)
  pHandle->_Super.hAvrMecSpeed01Hz  = pHandle->erpm/pHandle->_Super.bElToMecRatio;
  else
    pHandle->_Super.hAvrMecSpeed01Hz = pHandle->erpm;
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
    Count1ms++; //1ms add
    //int16_t wAux = 0;
    //����hall��ƽ����е�ٶȺ͵������?
    //(void) HALL_CalcAvrgMecSpeed01Hz( &HALL_M1, &wAux );
    //PQD_CalcElMotorPower( pMPM[M1] );
    //AVrspeed = 6 * HALL_M1._Super.hAvrMecSpeed01Hz;
	Hall_CalcAvrgMecSpeed01Hz( &HALL_M1);
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
            //HALL_Clear( &HALL_M1 );

            if ( STM_NextState( &STM[M1], START ) == true )
            {
                FOC_Clear( M1 );

                R3F0XX_SwitchOnPWM( pwmcHandle[M1] );
            }

            break;

        case START:
        {
            //����Ҫ�ȼ��һ��? ��û��У׼hall �� ѧϰFOC���� û�еĻ�����У׼һ��
            if((FOCVars[M1].status == ready_RUN)||(FOCVars[M1].status == motor_run)){    //û�е�ѹ����
                //����ģʽ�ſ��Գ�ʼ��hall ����ʼУ׼
                //if(GetHallState(&HALL_M1)==hall_run){  //��ȡhall״̬
                    //hall ���������� ������һ���� ��ʼFOC
                    STM_NextState( &STM[M1], START_RUN );
                //}else{
                    //������������?ϰFOC ���� hallλ��
                //}
                //STM_NextState( &STM[M1], START_RUN );
            }   //�е�ѹ���� ��������һ��״̬
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
            //MPM_Clear( (MotorPowMeas_Handle_t*) pMPM[M1] );
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
 * get err max ready
 * return 1 max 2 -max 0 normal
 * 
*/
uint8_t GetMaxTerefReady(void){
  if(FOCVars[M1].Iqdref.qI_Component1>=CurrentInt16_MaxCur) //CurrentInt16(DefaultMaxCurrent)
  return 1;
  else if(FOCVars[M1].Iqdref.qI_Component1<=-CurrentInt16_MaxCur)   //CurrentInt16(-DefaultMaxCurrent)
  return 2;
  else
  return 0;
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
    FOCVars[bMotor].now_duty = 0;   //��ʼ��ǰdutyΪ0

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
//void FOC_InitAdditionalMethods(uint8_t bMotor)
//{
    /* USER CODE BEGIN FOC_InitAdditionalMethods 0 */

    /* USER CODE END FOC_InitAdditionalMethods 0 */
//}

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
    static int32_t ETestAngle = 0;
    static int32_t count=0;
    #ifdef speedLoopInt    //�ٶȻ���������
    static int pidInter;
    if(pidInter<speedLoopInt){
      pidInter++;
      return;
    }
    pidInter=0;
    #endif
    static int piddelay=0;
    if (FOCVars[bMotor].bDriveInput == INTERNAL)
    {
        #ifdef cTestSVPWM
        static Curr_Components iqdtemp={0};
        iqdtemp.qI_Component1 = 0;
        if(count<(cTestSVPWM*2)){
            count++;
            if(iqdtemp.qI_Component2<cTestSVPWM)
            iqdtemp.qI_Component2++;
        }else{
            //if((ETestAngle>>16)>=HALL_M1._Super.bElToMecRatio){
            //    if(iqdtemp.qI_Component2!=0)
            //        iqdtemp.qI_Component2--;
            //    ETestAngle = (ETestAngle&0xffff0000);
            //}else{
                ETestAngle += TesTAngAdd;
                //iqdtemp.qI_Component2 = cTestSVPWM;
            //}
        }
        HALL_M1.real_phase = (int16_t)ETestAngle;   //ÿ������ 1/65536��
        FOCVars[bMotor].Iqdref = iqdtemp;    //����SVPWM Iq��С
        #else
        Curr_Components iqdtemp;
        if(GetMSpeechEN()){ //�в�������
            FOCVars[bMotor].Iqdref.qI_Component1 = 0;  //ʵ��ת�����Ǹ�q��
            FOCVars[bMotor].Iqdref.qI_Component2  = speechVol;  //����
        }else{
            //�ж��Ƿ񿪻�����
            if((HALL_M1.hallState&0x80)==hall_no_ready){
                //hall ʧЧû���κι���
                FOCVars[bMotor].status = Hall_fail; //hall ����
            }else if(HALL_M1.hallState == hall_null){
                //��ʼѧϰ �ȸ�hallһ����ʼ�Ƕ� �ڸ�һ���̶�vd
                HALL_M1.real_phase = 0;     //��Ƕ��ȶ�λ��?0�� ����������vd ��һ��ֵ ���������ӽǶ�
                FOCVars[bMotor].Iqdref.qI_Component1 = 0;
                if(count<(HallCheckEndVd*2)){
                    count++;
                    if(FOCVars[bMotor].Iqdref.qI_Component2<HallCheckEndVd){
                        FOCVars[bMotor].Iqdref.qI_Component2 += HallCheckAddVd;
                    }
                }else{
                    //vd��ѹ�ﵽ�������ӽǶ�
                    FOCVars[bMotor].Iqdref.qI_Component2 = HallCheckEndVd;
                    HALL_M1.hallFastLearnAngDiff = 0;
                    count = 0;
                    piddelay = 0;
                    ETestAngle = 0;
                    //HALL_M1.real_phase += HallFastStep;   //ÿ������ 1/65536��
                    HALL_M1.hallState = hall_learnStart;    //hall ��ʼѧϰ
                }
            }else if((HALL_M1.hallState >= 1)&&(HALL_M1.hallState < 50)){
                //hall ѧϰ�дﵽ��һ��������ȷ����
                //if((uint8_t)HALL_M1.hallState < 13){
                    piddelay++;
                    if(piddelay>OpenLearnTime){
                        piddelay = 0;
                        if(HALL_M1.hallState>((hallLearnEnd/2))){
                            if(GetLastLearnAngDiff(&HALL_M1)<HALL_M1.hallFastLearnAngDiff){
                            //������ ÿ������1������ѧϰ��Ƕ�?
                                ETestAngle -= HallSlowStep;
                                //HALL_M1.real_phase += HallSlowStep;   //ÿ������ 1/65536��
                            }else{
                                ETestAngle -= HallFastStep;
                                //HALL_M1.real_phase += HallFastStep;   //ÿ������ 1/65536��
                            }
                        }else{
                            if(GetLastLearnAngDiff(&HALL_M1)>HALL_M1.hallFastLearnAngDiff){
                            //������ ÿ������1������ѧϰ��Ƕ�?
                                ETestAngle += HallSlowStep;
                                //HALL_M1.real_phase += HallSlowStep;   //ÿ������ 1/65536��
                            }else{
                                ETestAngle += HallFastStep;
                                //HALL_M1.real_phase += HallFastStep;   //ÿ������ 1/65536��
                            }
                        }
                        if (ETestAngle > 32767)
		                    ETestAngle -= 65536;
	                    else if (ETestAngle < -32768)
		                    ETestAngle += 65536;
                        HALL_M1.real_phase = (int16_t)ETestAngle;
                    }
                //}else{
                //    if(GetLastLearnAngDiff(&HALL_M1)<(-HALL_M1.hallFastLearnAngDiff)){
                        //������ ÿ������1������ѧϰ��Ƕ�?
                //        HALL_M1.real_phase -= HallSlowStep;   //ÿ������ 1/65536��
                //    }else{
                //        HALL_M1.real_phase -= HallFastStep;   //ÿ������ 1/65536��
                //    }
                //}
            }else if(HALL_M1.hallState==hall_learnOver){
                clearRefIdq();  //���iqd ����
            }else{
                //normal mode
                iqdtemp = STC_CalcTorqueReference(pSTC[bMotor]);
                FOCVars[bMotor].Iqdref = iqdtemp;
            }
        }
        //FOCVars[bMotor].hTeref = STC_CalcTorqueReference(pSTC[bMotor]);
        //FOCVars[bMotor].Iqdref.qI_Component1 = FOCVars[bMotor].hTeref;
		#endif
    }
}
/**
 * @brief ���iqdֵ
 * 
 * 
*/
void clearRefIdq(void){
    FOCVars[M1].Iqdref.qI_Component1 = 0;
    FOCVars[M1].Iqdref.qI_Component2 = 0;
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
//uint8_t TSK_HighFrequencyTask(void)
void ADC_IRQHandler(void)
{
    //uint8_t bMotorNbr = 0;
    if ((ADC->ADIFR & ADC_IT_GROUPB0_FINISH)!= RESET)
    {
        /* Clears the ADCx's interrupt pending bits */
        ADC->ADIFR |= ADC_IT_GROUPB0_FINISH;        
        //uint16_t hFOCreturn;
        //HALL_CalcElAngle (&HALL_M1);

        //hFOCreturn = FOC_CurrController(M1);
		FOC_CurrController(M1);

        //if (hFOCreturn == MC_FOC_DURATION)
        //{
        //    STM_FaultProcessing(&STM[M1], MC_FOC_DURATION, 0);
        //}
        //else
        //{
            /* USER CODE BEGIN HighFrequencyTask SINGLEDRIVE_3 */
            /* USER CODE END HighFrequencyTask SINGLEDRIVE_3 */
        //}
    }

    /* USER CODE BEGIN HighFrequencyTask 1 */

    /* USER CODE END HighFrequencyTask 1 */
    //return bMotorNbr;
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
    PWMC_GetPhaseCurrents(pwmcHandle[bMotor], &Iab);    //��ȡ����
    if(HALL_M1.hallState==hall_run){
        //��������ģʽ
        #ifdef duty_ControlMode //duty����ģʽ
	    int16_t duty_set = FOCVars[bMotor].m_duty_cycle_set;	//��ǰ���õ�Ŀ��duty
		int16_t duty_now =  FOCVars[bMotor].now_duty;	//�ϴ������duty��С
		int16_t duty_abs = fabsf(duty_now);
		// Duty cycle control
		if ((fabsf(duty_set) < duty_abs) &&
				((!FOCVars[bMotor].duty_was_pi) || (SIGN_int16(FOCVars[bMotor].duty_pi_duty_last) == SIGN_int16(duty_now)))) {
			// Truncating the duty cycle here would be dangerous, so run a PI controller.
			FOCVars[bMotor].duty_pi_duty_last = duty_now;
			FOCVars[bMotor].duty_was_pi = true;	//�ϴο�����PI�������?
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
			//����v_bus �������� ��ͬ��ѹ�� ��һ�������ο� �õ�ѹһ��
			// Compute parameters
			//float scale = 1.0 / state_now->v_bus;
			int32_t p_term = (error * FOCVars[bMotor].foc_duty_dowmramp_kp<<15) / FOCVars[bMotor].vBus;
            //*dt �ǲ�ͬ�ж�ʱ�� ki��Ӱ�첻�� FOCVars[bMotor].foc_hall.intTime
			// �ȼ�������
            int32_t i_inc = (error * (FOCVars[bMotor].foc_duty_dowmramp_ki << 15)) / FOCVars[bMotor].vBus;

            // ����ȫд������ֹ�ۼӺ����?
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
            //Ŀ���Ǹ�һ������,���´ε�����Сʱ������������������? ��Ҫֱ��������duty�� ���������õ����仯��ƽ��
			FOCVars[bMotor].m_duty_i_term = (FOCVars[bMotor].Iqd.qI_Component1<<15) / current_max_for_duty;   //��ǰiq/max
			//state_now->max_duty = duty_set; //�ͼ���ʱ��D���?��һ��  ת�ٲ��߲�����
			if (duty_set > 0) {
				FOCVars[bMotor].Iqdref.qI_Component1 = current_max_for_duty;	//������õ�Ŀ��duty������ �͸�����������
			} else {
				FOCVars[bMotor].Iqdref.qI_Component1 = -current_max_for_duty;
			}
			FOCVars[bMotor].duty_was_pi = false;
		}
        FOCVars[bMotor].Iqdref.qI_Component2 = 0; // ��ʼ��id_set_tmp
        #endif
        //duty ����ģʽ�� �� �ϴ�   sqrt(vq*vq+vd*vd)*2/sqrt(3)*sign(vq) = duty_now
        //�Ƕ���hall�����?
        if(HALL_M1.angUpdate==true){ //�л������?
            //HALL_M1.real_phase = HALL_M1.m_ang_hall_int_prev;    //��ʵ��λ
            HALL_M1.real_phase = HALL_M1.m_ang_hall_int_Next; //�´���λ
            HALL_M1.LastHallNext = HALL_M1.m_ang_hall_int_prev; //�е�
            HALL_M1.angUpdate = false;   //����������
            HALL_M1.Nowanginc = HALL_M1.anginc;
        }
        //int16_t minDec = HALL_M1.Nowanginc >>3; 
        //if((minDec==0)&&(HALL_M1.Nowanginc!=0)){
        //    if(HALL_M1.Nowanginc>0)
        //    minDec = 1; //��С�ķ���
        //    else
        //    minDec = -1;
        //}
        //int16_t diff = 0;
        //min speed is 1*83us 5s-1 1min - 12erpm
        int ang = (((int)HALL_M1.real_phase)<<4);   //����16����Ӧ��ֵ
        int16_t diff =  HALL_M1.real_phase - HALL_M1.LastHallNext;
        int16_t abs_diff = diff;
        if(abs_diff<0)
            abs_diff = -abs_diff;
        //dir different / abs(diff) <= 30du
        if((DirCMPint16(diff,HALL_M1.Nowanginc))||(abs_diff<5461)||(HALL_M1.Nowanginc==0)){
            //ֱ�Ӳ�ֵ
            ang +=  HALL_M1.Nowanginc;    //��ʵ��λ
            ang = ang>>4;   //��ԭ�Ƕ�
            if(ang>32767)
                ang -= 65536;
            else if(ang<-32768)
                ang += 65536;
            HALL_M1.real_phase = (int16_t)ang;
        }else{
            //�Ƕȳ��� Ҫ���������� ����һ���ٶ� - 1/1000 ��Ϊ�ٶȲ�����
            HALL_M1.erpm = UTILS_LPInt32_FAST(HALL_M1.erpm,0,(int32_t)(0.001*32767)); //��ȡ���ε���ٶ�
            HALL_M1.real_phase -= (diff>>7);   //�ؼ�1%
        }
        #if 0   //����������
        if(HALL_M1.Nowanginc > 0)  // �ٶ����� diff���� next-now>0 now-next<0 -> >0 now over
        {   
            diff = HALL_M1.real_phase-HALL_M1.real_phase_Next;
            //�ǶȲ�ֵ�Ѿ�����Ԥ����¸��Ƕ�? ����ʵ�ʻ�û�д����´νǶ�
            if(diff >= 0)
            {
                // ���ýǶȳ���Ŀ��
                HALL_M1.real_phase = HALL_M1.real_phase_Next;  
                // �ٶ��Զ���С��˥��������ֹ��ǰ̫��
                // �´��Ը�С�����ߣ��ȴ���ʵ��������
                HALL_M1.Nowanginc -= minDec;  
                // ��ֹ�ٶȱ��?0�����ּ�С�䶯  
                if(HALL_M1.Nowanginc < 1){
                    //�Ƕȱ仯̫���� Ҫ�����ٶ�?
                    //�ﵽ�´λ����?�ǲ�����,�Ƕȱ仯������ ��ʱҪ����duty_set
                    //�����ٴ���
                    //duty_set = (duty_set *7) >>3;   //87.5%
                    //if(duty_set < 10) duty_set = 10; // ����޷�?
                    HALL_M1.Nowanginc = 1;
                }
            }
        }else if(HALL_M1.Nowanginc < 0){
            //now-next>0 -> <0 keep
            diff = HALL_M1.real_phase-HALL_M1.real_phase_Next;
            if(diff <= 0)
            {
                HALL_M1.real_phase = HALL_M1.real_phase_Next;
                // �����ٶ�˥��
                HALL_M1.Nowanginc -= minDec;
                if(HALL_M1.Nowanginc > -1){
                    //duty_set = (duty_set *7) >>3;   //87.5%
                    //if(duty_set > -10) duty_set = -10;
                    HALL_M1.Nowanginc = -1;
                }
            }
        }else{
            //û�нǶ����� ��ֹͣ�� ��ʱdutyҪ = 0;
            //duty_set = 0; ʱ��̫�� �ٶ�̫�� ����ֵ
        }
        #endif
    }   //����ģʽ�Ƕ����ⲿ�ṩ
    hElAngle = HALL_M1.real_phase;    //hall ���������ʵ�Ƕ�?
    //����ʱ���ٶȻ�ֱ��vd vq����  ����ʱ�����л��� �������Ʊջ�
    //hall ѧϰУ׼ʱ���õ����� �ܵ���ʱ���õ����� �޸�����ʱ��Ҳ���õ�����
    int16_t speed = SPD_GetAvrgMecSpeed01Hz(pSTC[bMotor]->SPD);
    if(GetMSpeechEN()){
          static uint8_t phase = 0;
          //static int16_t sine_table[4] = {0, 707, 1000, 707};  // ���Ҳ�����
          HALL_M1.feed_v = 0;
          phase = (phase + 1) & 0x0f;  // 0-3ѭ��
          // ע����ת�ĵ�ѹʸ������ƽ��ת��Ϊ0
          int16_t amplitude = FOCVars[bMotor].Iqdref.qI_Component2;
          // ��d-q����ϵ�л�Բ
          switch((phase>>1)){
                case 0:  // 0��
                    Vqd.qV_Component1 = 0;
                    Vqd.qV_Component2 = amplitude;
                    break;
                case 1: //45��
                    Vqd.qV_Component1 = amplitude>>1;
                    Vqd.qV_Component2 = amplitude>>1;
                    break;
                case 2:  // 90��
                    Vqd.qV_Component1 = amplitude;
                    Vqd.qV_Component2 = 0;
                    break;
                case 3:
                    Vqd.qV_Component1 = (amplitude>>1);
                    Vqd.qV_Component2 = -(amplitude>>1);
                    break;
                case 4:  // 180��
                    Vqd.qV_Component1 = 0;
                    Vqd.qV_Component2 = -amplitude;
                    break;
                case 5:
                    Vqd.qV_Component1 = -(amplitude>>1);
                    Vqd.qV_Component2 = -(amplitude>>1);
                    break;
                case 6:  // 270��
                    Vqd.qV_Component1 = -amplitude;
                    Vqd.qV_Component2 = 0;
                    break;
                case 7:
                    Vqd.qV_Component1 = -(amplitude>>1);
                    Vqd.qV_Component2 = (amplitude>>1);
                    break;
          }
    }
    #if 1
    #ifndef cTestSVPWM
    else if((HALL_M1.hallState==hall_run)){  //
        if(HALL_M1.I_feed == false){
            //���������?
            if((speed>IloopTrigH)||(speed<-IloopTrigH)){  //650 0x400 //0x300 //0x500
              if(IScount>ISDelayT){
                HALL_M1.I_feed = true;
              }else{
                IScount++;
              }
            }else{
                if(HALL_M1.feed_v>0){
                    HALL_M1.feed_v -= 2;
                    if(HALL_M1.feed_v<0)
                        HALL_M1.feed_v = 0;
                }
                if(IScount!=0)
                    IScount--;
            }
        }else{
            //����������
            if((speed<IloopTrigL)&&(speed>-IloopTrigL)){  //0x500
              //ENC_KTH7823_M1.I_feed = false;
              if(IScount==0){
                HALL_M1.I_feed = false;
              }else{
                IScount--;
              }
            }else{
                if(HALL_M1.feed_v<16384){
                    HALL_M1.feed_v += 1;
                }
              if(IScount<ISDelayT)
                IScount++;
            }
        }
        //������ʱ��hallһ��У׼����
        //hElAngle = SPD_GetElAngle(STC_GetSpeedSensor(pSTC[bMotor]));
        Ialphabeta = MCM_Clarke(Iab);
        Iqd = MCM_Park(Ialphabeta, hElAngle);   
        //Iqd.qI_Component1 = -Iqd.qI_Component1;
        //Iqd.qI_Component2 = -Iqd.qI_Component2;
        Vqd.qV_Component1 = PI_Controller(pPIDIq[bMotor],
                                          (int32_t)(FOCVars[bMotor].Iqdref.qI_Component1) - Iqd.qI_Component1);
        Vqd.qV_Component2 = PI_Controller(pPIDId[bMotor],
                                          (int32_t)(FOCVars[bMotor].Iqdref.qI_Component2) - Iqd.qI_Component2);
        Vqd.qV_Component1 = (((Vqd.qV_Component1* HALL_M1.feed_v)>>14) + ((FOCVars[bMotor].Iqdref.qI_Component1*(16384-HALL_M1.feed_v))>>14));
        Vqd.qV_Component2 = (((Vqd.qV_Component2* HALL_M1.feed_v)>>14) + ((FOCVars[bMotor].Iqdref.qI_Component2*(16384-HALL_M1.feed_v))>>14));
        #ifdef decodedqEm            //         ����             
        //=====================================================================
        //  VESC ��׼ d/q ������MXlemming �������űؼӣ��Ƕȸ��ȣ�
        //=====================================================================
        #define ELEC_SPEED   hElSpeed         // ��ĵ���ٶ� Q15 �� int32
        #define FLUX        pMC[bMotor]->Flux // ����
        #define Lq_Ld       (pMC[bMotor]->Lq - pMC[bMotor]->Ld)

        Vqd.qV_Component1 += (int32_t)ELEC_SPEED * FLUX / 32768;       // ���綯��ǰ��
        Vqd.qV_Component2 -= (int32_t)ELEC_SPEED * Lq_Ld * Iqd.qI_Component1 / 32768; // ����
        //FOCVars[bMotor].Vqd = Vqd;
        //Vqd = Circle_Limitation(pCLM[bMotor], Vqd);
        #endif
    }
    #endif
    #endif
    else{
        HALL_M1.feed_v = 0; //����������
        Vqd.qV_Component1 = FOCVars[bMotor].Iqdref.qI_Component1;
        Vqd.qV_Component2 = FOCVars[bMotor].Iqdref.qI_Component2;
    }
    //=====================================================================
    //  VESC ��������? �� ������ ƽ�����������ƣ�������Ĵ���?
    //=====================================================================
    //#ifndef  cTestSVPWM
    Volt_Components Vqd_circle;
    int32_t mag_sq;
    int32_t circle_max_sq;
    int32_t hex_max_sq;
    int32_t diff;
    int32_t ratio;
    int32_t one_minus_ratio;

    // 1. ������ԭ����Բ������
    Vqd_circle = Circle_Limitation(pCLM[bMotor], Vqd);

    // 2. �����ѹģ��ƽ�������ÿ�������?
    mag_sq = (int32_t)Vqd.qV_Component1 * Vqd.qV_Component1
           + (int32_t)Vqd.qV_Component2 * Vqd.qV_Component2;

    // 3. ������Բ������ֵƽ��
    circle_max_sq = (int32_t)pCLM[bMotor]->MaxModule * pCLM[bMotor]->MaxModule;

    // 4. ����������ֵƽ������Բ�� 15% ���ң�VESC��׼��
    //hex_max_sq = (circle_max_sq * 130) / 100;  // �Ŵ�1.3�����ȫ
    hex_max_sq = ((circle_max_sq * 334) >> 8);  // �Ŵ�1.3�����ȫ

    Volt_Components Vqd_final;

    if (mag_sq <= circle_max_sq)
    {
        // ��ȫ�������� �� ֻ��Բ������
        Vqd_final = Vqd_circle;
    }
    else if (mag_sq >= hex_max_sq)
    {
        // ��ȫ���������? �� ��������
        Vqd_final = Vqd;
    }
    else
    {
        // ==============================
        // VESC ���ģ�ƽ�����ɣ�ȫ���㣩
        // ==============================
        diff = mag_sq - circle_max_sq;
        //ratio = (diff * 32767) / (hex_max_sq - circle_max_sq); // 0~32767
        ratio = (diff << 15) / (hex_max_sq - circle_max_sq); // 0~32767
        //one_minus_ratio = 32767 - ratio;
        one_minus_ratio = 32768 - ratio;

        // ƽ����ϣ�Բ������? + ԭʼ��ѹ
        //Vqd_final.qV_Component1 = (Vqd_circle.qV_Component1 * one_minus_ratio
        //                         + Vqd.qV_Component1 * ratio) / 32767;
        Vqd_final.qV_Component1 = (Vqd_circle.qV_Component1 * one_minus_ratio
                                 + Vqd.qV_Component1 * ratio) >> 15;

        //Vqd_final.qV_Component2 = (Vqd_circle.qV_Component2 * one_minus_ratio
        //                         + Vqd.qV_Component2 * ratio) / 32767;
        Vqd_final.qV_Component2 = (Vqd_circle.qV_Component2 * one_minus_ratio
                                 + Vqd.qV_Component2 * ratio) >>15;
    }

    // �������?
    Vqd = Vqd_final;
    //#endif

    Valphabeta = MCM_Rev_Park(Vqd, hElAngle);
    hCodeError = PWMC_SetPhaseVoltage(pwmcHandle[bMotor], Valphabeta);

    //int32_t vq2 = (int32_t)Vqd.qV_Component1*Vqd.qV_Component1;
    //int32_t vd2 = (int32_t)Vqd.qV_Component2*Vqd.qV_Component2;
    ////motor_now->m_motor_state.duty_now = ((vq2+vd2)>>16);    //��ǰ�����duty��С
    //if(Vqd.qV_Component1<0)
    //    FOCVars[bMotor].now_duty = -((vq2+vd2)>>16);
    //else
    //    FOCVars[bMotor].now_duty = ((vq2+vd2)>>16);     //max is 0x2fff
    FOCVars[bMotor].Vqd = Vqd;
    FOCVars[bMotor].Iab = Iab;
    FOCVars[bMotor].Ialphabeta = Ialphabeta;
    FOCVars[bMotor].Iqd = Iqd;
    FOCVars[bMotor].Valphabeta = Valphabeta;
    FOCVars[bMotor].hElAngle = hElAngle;
    return (hCodeError);
}

/**
 * @brief printf Motor control int data
 * 
 * 
 * 
 */
void printfMC(uint8_t mode){
    #if 0
    uint8_t dataUart[50];
    #endif
    int16_t a = FOCVars[0].Iab.qI_Component1;
    int16_t b = FOCVars[0].Iab.qI_Component2;
    int16_t c = -a-b;
    uint16_t vol = get_input_voltageX10(NowVBusAD);
	switch(mode){
		case 0:
            printf("motor ok\n");
			//dataUart[0] = 'm';
			//dataUart[1] = 'o';
			//dataUart[2] = 't';
			//dataUart[3] = 'o';
			//dataUart[4] = 'r';
			//dataUart[5] = ' ';
			//dataUart[6] = 'o';
			//dataUart[7] = 'k';
			//dataUart[8] = '\n';
			//UartSendDatas(dataUart,9);
			break;
		case 1:
            #if 1
            printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%u,%d\n",FOCVars[0].hElAngle,FOCVars[0].Iqdref.qI_Component1,\
            FOCVars[0].Iqdref.qI_Component2,FOCVars[0].Valphabeta.qV_Component1,FOCVars[0].Valphabeta.qV_Component2,\
            HALL_M1.erpm,a,b,c,HALL_M1.hall_val,vol,oMCInterface[0]->hFinalSpeed);
			#else
			dataUart[0] = (FOCVars[0].hElAngle&0xff);
			dataUart[1] = (FOCVars[0].hElAngle>>8)&0xff;
            dataUart[2] = ',';
			dataUart[3] = (FOCVars[0].Iqdref.qI_Component1&0xff);
			dataUart[4] = (FOCVars[0].Iqdref.qI_Component1>>8)&0xff;
            dataUart[5] = ',';
			dataUart[6] = (FOCVars[0].Iqdref.qI_Component2&0xff);
			dataUart[7] = (FOCVars[0].Iqdref.qI_Component2>>8)&0xff;
            dataUart[8] = ',';
			dataUart[9] = (FOCVars[0].Valphabeta.qV_Component1&0xff);
			dataUart[10] = (FOCVars[0].Valphabeta.qV_Component1>>8)&0xff;
            dataUart[11] = ',';
			dataUart[12] = (FOCVars[0].Valphabeta.qV_Component2&0xff);
			dataUart[13] = (FOCVars[0].Valphabeta.qV_Component2>>8)&0xff;
            dataUart[14] = ',';
			dataUart[15] = HALL_M1.erpm&0xff;
			dataUart[16] = (HALL_M1.erpm>>8)&0xff;
			dataUart[17] = (HALL_M1.erpm>>16)&0xff;
			dataUart[18] = (HALL_M1.erpm>>24)&0xff;
            dataUart[19] = ',';
			dataUart[20] = (a&0xff);
			dataUart[21] = (a>>8)&0xff;
            dataUart[22] = ',';
			dataUart[23] = (b&0xff);
			dataUart[24] = (b>>8)&0xff;
            dataUart[25] = ',';
			dataUart[26] = (c&0xff);
			dataUart[27] = (c>>8)&0xff;
            dataUart[28] = ',';
			dataUart[29] = (HALL_M1.hall_val);
			dataUart[30] = (vol)&0xff;
			dataUart[31] = (vol>>8)&0xff;
            dataUart[32] = ',';
			dataUart[33] = (oMCInterface[0]->hFinalSpeed)&0xff;
			dataUart[34] = (oMCInterface[0]->hFinalSpeed>>8)&0xff;
			UartSendDatas(dataUart,35);
            #endif
			break;
	}
}
/**
  * @brief  Retargets the C library printf function to the UART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
    uint32_t Timeout = 0;
    FlagStatus Status;

    UART_SendData(UART2, (uint8_t) ch);

    do
    {
        Status = UART_GetFlagStatus(UART2, UART_FLAG_TXE);
        Timeout++;
    } while ((Status == RESET) && (Timeout != 0xFFFFFF));

    return (ch);
}
/**
  * @brief  This function handles SysTick exception,Run MotorControl Tasks.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    Inc1msTick();
    MC_RunMotorControlTasks();
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
  * 500us ִ��һ�ΰ�ȫ���񣬼���ѹǷѹ��
  */
void TSK_SafetyTask_PWMOFF(uint8_t bMotor)
{
    uint16_t vBusAD;   //ĸ�ߵ�ѹ*100
    uint16_t vTempMotorx100;   //�����¶�*100
    uint16_t vTempPCBx100;   //PCB�¶�*100
    static int initTime = 0;
    static int countVolUnder = 0;   //������ѹǷѹ�Ĵ���
    static int countVolOver = 0;   //������ѹǷѹ�Ĵ���
	#ifdef MotorTempEn
    static int countTempMotorOver = 0;   //������ѹǷѹ�Ĵ���
	#endif
	#ifdef MosTempEn
    static int countTempMosOver = 0;   //������ѹǷѹ�Ĵ���
	#endif
    //uint16_t CodeReturn = MC_NO_ERROR;
    //uint16_t errMask[NBR_OF_MOTORS] = {VBUS_TEMP_ERR_MASK};
    vBusAD = GetVBusAD(); //GET_INPUT_VOLTAGE();     //��ȡVDD ��ADֵ
    vTempMotorx100 = NTC_TEMP_MOTOR(TempBeta);
    vTempPCBx100 = NTC_TEMP_PCB(TempBeta);
    if(FOCVars[bMotor].status == not_ready){
        //���׼����?
        if(initTime==0){
            NowVBusAD = vBusAD;   //���µ�ǰ��ѹֵ
            NowTempMotorx100 = vTempMotorx100;
            NowTempPCBx100 = vTempPCBx100;
        }else{
            vBusAD = GetVBusAD(); //GET_INPUT_VOLTAGE();     //��ȡVDD ��ADֵ
            UTILS_LPInt16_FAST(NowVBusAD, vBusAD, VBusFilterConstant);   //���µ�ǰ��ѹֵ 
            UTILS_LPInt16_FAST(NowTempMotorx100, vTempMotorx100, VTempMotorFilterConstant);   //����Motor�¶�
            UTILS_LPInt16_FAST(NowTempPCBx100, vTempPCBx100, VTempPCBFilterConstant);   //����PCB�¶�
        }
        initTime++;
        if(initTime>200){
            if(NowVBusAD < vMinBus){   //
                //�������? ������
                FOCVars[bMotor].status = mc_under_voltage;   //Ƿѹ��
            }else if(NowVBusAD > vMaxBus){
                FOCVars[bMotor].status = mc_over_voltage;   //��ѹ��
            }else{
                Maxspeed = get_MaxSpeed(NowVBusAD,mcconf.mc_KV)+baseSpeed;
                //Maxspeed = GET_INPUT_VOLTAGE(vBusAD)*FOCVars[bMotor].mc_KV/100;   //������ת��
                //��ʼ����ѹ �����ϵ��ѹ�������?��
                FOCVars[bMotor].status = ready_RUN;   //׼������ ����������
            }
        }
        return; //��������û�п�ʼ���� �Ͳ���鰲�?��
    }else{
        //vBusAD = GetVBusAD(); //GET_INPUT_VOLTAGE();     //��ȡVDD ��ADֵ
        UTILS_LPInt16_FAST(NowVBusAD, vBusAD, VBusFilterConstant);   //���µ�ǰ��ѹֵ 
        UTILS_LPInt16_FAST(NowTempMotorx100, vTempMotorx100, VTempMotorFilterConstant);   //����Motor�¶�
        UTILS_LPInt16_FAST(NowTempPCBx100, vTempPCBx100, VTempPCBFilterConstant);   //����PCB�¶�
		if((FOCVars[bMotor].status != ready_RUN)&&(FOCVars[bMotor].status != motor_run))
            return;     //ûӐս³£ԋА
        if(NowVBusAD < vMinBus){   //
            //�������? ������
            countVolUnder++;
            if(countVolUnder > 2000){    //1s
                FOCVars[bMotor].status = mc_under_voltage;   //Ƿѹ��
            }
        }else
            countVolUnder = 0;
        if(NowVBusAD > vMaxBus){
            countVolOver++;
            if(countVolOver > 2000){    //1s
                FOCVars[bMotor].status = mc_over_voltage;   //��ѹ��
            }
        }else   
            countVolOver = 0;
        #ifdef MotorTempEn
        if(NowTempMotorx100>vMaxMotorTemp){   //����¶ȳ���?
            countTempMotorOver++;
            if(countTempMotorOver>6000){
                //3s
                FOCVars[bMotor].status = mc_over_MotorTemp;   //motor ����
            }
        }else
            countTempMotorOver = 0;
        #endif
        #ifdef MosTempEn
        if(NowTempPCBx100>vMaxPCBTemp){   //MOS �¶ȳ���
            countTempMosOver++;
            if(countTempMosOver>6000){
                //3s
                FOCVars[bMotor].status = mc_over_MosTemp;   //mos ����
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
 * @brief get max speed
 * 
 */
int32_t GetMaxSpeed(void){
    return Maxspeed;
}

/**
 * @brief ��flash�ж�ȡ APPConfig �����������Ƿ���ȷ
 * 
 * 
 */
void GetAPPConfig(void){
  EE_ReadConfig((uint32_t)ADDR_FLASH_EEPROM_APPCONF,&appconf,sizeof(app_config_t));
  uint16_t crc = crc16((uint8_t* )&appconf,sizeof(app_config_t)-2);
  if(appconf.CRC_Data!=crc){
    DefaultAPPConfig(&appconf);      //��Ĭ���������� ��ȡ����ʧ�� Ҫ����ѧϰ
    //HALL_M1.hallState = hall_null;  //hall ������ Ҫ����ѧϰ
  }else{ 
    //HALL_M1.hallState = hall_run;  //��ȡ���ݳɹ� hall����ֱ������
  }
}
/**
 * @brief ��flash�ж�ȡ MCConfig �����������Ƿ���ȷ
 * 
 * 
 */
void GetMCConfig(void){
  EE_ReadConfig((uint32_t)ADDR_FLASH_EEPROM_MCCONF,&mcconf,sizeof(mc_config_t));
  uint16_t tmp = mcconf.CRC_Data;
  mcconf.CRC_Data = 0;
  uint16_t crc = crc16
	((uint8_t* )&mcconf,sizeof(mc_config_t));
  if((tmp!=crc)||(GetDir()==0)){
    DefaultMCConfig(&mcconf);      //��Ĭ���������� ��ȡ����ʧ�� Ҫ����ѧϰ
    HALL_M1.hallState = hall_null;  //hall ������ Ҫ����ѧϰ
  }else{ 
    mcconf.CRC_Data = tmp;
    HALL_M1.hallState = hall_run;  //��ȡ���ݳɹ� hall����ֱ������
  }
}
/**
 * @brief set Motor config
 * 
 */
void SetMCConfig(void){
    mcconf.CRC_Data = 0;
    uint16_t crc = crc16((uint8_t* )&mcconf,sizeof(mc_config_t));
    mcconf.CRC_Data = crc;
    EE_WriteConfig((uint32_t)ADDR_FLASH_EEPROM_MCCONF,&mcconf,sizeof(mc_config_t));
	//EE_ReadConfig((uint32_t)ADDR_FLASH_EEPROM_MCCONF,&mcconf,sizeof(mc_config_t));
	//crc = crc16((uint8_t* )&mcconf,sizeof(mc_config_t)-2);
	//if(mcconf.CRC_Data == crc){
	//}
}
/**
 * @brief set App config
 * 
 */
void SetAPPConfig(void){
    uint16_t crc = crc16((uint8_t* )&appconf,sizeof(app_config_t)-2);
    appconf.CRC_Data = crc;
    EE_WriteConfig((uint32_t)ADDR_FLASH_EEPROM_APPCONF,&appconf,sizeof(app_config_t));
}

/**
 * @brief  get app mode
 * 
 * 
 * 
 */
app_mode_t GetAPPMode(void){
    return appconf.app_mode;
}

uint8_t aRxBuffer[64];
uint8_t RxCounter = 0;  //���ռ���
uint8_t NewDataBuffLen = 0; //�����ݵĳ���
uint8_t TempDataBuffLen = 0; //�����ݵĳ���
uint8_t NewDataStart = 0;   //�����ݿ�ʼλ��
uint8_t LastDataStart = 0;  //�ϴ����ݿ�ʼ��λ��
uint8_t IDLEflag = 0;   //�Ƿ��������ж�
/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void UART2_IRQHandler(void)
{
    // ===================== ���ؼ���������д���������ļĴ����� =====================
    if(UART2->ISR & (UART_ISR_ORE | UART_ISR_FE | UART_ISR_NF | UART_ISR_PE))
    {
        // �� ICR ����������MCUΨһ��ȷ��ʽ��
        UART2->ICR = UART_ICR_ORECF | UART_ICR_FECF | UART_ICR_NCF | UART_ICR_PECF;
        return;
    }
    if (UART_GetITStatus(UART2, UART_IT_RXNE) == SET)
    {
        // 1. ������ �� �Զ��� RXNE������Ҫ�ֶ��壡
        aRxBuffer[RxCounter++] = UART_ReceiveData(UART2);
        if(RxCounter >= 64) RxCounter = 0;

        // 2. ���㵱ǰ֡�ѽ��ճ��ȣ����λ�����ȷ�㷨��
        uint16_t curr_len;
        if(RxCounter >= NewDataStart)
            curr_len = RxCounter - NewDataStart;
        else
            curr_len = 64 + RxCounter - NewDataStart;

        // 3. �յ���1���ֽ� �� ��¼����
        if(curr_len == 1)
        {
            uint8_t len = aRxBuffer[NewDataStart];
            if(len > 0 && len < 32)
                TempDataBuffLen = len;
            else
                TempDataBuffLen = 0;
        }

        // 4. �չ����� �� ֡���?
        if(TempDataBuffLen > 0 && curr_len >= TempDataBuffLen)
        {
            NewDataBuffLen = TempDataBuffLen;
            LastDataStart = NewDataStart;
            NewDataStart = RxCounter;
            TempDataBuffLen = 0;
        }
    }
}
/**
 * @brief Get Uart RX Data
 * 
 * 
 */
void ScanUartRX(void){
    if(NewDataBuffLen>0){
        //�������ݽ��� ����������uartRX ����
        NewDataBuffLen = 0;
    }
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

