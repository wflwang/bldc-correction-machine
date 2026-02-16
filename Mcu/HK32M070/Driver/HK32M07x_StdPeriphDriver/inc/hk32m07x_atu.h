/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_atu.h
* @brief:     This file contains all the functions prototypes for the ATU firmware
*             library
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_ATU_H
#define __HK32M07X_ATU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup ATU
  * @{
  */

/** @defgroup ATU_Exported_Types ATU_Exported_Types
  * @{
  */

typedef enum {UP_DIR = 0, DOWN_DIR = 1} ATU_CounterDirection;

typedef struct
{
    uint8_t ATU_ClockDivision;     /*!< Specifies the prescaler value used to divide the ATU clock.
                                       This parameter can be one of 1,2,4,8 */

    uint8_t ATU_CounterMode;       /*!< Specifies the counter mode.
                                       This parameter can be a value of @ref ATU_COUNTER_MODE */

    uint16_t ATU_Period;            /*!< Specifies the period value to be loaded into the active
                                       Time_period Register at the next update event.
                                       This parameter must be a number between 0x0000 and 0xFFFF.  */

    uint16_t ATU_PeriodAutoReload;   /*!< Specifies the period value to be loaded into the active
                                       Time_period Register at the next update event.
                                       This parameter must be a number between 0x0000 and 0xFFFF.  */
} ATU_TimeBaseInitTypeDef;



/**
  * @brief  ATU Output PWM Init structure definition
  */

typedef struct
{
    uint16_t ATU_OutputMode;           /*!< independent£¬toggle£¬pwm */

    uint16_t ATU_CompareValuexA;       /*!< compare value A */

    uint16_t ATU_CompareValuexB;       /*!<  compare value B */

    uint16_t ATU_DeadBandTimeA;        /*!< dead band time A */

    uint16_t ATU_DeadBandTimeB;        /*!< dead band time B */

    uint16_t ATU_OutputPolarityA;      /*!< Specifies the complementary output polarity A */

    uint16_t ATU_OutputPolarityB;      /*!< Specifies the complementary output polarityB */ 

    uint16_t ATU_OutputSourceA;        /*!< compare CNT , the set level at TIOMSxA */

    uint16_t ATU_OutputSourceB;        /*!< compare CNT , the set level at TIOMSxB */       
 
    uint16_t ATU_OutputLevelSetA;      /*!< set level at TIOMSxA */

    uint16_t ATU_OutputLevelSetB;      /*!< set level at TIOMSxB */

    uint16_t ATU_CRxAReloadMode;       /*!< CRxA reload mode*/

    uint16_t ATU_CRxBReloadMode;       /*!< CRxB reload mode */

} ATU_OutputInitTypeDef;


/**
  * @brief  ATU_BDTR structure definition
  * @note   This sturcture is used only with ATU1.
  */

typedef struct
{

    uint16_t ATU_ProtectSource;    /*!< bkin,cmp1,cmp2 >*/

    uint16_t ATU_ProtectPolarity;  /*!< Specifies the protect-in polarity . */

    uint16_t ATU_ProtectCmd;       /*!< enable£¬disable */

    uint16_t ATU_Filter;           /*!< Specifies the protect-in digital Filter value  */

    uint16_t ATU_AutoClearProtect; /*!< auto clear protect status and then output pwm */

} ATU_ProtectIFInitTypeDef;


typedef struct
{
    uint32_t ATU_ProtectOutput0DefaultLevelA;    /*!<  Specifies the TIO0A output Default Level when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput0DefaultLevelB;    /*!<  Specifies the TIO0B output Default Level when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput1DefaultLevelA;    /*!<  Specifies the TIO1A output Default Level when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput1DefaultLevelB;    /*!<  Specifies the TIO1B output Default Level when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput2DefaultLevelA;    /*!<  Specifies the TIO2A output Default Level when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput2DefaultLevelB;    /*!<  Specifies the TIO2B output Default Level when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput0SelectA;          /*!<  Specifies the TIO0A output Source when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput0SelectB;          /*!<  Specifies the TIO0B output Source when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput1SelectA;          /*!<  Specifies the TIO1A output Source when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput1SelectB;          /*!<  Specifies the TIO1B output Source when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput2SelectA;          /*!<  Specifies the TIO2A output Source when ProtectIF is active >*/

    uint32_t ATU_ProtectOutput2SelectB;          /*!<  Specifies the TIO2B output Source when ProtectIF is active >*/

} ATU_ProtectOutputInitTypeDef;



/**
  * @}
  */
/**
  * @brief  ATU Input Capture Init structure definition
  */

/* Exported constants --------------------------------------------------------*/


/** @defgroup ATU_Exported_constants  ATU_Exported_constants
  * @{
  */
  
/** @defgroup ATU_OUTPUT_CHANNEL ATU_OUTPUT_CHANNEL 
  * @{
  */ 
#define ATU_PWMCHANNEL_0      ((uint16_t)0x0000)
#define ATU_PWMCHANNEL_1      ((uint16_t)0x0008)
#define ATU_PWMCHANNEL_2      ((uint16_t)0x0010)
#define IS_ATU_OUTPUT_CHANNEL(ATU_PWMCHANNEL_x)  (((ATU_PWMCHANNEL_x) == ATU_PWMCHANNEL_0) || \
                                                  ((ATU_PWMCHANNEL_x) == ATU_PWMCHANNEL_1) || \
                                                  ((ATU_PWMCHANNEL_x) == ATU_PWMCHANNEL_2))
/**
  * @}
  */ 

/** @defgroup ATU_COUNTER_MODE ATU_COUNTER_MODE 
  * @{
  */ 
#define ATU_COUNTER_MODE_UP                     ((uint8_t)0x00)
#define ATU_COUNTER_MODE_DOWM                   ((uint8_t)0x10)
#define ATU_COUNTER_MODE_UPDOWM                 ((uint8_t)0x20)
#define IS_ATU_COUNTER_MODE(ATU_CounterMode)    (((ATU_CounterMode) == ATU_COUNTER_MODE_UP    ) || \
                                                 ((ATU_CounterMode) == ATU_COUNTER_MODE_DOWM  ) || \
                                                 ((ATU_CounterMode) == ATU_COUNTER_MODE_UPDOWM))  
/**
  * @}
  */ 

/** @defgroup ATU_CKD_DIV ATU_CKD_DIV 
  * @{
  */ 
#define ATU_CLK_DIV_1                      ((uint8_t)0x00)
#define ATU_CLK_DIV_2                      ((uint8_t)0x01)
#define ATU_CLK_DIV_4                      ((uint8_t)0x02)
#define ATU_CLK_DIV_8                      ((uint8_t)0x03)
#define IS_ATU_CKD_DIV(ATU_ClockDiv)       (((ATU_ClockDiv) == ATU_CLK_DIV_1 ) || \
                                            ((ATU_ClockDiv) == ATU_CLK_DIV_2 ) || \
                                            ((ATU_ClockDiv) == ATU_CLK_DIV_4 ) || \
                                            ((ATU_ClockDiv) == ATU_CLK_DIV_8 ))  
/**
  * @}
  */ 
 

/** @defgroup ATU_OUTPUT_MODE ATU_OUTPUT_MODE 
  * @{
  */  
#define ATU_OUTPUT_INDEPENDENT_PWM           ((uint16_t)0x0000)
#define ATU_OUTPUT_COMPLEMENTARY_PWM         ((uint16_t)0x0040)
#define ATU_OUTPUT_INDEPENDENT_TOGGLE        ((uint16_t)0x0080)
#define IS_ATU_OUTPUT_MODE(ATU_OutputMode)   (((ATU_OutputMode) == ATU_OUTPUT_INDEPENDENT_PWM   ) || \
                                              ((ATU_OutputMode) == ATU_OUTPUT_COMPLEMENTARY_PWM ) || \
                                              ((ATU_OutputMode) == ATU_OUTPUT_INDEPENDENT_TOGGLE))
/**
  * @}
  */ 


/** @defgroup ATU_OUTPUT_POLARITYA ATU_OUTPUT_POLARITYA 
  * @{
  */ 
#define ATU_OUTPUT_POLARITYA_NORMAL                     ((uint16_t)0x0000)
#define ATU_OUTPUT_POLARITYA_REVERSE                    ((uint16_t)0x0010)
#define IS_ATU_OUTPUT_POLARITYA(ATU_OutputPolarity)     (((ATU_OutputPolarity) == ATU_OUTPUT_POLARITYA_NORMAL ) || \
                                                         ((ATU_OutputPolarity) == ATU_OUTPUT_POLARITYA_REVERSE))
/**
  * @}
  */ 


/** @defgroup ATU_OUTPUT_POLARITYB ATU_OUTPUT_POLARITYB
  * @{
  */ 
#define ATU_OUTPUT_POLARITYB_NORMAL                     ((uint16_t)0x0000)
#define ATU_OUTPUT_POLARITYB_REVERSE                    ((uint16_t)0x0020)
#define IS_ATU_OUTPUT_POLARITYB(ATU_OutputPolarity)     (((ATU_OutputPolarity) == ATU_OUTPUT_POLARITYB_NORMAL ) || \
                                                         ((ATU_OutputPolarity) == ATU_OUTPUT_POLARITYB_REVERSE))
/**
  * @}
  */ 


/** @defgroup ATU_OUTPUT_SOURCEA ATU_OUTPUT_SOURCEA 
  * @{
  */ 
#define ATU_OUTPUT_SOURCEA_COUNTER                   ((uint16_t)0x0000)
#define ATU_OUTPUT_SOURCEA_IO_SET                    ((uint16_t)0x0004)
#define IS_ATU_OUTPUT_SOURCEA(ATU_OutputSource)      (((ATU_OutputSource) == ATU_OUTPUT_SOURCEA_COUNTER) || \
                                                      ((ATU_OutputSource) == ATU_OUTPUT_SOURCEA_IO_SET ))
/**
  * @}
  */ 


/** @defgroup ATU_OUTPUT_SOURCEB ATU_OUTPUT_SOURCEB 
  * @{
  */ 
#define ATU_OUTPUT_SOURCEB_COUNTER                   ((uint16_t)0x0000)
#define ATU_OUTPUT_SOURCEB_IO_SET                    ((uint16_t)0x0008)
#define IS_ATU_OUTPUT_SOURCEB(ATU_OutputSource)      (((ATU_OutputSource) == ATU_OUTPUT_SOURCEB_COUNTER) || \
                                                      ((ATU_OutputSource) == ATU_OUTPUT_SOURCEB_IO_SET ))
/**
  * @}
  */ 


/** @defgroup ATU_OUTPUT_LEVEL_SETA ATU_OUTPUT_LEVEL_SETA 
  * @{
  */ 
#define ATU_OUTPUT_LEVEL_SETA_LOW                         ((uint16_t)0x0000)
#define ATU_OUTPUT_LEVEL_SETA_HIGH                        ((uint16_t)0x0001)
#define IS_ATU_OUTPUT_LEVEL_SETA(ATU_OutputSetLevel)      (((ATU_OutputSetLevel) == ATU_OUTPUT_LEVEL_SETA_LOW ) || \
                                                           ((ATU_OutputSetLevel) == ATU_OUTPUT_LEVEL_SETA_HIGH))
/**
  * @}
  */ 


/** @defgroup IS_ATU_OUTPUT_LEVEL_SETB IS_ATU_OUTPUT_LEVEL_SETB 
  * @{
  */ 
#define ATU_OUTPUT_LEVEL_SETB_LOW                         ((uint16_t)0x0000)
#define ATU_OUTPUT_LEVEL_SETB_HIGH                        ((uint16_t)0x0002)
#define IS_ATU_OUTPUT_LEVEL_SETB(ATU_OutputSetLevel)      (((ATU_OutputSetLevel) == ATU_OUTPUT_LEVEL_SETB_LOW ) || \
                                                           ((ATU_OutputSetLevel) == ATU_OUTPUT_LEVEL_SETB_HIGH))
/**
  * @}
  */ 


/** @defgroup ATU_CRxA_RELOAD_MODE ATU_CRxA_RELOAD_MODE 
  * @{
  */ 
#define ATU_CRxA_IMMEDIATELY_RELOAD                 ((uint16_t)0x0000)
#define ATU_CRxA_AUTO_RELOAD                        ((uint16_t)0x0001)
#define IS_ATU_CRxA_RELOAD_MODE(ATU_ReloadMode)     (((ATU_ReloadMode) == ATU_CRxA_IMMEDIATELY_RELOAD) || \
                                                     ((ATU_ReloadMode) == ATU_CRxA_AUTO_RELOAD       ))
/**
  * @}
  */ 

/** @defgroup ATU_CRxB_RELOAD_MODE ATU_CRxB_RELOAD_MODE 
  * @{
  */ 
#define ATU_CRxB_IMMEDIATELY_RELOAD                 ((uint16_t)0x0000)
#define ATU_CRxB_AUTO_RELOAD                        ((uint16_t)0x0002)
#define IS_ATU_CRxB_RELOAD_MODE(ATU_ReloadMode)     (((ATU_ReloadMode) == ATU_CRxB_IMMEDIATELY_RELOAD) || \
                                                     ((ATU_ReloadMode) == ATU_CRxB_AUTO_RELOAD       ))
/**
  * @}
  */ 


/** @defgroup ATU_PERIOD_RELOAD_MODE ATU_PERIOD_RELOAD_MODE 
  * @{
  */ 
#define ATU_PERIOD_IMMEDIATELY_RELOAD                 ((uint16_t)0x0000)
#define ATU_PERIOD_AUTO_RELOAD                        ((uint16_t)0x2000)
#define IS_ATU_PERIOD_RELOAD_MODE(ATU_ReloadMode)     (((ATU_ReloadMode) == ATU_PERIOD_IMMEDIATELY_RELOAD) || \
                                                       ((ATU_ReloadMode) == ATU_PERIOD_AUTO_RELOAD       ))
/**
  * @}
  */ 


/** @defgroup ATU_CNT_RELOAD_MODE ATU_CNT_RELOAD_MODE 
  * @{
  */ 
#define ATU_CNT_IMMEDIATELY_RELOAD                 ((uint16_t)0x0000)
#define ATU_CNT_AUTO_RELOAD                        ((uint16_t)0x1000)
#define IS_ATU_CNT_RELOAD_MODE(ATU_ReloadMode)     (((ATU_ReloadMode) == ATU_CNT_IMMEDIATELY_RELOAD) || \
                                                    ((ATU_ReloadMode) == ATU_CNT_AUTO_RELOAD       ))
/**
  * @}
  */ 


/** @defgroup ATU_TRIGGER_DOUBLE_DATAB_RELOAD_MODE ATU_TRIGGER_DOUBLE_DATAB_RELOAD_MODE 
  * @{
  */ 
#define ATU_TRIGGER_DOUBLE_DATAB_IMMEDIATELY_RELOAD                 ((uint16_t)0x0000)
#define ATU_TRIGGER_DOUBLE_DATAB_AUTO_RELOAD                        ((uint16_t)0x0800)
#define IS_ATU_TRIGGER_DOUBLE_DATAB_RELOAD_MODE(ATU_ReloadMode)     (((ATU_ReloadMode) == ATU_TRIGGER_DOUBLE_DATAB_IMMEDIATELY_RELOAD) || \
                                                                     ((ATU_ReloadMode) == ATU_TRIGGER_DOUBLE_DATAB_AUTO_RELOAD       ))
/**
  * @}
  */ 


/** @defgroup ATU_TRG1_RELOAD_MODE ATU_TRG1_RELOAD_MODE 
  * @{
  */ 
#define ATU_TRG1_IMMEDIATELY_RELOAD                 ((uint16_t)0x0000)
#define ATU_TRG1_AUTO_RELOAD                        ((uint16_t)0x0200)
#define IS_ATU_TRG1_RELOAD_MODE(ATU_ReloadMode)     (((ATU_ReloadMode) == ATU_TRG1_IMMEDIATELY_RELOAD) || \
                                                     ((ATU_ReloadMode) == ATU_TRG1_AUTO_RELOAD       ))
/**
  * @}
  */ 


/** @defgroup ATU_TRG0_RELOAD_MODE ATU_TRG0_RELOAD_MODE 
  * @{
  */ 
#define ATU_TRG0_IMMEDIATELY_RELOAD                 ((uint16_t)0x0000)
#define ATU_TRG0_AUTO_RELOAD                        ((uint16_t)0x0100)
#define IS_ATU_TRG0_RELOAD_MODE(ATU_ReloadMode)     (((ATU_ReloadMode) == ATU_TRG0_IMMEDIATELY_RELOAD) || \
                                                     ((ATU_ReloadMode) == ATU_TRG0_AUTO_RELOAD       ))
/**
  * @}
  */ 


/** @defgroup ATU_EXT_TRIG_SOURCE ATU_EXT_TRIG_SOURCE 
  * @{
  */ 
#define ATU_EXT_TRIG_UTU1_TRGO                         ((uint16_t)0x0004)
#define ATU_EXT_TRIG_UTU2_TRGO                         ((uint16_t)0x0006)
#define IS_ATU_EXT_TRIG_SOURCE(ATU_ExternalSource)     (((ATU_ExternalSource) == ATU_EXT_TRIG_UTU1_TRGO) || \
                                                        ((ATU_ExternalSource) == ATU_EXT_TRIG_UTU2_TRGO))
/**
  * @}
  */ 


/** @defgroup ATU_TRG0_POINT ATU_TRG0_POINT 
  * @{
  */ 
#define ATU_TRG0_POINT_UP                          ((uint16_t)0x0000)
#define ATU_TRG0_POINT_DOWN                        ((uint16_t)0x0001)
#define ATU_TRG0_POINT_UPDOWN                      ((uint16_t)0x0002)
#define IS_ATU_TRG0_POINT(ATU_TriggerPoint)        (((ATU_TriggerPoint) == ATU_TRG0_POINT_UP    ) || \
                                                    ((ATU_TriggerPoint) == ATU_TRG0_POINT_DOWN  ) || \
                                                    ((ATU_TriggerPoint) == ATU_TRG0_POINT_UPDOWN))
/**
  * @}
  */ 


/** @defgroup ATU_TRG1_POINT ATU_TRG1_POINT 
  * @{
  */ 
#define ATU_TRG1_POINT_UP                          ((uint16_t)0x0000)
#define ATU_TRG1_POINT_DOWN                        ((uint16_t)0x0004)
#define ATU_TRG1_POINT_UPDOWN                      ((uint16_t)0x0008)
#define IS_ATU_TRG1_POINT(ATU_TriggerPoint)        (((ATU_TriggerPoint) == ATU_TRG1_POINT_UP    ) || \
                                                    ((ATU_TriggerPoint) == ATU_TRG1_POINT_DOWN  ) || \
                                                    ((ATU_TriggerPoint) == ATU_TRG1_POINT_UPDOWN))
/**
  * @}
  */ 


/** @defgroup ATU_TRGDB_POINT ATU_TRGDB_POINT 
  * @{
  */ 
#define ATU_TRGDB_POINT_UP                         ((uint16_t)0x0000)
#define ATU_TRGDB_POINT_DOWN                       ((uint16_t)0x0040)
#define ATU_TRGDB_POINT_UPDOWN                     ((uint16_t)0x0080)
#define IS_ATU_TRGDB_POINT(ATU_TriggerPoint)       (((ATU_TriggerPoint) == ATU_TRGDB_POINT_UP    ) || \
                                                    ((ATU_TriggerPoint) == ATU_TRGDB_POINT_DOWN  ) || \
                                                    ((ATU_TriggerPoint) == ATU_TRGDB_POINT_UPDOWN))
/**
  * @}
  */ 


/** @defgroup ATU_PWMIO_CHNANEL ATU_PWMIO_CHNANEL 
  * @{
  */ 
#define ATU_PWMIO_CHNANEL_0                        ((uint16_t)0x0000)
#define ATU_PWMIO_CHNANEL_1                        ((uint16_t)0x0004)
#define ATU_PWMIO_CHNANEL_2                        ((uint16_t)0x0008)
#define ATU_PWMIO_CHNANEL_3                        ((uint16_t)0x000C)
#define ATU_PWMIO_CHNANEL_4                        ((uint16_t)0x0010)
#define ATU_PWMIO_CHNANEL_5                        ((uint16_t)0x0014)
#define IS_ATU_PWMIO_CHNANEL(ATU_IOChannel)        (((ATU_IOChannel) == ATU_PWMIO_CHNANEL_0 ) || \
                                                    ((ATU_IOChannel) == ATU_PWMIO_CHNANEL_1 ) || \
                                                    ((ATU_IOChannel) == ATU_PWMIO_CHNANEL_2 ) || \
                                                    ((ATU_IOChannel) == ATU_PWMIO_CHNANEL_3 ) || \
                                                    ((ATU_IOChannel) == ATU_PWMIO_CHNANEL_4 ) || \
                                                    ((ATU_IOChannel) == ATU_PWMIO_CHNANEL_5 ))
/**
  * @}
  */ 


/** @defgroup ATU_PWM_REMAP_SOURCE ATU_PWM_REMAP_SOURCE 
  * @{
  */ 
#define ATU_PWM_REMAP_SOURCE_TIO0A                        ((uint16_t)0x0000)
#define ATU_PWM_REMAP_SOURCE_TIO0B                        ((uint16_t)0x0001)
#define ATU_PWM_REMAP_SOURCE_TIO1A                        ((uint16_t)0x0002)
#define ATU_PWM_REMAP_SOURCE_TIO1B                        ((uint16_t)0x0003)
#define ATU_PWM_REMAP_SOURCE_TIO2A                        ((uint16_t)0x0004)
#define ATU_PWM_REMAP_SOURCE_TIO2B                        ((uint16_t)0x0005)
#define IS_ATU_PWM_REMAP_SOURCE(ATU_RemapSource)          (((ATU_RemapSource) == ATU_PWM_REMAP_SOURCE_TIO0A ) || \
                                                           ((ATU_RemapSource) == ATU_PWM_REMAP_SOURCE_TIO0B ) || \
                                                           ((ATU_RemapSource) == ATU_PWM_REMAP_SOURCE_TIO1A ) || \
                                                           ((ATU_RemapSource) == ATU_PWM_REMAP_SOURCE_TIO1B ) || \
                                                           ((ATU_RemapSource) == ATU_PWM_REMAP_SOURCE_TIO2A ) || \
                                                           ((ATU_RemapSource) == ATU_PWM_REMAP_SOURCE_TIO2B ))
/**
  * @}
  */ 


/** @defgroup ATU_CREST_RELOAD_TIMING_POINT ATU_CREST_RELOAD_TIMING_POINT 
  * @{
  */ 
#define ATU_CREST_RELOAD_EVERY_1_EVENT                                 ((uint16_t)0x0000)
#define ATU_CREST_RELOAD_EVERY_2_EVENT                                 ((uint16_t)0x0001)
#define ATU_CREST_RELOAD_EVERY_3_EVENT                                 ((uint16_t)0x0002)
#define ATU_CREST_RELOAD_EVERY_4_EVENT                                 ((uint16_t)0x0003)
#define IS_ATU_CREST_RELOAD_TIMING_POINT(ATU_ReloadTimingPoint)        (((ATU_ReloadTimingPoint) == ATU_CREST_RELOAD_EVERY_1_EVENT) || \
                                                                        ((ATU_ReloadTimingPoint) == ATU_CREST_RELOAD_EVERY_2_EVENT) || \
                                                                        ((ATU_ReloadTimingPoint) == ATU_CREST_RELOAD_EVERY_3_EVENT) || \
                                                                        ((ATU_ReloadTimingPoint) == ATU_CREST_RELOAD_EVERY_4_EVENT))
/**
  * @}
  */ 


/** @defgroup ATU_TROUGH_RELOAD_TIMING_POINT ATU_TROUGH_RELOAD_TIMING_POINT 
  * @{
  */ 
#define ATU_TROUGH_RELOAD_EVERY_1_EVENT                                ((uint16_t)0x0000)
#define ATU_TROUGH_RELOAD_EVERY_2_EVENT                                ((uint16_t)0x0010)
#define ATU_TROUGH_RELOAD_EVERY_3_EVENT                                ((uint16_t)0x0020)
#define ATU_TROUGH_RELOAD_EVERY_4_EVENT                                ((uint16_t)0x0030)
#define IS_ATU_TROUGH_RELOAD_TIMING_POINT(ATU_ReloadTimingPoint)       (((ATU_ReloadTimingPoint) == ATU_TROUGH_RELOAD_EVERY_1_EVENT ) || \
                                                                        ((ATU_ReloadTimingPoint) == ATU_TROUGH_RELOAD_EVERY_2_EVENT ) || \
                                                                        ((ATU_ReloadTimingPoint) == ATU_TROUGH_RELOAD_EVERY_3_EVENT ) || \
                                                                        ((ATU_ReloadTimingPoint) == ATU_TROUGH_RELOAD_EVERY_4_EVENT ))
/**
  * @}
  */ 


/** @defgroup ATU_PROTECT_IF ATU_PROTECT_IF 
  * @{
  */ 
#define ATU_PROTECT_IF_0                                ((uint16_t)0x0000)
#define ATU_PROTECT_IF_1                                ((uint16_t)0x0008)
#define IS_ATU_PROTECT_IF(ATU_ProtectIF)                (((ATU_ProtectIF) == ATU_PROTECT_IF_0 ) || \
                                                         ((ATU_ProtectIF) == ATU_PROTECT_IF_1 ))
/**
  * @}
  */ 



/** @defgroup ATU_PROTECT_SOURCE ATU_PROTECT_SOURCE 
  * @{
  */ 
#define ATU_PROTECT_SOURCE_BKIN                                ((uint16_t)0x0000)
#define ATU_PROTECT_SOURCE_CMP1                                ((uint16_t)0x0002)
#define ATU_PROTECT_SOURCE_CMP2                                ((uint16_t)0x0004)
#define IS_ATU_PROTECT_SOURCE(ATU_ProtectSource)               (((ATU_ProtectSource) == ATU_PROTECT_SOURCE_BKIN ) || \
                                                                ((ATU_ProtectSource) == ATU_PROTECT_SOURCE_CMP1 ) || \
                                                                ((ATU_ProtectSource) == ATU_PROTECT_SOURCE_CMP2 ))
/**
  * @}
  */ 


/** @defgroup ATU_PROTECT_POLARITY ATU_PROTECT_POLARITY 
  * @{
  */ 
#define ATU_PROTECT_POLARITY_NORMAL                             ((uint16_t)0x0000)
#define ATU_PROTECT_POLARITY_REVERSE                            ((uint16_t)0x0020)
#define IS_ATU_PROTECT_POLARITY(ATU_ProtectPolarity)            (((ATU_ProtectPolarity) == ATU_PROTECT_POLARITY_NORMAL ) || \
                                                                 ((ATU_ProtectPolarity) == ATU_PROTECT_POLARITY_REVERSE))
/**
  * @}
  */ 



/** @defgroup ATU_PROTECT_STATE ATU_PROTECT_STATE 
  * @{
  */ 
#define ATU_PROTECT_STATE_DISABLE                            ((uint16_t)0x0000)
#define ATU_PROTECT_STATE_ENABLE                             ((uint16_t)0x0001)
#define IS_ATU_PROTECT_STATE(ATU_ProtrctState)               (((ATU_ProtrctState) == ATU_PROTECT_STATE_DISABLE) || \
                                                              ((ATU_ProtrctState) == ATU_PROTECT_STATE_ENABLE ))
/**
  * @}
  */ 

/** @defgroup ATU_PROTECT_FILTER ATU_PROTECT_FILTER 
  * @{
  */ 
#define IS_ATU_PROTECT_FILTER(ATU_ProtrctFilter)             ((ATU_ProtrctFilter) <= 0xFF)

/**
  * @}
  */ 


/** @defgroup ATU_PROTECT_AUTOCLEAR ATU_PROTECT_AUTOCLEAR 
  * @{
  */ 
#define ATU_PROTECT_AUTOCLEAR_DISABLE                         ((uint16_t)0x0000)
#define ATU_PROTECT_AUTOCLEAR_ENABLE                          ((uint16_t)0x0080)
#define IS_ATU_PROTECT_AUTOCLEAR(ATU_ProtrctAutoClear)        (((ATU_ProtrctAutoClear) == ATU_PROTECT_AUTOCLEAR_DISABLE) || \
                                                               ((ATU_ProtrctAutoClear) == ATU_PROTECT_AUTOCLEAR_ENABLE ))
/**
  * @}
  */ 

/** @defgroup ATU_PROTECT_DEFAULT_LEVEL_TIO0A ATU_PROTECT_DEFAULT_LEVEL_TIO0A 
  * @{
  */ 
#define ATU_PROTECT_TIO0A_DEFAULT_LEVEL_LOW                        ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO0A_DEFAULT_LEVEL_HIGH                       ((uint32_t)0x00010000)
#define IS_ATU_PROTECT_DEFAULT_LEVEL_TIO0A(ATU_DefaultLevelA)      (((ATU_DefaultLevelA) == ATU_PROTECT_TIO0A_DEFAULT_LEVEL_LOW ) || \
                                                                    ((ATU_DefaultLevelA) == ATU_PROTECT_TIO0A_DEFAULT_LEVEL_HIGH))
/**
  * @}
  */ 


/** @defgroup ATU_PROTECT_DEFAULT_LEVEL_TIO0B ATU_PROTECT_DEFAULT_LEVEL_TIO0B 
  * @{
  */ 
#define ATU_PROTECT_TIO0B_DEFAULT_LEVEL_LOW                        ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO0B_DEFAULT_LEVEL_HIGH                       ((uint32_t)0x00020000)
#define IS_ATU_PROTECT_DEFAULT_LEVEL_TIO0B(ATU_DefaultLevelB)      (((ATU_DefaultLevelB) == ATU_PROTECT_TIO0B_DEFAULT_LEVEL_LOW ) || \
                                                                    ((ATU_DefaultLevelB) == ATU_PROTECT_TIO0B_DEFAULT_LEVEL_HIGH))
/**
  * @}
  */ 

/** @defgroup ATU_PROTECT_OUTPUT_SELECT_TIO0A ATU_PROTECT_OUTPUT_SELECT_TIO0A 
  * @{
  */ 
#define ATU_PROTECT_TIO0A_OUTPUT_SELECT_DEFAULT_LEVLE                 ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO0A_OUTPUT_SELECT_TPOC_SETTING                  ((uint32_t)0x01000000)
#define IS_ATU_PROTECT_OUTPUT_SELECT_TIO0A(ProtectOutputSelA)         (((ProtectOutputSelA) == ATU_PROTECT_TIO0A_OUTPUT_SELECT_DEFAULT_LEVLE) || \
                                                                       ((ProtectOutputSelA) == ATU_PROTECT_TIO0A_OUTPUT_SELECT_TPOC_SETTING ))

/** @defgroup ATU_PROTECT_OUTPUT_SELECT_TIO0B ATU_PROTECT_OUTPUT_SELECT_TIO0B 
  * @{
  */ 
#define ATU_PROTECT_TIO0B_OUTPUT_SELECT_DEFAULT_LEVLE                 ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO0B_OUTPUT_SELECT_TPOC_SETTING                  ((uint32_t)0x02000000)
#define IS_ATU_PROTECT_OUTPUT_SELECT_TIO0B(ProtectOutputSelB)         (((ProtectOutputSelB) == ATU_PROTECT_TIO0B_OUTPUT_SELECT_DEFAULT_LEVLE) || \
                                                                       ((ProtectOutputSelB) == ATU_PROTECT_TIO0B_OUTPUT_SELECT_TPOC_SETTING ))
/**
  * @}
  */ 


/** @defgroup ATU_PROTECT_DEFAULT_LEVEL_TIO1A ATU_PROTECT_DEFAULT_LEVEL_TIO1A 
  * @{
  */ 
#define ATU_PROTECT_TIO1A_DEFAULT_LEVEL_LOW                        ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO1A_DEFAULT_LEVEL_HIGH                       ((uint32_t)0x00040000)
#define IS_ATU_PROTECT_DEFAULT_LEVEL_TIO1A(ATU_DefaultLevelA)      (((ATU_DefaultLevelA) == ATU_PROTECT_TIO1A_DEFAULT_LEVEL_LOW ) || \
                                                                    ((ATU_DefaultLevelA) == ATU_PROTECT_TIO1A_DEFAULT_LEVEL_HIGH))
/**
  * @}
  */ 

/** @defgroup ATU_PROTECT_DEFAULT_LEVEL_TIO1B ATU_PROTECT_DEFAULT_LEVEL_TIO1B 
  * @{
  */ 
#define ATU_PROTECT_TIO1B_DEFAULT_LEVEL_LOW                        ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO1B_DEFAULT_LEVEL_HIGH                       ((uint32_t)0x00080000)
#define IS_ATU_PROTECT_DEFAULT_LEVEL_TIO1B(ATU_DefaultLevelB)      (((ATU_DefaultLevelB) == ATU_PROTECT_TIO1B_DEFAULT_LEVEL_LOW ) || \
                                                                    ((ATU_DefaultLevelB) == ATU_PROTECT_TIO1B_DEFAULT_LEVEL_HIGH))
/**
  * @}
  */ 

/** @defgroup ATU_PROTECT_OUTPUT_SELECT_TIO1A ATU_PROTECT_OUTPUT_SELECT_TIO1A 
  * @{
  */ 
#define ATU_PROTECT_TIO1A_OUTPUT_SELECT_DEFAULT_LEVLE                 ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO1A_OUTPUT_SELECT_TPOC_SETTING                  ((uint32_t)0x04000000)
#define IS_ATU_PROTECT_OUTPUT_SELECT_TIO1A(ProtectOutputSelA)         (((ProtectOutputSelA) == ATU_PROTECT_TIO1A_OUTPUT_SELECT_DEFAULT_LEVLE) || \
                                                                       ((ProtectOutputSelA) == ATU_PROTECT_TIO1A_OUTPUT_SELECT_TPOC_SETTING ))
/**
  * @}
  */ 

/** @defgroup ATU_PROTECT_OUTPUT_SELECT_TIO1B ATU_PROTECT_OUTPUT_SELECT_TIO1B 
  * @{
  */ 
#define ATU_PROTECT_TIO1B_OUTPUT_SELECT_DEFAULT_LEVLE                 ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO1B_OUTPUT_SELECT_TPOC_SETTING                  ((uint32_t)0x08000000)
#define IS_ATU_PROTECT_OUTPUT_SELECT_TIO1B(ProtectOutputSelB)         (((ProtectOutputSelB) == ATU_PROTECT_TIO1B_OUTPUT_SELECT_DEFAULT_LEVLE) || \
                                                                       ((ProtectOutputSelB) == ATU_PROTECT_TIO1B_OUTPUT_SELECT_TPOC_SETTING ))
/**
  * @}
  */ 

/** @defgroup ATU_PROTECT_DEFAULT_LEVEL_TIO2A ATU_PROTECT_DEFAULT_LEVEL_TIO2A 
  * @{
  */ 
#define ATU_PROTECT_TIO2A_DEFAULT_LEVEL_LOW                        ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO2A_DEFAULT_LEVEL_HIGH                       ((uint32_t)0x00100000)
#define IS_ATU_PROTECT_DEFAULT_LEVEL_TIO2A(ATU_DefaultLevelA)      (((ATU_DefaultLevelA) == ATU_PROTECT_TIO2A_DEFAULT_LEVEL_LOW ) || \
                                                                    ((ATU_DefaultLevelA) == ATU_PROTECT_TIO2A_DEFAULT_LEVEL_HIGH))
/**
  * @}
  */ 

/** @defgroup ATU_PROTECT_DEFAULT_LEVEL_TIO2B ATU_PROTECT_DEFAULT_LEVEL_TIO2B 
  * @{
  */ 
#define ATU_PROTECT_TIO2B_DEFAULT_LEVEL_LOW                        ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO2B_DEFAULT_LEVEL_HIGH                       ((uint32_t)0x00200000)
#define IS_ATU_PROTECT_DEFAULT_LEVEL_TIO2B(ATU_DefaultLevelB)      (((ATU_DefaultLevelB) == ATU_PROTECT_TIO2B_DEFAULT_LEVEL_LOW ) || \
                                                                    ((ATU_DefaultLevelB) == ATU_PROTECT_TIO2B_DEFAULT_LEVEL_HIGH))
/**
  * @}
  */ 

/** @defgroup ATU_PROTECT_OUTPUT_SELECT_TIO2A ATU_PROTECT_OUTPUT_SELECT_TIO2A 
  * @{
  */ 
#define ATU_PROTECT_TIO2A_OUTPUT_SELECT_DEFAULT_LEVLE                 ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO2A_OUTPUT_SELECT_TPOC_SETTING                  ((uint32_t)0x10000000)
#define IS_ATU_PROTECT_OUTPUT_SELECT_TIO2A(ProtectOutputSelA)         (((ProtectOutputSelA) == ATU_PROTECT_TIO2A_OUTPUT_SELECT_DEFAULT_LEVLE) || \
                                                                       ((ProtectOutputSelA) == ATU_PROTECT_TIO2A_OUTPUT_SELECT_TPOC_SETTING ))
/**
  * @}
  */ 

/** @defgroup ATU_PROTECT_OUTPUT_SELECT_TIO2B ATU_PROTECT_OUTPUT_SELECT_TIO2B 
  * @{
  */ 
#define ATU_PROTECT_TIO2B_OUTPUT_SELECT_DEFAULT_LEVLE                 ((uint32_t)0x00000000)
#define ATU_PROTECT_TIO2B_OUTPUT_SELECT_TPOC_SETTING                  ((uint32_t)0x20000000)
#define IS_ATU_PROTECT_OUTPUT_SELECT_TIO2B(ProtectOutputSelB)         (((ProtectOutputSelB) == ATU_PROTECT_TIO2B_OUTPUT_SELECT_DEFAULT_LEVLE) || \
                                                                       ((ProtectOutputSelB) == ATU_PROTECT_TIO2B_OUTPUT_SELECT_TPOC_SETTING ))
/**
  * @}
  */ 

/** @defgroup DEBUG_HALT_PWM_OUTPUT_SEL DEBUG_HALT_PWM_OUTPUT_SEL 
  * @{
  */ 
#define ATU_DEBUG_HALT_PWM_OUTPUT_DEFAULT_LEVLE                       ((uint32_t)0x00000000)
#define ATU_DEBUG_HALT_PWM_OUTPUT_NORMAL                              ((uint32_t)0x00001000)
#define IS_DEBUG_HALT_PWM_OUTPUT_SEL(DebugHaltPWMOutputSel)           (((DebugHaltPWMOutputSel) ==  ATU_DEBUG_HALT_PWM_OUTPUT_DEFAULT_LEVLE ) || \
                                                                       ((DebugHaltPWMOutputSel) ==  ATU_DEBUG_HALT_PWM_OUTPUT_NORMAL        ))
/**
  * @}
  */ 

/** @defgroup ATU_IT_BIT ATU_IT_BIT
  * @{
  */ 
#define ATU_IT_CR0A                      ((uint32_t)0x00000001)
#define ATU_IT_CR0B                      ((uint32_t)0x00000002)
#define ATU_IT_CR1A                      ((uint32_t)0x00000004)
#define ATU_IT_CR1B                      ((uint32_t)0x00000008)
#define ATU_IT_CR2A                      ((uint32_t)0x00000010)
#define ATU_IT_CR2B                      ((uint32_t)0x00000020)
#define ATU_IT_CREST                     ((uint32_t)0x00000100)
#define ATU_IT_TROUGH                    ((uint32_t)0x00000200)
#define ATU_IT_RELOAD                    ((uint32_t)0x00000800)
#define ATU_IT_TRG0                      ((uint32_t)0x00001000)
#define ATU_IT_TRG1                      ((uint32_t)0x00002000)
#define ATU_IT_TRGDB                     ((uint32_t)0x00008000)
#define ATU_IT_TPP0                      ((uint32_t)0x00010000)
#define ATU_IT_TPP1                      ((uint32_t)0x00020000)

#define IS_ATU_IT(IT) ((((IT) & (uint32_t)0xFFFC44C0) == 0x00000000) && ((IT) != 0x00000000))

#define IS_ATU_GET_IT(IT)    ((((IT) & (uint32_t)0xFFFC44C0) == 0x00000000) && ((IT) != 0x00000000))

/**
  * @}
  */ 

/** @defgroup ATU_FLAG ATU_FLAG 
  * @{
  */ 
#define ATU_FLAG_CR0A                   ((uint32_t)0x00000001)
#define ATU_FLAG_CR0B                   ((uint32_t)0x00000002)
#define ATU_FLAG_CR1A                   ((uint32_t)0x00000004)
#define ATU_FLAG_CR1B                   ((uint32_t)0x00000008)
#define ATU_FLAG_CR2A                   ((uint32_t)0x00000010)
#define ATU_FLAG_CR2B                   ((uint32_t)0x00000020)
#define ATU_FLAG_CREST                  ((uint32_t)0x00000100)
#define ATU_FLAG_TROUGH                 ((uint32_t)0x00000200)
#define ATU_FLAG_RELOAD                 ((uint32_t)0x00000800)
#define ATU_FLAG_TRG0                   ((uint32_t)0x00001000)
#define ATU_FLAG_TRG1                   ((uint32_t)0x00002000)
#define ATU_FLAG_TRGDB                  ((uint32_t)0x00008000)
#define ATU_FLAG_TPP0                   ((uint32_t)0x00010000)
#define ATU_FLAG_TPP1                   ((uint32_t)0x00020000)

#define IS_ATU_CLEAR_FLAG(FLAG)         ((((FLAG) & (uint32_t)0xFFFC44C0) == 0x00000000) && ((FLAG) != 0x00000000))

#define IS_ATU_GET_FLAG(FLAG)           ((((FLAG) & (uint32_t)0xFFFC44C0) == 0x00000000) && ((FLAG) != 0x00000000))
/**
  * @}
  */ 

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @defgroup ATU_Exported_Functions ATU_Exported_Functions
  * @{
  */

/* TimeBase management ********************************************************/
void ATU_DeInit(void);
void ATU_TimeBaseInit(ATU_TimeBaseInitTypeDef* ATU_TimeBaseInitStruct);
void ATU_TimeBaseStructInit(ATU_TimeBaseInitTypeDef* ATU_TimeBaseInitStruct);
void ATU_CounterModeConfig(uint16_t ATU_CounterMode);
void ATU_SetCounter(uint16_t Counter,uint16_t ReloadMode);
void ATU_SetTimePeriod(uint16_t TimePeriod ,uint16_t ReloadMode);
uint16_t ATU_GetCounter(void);
uint16_t ATU_GetClockDivision(void);

void ATU_OutputInit(uint32_t ATU_PWMChannelx, ATU_OutputInitTypeDef* ATU_OutputInitStruct);
void ATU_OutputStructInit(ATU_OutputInitTypeDef* ATU_OutputInitStruct);
void ATU_SetCompare0(uint16_t CompareA, uint16_t CompareB);
void ATU_SetCompare1(uint16_t CompareA, uint16_t CompareB);
void ATU_SetCompare2(uint16_t CompareA, uint16_t CompareB);
void ATU_SetDeadBandTime(uint16_t DeadBandTimeA, uint16_t DeadBandTimeB);
void ATU_SetBlankAera(uint16_t BlankAreaA, uint16_t BlankAreaB);
void ATU_SetTrigger0(uint16_t TriggerPoint, uint16_t ReloadMode, uint16_t TriggerValue);
void ATU_SetTrigger1(uint16_t TriggerPoint, uint16_t ReloadMode, uint16_t TriggerValue);
void ATU_SetTriggerDoubleDataB(uint16_t TriggerPoint, uint16_t ReloadMode, uint16_t TriggerValue);

void ATU_SetReloadModeCompare0(uint16_t ReloadModeA, uint16_t ReloadModeB);
void ATU_SetReloadModeCompare1(uint16_t ReloadModeA, uint16_t ReloadModeB);
void ATU_SetReloadModeCompare2(uint16_t ReloadModeA, uint16_t ReloadModeB);

void ATU_ExternalTriggerConfig(uint16_t ExtSource, FunctionalState NewState);
void ATU_CounterCMD(FunctionalState NewState);

void ATU_ConfigOutputModeChannel0(uint16_t OutputMode);
void ATU_ConfigOutputModeChannel1(uint16_t OutputMode);
void ATU_ConfigOutputModeChannel2(uint16_t OutputMode);

void ATU_ConfigOutputPolarityChannel0(uint16_t PolarityA, uint16_t PolarityB);
void ATU_ConfigOutputPolarityChannel1(uint16_t PolarityA, uint16_t PolarityB);
void ATU_ConfigOutputPolarityChannel2(uint16_t PolarityA, uint16_t PolarityB);

void ATU_ConfigOutputSourceChannel0(uint16_t SourceA, uint16_t SourceB);
void ATU_ConfigOutputSourceChannel1(uint16_t SourceA, uint16_t SourceB);
void ATU_ConfigOutputSourceChannel2(uint16_t SourceA, uint16_t SourceB);

void ATU_ConfigOutputLeveLChannel0(uint16_t LevelA, uint16_t LevelB);
void ATU_ConfigOutputLeveLChannel1(uint16_t LevelA, uint16_t LevelB);
void ATU_ConfigOutputLeveLChannel2(uint16_t LevelA, uint16_t LevelB);

void ATU_PWMOutputCmd(FunctionalState NewState);


void ATU_PWMIORemap(uint16_t PWMChannel, uint16_t PWMRemapSource);

ATU_CounterDirection ATU_GetCounterDirection(void);

void ATU_CrestReloadTimingPoint(uint16_t ReloadTimingPoint, FunctionalState NewState);
void ATU_TroughReloadTimingPoint(uint16_t ReloadTimingPoint, FunctionalState NewState);

void ATU_ProtectIFConfig(uint32_t ATU_ProtectIFx, ATU_ProtectIFInitTypeDef* ATU_ProtectIFInitStruct);
void ATU_ProtectIFInitStructInit(ATU_ProtectIFInitTypeDef* ATU_ProtectIFInitStruct);
void ATU_ProtectOutputConfig(ATU_ProtectOutputInitTypeDef* ATU_ProtectOutputInitStruct);
void ATU_ProtectOutputStructInit(ATU_ProtectOutputInitTypeDef* ATU_ProtectOutputInitStruct);
void ATU_ProtectOutputSetting0(uint32_t DefaultLevelA, uint32_t DefaultLevelB, uint32_t ProtectOutputSelA, uint32_t ProtectOutputSelB);
void ATU_ProtectOutputSetting1(uint32_t DefaultLevelA, uint32_t DefaultLevelB, uint32_t ProtectOutputSelA, uint32_t ProtectOutputSelB);
void ATU_ProtectOutputSetting2(uint32_t DefaultLevelA, uint32_t DefaultLevelB, uint32_t ProtectOutputSelA, uint32_t ProtectOutputSelB);
void ATU_DebugHaltPWMOutput(uint32_t DebugHaltPWMOutputSel);
void ATU_ITConfig(uint32_t ATU_IT, FunctionalState NewState);
FlagStatus ATU_GetFlagStatus(uint32_t ATU_FLAG);
void ATU_ClearFlag(uint32_t ATU_FLAG);
ITStatus ATU_GetITStatus(uint32_t ATU_IT);
void ATU_ClearITPendingBit(uint32_t ATU_IT);
void ATU_RegisterProtectUnlockCmd(FunctionalState NewState);


#endif /*__HK32M07X_ATU_H */
/**
  * @}
  */ 

/**
  * @}
  */

/***************************** (C) COPYRIGHT HKMicroChip  *******************************/
