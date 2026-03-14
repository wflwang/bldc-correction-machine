/*
 * Main program for HK32M070-based BLDC controller
 * VESC-style FOC control with Hall sensor and observer
 */

#include "hk32m07x.h"
#include "hk32m07x_rcc.h"
#include "hk32m07x_gpio.h"

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

// GPIO initialization
void GPIO_Config(void) {
    // Enable GPIO clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    
    // Configure GPIO pins
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // Set GPIOA pins as input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // Set GPIOB pins as output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// Main function
int main(void) {
    // System initialization
    SystemClock_Config();
    
    // GPIO initialization
    GPIO_Config();
    
    // Main loop
    while (1) {
        // Toggle GPIOB pins
        GPIO_ToggleBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
        
        // Simple delay
        for (volatile int i = 0; i < 1000000; i++);
    }
}
