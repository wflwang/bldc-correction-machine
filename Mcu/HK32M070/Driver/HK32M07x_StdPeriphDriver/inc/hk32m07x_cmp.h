/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_cmp.h
* @brief:     API file of cmp module
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_CMP_H
#define __HK32M07X_CMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup CMP
  * @{
  */

/** @defgroup CMP_Exported_Types CMP_Exported_Types
  * @{
  */

/**
  * @brief CMP Init structure definition
  */
typedef struct
{
    uint32_t CMP_NInputSel;
    uint32_t CMP_PInputSel;
    uint32_t CMP_OutputPolarity;
    uint32_t CMP_HysteresisMode;
    uint32_t CMP_Hysteresis;
} CMP_InitTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup CMP_Exported_Constants CMP_Exported_Constants
  * @{
  */
/** @defgroup CMP_Define CMP_Define
  * @{
  */
#define CMP1                                 1
#define CMP2                                 2
#define IS_CMP_NAME(COMP) (((COMP) == CMP1) || ((COMP) == CMP2))
/**
  * @}
  */

/** @defgroup CMP_Filter_Count_Configuration CMP_Filter_Count_Configuration
  * @{
  */
#define IS_CMP_FILTER_COUNT(COUNT) ((COUNT) <= 0xF)

/**
  * @}
  */

/** @defgroup CMP_Blank_Channel_Selection CMP_Blank_Channel_Selection
  * @{
  */
#define CMP_Blank_Channel0              ((uint32_t)0x00000001)
#define CMP_Blank_Channel1              ((uint32_t)0x00000002)
#define CMP_Blank_Channel2              ((uint32_t)0x00000004)
#define IS_CMP_BLANK_CHANNEL(CHANNEL)  (((CHANNEL) == CMP_Blank_Channel0) || ((CHANNEL) == CMP_Blank_Channel1) || \
                                        ((CHANNEL) == CMP_Blank_Channel2))
/**
  * @}
  */

/** @defgroup CMP_Blank_Polarity_Selection CMP_Blank_Polarity_Selection
  * @{
  */
#define CMP_Blank_Polarity_LOW              ((uint32_t)0x00000000)  
#define CMP_Blank_Polarity_High             ((uint32_t)0x00000001)
#define IS_CMP_BLANK_POLAR(POLAR)  (((POLAR) == CMP_Blank_Polarity_LOW) || ((POLAR) == CMP_Blank_Polarity_High))
/**
  * @}
  */

/** @defgroup CMP_Filter_Prescale_Configuration CMP_Filter_Prescale_Configuration
  * @{
  */
#define CMP_Filter_Prescaler_1                ((uint32_t)0x00000000)
#define CMP_Filter_Prescaler_2                ((uint32_t)0x00000001)
#define CMP_Filter_Prescaler_4                ((uint32_t)0x00000002)
#define CMP_Filter_Prescaler_8                ((uint32_t)0x00000003)
#define CMP_Filter_Prescaler_16               ((uint32_t)0x00000004)
#define CMP_Filter_Prescaler_32               ((uint32_t)0x00000005)
#define CMP_Filter_Prescaler_64               ((uint32_t)0x00000006)
#define CMP_Filter_Prescaler_128              ((uint32_t)0x00000007)
#define IS_CMP_FILTER_PRESCALER(CLK_DIV)   (((CLK_DIV) == CMP_Filter_Prescaler_1) || ((CLK_DIV) == CMP_Filter_Prescaler_2) || \
                                            ((CLK_DIV) == CMP_Filter_Prescaler_4) || ((CLK_DIV) == CMP_Filter_Prescaler_8) || \
                                            ((CLK_DIV) == CMP_Filter_Prescaler_16) || ((CLK_DIV) == CMP_Filter_Prescaler_32) || \
                                            ((CLK_DIV) == CMP_Filter_Prescaler_64) || ((CLK_DIV) == CMP_Filter_Prescaler_128))
/**
  * @}
  */

/** @defgroup CMP_Hysteresis_Configuration CMP_Hysteresis_Configuration
  * @{
  */
#define CMP_HYSTERESIS_DISABLE             ((uint32_t)0x00000000)
#define CMP_HYSTERESIS_LEVEL1              ((uint32_t)0x00000040)
#define CMP_HYSTERESIS_LEVEL2              ((uint32_t)0x00000080)
#define CMP_HYSTERESIS_LEVEL3              ((uint32_t)0x000000C0)
#define IS_CMP_HYSTERESIS(HYSTERESIS)  (((HYSTERESIS) == CMP_HYSTERESIS_DISABLE) || ((HYSTERESIS) == CMP_HYSTERESIS_LEVEL1) || \
                                        ((HYSTERESIS) == CMP_HYSTERESIS_LEVEL2) || ((HYSTERESIS) == CMP_HYSTERESIS_LEVEL3))
/**
  * @}
  */

/** @defgroup CMP_Hysteresis_Mode_Configuration CMP_Hysteresis_Mode_Configuration
  * @{
  */
#define CMP_NO_HYSTERESIS                     ((uint32_t)0x00000000)
#define CMP_POSITIVE_HYSTERESIS               ((uint32_t)0x00000004)
#define CMP_NEGATIVE_HYSTERESIS               ((uint32_t)0x00000008)
#define CMP_NEGATIVE_POSITIVE_HYSTERESIS      ((uint32_t)0x0000000C)
#define IS_CMP_HYSTERESIS_MODE(MODE)   (((MODE) == CMP_NO_HYSTERESIS) || ((MODE) == CMP_POSITIVE_HYSTERESIS) || \
                                        ((MODE) == CMP_NEGATIVE_HYSTERESIS) || ((MODE) == CMP_NEGATIVE_POSITIVE_HYSTERESIS))
/**
  * @}
  */

/** @defgroup CMP_Output_Polarity_Configuration CMP_Output_Polarity_Configuration
  * @{
  */

#define CMP_OUTPUT_POLARITY_NORMAL                 ((uint32_t)0x00000000)
#define CMP_OUTPUT_POLARITY_REVERSE                ((uint32_t)0x00000010)
#define IS_CMP_OUTPUT_POLARITY(POLAR) (((POLAR) == CMP_OUTPUT_POLARITY_NORMAL) || ((POLAR) == CMP_OUTPUT_POLARITY_REVERSE))
/**
  * @}
  */

/** @defgroup CMP_BEMF_Input_Select CMP_BEMF_Input_Select
  * @{
  */

#define CMP_BEMF_INPUT_HIZ                      ((uint32_t)0x00000000)
#define CMP_BEMF_INPUT_CMP1                     ((uint32_t)CMP_CMISS_CMBMF_0)
#define CMP_BEMF_INPUT_CMP2                     ((uint32_t)CMP_CMISS_CMBMF_1)
#define IS_CMP_BEMF_INPUT_SEL(INPUT) (((INPUT) == CMP_BEMF_INPUT_HIZ) || ((INPUT) == CMP_BEMF_INPUT_CMP1) || ((INPUT) == CMP_BEMF_INPUT_CMP2))
/**
  * @}
  */

/** @defgroup CMP_Positive_Input_Configuration CMP_Positive_Input_Configuration
  * @{
  */
#define CMP_POSITIVE_INPUT_P0                  ((uint32_t)0x00000000)
#define CMP_POSITIVE_INPUT_P1                  ((uint32_t)0x00000001)
#define CMP_POSITIVE_INPUT_P2                  ((uint32_t)0x00000002)
#define CMP_POSITIVE_INPUT_P3                  ((uint32_t)0x00000003)
#define CMP_POSITIVE_INPUT_PGA1OUT_A_B         ((uint32_t)0x00000004)
#define CMP_POSITIVE_INPUT_PGA2OUT             ((uint32_t)0x00000005)
#define CMP_POSITIVE_INPUT_PGA1_PA_PB          ((uint32_t)0x00000008)
#define CMP_POSITIVE_INPUT_PGA2_P              ((uint32_t)0x00000009)
#define IS_CMP_POSITIVE_INPUT(INPUT_SOURCE) (((INPUT_SOURCE) == CMP_POSITIVE_INPUT_P0) || ((INPUT_SOURCE) == CMP_POSITIVE_INPUT_P1) || \
                                             ((INPUT_SOURCE) == CMP_POSITIVE_INPUT_P2) || ((INPUT_SOURCE) == CMP_POSITIVE_INPUT_P3) || \
                                             ((INPUT_SOURCE) == CMP_POSITIVE_INPUT_PGA1OUT_A_B) || ((INPUT_SOURCE) == CMP_POSITIVE_INPUT_PGA2OUT) || \
                                             ((INPUT_SOURCE) == CMP_POSITIVE_INPUT_PGA1_PA_PB) || ((INPUT_SOURCE) == CMP_POSITIVE_INPUT_PGA2_P))
/**
  * @}
  */

/** @defgroup CMP_Negative_Input_Configuration CMP_Negative_Input_Configuration
  * @{
  */

#define CMP_NEGATIVE_INPUT_N                   ((uint32_t)0x00000000)
#define CMP_NEGATIVE_INPUT_BEMF                ((uint32_t)0x00000010)
#define CMP_NEGATIVE_INPUT_DAC_OUT             ((uint32_t)0x00000020)
#define IS_CMP_NEGATIVE_INPUT(INPUT_SOURCE)    (((INPUT_SOURCE) == CMP_NEGATIVE_INPUT_N) || ((INPUT_SOURCE) == CMP_NEGATIVE_INPUT_BEMF) || \
                                                ((INPUT_SOURCE) == CMP_NEGATIVE_INPUT_DAC_OUT))
/**
  * @}
  */

/** @defgroup CMP_Interrupt_Flag CMP_Interrupt_Flag
  * @{
  */
#define CMP_CMP1_Rising_Flag                    ((uint32_t)0x00000001)
#define CMP_CMP1_Falling_Flag                   ((uint32_t)0x00000002)
#define CMP_CMP2_Rising_Flag                    ((uint32_t)0x00000004)
#define CMP_CMP2_Falling_Flag                   ((uint32_t)0x00000008)

#define IS_CMP_IT(FLAG)     (((FLAG) == CMP_CMP1_Rising_Flag) || ((FLAG) == CMP_CMP1_Falling_Flag) || ((FLAG) == CMP_CMP2_Rising_Flag) || \
                             ((FLAG) == CMP_CMP2_Falling_Flag))
/**
  * @}
  */

/**
  * @}
  */


/** @defgroup CMP_Exported_Functions  CMP_Exported_Functions
  * @{
  */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void CMP_DeInit(void);
void CMP_StructInit(CMP_InitTypeDef *CMP_InitStruct);
void CMP_Init(uint8_t CMPx, CMP_InitTypeDef *CMP_InitStruct);
void CMP_SetPositiveInput(uint8_t CMPx, uint32_t PositiveInput);
uint32_t CMP_GetPositiveInput(uint8_t CMPx);
void CMP_SetNegativeInput(uint8_t CMPx, uint32_t NegativeInput);
uint32_t CMP_GetNegativeInput(uint8_t CMPx);
void CMP_SetOutputPolarity(uint8_t CMPx, uint32_t OutputPolarity);
uint32_t CMP_GetOutputPolarity(uint8_t CMPx);
void CMP_SetHysteresisMode(uint8_t CMPx, uint32_t Mode);
uint32_t CMP_GetHysteresisMode(uint8_t CMPx);
void CMP_SetHysteresis(uint8_t CMPx, uint32_t Hysteresis);
uint32_t CMP_GetHysteresis(uint8_t CMPx);
void CMP_Cmd(uint8_t CMPx, FunctionalState NewState);
uint32_t CMP_IsEnabled(uint8_t CMPx);
void CMP_BlankChannelSel(uint8_t Blank_Channel);
void CMP_BlankConfig(uint8_t CMPx);
void CMP_BlankPolaritySel(uint32_t Polar);
void CMP_FilterPrescaler(uint8_t CMPx, uint32_t Prescaler);
void CMP_SetFilterCnt(uint8_t CMPx, uint32_t FilterCLKCnt);
void CMP_FilterCmd(uint8_t CMPx, FunctionalState NewState);
uint32_t CMP_GetOriginOutput(uint8_t CMPx);
uint32_t CMP_GetFilterOutput(uint8_t CMPx);
void CMP_BEMFSel(uint32_t BEMFInput);
void CMP_ITConfig(uint32_t CMP_IT, FunctionalState NewState);
ITStatus CMP_GetITStatus(uint32_t FLAG);
void CMP_ClearITPendingBit(uint32_t CMP_IT);

/**
  * @}
  */
  
/**
  * @}
  */

#ifdef __cplusplus
}
#endif


#endif /*__HK32M07X_CMP_H */

