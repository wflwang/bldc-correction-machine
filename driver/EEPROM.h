/*
 * eeprom.h
 *
 *  Created on: Aug. 12, 2025
 *      Author: MaxwellWang
    */
#ifndef __EEPROM_H
#define __EEPROM_H
#include "main.h"
#include "flash_helper.h"
#include <stdbool.h>

#define EEPROM_BASE_MCCONF    ADDR_FLASH_EEPROM_PAGE
#define EEPROM_BASE_APPCONF   ADDR_FLASH_EEPROM_PAGE+4

//hall state & angle(256) tanɵ = hally/hallx actan hally/hallx = @(-1 - 1)
#define ADDR_HALL_Ready     ADDR_FLASH_EEPROM_PAGE
#define ADDR_GYPO_Ready     ADDR_FLASH_EEPROM_PAGE+4
#define GYPO_Wz_offset      0
#define Acc_x_offset        0
#define Acc_y_offset        0

#define MaxAnglePage    16    //最大16page 4k byte
#define LOW_16_BITS  0x0000FFFF
#define HIGH_16_BITS 0xFFFF0000
#define SUMANG_HIGH_12_BITS_MASK 0x000FFFF0

//EEPROM read data
//int EE_ReadFOC(Learn_Componets *lc);
//void EE_WriteFOC(Learn_Componets *lc);
FLASH_Status EE_WriteConfig(uint32_t flash_addr, const void *src, uint32_t size);
void EE_ReadConfig(uint32_t flash_addr, void *dst, uint32_t size);
#endif
