/**
  ******************************************************************************
  * @file    system_hk32m07x.h 
  ****************************************************************************** 
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup hk32m07x_system
  * @{
  */  
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_hk32m07x_H
#define __SYSTEM_hk32m07x_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup hk32m07x_System_Includes
  * @{
  */

/**
  * @}
  */


/** @addtogroup hk32m07x_System_Exported_types
  * @{
  */

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */

/**
  * @}
  */

/** @addtogroup hk32m07x_System_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @addtogroup hk32m07x_System_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup hk32m07x_System_Exported_Functions
  * @{
  */
  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_hk32m07x_H */

/**
  * @}
  */


