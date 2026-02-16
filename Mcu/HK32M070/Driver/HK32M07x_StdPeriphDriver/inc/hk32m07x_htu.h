/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_htu.h
* @brief:     This file contains all the functions prototypes for the HTU firmware
*             library
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_HTU_H
#define __HK32M07X_HTU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"
	
/** @addtogroup HTU
  * @{
  */	

/** @defgroup HTU_Exported_Types HTU_Exported_Types
  * @{
  */
/* Exported types ------------------------------------------------------------*/

typedef struct
{
    uint8_t HTU_NFCR;              /*!< Specifies the filter value used to filter the Hall input*/
    
    uint8_t HTU_ART;               /*!< Specifies the HTU average times*/
  
    uint8_t HTU_WPA;               /*!< Specifies the Hall phase error alarm function*/
  
    uint8_t HTU_ClockDivision;     /*!< Specifies the prescaler value used to divide the HTU clock.
                                       This parameter can be one of 1,2,4,8 */
    
    uint32_t HTU_Period;           /*!< Specifies the period value to be loaded into the active
                                       Time_period Register at the next update event.
                                       This parameter must be a number between 0x0000 and 0xFFFF.  */
} HTU_TimeBaseInitTypeDef;



/** @defgroup HTU_CKD_DIV HTU_CKD_DIV
  * @{
  */ 

#define HTU_CLK_DIV_1                      ((uint32_t)0x00)
#define HTU_CLK_DIV_2                      ((uint32_t)0x01)
#define HTU_CLK_DIV_4                      ((uint32_t)0x02)
#define HTU_CLK_DIV_8                      ((uint32_t)0x03)
#define IS_HTU_CKD_DIV(HTU_ClockDiv)       (((HTU_ClockDiv) == HTU_CLK_DIV_1 ) || \
                                            ((HTU_ClockDiv) == HTU_CLK_DIV_2 ) || \
                                            ((HTU_ClockDiv) == HTU_CLK_DIV_4 ) || \
                                            ((HTU_ClockDiv) == HTU_CLK_DIV_8 ))  
/**
  * @}
  */

/** @defgroup HTU_ART HTU_ART
  * @{
  */ 
#define HTU_ART_2                      ((uint32_t)0x00)
#define HTU_ART_4                      ((uint32_t)0x01<<(HTU_HCSR_HART_Pos))
#define HTU_ART_8                      ((uint32_t)0x02<<(HTU_HCSR_HART_Pos))
#define HTU_ART_0                      ((uint32_t)0x03<<(HTU_HCSR_HART_Pos))
#define IS_HTU_ART(HTU_ART)            (((HTU_ART) == HTU_ART_2 ) || \
                                        ((HTU_ART) == HTU_ART_4 ) || \
                                        ((HTU_ART) == HTU_ART_8 ) || \
                                        ((HTU_ART) == HTU_ART_0 ))  
/**
  * @}
  */

/** @defgroup HTU_WPA HTU_WPA
  * @{
  */ 
#define HTU_WPA_UNWARN                   ((uint32_t)0x00)
#define HTU_WPA_WARN                     ((uint32_t)0x01<<HTU_HCSR_HWPA_Pos)
#define IS_HTU_WPA(HTU_WPA)              (((HTU_WPA) == HTU_WPA_UNWARN ) || \
                                          ((HTU_WPA) == HTU_WPA_WARN ))
/**
  * @}
  */
                                     
 

/** @defgroup HTU_IT_BIT HTU_IT_BIT
  * @{
  */ 
#define HTU_IT_OVF                      ((uint32_t)0x00000001)
#define HTU_IT_SW                       ((uint32_t)0x00000002)
#define HTU_IT_ERR                      ((uint32_t)0x00000004)

#define IS_HTU_IT(IT) ((((IT) & (uint32_t)0xFFFFFFF8) == 0x00000000) && ((IT) != 0x00000000))

#define IS_HTU_GET_IT(IT)               (((IT) == HTU_IT_OVF   ) || \
                                         ((IT) == HTU_IT_SW   ) || \
                                         ((IT) == HTU_IT_ERR   ))
/**
  * @}
  */

/** @defgroup HTU_FLAG HTU_FLAG
  * @{
  */ 
#define HTU_FLAG_OVF                   ((uint32_t)0x00000001)
#define HTU_FLAG_SW                    ((uint32_t)0x00000002)
#define HTU_FLAG_ERR                   ((uint32_t)0x00000004)


#define IS_HTU_CLEAR_FLAG(FLAG)         ((((FLAG) & (uint32_t)0xFFFFFFF8) == 0x00000000) && ((FLAG) != 0x00000000))

#define IS_HTU_GET_FLAG(FLAG)           (((FLAG) == HTU_FLAG_OVF ) || \
                                         ((FLAG) == HTU_FLAG_SW ) || \
                                         ((FLAG) == HTU_FLAG_ERR ))
/**
  * @}
  */



/** @defgroup HTU_HCTS HTU_HCTS
  * @{
  */ 
#define HTU_HCTS_Rising                    ((uint32_t)0x00)
#define HTU_HCTS_Falling                   ((uint32_t)0x01<<HTU_HCCT_HCTS_Pos)
#define HTU_HCTS_Rising_Falling            ((uint32_t)0x02<<HTU_HCCT_HCTS_Pos)
#define IS_HTU_HCTS(HTU_HCTS)             (((HTU_HCTS) == HTU_HCTS_Rising ) || \
                                           ((HTU_HCTS) == HTU_HCTS_Falling ) || \
                                           ((HTU_HCTS) == HTU_HCTS_Rising_Falling ))
/**
  * @}
  */
                                          
/** @defgroup HTU_HCIS HTU_HCIS
  * @{
  */ 
#define HTU_HCIS_CMP1                      ((uint32_t)0x00)
#define HTU_HCIS_CMP2                      ((uint32_t)0x01)
#define IS_HTU_HCIS(HTU_HCIS)             (((HTU_HCIS) == HTU_HCIS_CMP1 ) || \
                                           ((HTU_HCIS) == HTU_HCIS_CMP2 ))
/**
  * @}
  */

/** @defgroup HTU_WBRx HTU_WBRx
  * @{
  */ 
#define HTU_WBR0                   ((uint32_t)0x18)
#define HTU_WBR1                   ((uint32_t)0x1C)
#define HTU_WBR2                   ((uint32_t)0x20)
#define HTU_WBR3                   ((uint32_t)0x24)
#define HTU_WBR4                   ((uint32_t)0x28)
#define HTU_WBR5                   ((uint32_t)0x2C)
#define HTU_WBR6                   ((uint32_t)0x30)
#define HTU_WBR7                   ((uint32_t)0x34)
#define IS_HTU_WBRx(HTU_WBRx)             (((HTU_WBRx) == HTU_WBR0 ) || \
                                           ((HTU_WBRx) == HTU_WBR1 ) || \
                                           ((HTU_WBRx) == HTU_WBR2 ) || \
                                           ((HTU_WBRx) == HTU_WBR3 ) || \
                                           ((HTU_WBRx) == HTU_WBR4 ) || \
                                           ((HTU_WBRx) == HTU_WBR5 ) || \
                                           ((HTU_WBRx) == HTU_WBR6 ) || \
                                           ((HTU_WBRx) == HTU_WBR7))         
/**
  * @}
  */

/** @defgroup HTU_NFCR HTU_NFCR
  * @{
  */ 
#define IS_HTU_NFCR(LENGTH) ((LENGTH) <= 0xFF)

/**
  * @}
  */


void HTU_DeInit(void);
void HTU_TimeBaseInit(HTU_TimeBaseInitTypeDef* HTU_TimeBaseInitStruct);
void HTU_TimeBaseStructInit(HTU_TimeBaseInitTypeDef* HTU_TimeBaseInitStruct);
void HTU_SetCounter(uint16_t Counter);
void HTU_SetTimePeriod(uint16_t TimePeriod);
uint32_t HTU_GetCounter(void);
uint16_t HTU_GetClockDivision(void);
void HTU_ITConfig(uint32_t HTU_IT, FunctionalState NewState);
FlagStatus HTU_GetFlagStatus(uint32_t HTU_FLAG);
void HTU_ClearFlag(uint32_t HTU_FLAG);
ITStatus HTU_GetITStatus(uint32_t HTU_IT);
void HTU_ClearITPendingBit(uint32_t HTU_IT);
uint16_t HTU_GetHallRawSign(void);
uint16_t HTU_GetHallFilterSign(void);
uint32_t HTU_GetWBRValue(uint32_t HTU_WBRx);
uint32_t HTU_GetAverageValue(void);
void HTU_CompareConfig(uint32_t HCIS_CMPx , uint32_t TriggerEdge);
void HTU_CompareCmd(FunctionalState NewState);
void HTU_CounterCmd(FunctionalState NewState);
                                          

#endif /*__HK32M07X_HTU_H */
/**
  * @}
  */ 

/**
  * @}
  */

/***************************** (C) COPYRIGHT HKMicroChip  *******************************/
