/*
 * flash helper.c
 *
 *  Created on: Aug. 12, 2025
 *      Author: MaxwellWang
    */

#include "flash_helper.h"
//#include "ch.h"
//#include "hal.h"
//#include "stm32f4xx_conf.h"
//#include "utils_sys.h"
//#include "mc_interface.h"
//#include "timeout.h"
//#include "hw.h"
//#include "crc.h"
//#include "buffer.h"
//#include <string.h>
//#include	"main.h"

#ifdef USE_LISPBM
#include "lispif.h"
#endif

//#define VECTOR_TABLE_ADDRESS					((uint32_t*)ADDR_FLASH_SECTOR_0)
//#define VECTOR_TABLE_SIZE						((uint32_t)(ADDR_FLASH_SECTOR_1 - ADDR_FLASH_SECTOR_0))
//#define EEPROM_EMULATION_SIZE					((uint32_t)(ADDR_FLASH_SECTOR_4 - ADDR_FLASH_SECTOR_2))
//#define	APP_CRC_WAS_CALCULATED_FLAG				((uint32_t)0x00000000)
//#define	APP_CRC_WAS_CALCULATED_FLAG_ADDRESS		((uint32_t*)(ADDR_FLASH_SECTOR_0 + APP_MAX_SIZE - 8))
//#define APP_CRC_ADDRESS							((uint32_t*)(ADDR_FLASH_SECTOR_0 + APP_MAX_SIZE - 4))
//#define ERASE_VOLTAGE_RANGE						(uint8_t)((PWR->CSR & PWR_CSR_PVDO) ? VoltageRange_2 : VoltageRange_3)

// Private functions

/**
 * @param new_app_size  max is 64 page
 * @param pageNumc 擦除page的数量
 * 擦除目标APP FLASH 数据
*/
FLASH_Status erase_page(uint32_t start_addr_page,uint32_t pageNum) {
	if((start_addr_page+pageNum)>FLASH_PAGE){
		return FLASH_ERASE_PAGE_OVER;
	}
	FLASH_Unlock();
	/* Clear pending flags (if any) */
  	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR );
	//timeout_configure_IWDT_slowest();

	for (uint32_t i = 0;i < pageNum ;i++) {
    	if(FLASH_ErasePage(ADDR_FLASH_START+((start_addr_page+i)*FLASH_PAGE_SIZE)) != FLASH_COMPLETE)
    	{
    	  /* Error occurred while sector erase.
    	      User can add here some code to deal with this error */
			  //没有擦除成功
			  break;
    	}
	}

	FLASH_Lock();
	//timeout_configure_IWDT();
	//mc_interface_ignore_input_both(100);
	//utils_sys_unlock_cnt();

	return FLASH_COMPLETE;
}
/**
 * @param new_app_size  max is 64 page
 * 写 目标 APP FLASH 数据
*/
FLASH_Status write_data(uint32_t base, uint32_t *data, uint32_t len) {
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);
	/* 
	mc_interface_ignore_input_both(5000);
	mc_interface_release_motor_override_both();
	if (!mc_interface_wait_for_motor_release_both(3.0)) {
		return 100;
	}
	*/
	//utils_sys_lock_cnt();
	//timeout_configure_IWDT_slowest();
	if(len>64){
		len = 64;	//max wirte 64 word
	}

	for (uint32_t i = 0;i < len;i++) {
		if(FLASH_ProgramWord(base + i*4, data[i])!=FLASH_COMPLETE){
			break;
		}
	}

	FLASH_Lock();
	//timeout_configure_IWDT();
	//mc_interface_ignore_input_both(100);
	//utils_sys_unlock_cnt();

	return FLASH_COMPLETE;
}

/**
 * @param new_app_size  max is 64 page
 * 擦除目标APP FLASH 数据
*/
FLASH_Status flash_helper_erase_new_app(void) {
	return erase_page(APP_BASE,APP_BASE_SIZE);
}

FLASH_Status flash_helper_erase_bootloader(void) {
	return erase_page(BOOTLOADER_BASE_PAGE,BOOTLOADER_SIZE);
}

FLASH_Status flash_helper_write_new_app_data(uint32_t offset, uint32_t *data, uint32_t len) {
	return write_data(ADDR_FLASH_APP_PAGE + offset, data, len);
}

/**
 * Stop the system and jump to the bootloader.
 */
void flash_helper_jump_to_bootloader(void) {
    typedef void (*pFunction)(void);
    pFunction jump_to_bootloader;

    // 指向 Bootloader 向量表的基地址（volatile 访问是必要的）
    const volatile uint32_t* bootloader_address = (volatile uint32_t*)BOOTLOADER_BASE_PAGE;

    // 直接读取堆栈指针初始值（向量表第 0 项）和复位向量（第 1 项）
    uint32_t boot_msp   = bootloader_address[0];
    uint32_t boot_reset = bootloader_address[1];

    // 将复位向量地址转换为函数指针（此时 boot_reset 是普通 uint32_t，无 volatile 属性）
    jump_to_bootloader = (pFunction) boot_reset;

    // 关闭全局中断
    __disable_irq();

    // 清除所有挂起的中断
#ifdef NVIC_ICPR0
    for (int i = 0; i < (int)(sizeof(NVIC->ICPR) / sizeof(NVIC->ICPR[0])); i++) {
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }
#else
    NVIC->ICPR[0] = 0xFFFFFFFF;
#endif

    // 禁用所有中断
#ifdef NVIC_ICER0
    for (int i = 0; i < (int)(sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0])); i++) {
        NVIC->ICER[i] = 0xFFFFFFFF;
    }
#else
    NVIC->ICER[0] = 0xFFFFFFFF;
#endif

    // 设置主堆栈指针（MSP）为 Bootloader 的初始栈地址
    __set_MSP(boot_msp);

    // 跳转到 Bootloader
    jump_to_bootloader();
}
