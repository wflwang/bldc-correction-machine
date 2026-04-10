/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  main.h
* @brief:     This file contains all the functions prototypes for the main.c file.
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_conf.h"
//#include "hk32m07x_valid.h"

//#define M1_VR_VOLTAGE_ADC_RESULT_REG   (ADC->ADDR2B)
//#define M1_BUS_VOLTAGE_ADC_RESULT_REG  (ADC->ADDR3B)
//#define M1_TEMPERATURE_ADC_RESULT_REG  (ADC->ADDR4B)
//
////ATU PWM�������
//#define PWMIO_PB10      ATU_PWM_REMAP_SOURCE_TIO2A
//#define PWMIO_PB11      ATU_PWM_REMAP_SOURCE_TIO2B
//#define PWMIO_PB12      ATU_PWM_REMAP_SOURCE_TIO1A
//#define PWMIO_PB13      ATU_PWM_REMAP_SOURCE_TIO1B
//#define PWMIO_PB14      ATU_PWM_REMAP_SOURCE_TIO0A
//#define PWMIO_PB15      ATU_PWM_REMAP_SOURCE_TIO0B
//
//
//#define M1_HALL_H1_Pin             GPIO_Pin_0
//#define M1_HALL_H1_Pin_NO          GPIO_PinSource0
//#define M1_HALL_H1_GPIO_Port       GPIOB
//
//#define M1_HALL_H2_Pin             GPIO_Pin_1
//#define M1_HALL_H2_Pin_NO          GPIO_PinSource1
//#define M1_HALL_H2_GPIO_Port       GPIOB
//
//#define M1_HALL_H3_Pin             GPIO_Pin_2
//#define M1_HALL_H3_Pin_NO          GPIO_PinSource2
//#define M1_HALL_H3_GPIO_Port       GPIOB
//
//#define HALL_TIMER                 HTU

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void speechEn(uint32_t time,uint32_t count);
#ifdef __cplusplus
}
#endif

#endif /*__MAIN_H */

