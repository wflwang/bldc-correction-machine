/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_utu.h
* @brief:     This file provides firmware functions prototypes for the UTU
*             firmware library
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_UTU_H
#define __HK32M07X_UTU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup UTU
  * @{
  */

#if 0
/* temp local */
#define RCC_APB2Periph_UTU1                 ((uint32_t)(0x1 << 15))
#define RCC_APB1Periph_UTU2                 ((uint32_t)(0x1 << 11))


/*****************************************************************************************
*                                    Peripheral UTU                                    *
*****************************************************************************************/
typedef struct
{
    __IO uint32_t UPR;                    /* UPR */
    __IO uint32_t CNT;                    /* CNT */
    __IO uint32_t UCCRA;                  /* UCCRA */
    __IO uint32_t UCCRB;                  /* UCCRB */
    __IO uint32_t DBTA;                   /* DBTA */
    __IO uint32_t DBTB;                   /* DBTB */
    __IO uint32_t TRG0;                   /* TRG0 */
    __IO uint32_t UCSR;                   /* UCSR */
    __IO uint32_t UCCSA;                  /* UCCSA */
    __IO uint32_t UCCSB;                  /* UCCSB */
    __IO uint32_t NFCR;                   /* NFCR */
    __IO uint32_t UTSS;                   /* UTSS */
    __IO uint32_t UPPS;                   /* UPPS */
    __IO uint32_t UIER;                   /* UIER */
    __IO uint32_t UIFR;                   /* UIFR */
    __IO uint32_t USR;                    /* USR */
} UTU_TypeDef;

#if 1
#define UTU1_BASE                            ((uint32_t)0x40004800)
#define UTU1                                 ((UTU_TypeDef *)UTU1_BASE)

#define UTU2_BASE                            ((uint32_t)0x40014000)
#define UTU2                                 ((UTU_TypeDef *)UTU2_BASE)
#endif

/************************************* UTU->UCSR  **************************************/
#define UTU_UCSR_CNT_EN_Pos                  ((uint32_t)15)                     /* Prefetch buffer status */
#define UTU_UCSR_CNT_EN_Mask                 (((uint32_t)0x01) << UTU_UCSR_CNT_EN_Pos)
#define UTU_UCSR_CNT_EN                      UTU_UCSR_CNT_EN_Mask
#define UTU_UCSR_SCWM_Pos                    ((uint32_t)14)                     /* Prefetch buffer enable */
#define UTU_UCSR_SCWM_Mask                   (((uint32_t)0x01) << UTU_UCSR_SCWM_Pos)
#define UTU_UCSR_SCWM                        UTU_UCSR_SCWM_Mask
#define UTU_UCSR_ARPR_Pos                    ((uint32_t)13)                     /* Latency */
#define UTU_UCSR_ARPR_Mask                   (((uint32_t)0x01) << UTU_UCSR_ARPR_Pos)
#define UTU_UCSR_ARPR                        UTU_UCSR_ARPR_Mask
#define UTU_UCSR_ARCR_Pos                    ((uint32_t)12)                     /* Latency */
#define UTU_UCSR_ARCR_Mask                   (((uint32_t)0x01) << UTU_UCSR_ARCR_Pos)
#define UTU_UCSR_ARCR                        UTU_UCSR_ARCR_Mask

#define UTU_UCSR_UTMS_Pos                    ((uint32_t)8)                     /* Latency */
#define UTU_UCSR_UTMS_Mask                   (((uint32_t)0x03) << UTU_UCSR_UTMS_Pos)
#define UTU_UCSR_UTMS_NORMAL_OUT             (((uint32_t)0x00) << UTU_UCSR_UTMS_Pos)
#define UTU_UCSR_UTMS_DUAL_OUT               (((uint32_t)0x01) << UTU_UCSR_UTMS_Pos)
#define UTU_UCSR_UTMS_CAPTURE_IN             (((uint32_t)0x02) << UTU_UCSR_UTMS_Pos)
#define UTU_UCSR_UTMS_PULSE_OUT               UTU_UCSR_UTMS_Mask

#define UTU_UCSR_CLK_DEV_Pos                 ((uint32_t)0)                     /* Latency */
#define UTU_UCSR_CLK_DEV_Mask                (((uint32_t)0x07) << UTU_UCSR_CLK_DEV_Pos)
#define UTU_UCSR_CLK_DEV_NONE                (((uint32_t)0x00) << UTU_UCSR_CLK_DEV_Pos)
#define UTU_UCSR_CLK_DEV_2                   (((uint32_t)0x01) << UTU_UCSR_CLK_DEV_Pos)
#define UTU_UCSR_CLK_DEV_4                   (((uint32_t)0x02) << UTU_UCSR_CLK_DEV_Pos)
#define UTU_UCSR_CLK_DEV_8                   (((uint32_t)0x03) << UTU_UCSR_CLK_DEV_Pos)
#define UTU_UCSR_CLK_DEV_16                  (((uint32_t)0x04) << UTU_UCSR_CLK_DEV_Pos)
#define UTU_UCSR_CLK_DEV_32                  (((uint32_t)0x05) << UTU_UCSR_CLK_DEV_Pos)
#define UTU_UCSR_CLK_DEV_64                  (((uint32_t)0x06) << UTU_UCSR_CLK_DEV_Pos)
#define UTU_UCSR_CLK_DEV_128                 (((uint32_t)0x07) << UTU_UCSR_CLK_DEV_Pos)

/************************************* UTU->UCCSA/B  **************************************/
#define UTU_UCCS_CCL_EN_Pos                  ((uint32_t)7)                     /* Prefetch buffer status */
#define UTU_UCCS_CCL_EN_Mask                 (((uint32_t)0x01) << UTU_UCCS_CCL_EN_Pos)
#define UTU_UCCS_CCL_EN                      UTU_UCCS_CCL_EN_Mask
#define UTU_UCCS_SCSR_Pos                    ((uint32_t)4)                     /* Prefetch buffer enable */
#define UTU_UCCS_SCSR_Mask                   (((uint32_t)0x03) << UTU_UCCS_SCSR_Pos)
#define UTU_UCCS_SCSR                        UTU_UCCS_SCSR_Mask
#define UTU_UCCS_SCSR_SELF                   ((uint32_t)0x00 << UTU_UCCS_SCSR_Pos)
#define UTU_UCCS_SCSR_OTHER                  ((uint32_t)0x01 << UTU_UCCS_SCSR_Pos)
#define UTU_UCCS_CRS_EN_Pos                  ((uint32_t)3)                     /* Latency */
#define UTU_UCCS_CRS_EN_Mask                 (((uint32_t)0x01) << UTU_UCCS_CRS_EN_Pos)
#define UTU_UCCS_CRS_EN                      UTU_UCCS_CRS_EN_Mask
#define UTU_UCCS_CFS_EN_Pos                  ((uint32_t)2)                     /* Latency */
#define UTU_UCCS_CFS_EN_Mask                 (((uint32_t)0x01) << UTU_UCCS_CFS_EN_Pos)
#define UTU_UCCS_CFS_EN                      UTU_UCCS_CFS_EN_Mask
#define UTU_UCCS_POL_Pos                    ((uint32_t)0)                     /* Latency */
#define UTU_UCCS_POL_Mask                   (((uint32_t)0x01) << UTU_UCCS_POL_Pos)
#define UTU_UCSR_POL_INVERT                 UTU_UCCS_POL_Mask

/************************************* UTU->NFCR  **************************************/
#define UTU_NFCR_UPI_FLT_Pos                  ((uint32_t)8)                     /* Prefetch buffer status */
#define UTU_NFCR_UPI_FLT_Mask                 (((uint32_t)0xFF) << UTU_NFCR_UPI_FLT_Pos)
#define UTU_NFCR_UIO_FLT_Pos                  ((uint32_t)0)                     /* Prefetch buffer status */
#define UTU_NFCR_UIO_FLT_Mask                 (((uint32_t)0xFF) << UTU_NFCR_UIO_FLT_Pos)

/************************************* UTU->UTSS  **************************************/
#define UTU_UTSS_UTEN_Pos                     ((uint32_t)7)                     /* Prefetch buffer status */
#define UTU_UTSS_UTEN_Mask                    (((uint32_t)0x1) << UTU_UTSS_UTEN_Pos)
#define UTU_UTSS_UTEN                         (UTU_UTSS_UTEN_Mask)
#define UTU_UTSS_UTRGS_Pos                     ((uint32_t)0)                     /* Prefetch buffer status */
#define UTU_UTSS_UTRGS_Mask                    (((uint32_t)0xF) << UTU_UTSS_UTRGS_Pos)
#define UTU_UTSS_UTRGS_ATU_TRG0                (((uint32_t)0x00) << UTU_UTSS_UTRGS_Pos)
#define UTU_UTSS_UTRGS_ATU_TRG1                (((uint32_t)0x01) << UTU_UTSS_UTRGS_Pos)
#define UTU_UTSS_UTRGS_UTU1_TRG0               (((uint32_t)0x04) << UTU_UTSS_UTRGS_Pos)
#define UTU_UTSS_UTRGS_UTU2_TRG0               (((uint32_t)0x06) << UTU_UTSS_UTRGS_Pos)

/************************************* UTU->UPPS  **************************************/
#define UTU_UPPS_UDOB_Pos                     ((uint32_t)11)                     /* Prefetch buffer status */
#define UTU_UPPS_UDOB_Mask                    (((uint32_t)0x1) << UTU_UPPS_UDOB_Pos)
#define UTU_UPPS_UDOB_LEV_H                   (UTU_UPPS_UDOB_Mask)
#define UTU_UPPS_UDOA_Pos                     ((uint32_t)10)                     /* Prefetch buffer status */
#define UTU_UPPS_UDOA_Mask                    (((uint32_t)0x1) << UTU_UPPS_UDOA_Pos)
#define UTU_UPPS_UDOA_LEV_H                   (UTU_UPPS_UDOA_Mask)
#define UTU_UPPS_UPAC_Pos                     ((uint32_t)9)                     /* Prefetch buffer status */
#define UTU_UPPS_UPAC_Mask                    (((uint32_t)0x1) << UTU_UPPS_UPAC_Pos)
#define UTU_UPPS_UPAC_EN                      (UTU_UPPS_UPAC_Mask)
#define UTU_UPPS_UPOE_Pos                     ((uint32_t)8)                     /* Prefetch buffer status */
#define UTU_UPPS_UPOE_Mask                    (((uint32_t)0x1) << UTU_UPPS_UPOE_Pos)
#define UTU_UPPS_UPOE_EN                      (UTU_UPPS_UPOE_Mask)
#define UTU_UPPS_UPPC_Pos                     ((uint32_t)5)                     /* Prefetch buffer status */
#define UTU_UPPS_UPPC_Mask                    (((uint32_t)0x1) << UTU_UPPS_UPPC_Pos)
#define UTU_UPPS_UPPC_INVERT                  (UTU_UPPS_UPPC_Mask)
#define UTU_UPPS_UPHMS_Pos                     ((uint32_t)4)                     /* Prefetch buffer status */
#define UTU_UPPS_UPHMS_Mask                    (((uint32_t)0x1) << UTU_UPPS_UPHMS_Pos)
#define UTU_UPPS_UPHMS_PWM                    (UTU_UPPS_UPHMS_Mask)
#define UTU_UPPS_UPSS_Pos                     ((uint32_t)1)                     /* Prefetch buffer status */
#define UTU_UPPS_UPSS_Mask                    (((uint32_t)0x3) << UTU_UPPS_UPSS_Pos)
#define UTU_UPPS_UPSS_IO                      (((uint32_t)0x0) << UTU_UPPS_UPSS_Pos)
#define UTU_UPPS_UPSS_CMP1                    (((uint32_t)0x1) << UTU_UPPS_UPSS_Pos)
#define UTU_UPPS_UPSS_CMP2                    (((uint32_t)0x2) << UTU_UPPS_UPSS_Pos)
#define UTU_UPPS_UPE_Pos                      ((uint32_t)0)                     /* Prefetch buffer status */
#define UTU_UPPS_UPE_Mask                     (((uint32_t)0x1) << UTU_UPPS_UPE_Pos)
#define UTU_UPPS_UPE_EN                       (UTU_UPPS_UPE_Mask)

/************************************* UTU->UIER  **************************************/
#define UTU_UIER_UPP_IE_Pos                     ((uint32_t)5)                     /* Prefetch buffer status */
#define UTU_UIER_UPP_IE_Mask                    (((uint32_t)0x1) << UTU_UIER_UPP_IE_Pos)
#define UTU_UIER_UPP_IE_EN                      (UTU_UIER_UPP_IE_Mask)
#define UTU_UIER_OVF_IE_Pos                     ((uint32_t)4)                     /* Prefetch buffer status */
#define UTU_UIER_OVF_IE_Mask                    (((uint32_t)0x1) << UTU_UIER_OVF_IE_Pos)
#define UTU_UIER_OVF_IE_EN                      (UTU_UIER_OVF_IE_Mask)
#define UTU_UIER_UCFB_IE_Pos                     ((uint32_t)3)                     /* Prefetch buffer status */
#define UTU_UIER_UCFB_IE_Mask                    (((uint32_t)0x1) << UTU_UIER_UCFB_IE_Pos)
#define UTU_UIER_UCFB_IE_EN                      (UTU_UIER_UCFB_IE_Mask)
#define UTU_UIER_UCRB_IE_Pos                     ((uint32_t)2)                     /* Prefetch buffer status */
#define UTU_UIER_UCRB_IE_Mask                    (((uint32_t)0x1) << UTU_UIER_UCRB_IE_Pos)
#define UTU_UIER_UCRB_IE_EN                      (UTU_UIER_UCRB_IE_Mask)
#define UTU_UIER_UCFA_IE_Pos                     ((uint32_t)1)                     /* Prefetch buffer status */
#define UTU_UIER_UCFA_IE_Mask                    (((uint32_t)0x1) << UTU_UIER_UCFA_IE_Pos)
#define UTU_UIER_UCFA_IE_EN                      (UTU_UIER_UCFA_IE_Mask)
#define UTU_UIER_UCRA_IE_Pos                     ((uint32_t)0)                     /* Prefetch buffer status */
#define UTU_UIER_UCRA_IE_Mask                    (((uint32_t)0x1) << UTU_UIER_UCRA_IE_Pos)
#define UTU_UIER_UCRA_IE_EN                      (UTU_UIER_UCRA_IE_Mask)

/************************************* UTU->UIFR  **************************************/
#define UTU_UIFR_UPP_IE_Pos                     ((uint32_t)5)                     /* Prefetch buffer status */
#define UTU_UIFR_UPP_IE_Mask                    (((uint32_t)0x1) << UTU_UIFR_UPP_IE_Pos)
#define UTU_UIFR_UPP_IE_EN                      (UTU_UIFR_UPP_IE_Mask)
#define UTU_UIFR_OVF_IE_Pos                     ((uint32_t)4)                     /* Prefetch buffer status */
#define UTU_UIFR_OVF_IE_Mask                    (((uint32_t)0x1) << UTU_UIFR_OVF_IE_Pos)
#define UTU_UIFR_OVF_IE_EN                      (UTU_UIFR_OVF_IE_Mask)
#define UTU_UIFR_UCFB_IE_Pos                     ((uint32_t)3)                     /* Prefetch buffer status */
#define UTU_UIFR_UCFB_IE_Mask                    (((uint32_t)0x1) << UTU_UIFR_UCFB_IE_Pos)
#define UTU_UIFR_UCFB_IE_EN                      (UTU_UIFR_UCFB_IE_Mask)
#define UTU_UIFR_UCRB_IE_Pos                     ((uint32_t)2)                     /* Prefetch buffer status */
#define UTU_UIFR_UCRB_IE_Mask                    (((uint32_t)0x1) << UTU_UIFR_UCRB_IE_Pos)
#define UTU_UIFR_UCRB_IE_EN                      (UTU_UIFR_UCRB_IE_Mask)
#define UTU_UIFR_UCFA_IE_Pos                     ((uint32_t)1)                     /* Prefetch buffer status */
#define UTU_UIFR_UCFA_IE_Mask                    (((uint32_t)0x1) << UTU_UIFR_UCFA_IE_Pos)
#define UTU_UIFR_UCFA_IE_EN                      (UTU_UIFR_UCFA_IE_Mask)
#define UTU_UIFR_UCRA_IE_Pos                     ((uint32_t)0)                     /* Prefetch buffer status */
#define UTU_UIFR_UCRA_IE_Mask                    (((uint32_t)0x1) << UTU_UIFR_UCRA_IE_Pos)
#define UTU_UIFR_UCRA_IE_EN                      (UTU_UIFR_UCRA_IE_Mask)

/************************************* UTU->USR  **************************************/
#define UTU_USR_OHSF_Pos                        ((uint32_t)1)                     /* Prefetch buffer status */
#define UTU_USR_OHSF_Mask                       (((uint32_t)0x1) << UTU_USR_OHSF_Pos)
#define UTU_USR_OHSF                            (UTU_USR_OHSF_Mask)
#define UTU_USR_OLSF_Pos                        ((uint32_t)0)                     /* Prefetch buffer status */
#define UTU_USR_OLSF_Mask                       (((uint32_t)0x1) << UTU_USR_OLSF_Pos)
#define UTU_USR_OLSF                            (UTU_USR_OLSF_Mask)
#endif

/** @defgroup UTU_Exported_Types UTU_Exported_Types
  * @{
  */
/* Exported types ------------------------------------------------------------*/

/**
  * @brief  UTU Time Base Init structure definition
  */

typedef struct
{
    uint16_t UTU_Prescaler;         /*!< Specifies the prescaler value used to divide the UTU clock.
                                       This parameter can be a value of @ref UTU_Clock_Division_CKD */

    uint32_t UTU_Period;            /*!< Specifies the period value to be loaded into the active
                                       Auto-Reload Register at the next update event.
                                       For UTU1 This parameter range is 0x00000000 - 0xFFFFFFFF.
                                       For UTU2 This parameter range is 0x0000 - 0xFFFF. */

    uint32_t UTU_PeriodMode;         /*!< Specifies the repetition and only used in OC mode. */
    /*  uint32_t UTU_TrigCnt; */           /*!< Specifies the triger counter .
                                            For UTU1 This parameter range is 0x00000000 - 0xFFFFFFFF.
                                            For UTU2 This parameter range is 0x0000 - 0xFFFF. */
} UTU_TimeBaseInitTypeDef;

/**
  * @brief  UTU Output Compare Init structure definition
  */

typedef struct
{
    uint32_t UTU_Chnl;          /*!< Specifies the OC channel.
                                   This parameter can be a value of @ref UTU_Channel */
    uint32_t UTU_OCMode;        /*!< Specifies the UTU mode.
                                   This parameter can be a value of @ref UTU_Output_Compare_and_PWM_modes */

    uint32_t UTU_OCPolarity;    /*!< Specifies the output polarity.
                                   This parameter can be a value of @ref UTU_Output_Compare_Polarity */

    uint32_t UTU_CMP_Value;       /*!< Specifies the pulse value to be loaded into the UCCSA or UCCSB */

    //uint32_t UTU_DBT;               /*!< Specifies the dead bank time counter for UTU_OCMode_Dual mode */

} UTU_OCInitTypeDef;

/**
  * @brief  UTU Input Capture Init structure definition
  */

typedef struct
{

    uint16_t UTU_Channel;      /*!< Specifies the UTU channel.
                                  This parameter can be a value of @ref UTU_Channel */

    uint16_t UTU_ICPolarity;   /*!< Specifies the active edge of the input signal.
                                  This parameter can be a value of @ref UTU_Input_Capture_Polarity */

    uint16_t UTU_ICSelection;  /*!< Specifies the input.
                                  This parameter can be a value of @ref UTU_Input_Capture_Selection */

    uint8_t UTU_ICFilter;      /*!< Specifies the input capture filter.
                                  This parameter can be a number between 0x0 and 0xFF.
                                  T_FLT=8 ¡Á T_UTU1_CLK ¡Á 2^CLK_DIV ¡Á NFCR.UPI_FLT */

    uint16_t UTU_ClrCnt;       /*!< Specifies whether clear CNT after capture.
                                  This parameter can be a value of @ref UTU_Input_Capture_Clear_CNT */
} UTU_ICInitTypeDef;

/**
  * @brief  UTU_GUARD structure definition
  */

typedef struct
{

    uint16_t UTU_ChnlBGuardLevel;        /*!< Specifies the channel B guard level.
                                      This parameter can be a value of @ref UTU_Guard_Polarity */

    uint16_t UTU_ChnlAGuardLevel;        /*!< Specifies the channel A guard level.
                                      This parameter can be a value of @ref UTU_Guard_Polarity */

    uint16_t UTU_GuardAutoCancle;        /*!< Specifies the triger source .
                                      This parameter can be a value of @ref UTU_AOE_Bit_Set_Reset */

    uint16_t UTU_GuardEnPolarity;       /*!< Specifies the triger source polarity.
                                      This parameter can be a value of @ref UTU_Guard_En_Polarity  */

    uint16_t UTU_HaltGuardEn;           /*!< Specifies whether the UTU Halt GUard is enabled or not.
                                      This parameter can be a value of @ref UTU_Halt_Protect_enable_disable */

    uint16_t UTU_GuardSource;            /*!< Specifies which source triger the UTU guard feature.
                                      This parameter can be a value of @ref UTU_Guard_Source */

    uint16_t UTU_GuardEn;               /*!< Specifies whether the UTU Guard is enabled or not.
                                      This parameter can be a value of @ref UTU_Guard_enable_disable */

    uint8_t UTU_GuardFilter;           /*!< Specifies the GuardFilter timer.
                                      This parameter range is 00-FF, T_FLT=16 ¡Á T_UTU1_CLK ¡Á 2^CLK_DIV ¡Á (NFCR.UPI_FLT+1) */

} UTU_BDTRInitTypeDef;


/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup UTU_Exported_Constants UTU_Exported_Constants
  * @{
  */

/** @defgroup UTU_Perioherals_Group UTU_Perioherals_Group
  * @{
  */

#define IS_UTU_ALL_PERIPH(PERIPH)  (((PERIPH) == UTU1) || \
                                     ((PERIPH) == UTU2))


/**
  * @}
  */

/** @defgroup UTU_Triger_Source UTU_Triger_Source
  * @{
  */

#define UTU_TRG_S_ATU_RTG0                  (((uint32_t)0x00) << UTU_UTSS_UTRGS_Pos)
#define UTU_TRG_S_ATU_RTG1                  (((uint32_t)0x01) << UTU_UTSS_UTRGS_Pos)
#define UTU_TRG_S_UTU1_RTG0                 (((uint32_t)0x04) << UTU_UTSS_UTRGS_Pos)
#define UTU_TRG_S_UTU2_RTG0                 (((uint32_t)0x06) << UTU_UTSS_UTRGS_Pos)
#define IS_UTU_TRG_S(Source) (((Source) == UTU_TRG_S_ATU_RTG0) || \
                              ((Source) == UTU_TRG_S_ATU_RTG1) || \
                              ((Source) == UTU_TRG_S_UTU1_RTG0) || \
                              ((Source) == UTU_TRG_S_UTU2_RTG0))

/**
  * @}
  */


/** @defgroup UTU_Period_Mode UTU_Period_Mode
  * @{
  */

#define UTU_PRD_REPEAT                      (0)
#define UTU_PRD_ONCE                        (UTU_UCSR_SCWM)
#define IS_PRD_MODE(mode)   (((mode) == UTU_PRD_REPEAT) || \
                              ((mode) == UTU_PRD_ONCE))

/** @defgroup UTU_Output_Compare_and_PWM_modes UTU_Output_Compare_and_PWM_modes
  * @{
  */
#define UTU_OCMode_NORMAL                  (((uint32_t)0x00) << UTU_UCSR_UTMS_Pos)
#define UTU_OCMode_Dual                    (((uint32_t)0x01) << UTU_UCSR_UTMS_Pos)
#define UTU_OCMode_PULSE                   (((uint32_t)0x03) << UTU_UCSR_UTMS_Pos)
#define IS_UTU_OC_MODE(MODE) (((MODE) == UTU_OCMode_NORMAL) || \
                              ((MODE) == UTU_OCMode_Dual) || \
                              ((MODE) == UTU_OCMode_PULSE))
/**
  * @}
  */

/** @defgroup UTU_Period_Update_modes UTU_Period_Update_modes
  * @{
  */
#define UTU_Period_Up_Mode_Now               ((uint32_t)0)
#define UTU_Period_Up_Mode_Next              ((uint32_t)UTU_UCSR_ARPR)
#define IS_PERIOD_UP_MODE(MODE) (((MODE) == UTU_Period_Up_Mode_Now) || \
                              ((MODE) == UTU_Period_Up_Mode_Next))
/**
  * @}
  */


/** @defgroup UTU_CMP_Update_Mode UTU_CMP_Update_Mode
  * @{
  */

#define UTU_CMP_Update_Now                  ((uint32_t)0x0)
#define UTU_CMP_Update_Next                 ((uint32_t)UTU_UCSR_ARCR)
#define IS_CMP_Update_MODE(MODE) (((MODE) == UTU_CMP_Update_Now) || \
                               ((MODE) == UTU_CMP_Update_Next))
/**
  * @}
  */

/** @defgroup UTU_CC_Trigger_ADC UTU_CC_Trigger_ADC
  * @{
  */

#define CC_TRIGADC_OC                        ((uint32_t)0x00000000)
#define CC_TRIGADC_OCREF                     ((uint32_t)0x10000000)
#define CC_TRIGADC_MASK                      ((uint32_t)0x10000000)
#define IS_UTU_CC_TRIGADC_SOURCE(SOURCE)            (((SOURCE) == CC_TRIGADC_OC) || \
                                                                                             ((SOURCE) == CC_TRIGADC_OCREF))
/**
  * @}
  */

/** @defgroup UTU_Clock_Division_CKD UTU_Clock_Division_CKD
  * @{
  */
#define UTU_CKD_DIV1                       (((uint32_t)0x00) << UTU_UCSR_CLK_DIV_Pos)
#define UTU_CKD_DIV2                       (((uint32_t)0x01) << UTU_UCSR_CLK_DIV_Pos)
#define UTU_CKD_DIV4                       (((uint32_t)0x02) << UTU_UCSR_CLK_DIV_Pos)
#define UTU_CKD_DIV8                       (((uint32_t)0x03) << UTU_UCSR_CLK_DIV_Pos)
#define UTU_CKD_DIV16                      (((uint32_t)0x04) << UTU_UCSR_CLK_DIV_Pos)
#define UTU_CKD_DIV32                      (((uint32_t)0x05) << UTU_UCSR_CLK_DIV_Pos)
#define UTU_CKD_DIV64                      (((uint32_t)0x06) << UTU_UCSR_CLK_DIV_Pos)
#define UTU_CKD_DIV128                     (((uint32_t)0x07) << UTU_UCSR_CLK_DIV_Pos)

#define IS_UTU_CKD_DIV(DIV) (((DIV) == UTU_CKD_DIV1) || \
                             ((DIV) == UTU_CKD_DIV2) || \
                             ((DIV) == UTU_CKD_DIV4) || \
                             ((DIV) == UTU_CKD_DIV8) || \
                             ((DIV) == UTU_CKD_DIV16) || \
                             ((DIV) == UTU_CKD_DIV32) || \
                             ((DIV) == UTU_CKD_DIV64) || \
                             ((DIV) == UTU_CKD_DIV128) )
/**
  * @}
  */


/** @defgroup UTU_Output_Compare_Polarity UTU_Output_Compare_Polarity
  * @{
  */
#define UTU_UCSR_POL_INVERT                 UTU_UCCSA_POL_Mask


#define UTU_OCPolarity_Normal              ((uint32_t)0x0)
#define UTU_OCPolarity_Invert              (UTU_UCSR_POL_INVERT)
#define IS_UTU_OC_POLARITY(POLARITY) (((POLARITY) == UTU_OCPolarity_Normal) || \
                                      ((POLARITY) == UTU_OCPolarity_Invert))
/**
  * @}
  */

/** @defgroup UTU_Channel UTU_Channel
  * @{
  */

#define UTU_CHANNEL_A                  ((uint16_t)0x0000)
#define UTU_CHANNEL_B                  ((uint16_t)0x0001)
#define IS_UTU_CHANNEL(Chnl)             (((Chnl) == UTU_CHANNEL_A) || \
                                       ((Chnl) == UTU_CHANNEL_B))
/**
  * @}
  */
/** @defgroup UTU_DBT UTU_DBT
  * @{
  */

#define UTU_DBT_MASK                   ((uint32_t)0x7ff)
#define IS_UTU_DBT(Time)               ((Time & (~UTU_DBT_MASK)) == 0)
/**
  * @}
  */

/** @defgroup UTU_Output_Compare_state UTU_Output_Compare_state
  * @{
  */

#define UTU_OutputState_Disable            ((uint16_t)0x0000)
#define UTU_OutputState_Enable             ((uint16_t)0x0001)
#define IS_UTU_OUTPUT_STATE(STATE) (((STATE) == UTU_OutputState_Disable) || \
                                    ((STATE) == UTU_OutputState_Enable))
/**
  * @}
  */


/** @defgroup UTU_Guard_enable_disable UTU_Guard_enable_disable
  * @{
  */

#define UTU_Guard_Enable                   ((uint16_t)0x1000)
#define UTU_Guard_Disable                  ((uint16_t)0x0000)
#define IS_UTU_GUARD_STATE(STATE) (((STATE) == UTU_Guard_Enable) || \
                                   ((STATE) == UTU_Guard_Disable))
/**
  * @}
  */
/** @defgroup UTU_Halt_Protect_enable_disable UTU_Halt_Protect_enable_disable
  * @{
  */

#define UTU_Halt_Guard_Enable                   ((uint16_t)0x0010)
#define UTU_Halt_Guard_Disable                  ((uint16_t)0x0000)
#define IS_UTU_HALT_GUARD_STATE(STATE)  (((STATE) == UTU_Halt_Guard_Enable) || \
                                            ((STATE) == UTU_Halt_Guard_Disable))
/**
  * @}
  */
/** @defgroup UTU_DTG2_Value UTU_DTG2_Value
  * @{
  */

#define IS_UTU_DTG2_VALUE(VALUE)    (((VALUE) <= 0xFF))

/**
  * @}
  */
/** @defgroup UTU_Guard_Source UTU_Guard_Source
  * @{
  */
#define UTU_Guard_S_BKIN                       ((uint16_t)((uint32_t)0x0) << UTU_UPPS_UPSS_Pos)
#define UTU_Guard_S_VComp_1                    ((uint16_t)((uint32_t)0x1) << UTU_UPPS_UPSS_Pos)
#define UTU_Guard_S_VComp_2                    ((uint16_t)((uint32_t)0x2) << UTU_UPPS_UPSS_Pos)

#define IS_UTU_GUART_SOURCE(BKIN_SEL)           (((BKIN_SEL) == UTU_Guard_S_BKIN) || \
                                                  ((BKIN_SEL) == UTU_Guard_S_VComp_1) || \
                                                  ((BKIN_SEL) == UTU_Guard_S_VComp_2))

/**
  * @}
  */

/** @defgroup UTU_Guard_Polarity UTU_Guard_Polarity
  * @{
  */

#define UTU_CHNL_B_GuardPolarity_High             ((uint32_t)UTU_UPPS_UDOB_Mask)
#define UTU_CHNL_A_GuardPolarity_High             ((uint32_t)UTU_UPPS_UDOA_Mask)
#define UTU_CHNL_A_B_GuardPolarity_Low            ((uint32_t)0x0000)
#define IS_UTU_CHNL_GUARD_POLARITY(POLARITY) (((POLARITY) == UTU_CHNL_B_GuardPolarity_High) || \
                                          ((POLARITY) == UTU_CHNL_A_GuardPolarity_High) || \
                                         ((POLARITY) == UTU_CHNL_A_B_GuardPolarity_Low))
/**
  * @}
  */

/** @defgroup UTU_AOE_Bit_Set_Reset UTU_AOE_Bit_Set_Reset
  * @{
  */

#define UTU_AutomaticOutput_Enable         ((uint16_t)UTU_UPPS_UPAC_Mask)
#define UTU_AutomaticOutput_Disable        ((uint16_t)0x0000)
#define IS_UTU_AUTOMATIC_OUTPUT_STATE(STATE) (((STATE) == UTU_AutomaticOutput_Enable) || \
                                              ((STATE) == UTU_AutomaticOutput_Disable))
/**
  * @}
  */

/** @defgroup UTU_Guard_En_Polarity UTU_Guard_En_Polarity
  * @{
  */

#define UTU_GuardPolarity_Low             ((uint32_t)UTU_UPPS_UPPC)
#define UTU_GuardPolarity_High            ((uint32_t)0x0000)
#define IS_UTU_GUARD_EN_POLARITY(POLARITY) (((POLARITY) == UTU_GuardPolarity_High) || \
                                         ((POLARITY) == UTU_GuardPolarity_Low))
/**
  * @}
  */


/** @defgroup UTU_CMPR_State UTU_CMPR_State
  * @{
  */

#define UTU_CMPR_ENABLE              ((uint32_t)0x10000000)
#define UTU_CMPR_DISABLE             ((uint32_t)0x00000000)
#define IS_UTU_CMPR_STATE(STATE)        (((STATE) == UTU_CMPR_ENABLE) || \
                                     ((STATE) == UTU_CMPR_DISABLE))
/**
  * @}
  */

/** @defgroup UTU_Input_Capture_Polarity UTU_Input_Capture_Polarity
  * @{
  */

#define  UTU_ICPolarity_Rising             ((uint16_t)UTU_UCCSA_CRS_EN)
#define  UTU_ICPolarity_Falling            ((uint16_t)UTU_UCCSA_CFS_EN)
#define  UTU_ICPolarity_BothEdge           ((uint16_t)(UTU_ICPolarity_Rising | UTU_ICPolarity_Falling))
#define IS_UTU_IC_POLARITY(POLARITY) (((POLARITY) == UTU_ICPolarity_Rising) || \
                                      ((POLARITY) == UTU_ICPolarity_Falling)|| \
                                      ((POLARITY) == UTU_ICPolarity_BothEdge))
/**
  * @}
  */

/** @defgroup UTU_Input_Capture_Selection UTU_Input_Capture_Selection
  * @{
  */

#define UTU_ICSelection_IO_SELF             ((uint16_t)0x00 << UTU_UCCSA_SCSR_Pos) /*!< UTU Input  channel A/B is selected to be
                                                                   connected to IOA , IOB respectively */
#define UTU_ICSelection_IO_OTHER             ((uint16_t)0x01 << UTU_UCCSA_SCSR_Pos) /*!< UTU Input channel A/B is selected to be
                                                                   connected to IOB , IOA respectively. */
#define IS_UTU_IC_SELECTION(SELECTION) (((SELECTION) == UTU_ICSelection_IO_SELF) || \
                                        ((SELECTION) == UTU_ICSelection_IO_OTHER))
/**
  * @}
  */

/** @defgroup UTU_Input_Capture_Clear_CNT UTU_Input_Capture_Clear_CNT
  * @{
  */

#define UTU_IC_CNT_Clr                   ((uint16_t)UTU_UCCSA_CCL_EN) /*!< UTU Input  Capture is set to clear CNT */
#define UTU_IC_CNT_Not_Clr               ((uint16_t)0) /*!< UTU Input  Capture is set not to clear CNT. */
#define IS_UTU_IC_CNT_CLR_SET(SELECTION) (((SELECTION) == UTU_IC_CNT_Clr) || \
                                        ((SELECTION) == UTU_IC_CNT_Not_Clr))
/**
  * @}
  */



/** @defgroup UTU_interrupt_sources UTU_interrupt_sources
  * @{
  */

#define UTU_IT_UCRA                        ((uint16_t)0x0001)/*!< UTU Channel A rising Edge interrupt when in
                                                                   Compare or Capture mode */
#define UTU_IT_UCFA                        ((uint16_t)0x0002)/*!< UTU Channel A falling Edge interrupt when in
                                                                   Compare or Capture mode */
#define UTU_IT_UCRB                        ((uint16_t)0x0004)/*!< UTU Channel B rising Edge interrupt when in
                                                                   Compare or Capture mode */
#define UTU_IT_UCFB                         ((uint16_t)0x0008)/*!< UTU Channel B falling Edge interrupt when in
                                                                   Compare or Capture mode */

#define UTU_IT_OVF                         ((uint16_t)0x0010)/*!< UTU over flow interrupt  */
#define UTU_IT_UPP                         ((uint16_t)0x0020)/*!< UTU Guard interrupt  */
#define IS_UTU_IT(IT) ((((IT) & (uint16_t)0xFFC0) == 0x0000) && ((IT) != 0x0000))

#define IS_UTU_GET_IT(IT) (((IT) == UTU_IT_UCRA) || \
                           ((IT) == UTU_IT_UCFA) || \
                           ((IT) == UTU_IT_UCRB) || \
                           ((IT) == UTU_IT_UCFB) || \
                           ((IT) == UTU_IT_OVF) || \
                           ((IT) == UTU_IT_UPP))
/**
  * @}
  */


/** @defgroup UTU_Output_Compare_Preload_State UTU_Output_Compare_Preload_State
  * @{
  */

#define UTU_OCPreload_Enable               ((uint16_t)0x0008)
#define UTU_OCPreload_Disable              ((uint16_t)0x0000)
#define IS_UTU_OCPRELOAD_STATE(STATE) (((STATE) == UTU_OCPreload_Enable) || \
                                       ((STATE) == UTU_OCPreload_Disable))
/**
  * @}
  */

/** @defgroup UTU_Output_Compare_Fast_State UTU_Output_Compare_Fast_State
  * @{
  */

#define UTU_OCFast_Enable                  ((uint16_t)0x0004)
#define UTU_OCFast_Disable                 ((uint16_t)0x0000)
#define IS_UTU_OCFAST_STATE(STATE) (((STATE) == UTU_OCFast_Enable) || \
                                    ((STATE) == UTU_OCFast_Disable))

/**
  * @}
  */

/** @defgroup UTU_Output_Compare_Clear_State UTU_Output_Compare_Clear_State
  * @{
  */

#define UTU_OCClear_Enable                 ((uint16_t)0x0080)
#define UTU_OCClear_Disable                ((uint16_t)0x0000)
#define IS_UTU_OCCLEAR_STATE(STATE) (((STATE) == UTU_OCClear_Enable) || \
                                     ((STATE) == UTU_OCClear_Disable))
/**
  * @}
  */


/** @defgroup UTU_Flags UTU_Flags
  * @{
  */

#define UTU_FLAG_UCRA                       ((uint16_t)0x0001)
#define UTU_FLAG_UCFA                       ((uint16_t)0x0002)
#define UTU_FLAG_UCRB                       ((uint16_t)0x0004)
#define UTU_FLAG_UCFB                       ((uint16_t)0x0008)
#define UTU_FLAG_OVF                       ((uint16_t)0x0010)
#define UTU_FLAG_UPP                       ((uint16_t)0x0020)

#define IS_UTU_GET_FLAG(FLAG) (((FLAG) == UTU_FLAG_UCRA) || \
                               ((FLAG) == UTU_FLAG_UCFA) || \
                               ((FLAG) == UTU_FLAG_UCRB) || \
                               ((FLAG) == UTU_FLAG_UCFB) || \
                               ((FLAG) == UTU_FLAG_OVF) || \
                               ((FLAG) == UTU_FLAG_UPP))


#define IS_UTU_CLEAR_FLAG(UTU_FLAG) ((((UTU_FLAG) & (uint16_t)0xFFC0) == 0x0000) && ((UTU_FLAG) != 0x0000))
/**
  * @}
  */

/** @defgroup UTU_IC_Status UTU_IC_Status
  * @{
  */

#define UTU_IC_OHSF                         ((uint32_t)UTU_USR_OHSF)
#define UTU_IC_OLSF                         ((uint32_t)UTU_USR_OLSF)
/**
  * @}
  */


/** @defgroup UTU_Break_filter UTU_Break_filter
  * @{
  */
#if 0
#define UTU_Break_filter_0           0
#define UTU_Break_filter_1          UTU_BDTR_BKF_0
#define UTU_Break_filter_2          UTU_BDTR_BKF_1
#define UTU_Break_filter_3          UTU_BDTR_BKF_0|UTU_BDTR_BKF_1
#define UTU_Break_filter_4          UTU_BDTR_BKF_2
#define UTU_Break_filter_5          UTU_BDTR_BKF_2|UTU_BDTR_BKF_0
#define UTU_Break_filter_6          UTU_BDTR_BKF_2|UTU_BDTR_BKF_1
#define UTU_Break_filter_7          UTU_BDTR_BKF_2|UTU_BDTR_BKF_1|UTU_BDTR_BKF_0
#define UTU_Break_filter_8          UTU_BDTR_BKF_3
#define UTU_Break_filter_9          UTU_BDTR_BKF_3|UTU_BDTR_BKF_0
#define UTU_Break_filter_10         UTU_BDTR_BKF_3|UTU_BDTR_BKF_1
#define UTU_Break_filter_11         UTU_BDTR_BKF_3|UTU_BDTR_BKF_1|UTU_BDTR_BKF_0
#define UTU_Break_filter_12         UTU_BDTR_BKF_3|UTU_BDTR_BKF_2
#define UTU_Break_filter_13         UTU_BDTR_BKF_3|UTU_BDTR_BKF_2|UTU_BDTR_BKF_0
#define UTU_Break_filter_14         UTU_BDTR_BKF_3|UTU_BDTR_BKF_2|UTU_BDTR_BKF_1
#define UTU_Break_filter_15         UTU_BDTR_BKF_3|UTU_BDTR_BKF_2|UTU_BDTR_BKF_1|UTU_BDTR_BKF_0
#endif
/**
  * @}
  */



/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* TimeBase management ********************************************************/
void UTU_DeInit(UTU_TypeDef* UTUx);
void UTU_TimeBaseInit(UTU_TypeDef* UTUx, UTU_TimeBaseInitTypeDef* UTU_TimeBaseInitStruct);
void UTU_TimeBaseStructInit(UTU_TimeBaseInitTypeDef* UTU_TimeBaseInitStruct);
void UTU_PrescalerConfig(UTU_TypeDef* UTUx, uint16_t Prescaler);
uint32_t UTU_GetPrescaler(UTU_TypeDef* UTUx);
void UTU_CounterModeConfig(UTU_TypeDef* UTUx, uint16_t UTU_CounterMode);
void UTU_SetCounter(UTU_TypeDef* UTUx, uint32_t Counter);
void UTU_SetPeriod(UTU_TypeDef* UTUx, uint32_t Counter);
uint32_t UTU_GetCounter(UTU_TypeDef* UTUx);
uint32_t UTU_GetPeriod(UTU_TypeDef* UTUx);
uint32_t UTU_GetPeriodUpdateMode(UTU_TypeDef* UTUx);
void UTU_SetPeriodUpdateMode(UTU_TypeDef* UTUx, uint32_t NewMode);
void UTU_SetClockDivision(UTU_TypeDef* UTUx, uint16_t UTU_CKD);
void UTU_Cmd(UTU_TypeDef* UTUx, FunctionalState NewState);
void UTU_UEVShift(UTU_TypeDef* UTUx, FunctionalState NewState);
void UTU_InputTrigerConfig(UTU_TypeDef* UTUx, uint32_t TrigerSource);
void UTU_InputTrigerCmd(UTU_TypeDef* UTUx, FunctionalState NewState);
void UTU_SetOutputTrigerReg(UTU_TypeDef* UTUx, uint32_t TrigerCnt);
/* Advanced-control timers (UTU1) specific features*******************/
void UTU_BDTRConfig(UTU_TypeDef* UTUx, UTU_BDTRInitTypeDef *UTU_BDTRInitStruct);
void UTU_BDTRStructInit(UTU_BDTRInitTypeDef* UTU_BDTRInitStruct);
void UTU_CtrlPWMOutputs(UTU_TypeDef* UTUx, FunctionalState NewState);

/* Output Compare management **************************************************/
void UTU_OCInit(UTU_TypeDef* UTUx, UTU_OCInitTypeDef* UTU_OCInitStruct);
void UTU_OCStructInit(UTU_OCInitTypeDef* UTU_OCInitStruct);
void UTU_SelectOCxM(UTU_TypeDef* UTUx, uint16_t UTU_Channel, uint16_t UTU_OCMode);
void UTU_SetCompare(UTU_TypeDef* UTUx, uint32_t Compare1);
void UTU_OCCmd(UTU_TypeDef* UTUx, FunctionalState NewState);

void UTU_OCPolarityConfig(UTU_TypeDef* UTUx, uint16_t UTU_OCPolarity);
void UTU_OCCmpUpModeConfig(UTU_TypeDef* UTUx, uint32_t mode);
void UTU_OCSetCompare(UTU_TypeDef* UTUx, uint32_t Chnl, uint32_t Compare1);
void UTU_OCSetDBT(UTU_TypeDef* UTUx, uint32_t Chnl, uint32_t Value);


/* Output Compare guard management ********************************************/
void UTU_OCGuardConfig(UTU_TypeDef* UTUx, UTU_BDTRInitTypeDef *UTU_BDTRInitStruct);
void UTU_OCGuardStructInit(UTU_BDTRInitTypeDef* UTU_BDTRInitStruct);
void UTU_OCGuardCmd(UTU_TypeDef* UTUx, FunctionalState NewState);

/* Input Capture management ***************************************************/
void UTU_ICInit(UTU_TypeDef* UTUx, UTU_ICInitTypeDef* UTU_ICInitStruct);
void UTU_ICStructInit(UTU_ICInitTypeDef* UTU_ICInitStruct);
void UTU_PWMIConfig(UTU_TypeDef* UTUx, UTU_ICInitTypeDef* UTU_ICInitStruct);
uint32_t UTU_GetCapture(UTU_TypeDef* UTUx, uint32_t Chnl);
uint32_t UTU_GetCaptureStatus(UTU_TypeDef* UTUx);


/* Interrupts, and flags management ***************************************/
void UTU_ITConfig(UTU_TypeDef* UTUx, uint16_t UTU_IT, FunctionalState NewState);
FlagStatus UTU_GetFlagStatus(UTU_TypeDef* UTUx, uint16_t UTU_FLAG);
void UTU_ClearFlag(UTU_TypeDef* UTUx, uint16_t UTU_FLAG);
ITStatus UTU_GetITStatus(UTU_TypeDef* UTUx, uint16_t UTU_IT);
void UTU_ClearITPendingBit(UTU_TypeDef* UTUx, uint16_t UTU_IT);

/* Synchronization management *************************************************/
void UTU_SelectInputTrigger(UTU_TypeDef* UTUx, uint16_t UTU_InputTriggerSource);
void UTU_SelectOutputTrigger(UTU_TypeDef* UTUx, uint16_t UTU_TRGOSource);

/* Specific remapping management **********************************************/
void UTU_BrakInputRemap(UTU_TypeDef* UTUx, uint32_t RemapSourse);
void UTU_ETRInputRemap(UTU_TypeDef* UTUx, uint32_t RemapSourse);

/* UTU CC trigger ADC Select ***************************************************/
void UTU_CC_TRIGADC(UTU_TypeDef* UTUx, uint16_t UTU_CC_TRIGADC_Channel, uint32_t CC_Source);


#endif /*__HK32M07X_UTU_H */
/**
  * @}
  */

/**
  * @}
  */

/***************************** (C) COPYRIGHT HKMicroChip  *******************************/
