/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_rcc.h
* @brief:     RCC initialization and configuration.
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_RCC_H
#define __HK32M07X_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup RCC RCC
  * @{
  */


/** @defgroup RCC_Exported_Types RCC_Exported_Types
  * @{
  */
typedef struct
{
    uint32_t SYSCLK_Frequency;
    uint32_t HCLK_Frequency;
    uint32_t PCLK_Frequency;
} RCC_ClocksTypeDef;

/**
  * @}
  */
/* Exported constants --------------------------------------------------------*/

/** @defgroup RCC_Exported_Constants RCC_Exported_Constants
  * @{
  */


/** @defgroup RCC_PREDIV_division_factor RCC_PREDIV_division_factor
  * @{
  */
#define  RCC_PREDIV_Div1               ((uint32_t)0x00 << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div2               ((uint32_t)0x01 << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div3               ((uint32_t)0x02 << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div4               ((uint32_t)0x03 << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div5               ((uint32_t)0x04 << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div6               ((uint32_t)0x05 << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div7               ((uint32_t)0x06 << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div8               ((uint32_t)0x07 << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div9               ((uint32_t)0x08 << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div10              ((uint32_t)0x09 << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div11              ((uint32_t)0x0A << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div12              ((uint32_t)0x0B << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div13              ((uint32_t)0x0C << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div14              ((uint32_t)0x0D << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div15              ((uint32_t)0x0E << RCC_CFGR2_PLLPREDIV_Pos)
#define  RCC_PREDIV_Div16              ((uint32_t)0x0F << RCC_CFGR2_PLLPREDIV_Pos)

#define IS_RCC_PREDIV(PREDIV)   (((PREDIV) == RCC_PREDIV_Div1) || ((PREDIV) == RCC_PREDIV_Div2) || \
                                 ((PREDIV) == RCC_PREDIV_Div3) || ((PREDIV) == RCC_PREDIV_Div4) || \
                                 ((PREDIV) == RCC_PREDIV_Div5) || ((PREDIV) == RCC_PREDIV_Div6) || \
                                 ((PREDIV) == RCC_PREDIV_Div7) || ((PREDIV) == RCC_PREDIV_Div8) || \
                                 ((PREDIV) == RCC_PREDIV_Div9) || ((PREDIV) == RCC_PREDIV_Div10) || \
                                 ((PREDIV) == RCC_PREDIV_Div11) || ((PREDIV) == RCC_PREDIV_Div12) || \
                                 ((PREDIV) == RCC_PREDIV_Div13) || ((PREDIV) == RCC_PREDIV_Div14) || \
                                 ((PREDIV) == RCC_PREDIV_Div15) || ((PREDIV) == RCC_PREDIV_Div16))
/**
  * @}
  */

/** @defgroup RCC_System_Clock_Source RCC_System_Clock_Source
  * @{
  */

#define RCC_SYSCLKSource_HSI            ((uint8_t)0x12)
#define RCC_SYSCLKSource_LSI            ((uint8_t)0x11)
#define RCC_SYSCLKSource_HSI8           ((uint8_t)0x00)
#define RCC_SYSCLKSource_EXTCLK         ((uint8_t)0x14)

#define IS_RCC_SYSCLK_SOURCE(SOURCE) (((SOURCE) == RCC_SYSCLKSource_HSI)   || \
                                      ((SOURCE) == RCC_SYSCLKSource_LSI)   || \
                                      ((SOURCE) == RCC_SYSCLKSource_HSI8)  || \
                                      ((SOURCE) == RCC_SYSCLKSource_EXTCLK))




/**
  * @}
  */


/** @defgroup RCC_EXTCLK_GPIO_source RCC_EXTCLK_GPIO_source
  * @{
  */
#define RCC_EXTCLK_GPIO_PA0              ((uint32_t)0x00 << RCC_CFGR4_EXTCLK_SEL_Pos)
#define RCC_EXTCLK_GPIO_PB15             ((uint32_t)0x01 << RCC_CFGR4_EXTCLK_SEL_Pos)

#define IS_RCC_EXTCLK_GPIO(GPIOCLK)      (((GPIOCLK) == RCC_EXTCLK_GPIO_PA0) || \
                                         ((GPIOCLK) == RCC_EXTCLK_GPIO_PB15))

/** @defgroup RCC_AHB_Clock_Source RCC_AHB_Clock_Source
  * @{
  */

#define RCC_SYSCLK_Div1                  ((uint32_t)0x00 << RCC_CFGR_HPRE_Pos)
#define RCC_SYSCLK_Div2                  ((uint32_t)0x08 << RCC_CFGR_HPRE_Pos)
#define RCC_SYSCLK_Div4                  ((uint32_t)0x09 << RCC_CFGR_HPRE_Pos)
#define RCC_SYSCLK_Div8                  ((uint32_t)0x0A << RCC_CFGR_HPRE_Pos)
#define RCC_SYSCLK_Div16                 ((uint32_t)0x0B << RCC_CFGR_HPRE_Pos)
#define RCC_SYSCLK_Div64                 ((uint32_t)0x0C << RCC_CFGR_HPRE_Pos)
#define RCC_SYSCLK_Div128                ((uint32_t)0x0D << RCC_CFGR_HPRE_Pos)
#define RCC_SYSCLK_Div256                ((uint32_t)0x0E << RCC_CFGR_HPRE_Pos)
#define RCC_SYSCLK_Div512                ((uint32_t)0x0F << RCC_CFGR_HPRE_Pos)
#define IS_RCC_HCLK(HCLK)                (((HCLK) == RCC_SYSCLK_Div1) || ((HCLK) == RCC_SYSCLK_Div2)  || \
                                         ((HCLK) == RCC_SYSCLK_Div4)  || ((HCLK) == RCC_SYSCLK_Div8)  || \
                                         ((HCLK) == RCC_SYSCLK_Div16) || ((HCLK) == RCC_SYSCLK_Div64) || \
                                         ((HCLK) == RCC_SYSCLK_Div128)|| ((HCLK) == RCC_SYSCLK_Div256)|| \
                                         ((HCLK) == RCC_SYSCLK_Div512))
/**
  * @}
  */

/** @defgroup RCC_APB_Clock_Source RCC_APB_Clock_Source
  * @{
  */

#define RCC_HCLK_Div1                    ((uint32_t)0x00 << RCC_CFGR_PPRE_Pos)
#define RCC_HCLK_Div2                    ((uint32_t)0x04 << RCC_CFGR_PPRE_Pos)
#define RCC_HCLK_Div4                    ((uint32_t)0x05 << RCC_CFGR_PPRE_Pos)
#define RCC_HCLK_Div8                    ((uint32_t)0x06 << RCC_CFGR_PPRE_Pos)
#define RCC_HCLK_Div16                   ((uint32_t)0x07 << RCC_CFGR_PPRE_Pos)
#define IS_RCC_PCLK(PCLK)                (((PCLK) == RCC_HCLK_Div1) || ((PCLK) == RCC_HCLK_Div2) || \
                                         ((PCLK) == RCC_HCLK_Div4) || ((PCLK) == RCC_HCLK_Div8)  || \
                                         ((PCLK) == RCC_HCLK_Div16))
/**
  * @}
  */


/** @defgroup RCC_I2C_clock_source RCC_I2C_clock_source
  * @{
  */

#define RCC_I2C1CLK_HSI                   ((uint32_t)0x00000000)
#define RCC_I2C1CLK_SYSCLK                ((uint32_t)0x01 << RCC_CFGR4_I2C1CLKSW_Pos)
#define RCC_I2C1CLK_PCLk2                 ((uint32_t)0x02 << RCC_CFGR4_I2C1CLKSW_Pos)


#define IS_RCC_I2CCLK(I2CCLK) (((I2CCLK) == RCC_I2C1CLK_HSI)    ||  \
                               ((I2CCLK) == RCC_I2C1CLK_SYSCLK) || \
                               ((I2CCLK) == RCC_I2C1CLK_PCLk2))


/**
  * @}
  */

/** @defgroup RCC_USART_clock_source RCC_USART_clock_source
  * @{
  */

#define RCC_UARTCLK_PCLK                  ((uint32_t)0x00)
#define RCC_UARTCLK_LSE                   ((uint32_t)0x01)
#define RCC_UARTCLK_LSI                   ((uint32_t)0x02)
#define RCC_UARTCLK_HSI12                 ((uint32_t)0x03)

#define IS_RCC_SELECTIONCLK(UARTCLK) (((UARTCLK) == RCC_UARTCLK_PCLK)   || \
                                      ((UARTCLK) == RCC_UARTCLK_LSE)    || \
                                      ((UARTCLK) == RCC_UARTCLK_LSI)    || \
                                      ((UARTCLK) == RCC_UARTCLK_HSI12))
/**
  * @}
  */


/** @defgroup RCC_FLITF_Prescaler_Factor  RCC_FLITF_Prescaler_Factor
  * @{
  */
#define RCC_FLITF_Div1        ((uint32_t)0x00)
#define RCC_FLITF_Div2        ((uint32_t)0x01)
#define RCC_FLITF_Div3        ((uint32_t)0x02)
#define RCC_FLITF_Div4        ((uint32_t)0x03)
#define RCC_FLITF_Div5        ((uint32_t)0x04)
#define RCC_FLITF_Div6        ((uint32_t)0x05)
#define RCC_FLITF_Div7        ((uint32_t)0x06)
#define RCC_FLITF_Div8        ((uint32_t)0x07)
#define RCC_FLITF_Div9        ((uint32_t)0x08)
#define RCC_FLITF_Div10       ((uint32_t)0x09)
#define RCC_FLITF_Div11       ((uint32_t)0x0A)
#define RCC_FLITF_Div12       ((uint32_t)0x0B)

#define IS_RCC_FLITFCLK_PREDIV(FLITFPREDIV) (((FLITFPREDIV) == RCC_FLITF_Div1)     || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div2)     || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div3)     || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div4)     || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div5)     || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div6)     || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div7)     || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div8)     || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div9)     || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div10)    || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div11)    || \
                                             ((FLITFPREDIV) == RCC_FLITF_Div12))
/**
  * @}
  */


/** @defgroup RCC_Interrupt_Source RCC_Interrupt_Source
  * @{
  */

#define RCC_IT_LSIRDY                    ((uint32_t)0x00000001)
#define RCC_IT_HSIRDY                    ((uint32_t)0x00000004)

#define IS_RCC_GET_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_HSIRDY))

#define IS_RCC_IT(IT)       ((IT) != 0x00)
#define IS_RCC_CLEAR_IT(IT) ((IT) != 0x00)

/**
  * @}
  */

/** @defgroup RCC_AHB_Peripherals RCC_AHB_Peripherals
  * @{
  */

#define RCC_AHBPeriph_GPIOA               RCC_AHBENR_IOPAEN
#define RCC_AHBPeriph_GPIOB               RCC_AHBENR_IOPBEN
#define RCC_AHBPeriph_DIVSQ               RCC_AHBENR_DVSQEN
#define RCC_AHBPeriph_FLITF               RCC_AHBENR_FLITFEN
#define RCC_AHBPeriph_SRAM                RCC_AHBENR_SRAMEN

#define IS_RCC_AHB_PERIPH(PERIPH)      ((((PERIPH) & 0xFFFFF2DD) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB_RST_PERIPH(PERIPH)  ((((PERIPH) & 0xFFFFF2FF) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */

/** @defgroup RCC_APB2_Peripherals RCC_APB2_Peripherals
  * @{
  */

#define RCC_APB2Periph_PGA             RCC_APB2ENR_PGAEN
#define RCC_APB2Periph_DBG             RCC_APB2ENR_DBGMCUEN
#define RCC_APB2Periph_I2C1            RCC_APB2ENR_I2C1EN
#define RCC_APB2Periph_COMP            RCC_APB2ENR_CMPEN
#define RCC_APB2Periph_UTU2            RCC_APB2ENR_UTU2EN
#define RCC_APB2Periph_UART2           RCC_APB2ENR_UART2EN
#define RCC_APB2Periph_SYSCFG          RCC_APB2ENR_SYSCFGEN

#define IS_RCC_APB2_PERIPH(PERIPH) ((((PERIPH) & 0xFF32FDFE) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */


/** @defgroup RCC_APB1_Peripherals RCC_APB1_Peripherals
  * @{
  */
#define RCC_APB1Periph_VDIV              RCC_APB1ENR_VDIVEN
#define RCC_APB1Periph_PWR               RCC_APB1ENR_PWREN
#define RCC_APB1Periph_UTU1              RCC_APB1ENR_UTU1EN
#define RCC_APB1Periph_ATU1              RCC_APB1ENR_ATU1EN
#define RCC_APB1Periph_HTU1              RCC_APB1ENR_HTU1EN
#define RCC_APB1Periph_SPI1              RCC_APB1ENR_SPI1EN
#define RCC_APB1Periph_WWDG              RCC_APB1ENR_WWDGEN
#define RCC_APB1Periph_UART1             RCC_APB1ENR_UART1EN
#define RCC_APB1Periph_ADC               RCC_APB1ENR_ADCEN


#define IS_RCC_APB1_PERIPH(PERIPH) ((((PERIPH) & 0xCFF8B7DE) == 0x00) && ((PERIPH) != 0x00))
/**
  * @}
  */

/** @defgroup RCC_MCO_Clock_Source RCC_MCO_Clock_Source
  * @{
  */

#define RCC_MCOSource_NoClock            ((uint8_t)0x07)
#define RCC_MCOSource_HCLK               ((uint8_t)0x00)
#define RCC_MCOSource_LSI                ((uint8_t)0x01)
#define RCC_MCOSource_HSI8M              ((uint8_t)0x03)
#define RCC_MCOSource_HSI                ((uint8_t)0x04)


#define IS_RCC_MCO_SOURCE(SOURCE) (((SOURCE) == RCC_MCOSource_NoClock) || ((SOURCE) == RCC_MCOSource_LSI)  || \
                                   ((SOURCE) == RCC_MCOSource_HSI8M)   || ((SOURCE) == RCC_MCOSource_HSI)  || \
                                    ((SOURCE) == RCC_MCOSource_HCLK))
/**
  * @}
  */

/** @defgroup RCC_Flag RCC_Flag
  * @{
  */
#define RCC_FLAG_HSIRDY                  ((uint8_t)0x01)
#define RCC_FLAG_LSIRDY                  ((uint8_t)0x41)
#define RCC_FLAG_OBLRST                  ((uint8_t)0x59)
#define RCC_FLAG_PINRST                  ((uint8_t)0x5A)
#define RCC_FLAG_PORRST                  ((uint8_t)0x5B)
#define RCC_FLAG_SFTRST                  ((uint8_t)0x5C)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x5D)
#define RCC_FLAG_WWDGRST                 ((uint8_t)0x5E)

#define IS_RCC_FLAG(FLAG) (((FLAG) == RCC_FLAG_HSIRDY)  || ((FLAG) == RCC_FLAG_LSIRDY)  || \
                           ((FLAG) == RCC_FLAG_OBLRST)  || ((FLAG) == RCC_FLAG_PINRST)  || \
                           ((FLAG) == RCC_FLAG_PORRST)  || ((FLAG) == RCC_FLAG_SFTRST)  || \
                           ((FLAG) == RCC_FLAG_IWDGRST) || ((FLAG) == RCC_FLAG_WWDGRST))

#define IS_RCC_HSI_TRIM_VALUE(VALUE)         ((VALUE) <= 0x1FF)
#define IS_RCC_LSI_FINE_TRIM_VALUE(VALUE)    ((VALUE) <= 0x07)
#define IS_RCC_LSI_COARSE_TRIM_VALUE(VALUE)  ((VALUE) <= 0x1F)

/**
  * @}
  */

/**
  * @}
  */
/** @defgroup RCC_Function_Declaration RCC_Function_Declaration
  * @{
  */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* Function used to set the RCC clock configuration to the default reset state */
void RCC_DeInit(void);

/* Internal clocks and MCO configuration functions *********/
void RCC_HSITRIM(uint16_t HSITRIMValue);
void RCC_HSICmd(FunctionalState NewState);
void RCC_LSITRIM(uint8_t HSIFineTRIMValue, uint8_t HSICoarseTRIMValue);
void RCC_LSICmd(FunctionalState NewState);
void RCC_MCOConfig(uint8_t RCC_MCOSource);

/* System, AHB and APB busses clocks configuration functions ******************/
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);
uint8_t RCC_GetSYSCLKSource(void);
void RCC_HCLKConfig(uint32_t RCC_SYSCLK);
void RCC_PCLKConfig(uint32_t RCC_HCLK);
void RCC_FLITFPREDIVConfig(uint32_t RCC_FLITFPRE_VALUE);
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);

/* Peripheral clocks configuration functions **********************************/
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void RCC_AHBPeriphResetCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);

/* Interrupts and flags management functions **********************************/
void RCC_ITConfig(uint32_t RCC_IT, FunctionalState NewState);
FlagStatus RCC_GetFlagStatus(uint32_t RCC_FLAG);
void RCC_ClearFlag(void);
ITStatus RCC_GetITStatus(uint32_t RCC_IT);
void RCC_ClearITPendingBit(uint32_t RCC_IT);
void RCC_EXTCLKGPIOConfig(uint32_t RCC_EXTCLK_GPIO);
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /*__HK32M07X_RCC_H */

/**
  * @}
  */

/**
  * @}
  */






