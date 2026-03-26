/**
  ******************************************************************************
  * @file    mc_type.h 
  * @version V1.0.0
  * @date    2021-07-28
  * @brief   global types definitions.	
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MC_TYPE_H
#define __MC_TYPE_H

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "hk32m07x.h"
#include "main.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @addtogroup MOTOR
  * @{
  */

/** @addtogroup MC_Type Motor Control types
  * @{
  */

/**
  * @define MISRA_C_2004_BUILD
  * @brief Used to build the library with MISRA C support
  */
/*#define MISRA_C_2004_BUILD*/

#include <mc_hk_types.h>


/** @name Macros to use bit banding capability */
/** @{ */
#define BB_REG_BIT_SET(regAddr,bit_number) *(uint32_t *) (0x42000000+(((uint32_t)regAddr - 0x40000000 )<<5) + (bit_number <<2)) = (uint32_t)(0x1u)
#define BB_REG_BIT_CLR(regAddr,bit_number) (*(uint32_t *) (0x42000000+(((uint32_t)regAddr - 0x40000000)<<5) + (bit_number <<2)) = (uint32_t)(0x0u))
#define BB_REG_BIT_READ(regAddr,bit_number) (*(uint32_t *) (0x42000000+(((uint32_t)regAddr - 0x40000000)<<5) + (bit_number <<2)) )
/** @} */

/** @brief Not initialized pointer */
#define MC_NULL    (uint16_t)(0x0000u)

/** @name Motor identification macros */
/** @{ */
#define M1      (uint8_t)(0x0)  /*!< Motor 1.*/
#define M_NONE  (uint8_t)(0xFF) /*!< None motor.*/
/** @} */

/** @name Fault source error codes */
/** @{ */
#define  MC_NO_ERROR  (uint16_t)(0x0000u)      /**< @brief No error.*/
#define  MC_NO_FAULTS  (uint16_t)(0x0000u)     /**< @brief No error.*/
#define  MC_FOC_DURATION  (uint16_t)(0x0001u)  /**< @brief Error: FOC rate to high.*/
#define  MC_OVER_VOLT  (uint16_t)(0x0002u)     /**< @brief Error: Software over voltage.*/
#define  MC_UNDER_VOLT  (uint16_t)(0x0004u)    /**< @brief Error: Software under voltage.*/
#define  MC_OVER_TEMP  (uint16_t)(0x0008u)     /**< @brief Error: Software over temperature.*/
#define  MC_START_UP  (uint16_t)(0x0010u)      /**< @brief Error: Startup failed.*/
#define  MC_SPEED_FDBK  (uint16_t)(0x0020u)    /**< @brief Error: Speed feedback.*/
#define  MC_BREAK_IN  (uint16_t)(0x0040u)      /**< @brief Error: Emergency input (Over current).*/
#define  MC_SW_ERROR  (uint16_t)(0x0080u)      /**< @brief Software Error.*/
/** @} */

/** @name Dual motor Frequency comparison definition */
/** @{ */
//#define SAME_FREQ   0u
//#define HIGHER_FREQ 1u
//#define LOWER_FREQ  2u

//#define HIGHEST_FREQ 1u
//#define LOWEST_FREQ  2u
/** @} */

/**
  * @brief Two components stator current type definition
  */
typedef struct
{
  int16_t qI_Component1;
  int16_t qI_Component2;
} Curr_Components;

/**
  * @brief  Two components stator voltage type definition
  */
typedef struct
{
  int16_t qV_Component1;
  int16_t qV_Component2;
} Volt_Components;

/**
  * @brief  ADConv_t type definition, it is used by PWMC_ADC_SetSamplingTime method of PWMnCurrFdbk class for user defined A/D regular conversions
  */
typedef struct
{
  uint8_t Channel;   /*!< Integer channel number, from 0 to 15 */
  uint8_t SamplTime; /*!< Sampling time selection, ADC_SampleTime_nCycles5 */
} ADConv_t;

/**
  * @brief  SensorType_t type definition, it's used in BusVoltageSensor and TemperatureSensor component parameters structures
  *       to specify whether the sensor is real or emulated by SW
  */
typedef enum
{
  REAL_SENSOR, VIRTUAL_SENSOR
} SensorType_t;

/**
  * @brief  DOutputState_t type definition, it's used by DOUT_SetOutputState method of DigitalOutput class to specify the
  *     required output state
  */
typedef enum
{
  INACTIVE, ACTIVE
} DOutputState_t;


/**
  * @brief  STC_Modality_t type definition, it's used by STC_SetControlMode and STC_GetControlMode methods in
  *         SpeednTorqCtrl class to specify the control modality type
  */
typedef enum
{
  STC_TORQUE_MODE, /**< @brief Torque mode.*/
  STC_SPEED_MODE   /**< @brief Speed mode.*/
} STC_Modality_t;


/**
  * @brief IMFF_PMSM class, structure type definition for feed-forward constants tuning
  */
typedef struct
{
  int32_t wConst_1D;
  int32_t wConst_1Q;
  int32_t wConst_2;
} IMFF_TuningStruct_t, FF_TuningStruct_t;

/**
  * @brief  Current references source type, internal or external to FOCDriveClass
  */
typedef enum
{
  INTERNAL, EXTERNAL
} CurrRefSource_t ;

/**
  * @brief  FOC variables structure
  */
typedef struct
{
  Curr_Components Iab;         /**< @brief Stator current on stator reference frame abc */
  Curr_Components Ialphabeta;  /**< @brief Stator current on stator reference frame alfa-beta*/
  Curr_Components IqdHF;       /**< @brief Stator current on stator reference frame alfa-beta*/
  Curr_Components Iqd;         /**< @brief Stator current on rotor reference frame qd */
  Curr_Components Iqdref;      /**< @brief Stator current on rotor reference frame qd */
  int16_t UserIdref;           /**< @brief User value for the Idref stator current */
  Volt_Components Vqd;         /**< @brief Phase voltage on rotor reference frame qd */
  Volt_Components Valphabeta;  /**< @brief Phase voltage on stator reference frame alpha-beta*/
  int16_t hTeref;              /**< @brief Reference torque */
  int16_t hElAngle;            /**< @brief Electrical angle used for reference frame transformation  */
  uint16_t hCodeError;         /**< @brief error message */
  CurrRefSource_t bDriveInput; /**< @brief It specifies whether the current reference source must be
                                 *         #INTERNAL or #EXTERNAL*/
} FOCVars_t, *pFOCVars_t;

/**
  * @brief  Low side or enabling signal definition
  */
#define LS_DISABLED   0
#define LS_PWM_TIMER  1
#define ES_GPIO       2

typedef enum
{
  LS_DISABLED_VAL = LS_DISABLED,    /**< @brief Low side signals and enabling signals always off.
                                         It is equivalent to DISABLED. */
  LS_PWM_TIMER_VAL = LS_PWM_TIMER,  /**< @brief Low side PWM signals are generated by timer. It is
                                         equivalent to ENABLED. */
  ES_GPIO_VAL = ES_GPIO             /**< @brief Enabling signals are managed by GPIOs (L6230 mode).*/
} LowSideOutputsFunction_t, *pLowSideOutputsFunction_t;

/**
  * @brief  MPInfo structure (used only for serial communication)
  */
typedef struct
{
  uint8_t * data;
  uint8_t len;
} MPInfo_t, *pMPInfo_t;

/** @name UserInterface related exported definitions */
/** @{ */
#define OPT_NONE    0x00 /**< @brief No UI option selected. */
#define OPT_COM     0x02 /**< @brief Bit field indicating that the UI uses serial communication. */
/** @} */

#define MAIN_SCFG_POS (28)
#define AUX_SCFG_POS (24)

#define MAIN_SCFG_VALUE(x) (((x)>>MAIN_SCFG_POS)&0x0F)
#define AUX_SCFG_VALUE(x)  (((x)>>AUX_SCFG_POS)&0x0F)

/** @name PFC related exported definitions */
/** @{ */

#define PFC_SWE             0x0001u /**< @brief PFC Software error. */
#define PFC_HW_PROT         0x0002u /**< @brief PFC hardware protection. */
#define PFC_SW_OVER_VOLT    0x0004u /**< @brief PFC software over voltage. */
#define PFC_SW_OVER_CURRENT 0x0008u /**< @brief PFC software over current. */
#define PFC_SW_MAINS_FREQ   0x0010u /**< @brief PFC mains frequency error. */
#define PFC_SW_MAIN_VOLT    0x0020u /**< @brief PFC mains voltage error. */
/** @} */

/** @name Definitions exported for the DAC channel used as reference for protection */
/** @{ */
#define AO_DISABLED 0x00u /**< @brief Analog output disabled.*/
#define AO_DEBUG    0x01u /**< @brief Analog output debug.*/
#define VREF_OCPM1  0x02u /**< @brief Voltage reference for over current protection of motor 1.*/
#define VREF_OCPM2  0x03u /**< @brief Voltage reference for over current protection of motor 2.*/
#define VREF_OCPM12 0x04u /**< @brief Voltage reference for over current protection of both motors.*/
#define VREF_OVPM12 0x05u /**< @brief Voltage reference for over voltage protection of both motors.*/
/** @} */

/** @name ADC channel number definitions */
/** @{ */
#define MC_ADC_CHANNEL_0     0
#define MC_ADC_CHANNEL_1     1
#define MC_ADC_CHANNEL_2     2
#define MC_ADC_CHANNEL_3     3
#define MC_ADC_CHANNEL_4     4
#define MC_ADC_CHANNEL_5     5
#define MC_ADC_CHANNEL_6     6
#define MC_ADC_CHANNEL_7     7
#define MC_ADC_CHANNEL_8     8
#define MC_ADC_CHANNEL_9     9
#define MC_ADC_CHANNEL_10    10
#define MC_ADC_CHANNEL_11    11
#define MC_ADC_CHANNEL_12    12
#define MC_ADC_CHANNEL_13    13
#define MC_ADC_CHANNEL_14    14
#define MC_ADC_CHANNEL_15    15
#define MC_ADC_CHANNEL_16    16
#define MC_ADC_CHANNEL_17    17
#define MC_ADC_CHANNEL_18    18
/** @} */

/** @name Utility macros definitions */
/** @{ */
#define RPM2MEC01HZ(rpm) (int16_t)((int32_t)(rpm)/6)
/** @} */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __MC_TYPE_H */

