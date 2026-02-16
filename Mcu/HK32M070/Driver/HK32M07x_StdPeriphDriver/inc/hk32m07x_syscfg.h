/********************************************************************************
  * @Copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
  * @filename: hk32m07x_syscfg.h 
  * @brief: SYSCFG Initialization and Configuration functions
  * @Author: AE Team 
  * @version: V1.0.0/2022-05-24 
*********************************************************************************/

/*!< Define to prevent recursive inclusion -------------------------------------*/
#ifndef __hk32m07x_SYSCFG_H
#define __hk32m07x_SYSCFG_H

#ifdef __cplusplus
 extern "C" {
#endif

/*!< Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup SYSCFG SYSCFG
  * @{
  */

/** @defgroup SYSCFG_Exported_Constants SYSCFG_Exported_Constants
  * @{
  */ 
	
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/ 

/** @defgroup SYSCFG_EXTI_Port_Sources SYSCFG_EXTI_Port_Sources 
  * @{
  */ 
#define EXTI_PortSourceGPIOA       ((uint8_t)0x00)
#define EXTI_PortSourceGPIOB       ((uint8_t)0x01)
#define EXTI_PortSourceGPIOC       ((uint8_t)0x02)
#define EXTI_PortSourceGPIOF       ((uint8_t)0x05)

#define IS_EXTI_PORT_SOURCE(PORTSOURCE) (((PORTSOURCE) == EXTI_PortSourceGPIOA) || \
                                         ((PORTSOURCE) == EXTI_PortSourceGPIOB) || \
                                         ((PORTSOURCE) == EXTI_PortSourceGPIOC) || \
                                         ((PORTSOURCE) == EXTI_PortSourceGPIOF)) 
/**
  * @}
  */

/** @defgroup SYSCFG_EXTI_Pin_sources SYSCFG_EXTI_Pin_sources 
  * @{
  */ 
#define EXTI_PinSource0            ((uint8_t)0x00)
#define EXTI_PinSource1            ((uint8_t)0x01)
#define EXTI_PinSource2            ((uint8_t)0x02)
#define EXTI_PinSource3            ((uint8_t)0x03)
#define EXTI_PinSource4            ((uint8_t)0x04)
#define EXTI_PinSource5            ((uint8_t)0x05)
#define EXTI_PinSource6            ((uint8_t)0x06)
#define EXTI_PinSource7            ((uint8_t)0x07)
#define EXTI_PinSource8            ((uint8_t)0x08)
#define EXTI_PinSource9            ((uint8_t)0x09)
#define EXTI_PinSource10           ((uint8_t)0x0A)
#define EXTI_PinSource11           ((uint8_t)0x0B)
#define EXTI_PinSource12           ((uint8_t)0x0C)
#define EXTI_PinSource13           ((uint8_t)0x0D)
#define EXTI_PinSource14           ((uint8_t)0x0E)
#define EXTI_PinSource15           ((uint8_t)0x0F)

#define IS_EXTI_PIN_SOURCE(PINSOURCE) (((PINSOURCE) == EXTI_PinSource0) || \
                                       ((PINSOURCE) == EXTI_PinSource1) || \
                                       ((PINSOURCE) == EXTI_PinSource2) || \
                                       ((PINSOURCE) == EXTI_PinSource3) || \
                                       ((PINSOURCE) == EXTI_PinSource4) || \
                                       ((PINSOURCE) == EXTI_PinSource5) || \
                                       ((PINSOURCE) == EXTI_PinSource6) || \
                                       ((PINSOURCE) == EXTI_PinSource7) || \
                                       ((PINSOURCE) == EXTI_PinSource8) || \
                                       ((PINSOURCE) == EXTI_PinSource9) || \
                                       ((PINSOURCE) == EXTI_PinSource10) || \
                                       ((PINSOURCE) == EXTI_PinSource11) || \
                                       ((PINSOURCE) == EXTI_PinSource12) || \
                                       ((PINSOURCE) == EXTI_PinSource13) || \
                                       ((PINSOURCE) == EXTI_PinSource14) || \
                                       ((PINSOURCE) == EXTI_PinSource15))
/**
  * @}
  */

/** @defgroup SYSCFG_Memory_Remap_Config SYSCFG_Memory_Remap_Config 
  * @{
  */ 
#define SYSCFG_MemoryRemap_Flash                ((uint8_t)0x00)
#define SYSCFG_MemoryRemap_SRAM                 ((uint8_t)0x03)


#define IS_SYSCFG_MEMORY_REMAP(REMAP) (((REMAP) == SYSCFG_MemoryRemap_Flash) || \
                                       ((REMAP) == SYSCFG_MemoryRemap_SRAM))

/**
  * @}
  */

/** @defgroup SYSCFG_REF_LOCK_Config SYSCFG_REF_LOCK_Config
  * @{
  */

#define SYSCFG_REF_LOCK            SYSCFG_CFGR3_REF_LOCK
#define SYSCFG_REF_UNLOCK          ((uint32_t)0x00000000)

#define IS_SYSCFG_REF_LOCK_CONFIG(Buf) (((Buf) == SYSCFG_REF_LOCK)||\
																 ((Buf) == SYSCFG_REF_UNLOCK))

/**
  * @}
  */

/** @defgroup SYSCFG_LOCKUP_LOCK_Config SYSCFG_LOCKUP_LOCK_Config
  * @{
  */

#define SYSCFG_LOCKUP_LOCK            SYSCFG_CFGR2_LOCKUP_LOCK
#define SYSCFG_LOCKUP_UNLOCK          ((uint32_t)0x00000000)

#define IS_SYSCFG_LOCKUP_LOCK(NewStaus)    (((NewStaus) == SYSCFG_LOCKUP_LOCK)||\
																 ((NewStaus) == SYSCFG_LOCKUP_UNLOCK))

/**
  * @}
  */
  
 /** @defgroup SYSCFG_BUF1_EN_Config SYSCFG_BUF1_EN Config
  * @{
  */

#define SYSCFG_BUF1_EN                SYSCFG_CFGR3_EN_BUF1
#define SYSCFG_BUF1_DISABLE           ((uint32_t)0x00000000)

#define IS_SYSCFG_BUF1(Buf)           (((Buf) == SYSCFG_BUF1_EN)||\
																 ((Buf) == SYSCFG_BUF1_DISABLE))

/**
  * @}
  */
  
/**
  * @}
  
  */
 /** @defgroup SYSCFG_BUF0_EN_Config SYSCFG_BUF0_EN_Config
  * @{
  */

#define SYSCFG_BUF0_EN                SYSCFG_CFGR3_EN_BUF0
#define SYSCFG_BUF0_DISABLE           ((uint32_t)0x00000000)

#define IS_SYSCFG_Buf0(Buf)           (((Buf) == SYSCFG_BUF0_EN)||\
																 ((Buf) == SYSCFG_BUF0_DISABLE))

/**
  * @}
  */

 /** @defgroup SYSCFG_SW_VDIV_EN_Config SYSCFG_SW_VDIV_EN_Config 
  * @{
  */

#define SYSCFG_SW_VDIV_EN               SYSCFG_CFGR3_EN_SW_VDIV
#define SYSCFG_SW_VDIV_DISABLE          ((uint32_t)0x00000000)

#define IS_SYSCFG_SW_VDIV(Buf)          (((Buf) == SYSCFG_SW_VDIV_EN)||\
																 ((Buf) == SYSCFG_SW_VDIV_DISABLE))

/**
  * @}
  */

 /** @defgroup SYSCFG_SW_PGA1_EN_Config SYSCFG_SW_PGA1_EN_Config
  * @{
  */

#define SYSCFG_SW_PGA1_EN               SYSCFG_CFGR3_EN_SW_PGA1
#define SYSCFG_SW_PGA1_DISABLE          ((uint32_t)0x00000000)

#define IS_SYSCFG_SW_PGA1(Buf)          (((Buf) == SYSCFG_SW_PGA1_EN)||\
																 ((Buf) == SYSCFG_SW_PGA1_DISABLE))

/**
  * @}
  */

 /** @defgroup SYSCFG_SW_PGA2_EN_Config SYSCFG_SW_PGA2_EN_Config 
  * @{
  */

#define SYSCFG_SW_PGA2_EN                SYSCFG_CFGR3_EN_SW_PGA2
#define SYSCFG_SW_PGA2_DISABLE          ((uint32_t)0x00000000)

#define IS_SYSCFG_SW_PGA2(Buf)           (((Buf) == SYSCFG_SW_PGA2_EN)||\
																 ((Buf) == SYSCFG_SW_PGA2_DISABLE))

/**
  * @}
  */

 /** @defgroup SYSCFG_SW_PMU_EN_Config SYSCFG_SW_PMU_EN_Config
  * @{
  */

#define SYSCFG_SW_PMU_EN               SYSCFG_CFGR3_EN_SW_PMU
#define SYSCFG_SW_PMU_DISABLE          ((uint32_t)0x00000000)

#define IS_SYSCFG_SW_PMU(Buf)          (((Buf) == SYSCFG_SW_PMU_EN)||\
																 ((Buf) == SYSCFG_SW_PMU_DISABLE))

/**
  * @}
  */
  
  /** @defgroup SYSCFG_CFGR3_IBIAS_EN_Config SYSCFG_CFGR3_IBIAS_EN_Config
  * @{
  */

#define SYSCFG_IBIAS_EN               SYSCFG_CFGR3_EN_IBIAS
#define SYSCFG_IBIAS_DISABLE          ((uint32_t)0x00000000)

#define IS_SYSCFG_IBIAS(NewStaus)          (((NewStaus) == SYSCFG_IBIAS_EN)||\
																 ((NewStaus) == SYSCFG_IBIAS_DISABLE))


/** @defgroup SYSCFG_IBIAS_ADJ_GAIN_Config SYSCFG_IBIAS_ADJ_GAIN_Config
  * @{
  */ 
#define SYSCFG_IBIAS_ADJ_GAIN_0               ((uint32_t)0x00000000)
#define SYSCFG_IBIAS_ADJ_GAIN_1           	  ((uint32_t)0x00000001)
#define SYSCFG_IBIAS_ADJ_GAIN_2               ((uint32_t)0x00000002)
#define SYSCFG_IBIAS_ADJ_GAIN_3               ((uint32_t)0x00000003)


#define IS_SYSCFG_IBIAS_ADJ_GAIN(REMAP)       (((REMAP) == SYSCFG_IBIAS_ADJ_GAIN_0) || \
																				 ((REMAP) == SYSCFG_IBIAS_ADJ_GAIN_1) || \
																				 ((REMAP) == SYSCFG_IBIAS_ADJ_GAIN_2) || \
																				 ((REMAP) == SYSCFG_IBIAS_ADJ_GAIN_3) )				
																 
/**
  * @}
  */
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @defgroup SYSCFG_Exported_Functions SYSCFG_Exported_Functions
  * @{
  */

/*  Function used to set the SYSCFG configuration to the default reset state **/
void SYSCFG_DeInit(void);

/* SYSCFG configuration functions *********************************************/ 
void SYSCFG_MemoryRemapConfig(uint32_t SYSCFG_MemoryRemap);

void SYSCFG_EXTILineConfig(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex);

void SYSCFG_LOCKUP_LOCK_Break(FunctionalState NewStaus);

void SYSCFG_IOFLT_Config(uint32_t SYSCFG_IO_FLT);

void SYSCFG_REF_LOCK_Config(uint32_t REF_LOCK);

void SYSCFG_BUF1_Config(FunctionalState NewStaus);

void SYSCFG_BUF0_Config(FunctionalState NewStaus);

void SYSCFG_SW_VDIV_Config(FunctionalState NewStaus);

void SYSCFG_SW_PGA1_Config(FunctionalState NewStaus);

void SYSCFG_SW_PGA2_Config(FunctionalState NewStaus);

void SYSCFG_SW_PMU_Config(FunctionalState NewStaus);

void SYSCFG_IBIAS_Config(uint32_t SYSCFG_IBIAS_ADJ_GAIN,FunctionalState NewStaus);

/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__hk32m07x_SYSCFG_H */
