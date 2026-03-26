/**
  ******************************************************************************
  * @file    mc_parameters.c
  * @version V1.0.0
  * @date    2021-07-30
  * @brief   Header for main.c file.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "parameters_conversion.h"
#include "r3_f0xx_pwm_curr_fdbk.h"



#define MAX_TWAIT 0                 /* Dummy value for single drive */
#define FREQ_RATIO 1                /* Dummy value for single drive */
#define FREQ_RELATION HIGHEST_FREQ  /* Dummy value for single drive */

/**
  * @brief  Current sensor parameters Single Drive - three shunt
  */
const R3_F0XX_Params_t R3_F0XX_Params =
{
    /* Current reading A/D Conversions initialization -----------------------------*/
    .b_ISamplingTime =  ADC_SAMPLE_CYCLE_7_5,
    .bIaChannel = MC_ADC_CHANNEL_13,
    .bIbChannel = MC_ADC_CHANNEL_14,
    .bIcChannel = MC_ADC_CHANNEL_15,

    /* PWM generation parameters --------------------------------------------------*/
    .hDeadTime = DEAD_TIME_COUNTS,
    .bRepetitionCounter = REP_COUNTER,
    .hTafter = TW_AFTER,
    .hTbefore = TW_BEFORE,
    .ATUx = PWM_TIM1,

    /* PWM Driving signals initialization ----------------------------------------*/
    .LowSideOutputs = (LowSideOutputsFunction_t)LOW_SIDE_SIGNALS_ENABLING,

};

