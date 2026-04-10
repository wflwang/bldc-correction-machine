/*
 * peripherals.h
 *
 *  Created on: July. 31, 2025
 *      Author: MaxwellWang
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

//#include    "mc_type.h"
//#include <stdint.h>
#include "hk32m07x_conf.h"
//#include "parameters_conversion.h"
//缓冲区大小
#define RX_BUFFER_SIZE           32 //必须是2的倍数
#if ((RX_BUFFER_SIZE&0x1)!=0)
#error RX_BUFFER_SIZE must be an even number (multiple of 2)
#endif
typedef struct 
{
	uint8_t Data[RX_BUFFER_SIZE];
	uint8_t Index;
	uint8_t Len;
	uint8_t FinishedFlag;
}UartRxBufDef;

//init GPIO
void MX_GPIO_Init(void);
void ADC_Init_Config(void);
void SYSCFG_Config(void);
void MY_PGA_Init(void);
void MX_NVIC_init(void);
void ATU_Init_Config(void);
void HTU_Init_Config(void);
void initCorePeripherals(void);
void Delay_ms(__IO uint32_t Delay);
void UART2_Config(void);
void delay_us(uint16_t us_x10);
void GetUartDebug(void);
void UartSendDatas(uint8_t *p, uint8_t len);
uint32_t Get1msTick(void);
void UTU_Config(void);
void Inc1msTick(void);


#endif
