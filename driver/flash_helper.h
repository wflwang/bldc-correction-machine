/*
 * flash_helper.h
 *
 *  Created on: Aug. 12, 2025
 *      Author: MaxwellWang
    */

#ifndef FLASH_HELPER_H_
#define FLASH_HELPER_H_

//#include "conf_general.h"
#include "main.h"
//#include "hk32m06x_flash.h"

//#define CODE_IND_QML	0
//#define CODE_IND_LISP	1
/**
 * 070 64page page_size 512byte
 * 
 * 
 * Defines
 */
#define FLASH_PAGE							    64U	//总page大小
#define FLASH_PAGE_SIZE						    512U	//每个page字节数

#define EEPROM_SIZE_APPCONF				        1U		//EEPROM 使用大小
#define EEPROM_BASE_APPCONF_PAGE                (FLASH_PAGE-EEPROM_SIZE_APPCONF)		//EEPROM 使用大小
#define EEPROM_SIZE_MCCONF					    1U
#define EEPROM_BASE_MCCONF_PAGE                 (EEPROM_BASE_APPCONF_PAGE-EEPROM_SIZE_MCCONF)
#define BOOTLOADER_SIZE							8U   //4k IAP
#define BOOTLOADER_BASE_PAGE                    (EEPROM_BASE_MCCONF_PAGE-BOOTLOADER_SIZE)
#define APP_BASE								0U
#define APP_BASE_SIZE							50U		//APP 使用的page大小  50*512 = 25K

//#define EEPROM_AngleOffset                      240
//#define EEPROM_AngleOffset_Size                 1       //角度偏移量

//#define EEPROM_Mode                     241
//#define EEPROM_Mode_Size                1       //角度偏移量

// Base address of the Flash sectors
#define ADDR_FLASH_START					    ((uint32_t)0x08000000) // Base @ of Sector 0, 16 Kbytes
#define ADDR_FLASH_BOOTLOADER				    (ADDR_FLASH_START+(BOOTLOADER_BASE_PAGE*FLASH_PAGE_SIZE))
#define ADDR_FLASH_APP_PAGE						(ADDR_FLASH_START+(APP_BASE*FLASH_PAGE_SIZE))
#define ADDR_FLASH_EEPROM_MCCONF				(ADDR_FLASH_START+(EEPROM_BASE_MCCONF_PAGE*FLASH_PAGE_SIZE)) 
#define ADDR_FLASH_EEPROM_APPCONF				(ADDR_FLASH_START+(EEPROM_BASE_APPCONF_PAGE*FLASH_PAGE_SIZE)) 

// Functions
FLASH_Status write_data(uint32_t base, uint32_t *data, uint32_t len);
FLASH_Status erase_page(uint32_t start_addr_page,uint32_t new_app_size);
FLASH_Status flash_helper_erase_new_app(void);
FLASH_Status flash_helper_erase_bootloader(void);
FLASH_Status flash_helper_write_new_app_data(uint32_t offset, uint32_t *data, uint32_t len);
void flash_helper_jump_to_bootloader(void);

#endif /* FLASH_HELPER_H_ */
