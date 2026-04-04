
/**
  ******************************************************************************
  * @file    parameters_conversion.h
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   This file includes the proper Parameter conversion on the base
  *          of stdlib for the first drive
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PARAMETERS_CONVERSION_H
#define __PARAMETERS_CONVERSION_H
#include "parameters_conversion_f0xx.h"
#include "mc_math.h"


/************************* CONTROL FREQUENCIES & DELAIES **********************/
#define TF_REGULATION_RATE             (uint16_t) ((uint16_t)(PWM_FREQUENCY)/REGULATION_EXECUTION_RATE)
#define REP_COUNTER                    (uint16_t) ((REGULATION_EXECUTION_RATE *2u)-1u)

#define MEDIUM_FREQUENCY_TASK_RATE     (uint16_t)SPEED_LOOP_FREQUENCY_HZ

#define INRUSH_CURRLIMIT_DELAY_COUNTS  (uint16_t)(INRUSH_CURRLIMIT_DELAY_MS * \
                                       ((uint16_t)SPEED_LOOP_FREQUENCY_HZ)/1000u -1u)

#define SYS_TICK_FREQUENCY             2000

#define MF_TASK_OCCURENCE_TICKS        (SYS_TICK_FREQUENCY/SPEED_LOOP_FREQUENCY_HZ)-1u

/**************************   VOLTAGE CONVERSIONS  ****************************/
#define BUS_ADC_CONV_RATIO             VBUS_PARTITIONING_FACTOR

#define OVERVOLTAGE_THRESHOLD_d        (uint16_t)(OV_VOLTAGE_THRESHOLD_V*65535/\
                                       (ADC_REFERENCE_VOLTAGE/VBUS_PARTITIONING_FACTOR))
#define UNDERVOLTAGE_THRESHOLD_d       (uint16_t)((UD_VOLTAGE_THRESHOLD_V*65535)/\
                                       ((uint16_t)(ADC_REFERENCE_VOLTAGE/\
                                       BUS_ADC_CONV_RATIO)))
#define INT_SUPPLY_VOLTAGE             (uint16_t)(65536/ADC_REFERENCE_VOLTAGE)
                                       
#define DELTA_TEMP_THRESHOLD           (OV_TEMPERATURE_THRESHOLD_C- T0_C)
#define DELTA_V_THRESHOLD              (dV_dT * DELTA_TEMP_THRESHOLD)
#define OV_TEMPERATURE_THRESHOLD_d     ((V0_V + DELTA_V_THRESHOLD)*INT_SUPPLY_VOLTAGE)

#define DELTA_TEMP_HYSTERESIS          (OV_TEMPERATURE_HYSTERESIS_C)
#define DELTA_V_HYSTERESIS             (dV_dT * DELTA_TEMP_HYSTERESIS)
#define OV_TEMPERATURE_HYSTERESIS_d    (DELTA_V_HYSTERESIS*INT_SUPPLY_VOLTAGE)

/*************** Timer for PWM generation & currenst sensing parameters  ******/
#define PWM_PERIOD_CYCLES              (uint16_t)(ADV_TIM_CLK_MHz*\
                                       (unsigned long long)1000000u/((uint16_t)(PWM_FREQUENCY)))

#define DEADTIME_NS                    SW_DEADTIME_NS

#define DEAD_TIME_ADV_TIM_CLK_MHz      (ADV_TIM_CLK_MHz * TIM_CLOCK_DIVIDER)
#define DEAD_TIME_COUNTS_1             (DEAD_TIME_ADV_TIM_CLK_MHz * DEADTIME_NS/1000uL)

#if (DEAD_TIME_COUNTS_1 <= 255)
    #define DEAD_TIME_COUNTS (uint16_t) DEAD_TIME_COUNTS_1
#elif (DEAD_TIME_COUNTS_1 <= 508)
    #define DEAD_TIME_COUNTS (uint16_t)(((DEAD_TIME_ADV_TIM_CLK_MHz * DEADTIME_NS/2) /1000uL) + 128)
#elif (DEAD_TIME_COUNTS_1 <= 1008)
    #define DEAD_TIME_COUNTS (uint16_t)(((DEAD_TIME_ADV_TIM_CLK_MHz * DEADTIME_NS/8) /1000uL) + 320)
#elif (DEAD_TIME_COUNTS_1 <= 2015)
    #define DEAD_TIME_COUNTS (uint16_t)(((DEAD_TIME_ADV_TIM_CLK_MHz * DEADTIME_NS/16) /1000uL) + 384)
#else
    #define DEAD_TIME_COUNTS 510
#endif

#define DTCOMPCNT                      (uint16_t)((DEADTIME_NS * ADV_TIM_CLK_MHz) / 2000)
#define TON_NS                         500
#define TOFF_NS                        500
#define TON                            (uint16_t)((TON_NS * ADV_TIM_CLK_MHz)  / 2000)
#define TOFF                           (uint16_t)((TOFF_NS * ADV_TIM_CLK_MHz) / 2000)
                                       
#define MAX_TNTR_NS                    TRISE_NS

#define SAMPLING_TIME                  (uint16_t)(((uint16_t)(SAMPLING_TIME_NS) * ADV_TIM_CLK_MHz)/1000uL)
#define TRISE                          (uint16_t)((((uint16_t)(TRISE_NS)) * ADV_TIM_CLK_MHz)/1000uL)

#if (TRISE_NS > SAMPLING_TIME_NS)
    #define MAX_TRTS (2 * TRISE)
#else
    #define MAX_TRTS (2 * SAMPLING_TIME)
#endif

#define MAX_TNTR_NS                    TRISE_NS

#define TW_AFTER                       ((uint16_t)(((DEADTIME_NS+MAX_TNTR_NS)*ADV_TIM_CLK_MHz)/1000ul))

/*************** PI divisor  ***************/
#define SP_KPDIV_LOG LOG2(SP_KPDIV)
#define SP_KIDIV_LOG LOG2(SP_KIDIV)
#define SP_KDDIV_LOG LOG2(SP_KDDIV)
#define TF_KPDIV_LOG LOG2(TF_KPDIV)
#define TF_KIDIV_LOG LOG2(TF_KIDIV)
#define TF_KDDIV_LOG LOG2(TF_KDDIV)

/* USER CODE BEGIN virtual temperature */
#define M1_TEMP_SW_FILTER_BW_FACTOR    250u

/* USER CODE END virtual temperature */


#define PQD_CONVERSION_FACTOR          (int32_t)(( 1000 * 3 * ADC_REFERENCE_VOLTAGE ) /\
                                       ( 1.732 * RSHUNT * AMPLIFICATION_GAIN ))


/*** Temporary bridge between workbench data model****/
#define PWM_TIM1                       ATU
#define HALL_TIM2                      HTU

#define START_INDEX                    63
#define MAX_MODULE                     32767   // root(Vd^2+Vq^2) <= MAX_MODULE = 32767*100%
#define MMITABLE {\
32767,32390,32146,31907,31673,31444,31220,31001,30787,30577,30371,\
30169,29971,29777,29587,29400,29217,29037,28861,28687,28517,\
28350,28185,28024,27865,27709,27555,27404,27256,27110,26966,\
26824,26685,26548,26413,26280,26149,26019,25892,25767,25643,\
25521,25401,25283,25166,25051,24937,24825,24715,24606,24498,\
24392,24287,24183,24081,23980,23880,23782,23684,23588,23493,\
23400,23307,23215,23125\
}

#endif /*__PARAMETERS_CONVERSION_H*/

