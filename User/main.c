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
#include "motorcontrol.h"
#include "hw_correct.h"

extern int32_t Count1ms;        //1ms count

extern app_config_t appconf;   //app config
// System clock configuration
void SystemClock_Config(void)
{
    RCC_DeInit();  // 复位时钟

    // 开启内部 HSI
    RCC_HSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

    // 选择 HSI 作为系统时钟
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

    // ✅ 关键：HK32M070 必须判断 0x00
    while(RCC_GetSYSCLKSource() != 0x00);

    // HCLK = SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLKConfig(RCC_HCLK_Div1);
}


// Main function
int main(void) {
    // System initialization
    //SystemClock_Config();
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
    MX_NVIC_init();
    //初始化马达部分 和中断
    Delay_ms(50);
    MC_StartMotor1();   //启动电机 速度为0
    Delay_ms(50);
    #ifdef MainOutMCTime
    printfMC(0);
    #endif
    
    // Main loop
    while (1) {
        // Toggle GPIOB pins
        //GPIO_ToggleBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
        #ifdef MainOutMCTime
        if(Count1ms>MainOutMCTime){ //每10ms更新一次 VBus dp电压 三相电流 
            Count1ms = 0;   //10ms once
            printfMC(1); //输出电机信息
        }
        ScanUartRX();
        #endif
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

void speechEn(uint32_t time,uint32_t count){
  //发出一次声�?
  SetMSpeechEN(1);
  MC_StartMotor1();
  Delay_ms(100);
  MC_ProgramSpeedRampMotor1(0,0);  //400
  while(count--){
    SetMSpeechEN(1);
    Delay_ms(time);
    SetMSpeechEN(0);
    Delay_ms(100);
  }
  SetMSpeechEN(0);
  Delay_ms(20);
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */
    /* USER CODE END HardFault_IRQn 0 */
    TSK_HardwareFaultTask();

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {

    }

    /* USER CODE BEGIN HardFault_IRQn 1 */
    /* USER CODE END HardFault_IRQn 1 */
}
