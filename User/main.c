/*
 * Main program for HK32M070-based BLDC controller
 * VESC-style FOC control with Hall sensor and observer
 */

#include "hk32m07x.h"
#include "hk32m07x_rcc.h"
#include "hk32m07x_gpio.h"
#include "peripherals.h"
#include "mc_api.h"
#include "mc_tasks.h"

// System clock configuration
void SystemClock_Config(void) {
    RCC_DeInit();
    
    // Use 64MHz internal HSI clock
    RCC_HSICmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    while (RCC_GetSYSCLKSource() != 0x04);
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);    //64MHz
    RCC_PCLKConfig(RCC_HCLK_Div1);
}


// Main function
int main(void) {
    // System initialization
    SystemClock_Config();
    //初始化IO口
    initCorePeripherals();
    //读取电机配置
    GetMCConfig();
    GetAPPConfig(); //获取APP的配置
    //conf_general_init();
    /* Reconfigure the SysTick interrupt to fire every 500 us. */
    //SysTick_Config(SystemCoreClock / SYS_TICK_FREQUENCY);
    //初始化电机
    //mc_interface_init();
    /* Motor configuration */    
    MX_MotorControl_Init();
    /* Initialize interrupts */
    MX_NVIC_Init();
    //初始化马达部分 和中断
    Delay_ms(50);
    MC_StartMotor1();   //启动电机 速度为0
    Delay_ms(50);
    
    // Main loop
    while (1) {
        // Toggle GPIOB pins
        //GPIO_ToggleBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
        if(GetPPMUpdate()==1){
                //有新数据进来
            ClrPPMUpDate();
            int speed = UPDATA_SPEED(&appconf.app_PPM);
            MC_ProgramSpeedRampMotor1(speed,20);    //更新速度
        }
        // Simple delay
        //for (volatile int i = 0; i < 1000000; i++);
    }
}
