/**
  ******************************************************************************
  * @file    mc_config.c
  * @version V1.0.0
  * @date    2021-07-30
  ******************************************************************************
  */

#include "main.h"
#include "mc_type.h"
#include "parameters_conversion.h"
#include "mc_parameters.h"
#include "mc_config.h"
#include "hw_correct.h"

#define MAX_TWAIT 0                 /* Dummy value for single drive */
#define FREQ_RATIO 1                /* Dummy value for single drive */
#define FREQ_RELATION HIGHEST_FREQ  /* Dummy value for single drive */

#define OFFCALIBRWAIT_MS     0
#define OFFCALIBRWAIT_MS2    0

extern int16_t foc_hall_ang_table[8];


//mc_config_t mcconf;     //初始化 马达配置
//app_config_t appconf;   //app config
//#include "pqd_motor_power_measurement.h"

//PQD_MotorPowMeas_Handle_t PQD_MotorPowMeasM1 =
//{
//    .wConvFact = PQD_CONVERSION_FACTOR
//};
//PQD_MotorPowMeas_Handle_t *pPQD_MotorPowMeasM1 = &PQD_MotorPowMeasM1;

/**
  * @brief  PI / PID Speed loop parameters Motor 1
  */
PID_Handle_t PIDSpeedHandle_M1 =
{
    .hDefKpGain          = (int16_t)PID_SPEED_KP_DEFAULT,
    .hDefKiGain          = (int16_t)PID_SPEED_KI_DEFAULT,
    .hDefKdGain          = (int16_t)PID_SPEED_KD_DEFAULT,
    .wUpperIntegralLimit = (int32_t)IQMAX * (int32_t)SP_KIDIV,
    .wLowerIntegralLimit = -(int32_t)IQMAX * (int32_t)SP_KIDIV,
    .hUpperOutputLimit   = (int16_t)IQMAX,
    .hLowerOutputLimit   = -(int16_t)IQMAX,
    .hKpDivisor          = (uint16_t)SP_KPDIV,
    .hKiDivisor          = (uint16_t)SP_KIDIV,
    .hKpDivisorPOW2      = (uint16_t)SP_KPDIV_LOG,
    .hKiDivisorPOW2      = (uint16_t)SP_KIDIV_LOG,
    .hKdDivisor          = (uint16_t)SP_KDDIV_LOG,
    .hKdDivisorPOW2      = (uint16_t)SP_KDDIV_LOG,
    .hCompensation      = 0,
};

/**
  * @brief  PI / PID Iq loop parameters Motor 1
  */
PID_Handle_t PIDIqHandle_M1 =
{
    .hDefKpGain          = (int16_t)PID_TORQUE_KP_DEFAULT,
    .hDefKiGain          = (int16_t)PID_TORQUE_KI_DEFAULT,
    .wUpperIntegralLimit = (int32_t)INT16_MAX * TF_KIDIV,
    .wLowerIntegralLimit = (int32_t) - INT16_MAX * TF_KIDIV,
    .hUpperOutputLimit   = INT16_MAX,
    .hLowerOutputLimit   = -INT16_MAX,
    .hKpDivisor          = (uint16_t)TF_KPDIV,
    .hKiDivisor          = (uint16_t)TF_KIDIV,
    .hKpDivisorPOW2      = (uint16_t)TF_KPDIV_LOG,
    .hKiDivisorPOW2      = (uint16_t)TF_KIDIV_LOG,
    .hDefKdGain          = 0x0000U,
    .hKdDivisor          = 0x0000U,
    .hKdDivisorPOW2      = 0x0000U,
    .hCompensation      = 0,
};

/**
  * @brief  PI / PID Id loop parameters Motor 1
  */
PID_Handle_t PIDIdHandle_M1 =
{
    .hDefKpGain          = (int16_t)PID_FLUX_KP_DEFAULT,
    .hDefKiGain          = (int16_t)PID_FLUX_KI_DEFAULT,
    .wUpperIntegralLimit = (int32_t)INT16_MAX * TF_KIDIV,
    .wLowerIntegralLimit = (int32_t) - INT16_MAX * TF_KIDIV,
    .hUpperOutputLimit   = INT16_MAX,
    .hLowerOutputLimit   = -INT16_MAX,
    .hKpDivisor          = (uint16_t)TF_KPDIV,
    .hKiDivisor          = (uint16_t)TF_KIDIV,
    .hKpDivisorPOW2      = (uint16_t)TF_KPDIV_LOG,
    .hKiDivisorPOW2      = (uint16_t)TF_KIDIV_LOG,
    .hDefKdGain          = 0x0000U,
    .hKdDivisor          = 0x0000U,
    .hKdDivisorPOW2      = 0x0000U,
    .hCompensation      = 0,
};

/**
  * @brief  SpeednTorque Controller parameters Motor 1
  */
SpeednTorqCtrl_Handle_t SpeednTorqCtrlM1 =
{
    .STCFrequencyHz             =  MEDIUM_FREQUENCY_TASK_RATE,
    .MaxAppPositiveMecSpeed01Hz = (uint16_t)(MAX_APPLICATION_SPEED / 6),
    .MinAppPositiveMecSpeed01Hz = (uint16_t)(MIN_APPLICATION_SPEED / 6),
    .MaxAppNegativeMecSpeed01Hz = (int16_t)(-MIN_APPLICATION_SPEED / 6),
    .MinAppNegativeMecSpeed01Hz = (int16_t)(-MAX_APPLICATION_SPEED / 6),
    .MaxPositiveTorque          = (int16_t)NOMINAL_CURRENT,
    .MinNegativeTorque          = -(int16_t)NOMINAL_CURRENT,
    .ModeDefault                = DEFAULT_CONTROL_MODE,
    .MecSpeedRef01HzDefault     = (int16_t)(DEFAULT_TARGET_SPEED_RPM / 6),
    .TorqueRefDefault           = (int16_t)DEFAULT_TORQUE_COMPONENT,
    .IdrefDefault               = (int16_t)DEFAULT_FLUX_COMPONENT,
};

PWMC_R3_F0_Handle_t PWM_Handle_M1 =
{
    {
        .pFctGetPhaseCurrents              = &R3F0XX_GetPhaseCurrents,
        .pFctSwitchOffPwm                  = &R3F0XX_SwitchOffPWM,
        .pFctSwitchOnPwm                   = &R3F0XX_SwitchOnPWM,
        .pFctCurrReadingCalib              = &R3F0XX_CurrentReadingCalibration,
        .pFctTurnOnLowSides                = &R3F0XX_TurnOnLowSides,
        .pFctSetADCSampPointSect1          = &R3F0XX_SetADCSampPointSect1,
        .pFctSetADCSampPointSect2          = &R3F0XX_SetADCSampPointSect2,
        .pFctSetADCSampPointSect3          = &R3F0XX_SetADCSampPointSect3,
        .pFctSetADCSampPointSect4          = &R3F0XX_SetADCSampPointSect4,
        .pFctSetADCSampPointSect5          = &R3F0XX_SetADCSampPointSect5,
        .pFctSetADCSampPointSect6          = &R3F0XX_SetADCSampPointSect6,
        .pFctIsOverCurrentOccurred         = &R3F0XX_IsOverCurrentOccurred,
        .pFctOCPSetReferenceVoltage        = MC_NULL,
        .pFctRLDetectionModeEnable         = MC_NULL,
        .pFctRLDetectionModeDisable        = MC_NULL,
        .pFctRLDetectionModeSetDuty        = MC_NULL,
        .hT_Sqrt3 = (PWM_PERIOD_CYCLES * SQRT3FACTOR) / 16384u,
        .hSector = 0,
        .hCntPhA = 0,
        .hCntPhB = 0,
        .hCntPhC = 0,
        .SWerror = 0,
        .bTurnOnLowSidesAction = false,
        .hOffCalibrWaitTimeCounter = 0,
        .bMotor = 0,
        .RLDetectionMode = false,
        .hIa = 0,
        .hIb = 0,
        .hIc = 0,
        .DTTest = 0,
        .DTCompCnt = 0,
        .hPWMperiod = PWM_PERIOD_CYCLES,
        .hOffCalibrWaitTicks = (uint16_t)((SYS_TICK_FREQUENCY * OFFCALIBRWAIT_MS) / 1000),
        .hDTCompCnt          = DTCOMPCNT,
        .Ton                 = TON,
        .Toff                = TOFF
    },
    .wPhaseAOffset = 0,
    .wPhaseBOffset = 0,
    .wPhaseCOffset = 0,
    .Half_PWMPeriod = PWM_PERIOD_CYCLES / 2u,
    .hRegConv = 0,
    .OverCurrentFlag = false,
    .OverVoltageFlag = false,
    .BrakeActionLock = false,
    .bIndex = 0,
    .hFlags = 0,
    .ADC1_DMA_converted[0] = 0,
    .ADC1_DMA_converted[1] = 0,
    .ADC1_DMA_converted[2] = 0,
    .bCalib_A_index = 0,
    .bCalib_B_index = 0,
    .bCalib_C_index = 0,
    .pParams_str = &R3_F0XX_Params
};

/**
  * @brief  SpeedNPosition sensor parameters Motor 1 - Base Class
  */
foc_hall_t HALL_M1 =
{
    ._Super = {
        .bElToMecRatio                     =    POLE_PAIR_NUM,
        //.hMaxReliableMecSpeed01Hz          =    (uint16_t)(1.15 * MAX_APPLICATION_SPEED / 6),
        //.hMinReliableMecSpeed01Hz          =    (uint16_t)(MIN_APPLICATION_SPEED / 6),
        //.bMaximumSpeedErrorsNumber         =    MEAS_ERRORS_BEFORE_FAULTS,
        //.hMaxReliableMecAccel01HzP         =    65535,
        //.hMeasurementFrequency             =    TF_REGULATION_RATE,
    },
    .hallState = hall_null,
    .intTime = PWM_PERIOD_CYCLES,
    .last_ang_diff = 0,
    .I_feed = false,      //电流环跳过
    .anginc = 0,
    .angUpdate = false,
    .foc_hall_table = foc_hall_ang_table,
    //.hElAngle = 0,
    .real_phase = 0,
};

/**
  * temperature sensor parameters Motor 1
  */
//NTC_Handle_t TempSensorParamsM1 =
//{
//    .bSensorType = REAL_SENSOR,
//    .hLowPassFilterBW        = M1_TEMP_SW_FILTER_BW_FACTOR,
//    .hOverTempThreshold      = (uint16_t)(OV_TEMPERATURE_THRESHOLD_d),
//    .hOverTempDeactThreshold = (uint16_t)(OV_TEMPERATURE_THRESHOLD_d - OV_TEMPERATURE_HYSTERESIS_d),
//    .hSensitivity            = (uint16_t)(ADC_REFERENCE_VOLTAGE / dV_dT),
//    .wV0                     = (uint16_t)(V0_V * 65536 / ADC_REFERENCE_VOLTAGE),
//    .hT0                     = T0_C,
//};

/* Bus voltage sensor value filter buffer */
//uint16_t RealBusVoltageSensorFilterBufferM1[M1_VBUS_SW_FILTER_BW_FACTOR];
//uint16_t RealIBusFilterBufferM1[M1_VBUS_SW_FILTER_BW_FACTOR];
/**
  * Bus voltage sensor parameters Motor 1
  */
//RDivider_Handle_t RealBusVoltageSensorParamsM1 =
//{
//    ._Super                =
//    {
//        .SensorType        = REAL_SENSOR,
//        .ConversionFactor  = (uint16_t)(ADC_REFERENCE_VOLTAGE / BUS_ADC_CONV_RATIO),
//    },
//
//    .LowPassFilterBW       =  M1_VBUS_SW_FILTER_BW_FACTOR,
//    .OverVoltageThreshold  = OVERVOLTAGE_THRESHOLD_d,
//    .UnderVoltageThreshold =  UNDERVOLTAGE_THRESHOLD_d,
//    .aBuffer = RealBusVoltageSensorFilterBufferM1,
//    .bBuffer = RealIBusFilterBufferM1,
//};


/** RAMP for Motor1.
  *
  */
RampExtMngr_Handle_t RampExtMngrHFParamsM1 =
{
    .FrequencyHz = TF_REGULATION_RATE
};

/**
  * @brief  CircleLimitation Component parameters Motor 1 - Base Component
  */
CircleLimitation_Handle_t CircleLimitationM1 =
{
    .MaxModule          = MAX_MODULE,
    .Circle_limit_table = MMITABLE,
    .Start_index        = START_INDEX,
};


/**
 * @brief default motor config
 * 
 * 
 */
void DefaultMCConfig(mc_config_t *mcconf){
    mcconf->speed_Kp = PID_SPEED_KP_DEFAULT;
    mcconf->speed_Ki = PID_SPEED_KI_DEFAULT;
    mcconf->speed_Kd = PID_SPEED_KD_DEFAULT;
    mcconf->current_Kp = PID_TORQUE_KP_DEFAULT;
    mcconf->current_Ki = PID_TORQUE_KI_DEFAULT;
    mcconf->IntTime = PWM_PERIOD_CYCLES;    //1/64us  64*1000,000 / PWMFre = 
    mcconf->limitCurrentMax = CurrentInt16(DefaultMaxCurrent);     //最大限制电流
    mcconf->brakeCurrentMax = CurrentInt16(DefaultMaxBrakeCurrent);     //最大刹车电流
    mcconf->brakeCurrentMin = CurrentInt16(DefaultMinBrakeCurrent);     //最小刹车电流
    mcconf->deadCompCnt = SW_DEADTIME_NS;      //SVPWM死区时间a
    mcconf->LowVBusLVL1 = VBusVol(LvdLVL1);
    mcconf->LowVBusLVL2 = VBusVol(LvdLVL2);
    mcconf->LowVBusLVL3 = VBusVol(LvdLVL3);
    mcconf->LowVBusLVL1Speed = LvdLVL1_RPM;
    mcconf->LowVBusLVL1Speed = LvdLVL2_RPM;
    mcconf->LowVBusLVL1Speed = LvdLVL3_RPM;
    //mcconf->TempMotorLVL1 = 100;
    mcconf->foc_hall_table = foc_hall_ang_table;
    mcconf->mc_KV = default_KV;
    mcconf->Fluxlink = Q12RLF(FluxLink);    //0x7fffffff
    mcconf->R = Q12RLF(RS);    //0x7fffffff
    mcconf->Ld = Q12RLF(LdS);    //0x7fffffff
    mcconf->Lq = Q12RLF(LqS);    //0x7fffffff
    mcconf->CRC_Data = 0;
}



