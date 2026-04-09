/*
 * eeprom.c
 *
 *  Created on: Aug. 12, 2025
 *      Author: MaxwellWang
    */
   #include     "EEPROM.h"
   //#include   "mc_type.h"
   #include     <stdio.h>
/**
 * 
 * 初始化EEPROM 
 * 读出EEPROM中的数据 对应的是256*2 => 256 个角度的x y霍尔值
 * 
*/
void EE_init(void){
}
/**
 * @brief write APP config
 * 
 * 
*/
/**
 * @brief 把结构体数据写入Flash
 * @param flash_addr  目标Flash地址
 * @param src         源数据指针
 * @param size        数据长度
 * @return FLASH_Status 状态
 */
FLASH_Status EE_WriteConfig(uint32_t flash_addr, const void *src, uint32_t size)
{
    FLASH_Status FLASHStatus;  // 放最前面
    uint32_t halfword_cnt = (size + 1) / 2;
    const uint8_t *p_src = (const uint8_t*)src;
    uint32_t primask = __get_PRIMASK();

    __disable_irq();   // 关中断

    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);

    // 擦除页
    FLASHStatus = FLASH_ErasePage(flash_addr);
    if(FLASHStatus != FLASH_COMPLETE)
    {
        FLASH_Lock();
        if (!(primask & 0x1)) __enable_irq();
        return FLASHStatus;
    }

    // 半字写入（自动补齐奇数长度）
    for(uint32_t i = 0; i < halfword_cnt; i++)
    {
        uint32_t addr = flash_addr + i*2;
        uint16_t data;

        if((i*2 + 1) < size)
        {
            // 正常2字节
            data = (p_src[i*2+1] << 8) | p_src[i*2];
        }
        else
        {
            // 最后1字节 → 高8位补 0xFF
            data = 0xFF00 | p_src[i*2];
        }

        FLASHStatus = FLASH_ProgramHalfWord(addr, data);
        if(FLASHStatus != FLASH_COMPLETE)
        {
            FLASH_Lock();
            if (!(primask & 0x1)) __enable_irq();
            return FLASHStatus;
        }
    }

    FLASH_Lock();

    // 恢复中断
    if (!(primask & 0x1))
    {
        __enable_irq();
    }

    return FLASH_COMPLETE;
}
/**
 * @brief 读取flash -> config
 * @param flash_addr
 * @param dst struct config
 * @param size
 */
void EE_ReadConfig(uint32_t flash_addr, void *dst, uint32_t size)
{
    uint8_t *p_dst = (uint8_t*)dst;
    uint8_t *p_src = (uint8_t*)(uintptr_t)flash_addr;

    for(uint32_t i=0; i<size; i++)
    {
        p_dst[i] = p_src[i];
    }
}


