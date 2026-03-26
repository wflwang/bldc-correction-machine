/*
 * Main program for HK32M070-based BLDC controller
 * VESC-style FOC control with Hall sensor and observer
 */

#include "hk32m07x.h"
#include "hk32m07x_rcc.h"
#include "hk32m07x_gpio.h"
#include "peripherals.h"

// System clock configuration
void SystemClock_Config(void) {
    RCC_DeInit();
    
    // Use 64MHz internal HSI clock
    RCC_HSICmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    while (RCC_GetSYSCLKSource() != 0x04);
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLKConfig(RCC_HCLK_Div1);
}


// Main function
int main(void) {
    // System initialization
    SystemClock_Config();
    //初始化IO口
    initCorePeripherals();
    //读取电机配置
    conf_general_init();
    //初始化电机
    mc_interface_init();
    
    // Main loop
    while (1) {
        // Toggle GPIOB pins
        GPIO_ToggleBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
        
        // Simple delay
        for (volatile int i = 0; i < 1000000; i++);
    }
}
