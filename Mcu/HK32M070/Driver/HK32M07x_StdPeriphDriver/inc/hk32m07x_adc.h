/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_adc.h
* @brief:     This file contains all the functions prototypes for the ADC firmware
*             library
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_ADC_H
#define __HK32M07X_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup ADC
  * @{
  */

/** @defgroup ADC_Exported_Types ADC_Exported_Types
  * @{
  */
/* Exported types ------------------------------------------------------------*/


typedef struct
{
    uint32_t ADC_ClkDiv;                        /*!< Selects the clk division of ADC.
                                                 This parameter can be a value of @ref ADC_clk_div */


    uint32_t ADC_SampleCycle;                   /*!< Selects the Sampling time of ADC.
                                                 This parameter can be a value of @ref ADC_sample_cycle */


    uint32_t ADC_DateAlignment;                 /*!< Selects the Register data align format.
                                                 This parameter can be a value of @ref ADC_data_alignment */


    uint32_t ADC_VrefSourse;                    /*!< Selects the vref selection of ADC.
                                                 This parameter can be a value of @ref ADC_vref_sourse */

    uint32_t ADC_InternalVref;                  /*!< Selects the ADC internal vref selection.
                                                 This parameter can be a value of @ref ADC_internal_vref */ 

} ADC_InitTypeDef;


/**
  * @brief  ADC Queue Init structure definition
  */
#define GROUP_CHANNEL_MAX   12

typedef struct
{
    uint16_t ADC_Group;                     /*!< Selects the group of ADC.
                                                 This parameter can be a value of @ref ADC_group_name */

    FunctionalState ADC_GroupExtTrigCtrl;   /*!< Specifies the external triger of ADC group is enable or disable.
                                                 This parameter can be set to ENABLE or DISABLE. */

    uint16_t ADC_GroupExtTrigSourse;        /*!< Selects the clk division of ADC.
                                                 This parameter can be a value of @ref ADC_group_trigger_sourse */

    uint16_t ADC_GroupTrigSkip;             /*!< Selects the external triger source.
                                                 This parameter can be a value of @ref ADC_group_trigger_skip */

    uint16_t ADC_GroupSampleMode;           /*!< Selects the groupB sampling mode.
                                                 This parameter can be a value of @ref ADC_group_sample_mode */

    FunctionalState ADC_GroupDoubleSampleCtrl;     /*!< Specifies the groupB dual sampling mode enable or disable.
                                                 This parameter can be set to ENABLE or DISABLE. */

    uint16_t ADC_GroupChNumber;             /*!< Selects the Group sampling number.
                                                 This parameter can be a value of @ref ADC_group_channel_number */

    uint16_t ADC_GroupChannel[GROUP_CHANNEL_MAX]; /*!< Selects the Group sampling channel.
                                                 This parameter can be a value of @ref ADC_channel */
} Group_InitTypeDef;


/**
  * @}
  */

/* Exported constants ------------------------------------------------------------*/

/** @defgroup ADC_Exported_Constants ADC_Exported_Constants
    * @brief ADC exported constants
  * @{
    @verbatim
    @endverbatim
  */

/** @defgroup ADC_common_macro  ADC_common_macro
  * @{
  */

#define ADC_CAL_TIMEOUT             0xFFFF

#define TEMP25_CAL_ADDR 			((uint16_t*) ((uint32_t) 0x1FFFFA18))
#define VREFINT_CAL_ADDR 			((uint16_t*) ((uint32_t) 0x1FFFFA14))
#define VREFINT_CAL_FT_ADDR 		((uint16_t*) ((uint32_t) 0x1FFFFB14))

#define TEMPS_CAL_ADDR    	        ((uint32_t)0x1FFFFB18)

#define ADC_GAIN_FIX_SIGN_POS       6


/**
  * @}
  */

/** @defgroup ADC_group_name ADC_group_name
  * @{
  */
#define GROUP_A         1
#define GROUP_B         2
#define IS_GROUP_NAME(GROUP) (((GROUP) == GROUP_A) || ((GROUP) == GROUP_B))
/**
  * @}
  */


/** @defgroup ADC_name ADC_name
  * @{
  */
#define IS_ADC_ALL_PERIPH(PERIPH)  ((PERIPH) == ADC)
/**
  * @}
  */

/** @defgroup ADC_interrupts_definition ADC_interrupts_definition
  * @{
  */
#define ADC_IT_GROUPA_FINISH      0x00000001   /* Group A sampling conversion completed interrupt mask */
#define ADC_IT_GROUPB_FINISH      0x00000004   /* Group B sampling conversion completed interrupt mask */
#define ADC_IT_GROUPB0_FINISH     0x00000008   /* Group A B0 sampling conversion completed interrupt mask */
#define ADC_IT_GROUPB_INTERREPT   0x00000010   /* Group B sampling conversion Be interrupted interrupt mask */
#define ADC_IT_BUSY_HW_TRIG       0x00000040   /* Hardware trigger occurs with the ADC on busy status interrupt mask */
#define ADC_IT_BUSY_SW_TRIG       0x00000080   /* Software trigger occurs with the ADC on busy status interrupt mask */
#define ADC_IT_WINDOW_COMPARE0    0x00000100   /* Window 0 compare protection interrupt mask */
#define ADC_IT_WINDOW_COMPARE1    0x00000200   /* Window 1 compare protection interrupt mask */
#define ADC_IT_WINDOW_COMPARE2    0x00000400   /* Window 2 compare protection interrupt mask */
#define ADC_IT_WINDOW_COMPARE3    0x00000800   /* Window 3 compare protection interrupt mask */
#define IS_ADC_IT(ADC_IT)   (((ADC_IT) == ADC_IT_GROUPA_FINISH) || ((ADC_IT) == ADC_IT_GROUPB_FINISH) || \
                               ((ADC_IT) == ADC_IT_GROUPB0_FINISH) || ((ADC_IT) == ADC_IT_GROUPB_INTERREPT) || \
                               ((ADC_IT) == ADC_IT_BUSY_HW_TRIG) || ((ADC_IT) == ADC_IT_BUSY_SW_TRIG) || \
                               ((ADC_IT) == ADC_IT_WINDOW_COMPARE0) || ((ADC_IT) == ADC_IT_WINDOW_COMPARE1) || \
                               ((ADC_IT) == ADC_IT_WINDOW_COMPARE2) || ((ADC_IT) == ADC_IT_WINDOW_COMPARE3))
/**
  * @}
  */

/** @defgroup ADC_flags_definition ADC_flags_definition
  * @{
  */
#define ADC_STATUS_AWSF0      0x00000001    /* Window 0 compare protection flag */
#define ADC_STATUS_AWSF1      0x00000002    /* Window 1 compare protection flag */
#define ADC_STATUS_AWSF2      0x00000004    /* Window 2 compare protection flag */
#define ADC_STATUS_AWSF3      0x00000008    /* Window 3 compare protection flag */
#define ADC_STATUS_ADBF       0x00000080    /* ADC busy flag */
#define ADC_STATUS_ASA0       0x00001000    /* GroupA channel 0 convert complete flag */
#define ADC_STATUS_ASB0       0x00010000    /* GroupB channel 0 convert complete flag */
#define ADC_STATUS_ASB1       0x00020000    /* GroupB channel 1 convert complete flag */
#define ADC_STATUS_ASB2       0x00040000    /* GroupB channel 2 convert complete flag */
#define ADC_STATUS_ASB3       0x00080000    /* GroupB channel 3 convert complete flag */
#define ADC_STATUS_ASB4       0x00100000    /* GroupB channel 4 convert complete flag */
#define ADC_STATUS_ASB5       0x00200000    /* GroupB channel 5 convert complete flag */
#define ADC_STATUS_ASB6       0x00400000    /* GroupB channel 6 convert complete flag */
#define ADC_STATUS_ASB7       0x00800000    /* GroupB channel 7 convert complete flag */
#define ADC_STATUS_ASB8       0x01000000    /* GroupB channel 8 convert complete flag */
#define ADC_STATUS_ASB9       0x02000000    /* GroupB channel 9 convert complete flag */
#define ADC_STATUS_ASB10      0x04000000    /* GroupB channel 10 convert complete flag */
#define ADC_STATUS_ASB11      0x08000000    /* GroupB channel 11 convert complete flag */
#define IS_ADC_STATUS(ADC_STATUS)   (((ADC_STATUS) == ADC_STATUS_AWSF0) || ((ADC_STATUS) == ADC_STATUS_AWSF1) || \
                                       ((ADC_STATUS) == ADC_STATUS_AWSF2) || ((ADC_STATUS) == ADC_STATUS_AWSF3) || \
                                       ((ADC_STATUS) == ADC_STATUS_ADBF) || ((ADC_STATUS) == ADC_STATUS_ASA0)   || \
                                       ((ADC_STATUS) == ADC_STATUS_ASB0) || ((ADC_STATUS) == ADC_STATUS_ASB1)   || \
                                       ((ADC_STATUS) == ADC_STATUS_ASB2) || ((ADC_STATUS) == ADC_STATUS_ASB3)   || \
                                       ((ADC_STATUS) == ADC_STATUS_ASB4) || ((ADC_STATUS) == ADC_STATUS_ASB5)   || \
                                       ((ADC_STATUS) == ADC_STATUS_ASB6) || ((ADC_STATUS) == ADC_STATUS_ASB7)   || \
                                       ((ADC_STATUS) == ADC_STATUS_ASB8) || ((ADC_STATUS) == ADC_STATUS_ASB9)   || \
                                       ((ADC_STATUS) == ADC_STATUS_ASB10) || ((ADC_STATUS) == ADC_STATUS_ASB11))
/**
  * @}
  */


/** @defgroup ADC_group_sample_mode ADC_group_sample_mode
 * @{
 */
#define ADC_SAMPLE_MODE_SINGLE     0x00000000    /* single mode */
#define ADC_SAMPLE_MODE_CONTINUE   0x00000001    /* continue mode */
#define IS_ADC_SAMPLE_MODE(SAMPLE_MODE)   (((SAMPLE_MODE) == ADC_SAMPLE_MODE_SINGLE) || ((SAMPLE_MODE) == ADC_SAMPLE_MODE_CONTINUE))
/**
  * @}
  */

/** @defgroup ADC_group_trigger_sourse  ADC_group_trigger_sourse
 * @{
 */
#define   ADC_EXT_TRIG_ATU_TRG0      0x00000000
#define   ADC_EXT_TRIG_ATU_TRG1      0x00000001
#define   ADC_EXT_TRIG_UTU1_TRG0     0x00000004
#define   ADC_EXT_TRIG_UTU2_TRG0     0x00000006
#define   ADC_EXT_TRIG_CMP1OUT_FL    0x0000000C
#define   ADC_EXT_TRIG_CMP2OUT_FL    0x0000000D

#define   IS_ADC_EXT_TRIGGER(TRIG_SOURSE)   (((TRIG_SOURSE) == ADC_EXT_TRIG_ATU_TRG0) || ((TRIG_SOURSE) == ADC_EXT_TRIG_ATU_TRG1) || \
                                                ((TRIG_SOURSE) == ADC_EXT_TRIG_UTU1_TRG0) || ((TRIG_SOURSE) == ADC_EXT_TRIG_UTU2_TRG0) || \
                                                ((TRIG_SOURSE) == ADC_EXT_TRIG_CMP1OUT_FL) || ((TRIG_SOURSE) == ADC_EXT_TRIG_CMP2OUT_FL))
/**
  * @}
  */

/** @defgroup ADC_group_trigger_skip  ADC_group_trigger_skip
  * @{
  */
#define   ADC_TRIG_SKIP_NONE     0x00000000
#define   ADC_TRIG_SKIP_1        0x00000001
#define   ADC_TRIG_SKIP_2        0x00000002
#define   ADC_TRIG_SKIP_3        0x00000003
#define   ADC_TRIG_SKIP_4        0x00000004
#define   ADC_TRIG_SKIP_5        0x00000005
#define   ADC_TRIG_SKIP_6        0x00000006
#define   ADC_TRIG_SKIP_7        0x00000007
#define   ADC_TRIG_SKIP_8        0x00000008
#define   ADC_TRIG_SKIP_9        0x00000009
#define   ADC_TRIG_SKIP_10       0x0000000A
#define   ADC_TRIG_SKIP_11       0x0000000B
#define   ADC_TRIG_SKIP_12       0x0000000C
#define   ADC_TRIG_SKIP_13       0x0000000D
#define   ADC_TRIG_SKIP_14       0x0000000E
#define   ADC_TRIG_SKIP_15       0x0000000F

#define   IS_ADC_TRIGGER_SKIP(TRIG_SKIP)   (((TRIG_SKIP) == ADC_TRIG_SKIP_NONE) || ((TRIG_SKIP) == ADC_TRIG_SKIP_1)  || \
                                                ((TRIG_SKIP) == ADC_TRIG_SKIP_2) || ((TRIG_SKIP) == ADC_TRIG_SKIP_3)    || \
                                                ((TRIG_SKIP) == ADC_TRIG_SKIP_4) || ((TRIG_SKIP) == ADC_TRIG_SKIP_5)    || \
                                                ((TRIG_SKIP) == ADC_TRIG_SKIP_6) || ((TRIG_SKIP) == ADC_TRIG_SKIP_7)    || \
                                                ((TRIG_SKIP) == ADC_TRIG_SKIP_8) || ((TRIG_SKIP) == ADC_TRIG_SKIP_9)    || \
                                                ((TRIG_SKIP) == ADC_TRIG_SKIP_10) || ((TRIG_SKIP) == ADC_TRIG_SKIP_11)  || \
                                                ((TRIG_SKIP) == ADC_TRIG_SKIP_12) || ((TRIG_SKIP) == ADC_TRIG_SKIP_13)  || \
                                                ((TRIG_SKIP) == ADC_TRIG_SKIP_14) || ((TRIG_SKIP) == ADC_TRIG_SKIP_15))
/**
  * @}
  */

/** @defgroup ADC_group_ascs_select  ADC_group_channel_select
 * @{
 */
#define   ASCS_0         0
#define   ASCS_1         1
#define   ASCS_2         2
#define   ASCS_3         3
#define   ASCS_4         4
#define   ASCS_5         5
#define   ASCS_6         6
#define   ASCS_7         7
#define   ASCS_8         8
#define   ASCS_9         9
#define   ASCS_10        10
#define   ASCS_11        11
#define   IS_ADC_GROUP_ASCS(CHANNEL)   (((CHANNEL) == ASCS_0) || ((CHANNEL) == ASCS_1) || ((CHANNEL) == ASCS_2) || \
                                           ((CHANNEL) == ASCS_3) || ((CHANNEL) == ASCS_4) || ((CHANNEL) == ASCS_5) || \
                                           ((CHANNEL) == ASCS_6) || ((CHANNEL) == ASCS_7) || ((CHANNEL) == ASCS_8) || \
                                           ((CHANNEL) == ASCS_9) || ((CHANNEL) == ASCS_10) || ((CHANNEL) == ASCS_11))
/**
  * @}
  */


/** @defgroup ADC_channel ADC_channel
  * @{
  */
#define   ADC_CHANNEL_PGA1_OUTA         0x00000000
#define   ADC_CHANNEL_PGA1_OUTB         0x00000001
#define   ADC_CHANNEL_PGA2_OUT          0x00000002
#define   ADC_CHANNEL_5                 0x00000004
#define   ADC_CHANNEL_6                 0x00000005
#define   ADC_CHANNEL_7                 0x00000006
#define   ADC_CHANNEL_8                 0x00000007
#define   ADC_CHANNEL_9                 0x00000008
#define   ADC_CHANNEL_10                0x00000009
#define   ADC_CHANNEL_11                0x0000000A
#define   ADC_CHANNEL_12                0x0000000B
#define   ADC_CHANNEL_13                0x0000000C
#define   ADC_CHANNEL_14                0x0000000D
#define   ADC_CHANNEL_DAC_OUT           0x0000000E
#define   ADC_CHANNEL_TS                0x00000010
#define   ADC_CHANNEL_BGV               0x00000011
#define   ADC_CHANNEL_PGA2_AVG          0x00000015
#define   ADC_CHANNEL_18                0x00000018
#define   ADC_CHANNEL_19                0x00000019
#define   ADC_CHANNEL_20                0x0000001A
#define   ADC_CHANNEL_21                0x0000001B
#define   IS_ADC_CHANNEL(CHANNEL)   (((CHANNEL) == ADC_CHANNEL_PGA1_OUTA) || ((CHANNEL) == ADC_CHANNEL_PGA1_OUTB) || ((CHANNEL) == ADC_CHANNEL_PGA2_OUT) || \
                                     ((CHANNEL) == ADC_CHANNEL_5) || ((CHANNEL) == ADC_CHANNEL_6) || ((CHANNEL) == ADC_CHANNEL_7) || \
                                     ((CHANNEL) == ADC_CHANNEL_8) || ((CHANNEL) == ADC_CHANNEL_9) || ((CHANNEL) == ADC_CHANNEL_10) || \
                                     ((CHANNEL) == ADC_CHANNEL_11) || ((CHANNEL) == ADC_CHANNEL_12) || ((CHANNEL) == ADC_CHANNEL_13) || \
                                     ((CHANNEL) == ADC_CHANNEL_14) || ((CHANNEL) == ADC_CHANNEL_DAC_OUT) || ((CHANNEL) == ADC_CHANNEL_TS) || \
                                     ((CHANNEL) == ADC_CHANNEL_BGV) || ((CHANNEL) == ADC_CHANNEL_PGA2_AVG) || ((CHANNEL) == ADC_CHANNEL_18) || \
                                     ((CHANNEL) == ADC_CHANNEL_19) || ((CHANNEL) == ADC_CHANNEL_20) || ((CHANNEL) == ADC_CHANNEL_21))
/**
  * @}
  */

/** @defgroup ADC_group_channel_number  ADC_group_channel_number
  * @{
  */
#define IS_ADC_GROUPA_CHANNEL_NUMBER(NUMBE) ((NUMBE) < 0x2)
#define IS_ADC_GROUPB_CHANNEL_NUMBER(NUMBE) ((NUMBE) <= 0xC)
/**
  * @}
  */

/** @defgroup ADC_group_bias  ADC_group_bias
  * @{
  */
#define GROUPA_ADBA0   0   /* GroupA ADBA0 */
#define GROUPB_ADBB0   1   /* GroupB ADBB0 */
#define GROUPB_ADBB1   2   /* GroupB ADBB1 */
#define IS_ADC_GROUP_BIAS(BIAS) (((BIAS) == GROUPA_ADBA0) || ((BIAS) == GROUPB_ADBB0) || ((BIAS) == GROUPB_ADBB1))
/**
  * @}
  */

/** @defgroup ADC_group_bias_value  ADC_group_bias_value
  * @{
  */
#define IS_ADC_GROUP_BIAS_VALUE(BIASVALUE) ((BIASVALUE) <= 0xFFF)
/**
  * @}
  */


/** @defgroup ADC_cycle_sel  ADC_cycle_sel
  * @{
  */
#define ADC_CYCLE_SEL_12_5   0x00000000   /* adc cycle select 12.5 cycles */
#define ADC_CYCLE_SEL_13_5   0x00000002   /* adc cycle select 13.5 cycles */
#define IS_ADC_CYCLE_SEL(CYCLE) (((CYCLE) == ADC_CYCLE_SEL_12_5) || ((CYCLE) == ADC_CYCLE_SEL_13_5))
/**
  * @}
  */

/** @defgroup ADC_clk_div  ADC_clk_div
 * @{
 */
#define ADC_CLK_DIV_1         0x00000000  /* 1 division */
#define ADC_CLK_DIV_2         0x00000100  /* 2 division */
#define ADC_CLK_DIV_4         0x00000200  /* 4 division */
#define ADC_CLK_DIV_8         0x00000300  /* 8 division */
#define ADC_CLK_DIV_16        0x00000400  /* 16 division */
#define ADC_CLK_DIV_32        0x00000500  /* 32 division */
#define ADC_CLK_DIV_64        0x00000600  /* 64 division */
#define ADC_CLK_DIV_128       0x00000700  /* 128 division */
#define IS_ADC_CLK_DIV(DIV) (((DIV) == ADC_CLK_DIV_1) || ((DIV) == ADC_CLK_DIV_2) ||  \
                             ((DIV) == ADC_CLK_DIV_4) || ((DIV) == ADC_CLK_DIV_8) || \
                             ((DIV) == ADC_CLK_DIV_16) || ((DIV) == ADC_CLK_DIV_32) || \
                             ((DIV) == ADC_CLK_DIV_64) || ((DIV) == ADC_CLK_DIV_128))
/**
  * @}
  */

/** @defgroup ADC_vref_sourse  ADC_vref_sourse
  * @{
  */
#define ADC_VREF_EXTERNAL   0x01000000   /* vref select AVDD */
#define ADC_VREF_INTERNAL   0x00000000   /* vref select internal vref */
#define IS_ADC_VREF_SOURSE(VREF) (((VREF) == ADC_VREF_EXTERNAL) || ((VREF) == ADC_VREF_INTERNAL))
/**
  * @}
  */


/** @defgroup ADC_internal_vref  ADC_internal_vref
  * @{
  */
#define ADC_INTERNAL_VREF_2_4   0x00000000  /* Internal vref select 2.4V */
#define ADC_INTERNAL_VREF_3_6   0x04000000  /* Internal vref select 3.6V */
#define IS_ADC_INTERNAL_VREF(VREF) (((VREF) == ADC_INTERNAL_VREF_2_4) || ((VREF) == ADC_INTERNAL_VREF_3_6))
/**
  * @}
  */

/** @defgroup ADC_data_alignment  ADC_data_alignment
  * @{
  */
#define ADC_DATA_ALIGN_RIGHT                        0x80000000
#define ADC_DATA_ALIGN_LEFT                         0x00000000
#define IS_ADC_DATA_ALIGNMENT(ALIGNMENT) (((ALIGNMENT) == ADC_DATA_ALIGN_RIGHT) || ((ALIGNMENT) == ADC_DATA_ALIGN_LEFT))
/**
  * @}
  */



/** @defgroup ADC_sample_cycle  ADC_sample_cycle
  * @{
  */
#define ADC_SAMPLE_CYCLE_1_5    0x0000
#define ADC_SAMPLE_CYCLE_2_5    0x0001
#define ADC_SAMPLE_CYCLE_3_5    0x0002
#define ADC_SAMPLE_CYCLE_4_5    0x0003
#define ADC_SAMPLE_CYCLE_5_5    0x0004
#define ADC_SAMPLE_CYCLE_6_5    0x0005
#define ADC_SAMPLE_CYCLE_7_5    0x0006
#define ADC_SAMPLE_CYCLE_8_5    0x0007
#define ADC_SAMPLE_CYCLE_13_5   0x0008
#define ADC_SAMPLE_CYCLE_28_5   0x0009
#define ADC_SAMPLE_CYCLE_41_5   0x000B
#define ADC_SAMPLE_CYCLE_55_5   0x000C
#define IS_ADC_SAMPLE_CYCLE(CYCLE) (((CYCLE) == ADC_SAMPLE_CYCLE_1_5) || ((CYCLE) == ADC_SAMPLE_CYCLE_2_5) || \
                                    ((CYCLE) == ADC_SAMPLE_CYCLE_3_5) || ((CYCLE) == ADC_SAMPLE_CYCLE_4_5) || \
                                    ((CYCLE) == ADC_SAMPLE_CYCLE_5_5) || ((CYCLE) == ADC_SAMPLE_CYCLE_6_5) || \
                                    ((CYCLE) == ADC_SAMPLE_CYCLE_7_5) || ((CYCLE) == ADC_SAMPLE_CYCLE_8_5) || \
                                    ((CYCLE) == ADC_SAMPLE_CYCLE_13_5) || ((CYCLE) == ADC_SAMPLE_CYCLE_28_5) || \
                                    ((CYCLE) == ADC_SAMPLE_CYCLE_41_5) || ((CYCLE) == ADC_SAMPLE_CYCLE_55_5))
/**
  * @}
  */

/** @defgroup ADC_group_channel  ADC_group_channel
 * @{
 */
#define CHANNEL_GROUPA_0    0x0000
#define CHANNEL_GROUPB_0    0x0004
#define CHANNEL_GROUPB_1    0x0005
#define CHANNEL_GROUPB_2    0x0006
#define CHANNEL_GROUPB_3    0x0007
#define CHANNEL_GROUPB_4    0x0008
#define CHANNEL_GROUPB_5    0x0009
#define CHANNEL_GROUPB_6    0x000A
#define CHANNEL_GROUPB_7    0x000B
#define CHANNEL_GROUPB_8    0x000C
#define CHANNEL_GROUPB_9    0x000D
#define CHANNEL_GROUPB_10   0x000E
#define CHANNEL_GROUPB_11   0x000F
#define IS_ADC_GROUP_CHANNEL(CHANNEL) (((CHANNEL) == CHANNEL_GROUPA_0) || ((CHANNEL) == CHANNEL_GROUPB_0) || \
                                     ((CHANNEL) == CHANNEL_GROUPB_1) || ((CHANNEL) == CHANNEL_GROUPB_2) || \
                                     ((CHANNEL) == CHANNEL_GROUPB_3) || ((CHANNEL) == CHANNEL_GROUPB_4) || \
                                     ((CHANNEL) == CHANNEL_GROUPB_5) || ((CHANNEL) == CHANNEL_GROUPB_6) || \
                                     ((CHANNEL) == CHANNEL_GROUPB_7) || ((CHANNEL) == CHANNEL_GROUPB_8) || \
                                     ((CHANNEL) == CHANNEL_GROUPB_9) || ((CHANNEL) == CHANNEL_GROUPB_10) || \
                                     ((CHANNEL) == CHANNEL_GROUPB_11))
/**
  * @}
  */

/** @defgroup ADC_over_sample_avg_number  ADC_over_sample_avg_number
  * @{
  */
#define ADC_OVSMP_AVG_2     0x0000  /* average number 2 */
#define ADC_OVSMP_AVG_4     0x0100  /* average number 4 */
#define ADC_OVSMP_AVG_8     0x0200  /* average number 8 */
#define IS_ADC_OVER_SAMPLE_AVG_NUMBER(NUMBER) (((NUMBER) == ADC_OVSMP_AVG_2) || ((NUMBER) == ADC_OVSMP_AVG_4) || \
                                               ((NUMBER) == ADC_OVSMP_AVG_8))
/**
  * @}
  */

/** @defgroup ADC_over_sample_trigger_skip  ADC_over_sample_trigger_skip
  * @{
  */
#define   ADC_OVSMP_TRIG_SKIP_NONE      0x0000 /* No number of skip */
#define   ADC_OVSMP_TRIG_SKIP_1         0x1000 /* skip 1 */
#define   ADC_OVSMP_TRIG_SKIP_3         0x2000 /* skip 3 */
#define   ADC_OVSMP_TRIG_SKIP_7         0x3000 /* skip 7 */
#define   ADC_OVSMP_TRIG_SKIP_15        0x4000 /* skip 15 */
#define   ADC_OVSMP_TRIG_SKIP_31        0x5000 /* skip 31 */
#define   ADC_OVSMP_TRIG_SKIP_63        0x6000 /* skip 63 */
#define   ADC_OVSMP_TRIG_SKIP_127       0x7000 /* skip 127 */
#define   IS_ADC_OVSMP_TRIG_SKIP(TRIG_SKIP)   (((TRIG_SKIP) == ADC_OVSMP_TRIG_SKIP_NONE) || ((TRIG_SKIP) == ADC_OVSMP_TRIG_SKIP_1)  || \
                                                ((TRIG_SKIP) == ADC_OVSMP_TRIG_SKIP_3) || ((TRIG_SKIP) == ADC_OVSMP_TRIG_SKIP_7)    || \
                                                ((TRIG_SKIP) == ADC_OVSMP_TRIG_SKIP_15) || ((TRIG_SKIP) == ADC_OVSMP_TRIG_SKIP_31)    || \
                                                ((TRIG_SKIP) == ADC_OVSMP_TRIG_SKIP_63) || ((TRIG_SKIP) == ADC_OVSMP_TRIG_SKIP_127))
/**
  * @}
  */

/** @defgroup ADC_over_sample_mode  ADC_over_sample_mode
 * @{
 */
#define ADC_OVSMP_CONT      0x0000  /* Continuous oversampling mode */
#define ADC_OVSMP_DISCONT   0x8000  /* Time sharing sampling mode*/
#define IS_ADC_OVER_SAMPLE_MODE(MODE) (((MODE) == ADC_OVSMP_CONT) || ((MODE) == ADC_OVSMP_DISCONT))
/**
  * @}
  */

/** @defgroup ADC_buffer  ADC_buffer
 * @{
 */
#define ADC_BUFFER0    0
#define ADC_BUFFER1    1
#define ADC_BUFFER2    2
#define ADC_BUFFER3    3
#define ADC_BUFFER4    4
#define ADC_BUFFER5    5
#define ADC_BUFFER6    6
#define ADC_BUFFER7    7
#define IS_ADC_BUFFER(buffer) (((buffer) == ADC_BUFFER0) || ((buffer) == ADC_BUFFER1) || \
                               ((buffer) == ADC_BUFFER2) || ((buffer) == ADC_BUFFER3) || \
                               ((buffer) == ADC_BUFFER4) || ((buffer) == ADC_BUFFER5) || \
                               ((buffer) == ADC_BUFFER6) || ((buffer) == ADC_BUFFER7))
/**
  * @}
  */

/** @defgroup ADC_user_offset  ADC_user_offset
 * @{
 */
#define IS_ADC_USER_OFFSET(USEROFFSET) ((USEROFFSET) <= 0x3F)
/**
  * @}
  */

/** @defgroup ADC_gain_fix  ADC_gain_fix
 * @{
 */
#define IS_ADC_GAIN_FIX(GAINFIX) ((GAINFIX) <= 0x1F)
/**
  * @}
  */
  
/** @defgroup ADC_gain_direction  ADC_gain_direction
 * @{
 */
#define DIRECTION_POSITIVE    0
#define DIRECTION_NEGATIVE    1
#define IS_ADC_GAIN_DIRECTION(DIRECTION)    (((DIRECTION) == DIRECTION_POSITIVE) || ((DIRECTION) == DIRECTION_NEGATIVE)) 
/**
  * @}
  */


/** @defgroup ADC_analog_window_name  ADC_analog_window_name
  * @{
  */
#define AW_0 0
#define AW_1 1
#define AW_2 2
#define AW_3 3
#define IS_ADC_AW_NAME(NAME) ( ((NAME) == AW_0) || ((NAME) == AW_1) || ((NAME) == AW_2) || ((NAME) == AW_3) )
/**
  * @}
  */

/** @defgroup ADC_analog_window_mode  ADC_analog_window_mode
 * @{
 */
#define AW_MODE_0   0
#define AW_MODE_1   0x0040
#define IS_ADC_AW_MODE(MODE) (((MODE) == AW_MODE_0) || ((MODE) == AW_MODE_1))
/**
  * @}
  */

/** @defgroup ADC_analog_window_threshold  ADC_analog_window_threshold
  * @{
  */
#define IS_ADC_AW_THRESHOLD(THRESHOLD) ((THRESHOLD) <= 0xFFF)
/**
  * @}
  */


/** @defgroup ADC_pga2_out_extended  ADC_pga2_out_extended
  * @{
  */
#define PGA2_OUT_TO_NONE    0x0000
#define PGA2_OUT_TO_R_CH8   0x0001
#define PGA2_OUT_TO_CH8     0x0002
#define IS_ADC_EXTENDED(MODE) (((MODE) == PGA2_OUT_TO_NONE) || ((MODE) == PGA2_OUT_TO_CH8) || ((MODE) == PGA2_OUT_TO_R_CH8))
/**
  * @}
  */

/** @defgroup ADC_lpf_div  ADC_lpf_div
 * @{
 */
#define IS_ADC_LPF_DIV(DIV) (((DIV) <= 0x7ff))
/**
  * @}
  */


/** @defgroup ADC_lpf_stp  ADC_lpf_stp
 * @{
 */
#define IS_ADC_LPF_STP(STP) (((STP) <= 0xff))
/**
  * @}
  */

/** @defgroup ADC_ch13_pinremap  ADC_ch13_pinremap
 * @{
 */
#define ADC_CH13_PINREMAP_PA10       0x00000000
#define ADC_CH13_PINREMAP_PA4        0x00000004
#define IS_ADC_PIN_REMAP_CH13(REMAP) (((REMAP) == ADC_CH13_PINREMAP_PA10) || ((REMAP) == ADC_CH13_PINREMAP_PA4))
/**
  * @}
  */


/** @defgroup ADC_ch14_pinremap  ADC_ch14_pinremap
 * @{
 */
#define ADC_CH14_PINREMAP_PA11       0x00000000
#define ADC_CH14_PINREMAP_PA5        0x00000008
#define IS_ADC_PIN_REMAP_CH14(REMAP) (((REMAP) == ADC_CH14_PINREMAP_PA11) || ((REMAP) == ADC_CH14_PINREMAP_PA5))
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup ADC_Exported_Functions ADC_Exported_Functions
  * @{
  */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************* ADC and Group Init function*******************************/
void ADC_DeInit(ADC_TypeDef* ADCx);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_GroupStructInit(Group_InitTypeDef* Group_InitStruct);
void ADC_GroupInit(ADC_TypeDef* ADCx, Group_InitTypeDef* Group_InitStruct);

/******************************* ADC_IT and Status function*******************************/
void ADC_ITConfig(ADC_TypeDef* ADCx, uint32_t ADC_IT, FunctionalState NewState);
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint32_t ADC_IT);
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint32_t ADC_IT);
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint32_t ADC_Status);

/******************************* Group congig function *******************************/
void ADC_GroupBSampleMode(ADC_TypeDef* ADCx, uint32_t SampleMode);
void ADC_GroupBDoubleSampleCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_GroupExtTrigConfig(ADC_TypeDef* ADCx, uint16_t Group, uint16_t ExtTrigSourse, FunctionalState NewState);
void ADC_GroupExtTrigCmd(ADC_TypeDef* ADCx, uint16_t Group, FunctionalState NewState);
void ADC_GroupTrigSkipConfig(ADC_TypeDef* ADCx, uint16_t Group, uint16_t TrigSkip);
void ADC_GroupSwTrig(ADC_TypeDef* ADCx, uint16_t Group, FunctionalState NewState);
void ADC_GroupDataReadClearCmd(ADC_TypeDef* ADCx, uint16_t Group, FunctionalState NewState);
void ADC_GroupChannelSelect(ADC_TypeDef* ADCx, uint16_t Group, uint16_t ASCS, uint16_t ChannelSourse);
void ADC_GroupChannelNumber(ADC_TypeDef* ADCx, uint16_t Group, uint16_t ChNumber);
void ADC_GroupBias(ADC_TypeDef* ADCx, uint16_t GroupBias, uint16_t BiasValue);

/******************************* Base configure *******************************/
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ADCCycleSel(ADC_TypeDef* ADCx, uint32_t CycleSel);
void ADC_ClockDiv(ADC_TypeDef* ADCx, uint32_t ClkDiv);
uint32_t ADC_GetCalibrationFactor(ADC_TypeDef* ADCx);
void ADC_VrefSourseSelect(ADC_TypeDef* ADCx, uint32_t Vref);
void ADC_InternalVrefSelect(ADC_TypeDef* ADCx, uint32_t Internal);
void ADC_OverSamlpeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_DataAlignment(ADC_TypeDef* ADCx, uint32_t Alignment);
void ADC_SampleCycleConfig(ADC_TypeDef *ADCx, uint16_t Group, uint16_t ASCS, uint16_t SamlpeCycle);
void ADC_OverSampleChannel(ADC_TypeDef* ADCx, uint16_t Channel);
void ADC_OverSampleAvgeNumber(ADC_TypeDef* ADCx, uint16_t AvgeNumber);
void ADC_OverSampleTrigSkip(ADC_TypeDef* ADCx, uint16_t SkipNumber);
void ADC_OverSampleMode(ADC_TypeDef* ADCx, uint16_t Mode);
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx, uint16_t Channel);
uint16_t ADC_GetBufferValue(ADC_TypeDef* ADCx, uint16_t buffer);
void ADC_UserOffset(ADC_TypeDef* ADCx, uint16_t UserOffset);
void ADC_Gain(ADC_TypeDef* ADCx, uint8_t GainDir, uint32_t GainFix, FunctionalState NewState);

/******************************* Analog Window *******************************/
void ADC_AnalogWindowChannel(ADC_TypeDef* ADCx, uint16_t Window, uint16_t Channel);
void ADC_AnalogWindowMode(ADC_TypeDef* ADCx, uint16_t Window, uint16_t Mode);
void ADC_AnalogWindowCmd(ADC_TypeDef* ADCx, uint16_t Window, FunctionalState NewState);
void ADC_AnalogWindowThreshold(ADC_TypeDef* ADCx, uint16_t Window, uint16_t HighThreshold, uint16_t LowThreshold);


/******************************* Extended *******************************/
void ADC_PGA2OUTExtendedSelect(ADC_TypeDef* ADCx, uint16_t Select);
void ADC_PinRemapCH13(ADC_TypeDef *ADCx, uint32_t Remap);
void ADC_PinRemapCH14(ADC_TypeDef *ADCx, uint32_t Remap);
int ADC_GetTemperatureAtFT(void);
int ADC_GetTemperature(uint32_t tempADbuff, uint32_t VDDA_mV);
uint32_t ADC_GetVDDA(uint32_t VrefADbuff);


/**
  * @}
  */

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif 

#endif /*__HK32M07X_ADC_H */

/***************************** (C) COPYRIGHT HKMicroChip  *******************************/
