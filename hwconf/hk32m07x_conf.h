/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_conf.h
* @brief:     This file contains the configure information.
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_CONF_H
#define __HK32M07X_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#define HK32M07x

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @addtogroup HK32F4xx_System_Private_Defines HK32M07x_System_Private_Defines
  * @{
  */
 
#define SYSCLK_SRC_HSI8M        0x1
#define SYSCLK_SRC_HSI64M       0x2
#define SYSCLK_SRC_EXTCLK_IO    0x3
#define SYSCLK_SRC_LSI			0x4

/* select sysclk source here */
#define SYSCLK_SOURCE    SYSCLK_SRC_HSI64M     

// #define VECT_TAB_SRAM
#define VECT_TAB_OFFSET  0x0 /*!< Vector Table base offset field. This value must be a multiple of 0x200. */

#if(SYSCLK_SOURCE==SYSCLK_SRC_HSI64M)
	#define SYSCLK_FREQ_HSI_64M                64000000
#elif(SYSCLK_SOURCE == SYSCLK_SRC_HSI8M)
    #define SYSCLK_FREQ_HSI_8M                 8000000    
#elif(SYSCLK_SOURCE == SYSCLK_SRC_EXTCLK_IO)
    #define SYSCLK_FREQ_EXTCLK                 8000000
#elif(SYSCLK_SOURCE == SYSCLK_SRC_LSI)
	#define SYSCLK_FREQ_LSI    			LSI_VALUE
#endif

#if !defined  (HSI8_VALUE)
  #define HSI8_VALUE    ((uint32_t)8000000U) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

#if !defined  (LSI_VALUE)
  #define LSI_VALUE    ((uint32_t)40000U) /*!< Value of the Internal oscillator in Hz*/
#endif /* LSI_VALUE */

#if !defined  (HSI64_VALUE)
  #define HSI64_VALUE    ((uint32_t)64000000U) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI64_VALUE */

#if !defined  (EXTCLK_GPIOINPUT_VALUE)
  #define EXTCLK_GPIOINPUT_VALUE    ((uint32_t)8000000U) /*!< Value of the Internal oscillator in Hz*/
#endif /* EXTCLK_GPIOINPUT_VALUE */

#if !defined  (HSI_STARTUP_TIMEOUT)
#define HSI_STARTUP_TIMEOUT   ((uint16_t)0xFFFF) /*!< Time out for HSI start up */
#endif


/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file 
  */
  
/* Exported macro ------------------------------------------------------------*/
#if defined HK32M07x
    #include "hk32m07x_adc.h"
    #include "hk32m07x_dbgmcu.h"
    #include "hk32m07x_exti.h"
    #include "hk32m07x_flash.h"
    #include "hk32m07x_gpio.h"
    #include "hk32m07x_syscfg.h"
    #include "hk32m07x_i2c.h"
    #include "hk32m07x_iwdg.h"
    #include "hk32m07x_pwr.h"
    #include "hk32m07x_rcc.h"
    #include "hk32m07x_spi.h"
    #include "hk32m07x_divsqrt.h"
    #include "hk32m07x_uart.h"
    #include "hk32m07x_wwdg.h"
    #include "hk32m07x_misc.h"  
    #include "hk32m07x_cmp.h"
    #include "hk32m07x_pga.h"
    #include "hk32m07x_atu.h"
    #include "hk32m07x_utu.h"
    #include "hk32m07x_htu.h"
    #include "hk32m07x_vdiv.h"
#else
    #error "Please select first the target hk32m07x device used in your application (in hk32m07x.h file)"
#endif
/**
  * @}
  */
/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */
#ifdef __cplusplus
}
#endif  /* End of __cplusplus */  

#endif /*__HK32M07X_CONF_H */


/**
  * @}
  */
