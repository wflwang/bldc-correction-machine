/*
 * Hardware configuration for HK32M070-based BLDC controller
 */

#include "hw_conf.h"
#include "hk32m07x.h"
#include "hk32m07x_gpio.h"
#include "hk32m07x_rcc.h"

void hw_init_gpio(void) {
    // Enable GPIO clocks
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    // Configure Hall sensor pins
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = HW_HALL_ENC_PIN1 | HW_HALL_ENC_PIN2 | HW_HALL_ENC_PIN3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_4;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable;
    GPIO_Init(HW_HALL_ENC_GPIO1, &GPIO_InitStructure);
}

void hw_setup_adc_channels(void) {
    // ADC configuration is done in mcpwm_foc_init
}

void hw_start_i2c(void) {
    // I2C not used
}

void hw_stop_i2c(void) {
    // I2C not used
}

void hw_try_restore_i2c(void) {
    // I2C not used
}
