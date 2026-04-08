/****
 * @file peripherals.c
 * @date Created on: July. 28, 2025
 * @author MaxwellWang
 */

#include "peripherals.h"
#include "hw_correct.h"
//#include "conf_general.h"
#include "drive_parameters.h"
#include "parameters_conversion.h"
#include "main.h"


static __IO uint32_t msTick=0;
//extern FOC_Component FOC_Component_M1;
//#include "targets.h"
//extern void UART1_IRQHandler(void);
/**
 * system init
*/
void initCorePeripherals(void){
    /* ADC configuration */
    ADC_Init_Config();
    /* SYSCFG configuration */
    SYSCFG_Config();
    /* PGA configuration */
    MY_PGA_Init();
    //
    ATU_Init_Config();
    //init hall
    HTU_Init_Config();
    UTU_Config();   //PPM scan
    //设置tick time
    //SysTick_Config(SystemCoreClock /SYS_TICK_FREQUENCY);
    //EE_Read();
    MX_GPIO_Init();

    //while(GetONOFF()==0){
    //    //LEDG_Xor();
    //    Delay_ms(1);
	//	//readQmi8658b();	//读出参数	
    //    fScanButton();   //扫描按键功能
    //}
    UART2_Config();
    //MX_NVIC_init();
    //qmi8658x_init(GYPO_SDA_GPIO_PORT,GYPO_SDA_GPIO_PIN,GYPO_SCL_GPIO_PORT,GYPO_SCL_GPIO_PIN);
    //SysTick_Config(SystemCoreClock /SYS_TICK_FREQUENCY);
}
/**
  * @brief  SYSCFG configuration
  * @param  None
  * @retval None
  */
void SYSCFG_Config(void)
{
    /*Enable the SYSCFG clock*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /*UNLOCK the SYSCFG_CFGR3 Register*/
    SYSCFG_REF_LOCK_Config(SYSCFG_REF_UNLOCK);
    
    /*Enable the BUF0_OUT*/
    //SYSCFG_BUF0_Config(ENABLE);
    
    /*Enable the PGA1 out to BUF0_OUT*/
    //SYSCFG_SW_PGA1_Config(ENABLE);
}
/**
  * @brief  PGA configuration
  * @param  None
  * @retval None
  */
void MY_PGA_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    PGA1_InitTypeDef PGA1_InitStruct;
    PGA2_InitTypeDef PGA2_InitStruct;
    
    /* PGA Peripheral clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_PGA, ENABLE);
    
    /* GPIO Peripheral clock enable */
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE);
    
    /* Configure PGA1_Px/Nx as analog input */
    //PB7 5 P PB6 4 N
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_4 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_4;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable;    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* PGA DeInit */
    PGA_DeInit();
    
    /* PGA Software Calibration */
    PGA_SW_Calibration(PGA1, OPA_GAIN);
    //PGA_SW_Calibration(PGA2, PGA1_PGA_GAIN_12);
    
    /* PGAx configured as follow*/
    PGA1_InitStruct.PGA1_NPLockMode = PGA1_NPLock_N0; //PGA1_N NOLock
    PGA1_InitStruct.PGA1_NInputSelect = PGA1_NInputSelectIO; //PGA1_N is selected from an external pin input
    PGA1_InitStruct.PGA1_PAInputSelect = PGA1_PA1; //PGA1_PA Select input from external pin PA1
    PGA1_InitStruct.PGA1_PBInputSelect = PGA1_PB1; //PGA1_PA Select input from external pin PB1
    PGA1_InitStruct.PGA1_CHInputSelect = PGA1_InputSelectPANA; //PGA1 Input Select Group A(Handmode takes effect)
    PGA1_InitStruct.PSAMSelect = PGA1_PSAMSelectAuto; //Select the channel by ADC(Auto)
    PGA1_InitStruct.PGA1_GAIN = OPA_GAIN; //PGA_GAIN = 4
    PGA2_InitStruct.PGA2_NInputSelect = PGA2_NInputSelectAVSS;  //PGA2_NInputSelectN1; //PGA_N Select N1
    PGA2_InitStruct.PGA2_PInputSelect = PGA2_PInputSelect_PGA1OUT;  //PGA2_PInputSelect_P1; //PGA_P Select P1
    PGA2_InitStruct.PGA2_GAIN = PGA2_PGA_GAIN_12; //PGA_GAIN = 4
    PGA_Init(&PGA1_InitStruct, &PGA2_InitStruct);
    
    /* PGA user mode configured*/
    PGA_VIM_USER(PGA1);
    //PGA_VIM_USER(PGA2);
    
    /* Enable PGA*/
    PGA_Enable(PGA1);
    //PGA_Enable(PGA2);
    
    /* Configure the bias voltage*/
    // /2 /4 /8 /16
    PGA_SetBias( ENABLE, DISABLE, VrefDiv1); //1/2Vref
    PGA_Lock();
}
/**
  * @brief  ADC channel configuration
  * @param  None
  * @retval None
  */
void ADC_Init_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    Group_InitTypeDef Group_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    //NVIC_InitTypeDef NVIC_InitStructure;
    
    /* ADC GPIO Peripheral clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_ADC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    /* Configure PA3,PA8,PA9 as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_4;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable;    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //| GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0    这3个不是AD口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_9 ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* Configure groupA */
    Group_InitStructure.ADC_Group  = GROUP_A; /* Select Configure GROUP A */
    Group_InitStructure.ADC_GroupChannel[ASCS_0] = ADC_CHANNEL_PGA1_OUTA; /* Select the input source for the group channel */
    Group_InitStructure.ADC_GroupChNumber = 1;                    /* Select the number of channels sampled by group */
    Group_InitStructure.ADC_GroupSampleMode = ADC_SAMPLE_MODE_SINGLE; /* Select the sampling mode for group */
    Group_InitStructure.ADC_GroupExtTrigCtrl = ENABLE;        /* group's hardware triggers control */
    Group_InitStructure.ADC_GroupExtTrigSourse = ADC_EXT_TRIG_ATU_TRG0;/* Select the hardware trigger source for group */
    Group_InitStructure.ADC_GroupTrigSkip = ADC_TRIG_SKIP_NONE;  /* Select the number of times the group trigger signal is omitted */
    Group_InitStructure.ADC_GroupDoubleSampleCtrl = ENABLE;   /* Double sampling control */
    ADC_GroupInit(ADC, &Group_InitStructure);
    
    /* Configure groupB */
    Group_InitStructure.ADC_Group  = GROUP_B; /* Select Configure GROUP B */
    Group_InitStructure.ADC_GroupChannel[ASCS_0] = ADC_CHANNEL_PGA1_OUTB; /* Select the input source for the group channel */
    Group_InitStructure.ADC_GroupChannel[ASCS_1] = ADC_CHANNEL_8; /* VBUS */
    Group_InitStructure.ADC_GroupChannel[ASCS_2] = ADC_CHANNEL_18; /* PCB Temp */
    Group_InitStructure.ADC_GroupChannel[ASCS_3] = ADC_CHANNEL_21; /* Motor Temp */     
    Group_InitStructure.ADC_GroupChNumber = 4;                    /* Select the number of channels sampled by group */
    Group_InitStructure.ADC_GroupSampleMode = ADC_SAMPLE_MODE_SINGLE; /* Select the sampling mode for group */
    Group_InitStructure.ADC_GroupExtTrigCtrl = ENABLE;        /* group's hardware triggers control */
    Group_InitStructure.ADC_GroupExtTrigSourse = ADC_EXT_TRIG_ATU_TRG1;/* Select the hardware trigger source for group */
    Group_InitStructure.ADC_GroupTrigSkip = ADC_TRIG_SKIP_NONE;  /* Select the number of times the group trigger signal is omitted */
    Group_InitStructure.ADC_GroupDoubleSampleCtrl = ENABLE;   /* Double sampling control */
    ADC_GroupInit(ADC, &Group_InitStructure);
    
    //Temp
    //ADC_PinRemapCH13(ADC, ADC_CH13_PINREMAP_PA10);
    /* Configure ADC */
    ADC_InitStructure.ADC_ClkDiv = ADC_CLK_DIV_2; /* Configure clock division */
    ADC_InitStructure.ADC_DateAlignment = ADC_DATA_ALIGN_LEFT; /* Data Alignment */
    //用的外部Vref VDD3.3?
    ADC_InitStructure.ADC_VrefSourse = ADC_VREF_EXTERNAL;  /* ADC_Vref Source selection */
    ADC_InitStructure.ADC_InternalVref = ADC_INTERNAL_VREF_2_4;/* Internal ADC Vref voltage range selection */
    ADC_Init(ADC, &ADC_InitStructure);
    
    /* Configure ADC SamlpeCycles */
    ADC_SampleCycleConfig(ADC, GROUP_A, ASCS_0, ADC_SAMPLE_CYCLE_7_5);
    ADC_SampleCycleConfig(ADC, GROUP_B, ASCS_0, ADC_SAMPLE_CYCLE_1_5);
    ADC_SampleCycleConfig(ADC, GROUP_B, ASCS_1, ADC_SAMPLE_CYCLE_13_5);
    ADC_SampleCycleConfig(ADC, GROUP_B, ASCS_2, ADC_SAMPLE_CYCLE_13_5);
    ADC_SampleCycleConfig(ADC, GROUP_B, ASCS_3, ADC_SAMPLE_CYCLE_13_5);  
    ADC_ADCCycleSel(ADC, ADC_CYCLE_SEL_12_5);
    
    /* Configuring ADC interrupts */
//    ADC_ITConfig(ADC, ADC_IT_GROUPB0_FINISH, ENABLE);
    //NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    //NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //NVIC_Init(&NVIC_InitStructure);
    
    /* Enable ADC */
    ADC_Cmd(ADC, ENABLE);
}
/**
  * @brief  ATU configuration
  * @param  f_zv PWM 频率 中心对称PWM实际频率/2
  * @retval None
  */
void ATU_Init_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //NVIC_InitTypeDef NVIC_InitStructure;
    ATU_TimeBaseInitTypeDef ATU_TimeBaseInitStruct;
    ATU_OutputInitTypeDef ATU_OutputInitStruct;
    ATU_ProtectIFInitTypeDef ATU_ProtectIFInitStruct;
    ATU_ProtectOutputInitTypeDef ATU_ProtectOutputInitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_ATU1, DISABLE);
    /* ATU Peripheral clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_ATU1, ENABLE);
    
    /* GPIO Peripheral clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    ATU->CNT = 0;
    
    /* Configure PWM output PIN */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // I/O AF Function
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_4; // I/O output speed
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //Pull-DOWN
    GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable; //Schmit function
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);
    /* ATU Register Protect Unlock */
    ATU_RegisterProtectUnlockCmd(ENABLE);

    /*Configure IOmux*/
    ATU_PWMIORemap(ATU_PWMIO_CHNANEL_0, PWMIO_PB10);
    ATU_PWMIORemap(ATU_PWMIO_CHNANEL_1, PWMIO_PB11);
    ATU_PWMIORemap(ATU_PWMIO_CHNANEL_2, PWMIO_PB12);
    ATU_PWMIORemap(ATU_PWMIO_CHNANEL_3, PWMIO_PB13);
    ATU_PWMIORemap(ATU_PWMIO_CHNANEL_4, PWMIO_PB14);
    ATU_PWMIORemap(ATU_PWMIO_CHNANEL_5, PWMIO_PB15);

    /* Configure ATU */
    ATU_TimeBaseInitStruct.ATU_ClockDivision = ATU_CLK_DIV_1;
    ATU_TimeBaseInitStruct.ATU_CounterMode = ATU_COUNTER_MODE_UPDOWM ;
    ATU_TimeBaseInitStruct.ATU_Period = (PWM_PERIOD_CYCLES/2);// 2666 /12KHZ
    ATU_TimeBaseInitStruct.ATU_PeriodAutoReload = ATU_PERIOD_AUTO_RELOAD;
    ATU_TimeBaseInit(&ATU_TimeBaseInitStruct);
    
    /* Configure PWM duty cycle and dead time */
    ATU_OutputStructInit(&ATU_OutputInitStruct);
    ATU_OutputInitStruct.ATU_OutputMode = ATU_OUTPUT_COMPLEMENTARY_PWM;
    ATU_OutputInitStruct.ATU_CompareValuexA = (ATU->TPR/2);
    ATU_OutputInitStruct.ATU_CompareValuexB = (ATU->TPR/2);
    ATU_OutputInitStruct.ATU_DeadBandTimeA = DEAD_TIME_COUNTS;  //1us,ATUclk=64M
    ATU_OutputInitStruct.ATU_DeadBandTimeB = DEAD_TIME_COUNTS;   //1us,ATUclk=64M
    ATU_OutputInit(ATU_PWMCHANNEL_0, &ATU_OutputInitStruct);
    ATU_OutputInitStruct.ATU_CompareValuexA = (ATU->TPR/2);
    ATU_OutputInitStruct.ATU_CompareValuexB = (ATU->TPR/2);
    ATU_OutputInit(ATU_PWMCHANNEL_1, &ATU_OutputInitStruct);
    ATU_OutputInitStruct.ATU_CompareValuexA = (ATU->TPR/2);
    ATU_OutputInitStruct.ATU_CompareValuexB = (ATU->TPR/2);
    ATU_OutputInit(ATU_PWMCHANNEL_2, &ATU_OutputInitStruct);
    
    /* Configure Auto Reload */
    ATU_TroughReloadTimingPoint(ATU_TROUGH_RELOAD_EVERY_1_EVENT, ENABLE); //Update interval
    ATU_SetReloadModeCompare0(ATU_CRxA_AUTO_RELOAD, ATU_CRxB_AUTO_RELOAD);
    ATU_SetReloadModeCompare1(ATU_CRxA_AUTO_RELOAD, ATU_CRxB_AUTO_RELOAD);
    ATU_SetReloadModeCompare2(ATU_CRxA_AUTO_RELOAD, ATU_CRxB_AUTO_RELOAD);
    
    /* Configure the protected output polarity */
    ATU_ProtectOutputStructInit(&ATU_ProtectOutputInitStruct);
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput0DefaultLevelA = ATU_PROTECT_TIO0A_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput0DefaultLevelB = ATU_PROTECT_TIO0B_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput0SelectA = ATU_PROTECT_TIO0A_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput0SelectB = ATU_PROTECT_TIO0B_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput1DefaultLevelA = ATU_PROTECT_TIO1A_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput1DefaultLevelB = ATU_PROTECT_TIO1B_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput1SelectA = ATU_PROTECT_TIO1A_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput1SelectB = ATU_PROTECT_TIO1B_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput2DefaultLevelA = ATU_PROTECT_TIO2A_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput2DefaultLevelB = ATU_PROTECT_TIO2B_DEFAULT_LEVEL_LOW;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput2SelectA = ATU_PROTECT_TIO2A_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputInitStruct.ATU_ProtectOutput2SelectB = ATU_PROTECT_TIO2B_OUTPUT_SELECT_DEFAULT_LEVLE;
    ATU_ProtectOutputConfig(&ATU_ProtectOutputInitStruct);
    
    /* Configuration trigger point */
    ATU_SetTrigger0(ATU_TRG0_POINT_UP, ATU_TRG0_AUTO_RELOAD, 1); //Trg0
    ATU_SetTrigger1(ATU_TRG1_POINT_UP, ATU_TRG1_AUTO_RELOAD, 1); //Trg1
    ATU_SetTriggerDoubleDataB(ATU_TRGDB_POINT_UP, ATU_TRIGGER_DOUBLE_DATAB_AUTO_RELOAD, (((PWM_PERIOD_CYCLES/2))- 2)); //TrgB
    
    /* Enable PWM */
    ATU_PWMOutputCmd(ENABLE);
    
    /* Enable ATU Counter */
    ATU_CounterCMD(ENABLE);
}
/**
  * @brief  HTU configuration
  * @param  None
  * @retval None
  */
void HTU_Init_Config(void)
{
    HTU_TimeBaseInitTypeDef HTU_TimeBaseInitStruct;
    GPIO_InitTypeDef  GPIO_InitStructure;
    //NVIC_InitTypeDef NVIC_InitStruct;
    
    /* HTU Peripheral clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_HTU1, ENABLE);
    
    /* GPIO Peripheral clock enable */
    //PA0,1,2 hall
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    /* Configure hall PIN */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = M1_HALL_H1_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // I/O AF Function
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_4; // I/O output speed
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    //GPIO_PuPd_DOWN; //Pull-DOWN
    GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable; //Schmit function
    GPIO_Init(M1_HALL_H1_GPIO_Port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  =  M1_HALL_H2_Pin;
    GPIO_Init(M1_HALL_H2_GPIO_Port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  =  M1_HALL_H3_Pin;
    GPIO_Init(M1_HALL_H3_GPIO_Port, &GPIO_InitStructure);     
    
    GPIO_PinAFConfig(M1_HALL_H1_GPIO_Port, M1_HALL_H1_Pin_NO, GPIO_AF_1);
    GPIO_PinAFConfig(M1_HALL_H2_GPIO_Port, M1_HALL_H2_Pin_NO, GPIO_AF_1);
    GPIO_PinAFConfig(M1_HALL_H3_GPIO_Port, M1_HALL_H3_Pin_NO, GPIO_AF_1);

    /* Configure HTU TimeBase */
    HTU_TimeBaseStructInit(&HTU_TimeBaseInitStruct);
    HTU_TimeBaseInitStruct.HTU_NFCR = 0x0F;
    HTU_TimeBaseInitStruct.HTU_Period = 0xFFFFFF;
    HTU_TimeBaseInitStruct.HTU_ClockDivision = HTU_CLK_DIV_1;   //64/1 = 64M?
    HTU_TimeBaseInit(&HTU_TimeBaseInitStruct);
    //打开hall中断
    //HTU_ITConfig(HTU_IT_SW | HTU_IT_ERR | HTU_IT_OVF, ENABLE);

    /* Initializes the NVIC peripheral */
    //NVIC_InitStruct.NVIC_IRQChannel = HTU_IRQn;
    //NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    //NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
    //NVIC_Init(&NVIC_InitStruct);
    //使能中断
    /* Enable PWM output and Start Counter */
    //HTU_CounterCmd(ENABLE);    
					
}
/**
  * @brief  Configure the UART Device
  * @param  None
  * @retval None
  */
void UART2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

    /* 打开时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);


    /* 将UART Tx的GPIO配置为推挽复用模式*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_4;
    GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_3);
    /* 将UART Rx的GPIO配置为浮空输入模式*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_3);
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 配置串口的工作参数 */ 
    /* 配置波特率*/
    UART_InitStructure.UART_BaudRate = 115200;
    /* 配置 针数据字长*/
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    /* 配置停止位*/
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    /* 配置校验位*/
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    /* 配置工作模式，收发一起*/
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    /* 完成串口的初始化配置*/
    UART_Init(UART2, &UART_InitStructure); 
      
    /* 使能RXNE中断*/
    UART_ITConfig(UART2, UART_IT_RXNE, ENABLE);

    /* 使能串口*/
    UART_Cmd(UART2, ENABLE);    
}
/**/
/**
 * 初始化 跳转
*/
#if 0
void initAfterJump(void){
    SCB->VTOR = 0x08001000;
    __enable_irq();
}
#endif
/**
 * 时钟配置
*/
#if 0
void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
    if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2) {
        //  Error_Handler();
    };

    /* HSI configuration and activation */
    LL_RCC_HSI_Enable();
    while (LL_RCC_HSI_IsReady() != 1) {
    };

    /* LSI configuration and activation */
    LL_RCC_LSI_Enable();
    while (LL_RCC_LSI_IsReady() != 1) {
    };

    /* Main PLL configuration and activation */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8,
        LL_RCC_PLLR_DIV_2);
    LL_RCC_PLL_Enable();
    LL_RCC_PLL_EnableDomain_SYS();
    while (LL_RCC_PLL_IsReady() != 1) {
    };

    /* Set AHB prescaler*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

    /* Sysclk activation on the main PLL */
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
    };

    /* Set APB1 prescaler*/
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_Init1msTick(64000000);
    LL_SetSystemCoreClock(64000000);
    /* Update CMSIS variable (which can be updated also through
     * SystemCoreClockUpdate function) */
    LL_SetSystemCoreClock(64000000);
    LL_RCC_SetTIMClockSource(LL_RCC_TIM1_CLKSOURCE_PCLK1);
    LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_SYSCLK);
}
#endif
#if 0
//关闭所有PWM
void PWMC_OFFPWM(void){
    PHASE_A_duty(0);
    PHASE_B_duty(0);
    PHASE_C_duty(0);
    //关闭PWM
    TIM1->BDTR &= ( uint32_t )~TIM_BDTR_MOE;
    //TIM_CtrlPWMOutputs(TIM1, DISABLE);
}
//开启PWM 处于刹车状态
void PWMC_ONPWM(void){
    //TIM_ClearFlag_UPDATE( TIM1 );
    //while ( TIM_IsActiveFlag_UPDATE( TIM1 ) == RESET )
    //{}
    /* Clear Update Flag */
    //TIM_ClearFlag_UPDATE( TIM1 );
    PHASE_A_duty(PWM_PERIOD_CYCLES>>2);
    PHASE_B_duty(PWM_PERIOD_CYCLES>>2);
    PHASE_C_duty(PWM_PERIOD_CYCLES>>2);
    ADC_PHASE_duty((PWM_PERIOD_CYCLES>>1)-5u);
    //while ( TIM_IsActiveFlag_UPDATE( TIM1 ) == RESET )
    //{}
    //开启PWM
    TIM1->BDTR |= TIM_OSSIState_Enable;  //不工作时候输出空闲电平
    TIM1->BDTR |= TIM_BDTR_MOE;
    //TIM_CtrlPWMOutputs(TIM1, ENABLE);
    //ADC_ExtTrigConfig(ADC, QUEUE_0, EXT_TIM1_CC4, EXT_TRIG_RISING);
    //ADC_ITConfig(ADC,ADC_IT_EOC,ENABLE);  
    //ADC_ClearITPendingBit(ADC, ADC_IT_EOC);
    ADC_ITConfig(ADC,ADC_IT_EOSEQ,ENABLE);  
    ADC_ClearITPendingBit(ADC, ADC_IT_EOSEQ);
    ADC_Cmd(ADC, ENABLE);
    Delay_ms(10);
    ADC_ClearITPendingBit(ADC, ADC_IT_EOSEQ);
    ADC_StartOfConversion(ADC);
}
#endif
/**
 * @brief uart initial
 * @param   None
 * @retval  None
 * 
*/
void MX_Uart_Init(void){
    UART_InitTypeDef UART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable UART1 clock */
    RCC_AHBPeriphClockCmd(UartTX_CLK | UartRX_CLK, ENABLE);
    GPIO_PinAFConfig(UartTX_PORT, UartTX_SOURCE, UartTX_GPIO_AF);
    GPIO_PinAFConfig(UartRX_PORT, UartRX_SOURCE, UartRX_GPIO_AF);
    GPIO_InitStructure.GPIO_Pin = UartTX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(UartTX_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UartRX_PIN;
    GPIO_Init(UartRX_PORT, &GPIO_InitStructure);

    UartCLKEn();
    UART_InitStructure.UART_BaudRate = bps_rate;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(UartCH, &UART_InitStructure);
    UART_ITConfig(UartCH, UART_IT_RXNE, ENABLE);
    //UART_ITConfig(UartCH, UART_IT_IDLE, DISABLE);
    //Uart_t.Index = 0;
    //Uart_t.Len = 0;
    //Uart_t.FinishedFlag = RESET;
    //Uart1Rx.Index = 0;
    //Uart1Rx.Len = 0;
    //Uart1Rx.FinishedFlag = RESET;
    /* Enable the UART1 */
    UART_Cmd(UartCH, ENABLE);
}

/**
  * @brief  UART1 Send some data.
  * @param  p - the start address of data to be send
  * @param  len - the len of data to be send
  * @retval None
  */
void UartSendDatas(uint8_t *p, uint8_t len)
{
  while( len -- )
  {
    UART_SendData(UartCH, *p++);

    while( UART_GetFlagStatus(UartCH, UART_FLAG_TC) == RESET )
    {
    }
  }
}
/**
 * 
 * delay function
*/
void Delay_ms(__IO uint32_t Delay)
{
  uint32_t tickstart = msTick;
  uint32_t wait = Delay;
  
  while((msTick - tickstart) < wait)
  {
  }
}
uint32_t Get1msTick(void){
    return msTick;
}
/***
 * @brief 
 * @param us_x10 微秒*10 提高描述精度
*/
void delay_us(uint16_t us_x10)
{
    // 假设主频是 48 MHz
    // 每个循环大概 3 个周期左右（具体看编译器和架构）
    // 48,000,000 / 1,000,000 = 48 cycles per us
    // 假如你希望每个 count 对应 1us，那么 count ≈ 48
    // 但为了更安全，我们使用一个经验公式，并通过调试校准

    //volatile uint32_t i;  // 关键：使用 volatile 防止优化
    //uint32_t count = (uint32_t)us_x10 * (SystemCoreClock / 300000);  // 调整此系数以接近真实 us

    for (uint16_t i = 0; i < us_x10; i++) {
        __NOP();  // 确保插入一条空指令
    }
}
/**
  * @brief  UTU IC Port configuration
  * @param  None
  * @retval None
  */
static void UTU_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock */
    RCC_AHBPeriphClockCmd(AHPSrc, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);

//#if (TEST_UTU_IO == TEST_UTU_IOA)

    /* Config AF for Utu IOA */
    GPIO_PinAFConfig(UTU_IOA_PORT, UTU_IOA_PIN_SRC, UTU_IOA_AF);

    /* Configure IOA & IOB as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = UTU_IOA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(UTU_IOA_PORT, &GPIO_InitStructure);
//#else
//
//    /* Config AF for Utu IOB */
//    GPIO_PinAFConfig(UTU_IOB_PORT, UTU_IOB_PIN_SRC, UTU_IOB_AF);
//
//    /* Configure IOA & IOB as alternate function push-pull */
//    GPIO_InitStructure.GPIO_Pin = UTU_IOB_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_Init(UTU_IOA_PORT, &GPIO_InitStructure);
//#endif
}
/**
  * @brief  UTU configuration
  * @param  None
  * @retval None
  */
void UTU_Config(void)
{
    UTU_TimeBaseInitTypeDef UTU_timeBaseCfg;
    UTU_ICInitTypeDef UTU_ICCfg;

    /* config io-port for ICA & ICB */
    UTU_GPIOInit();

    /* Enable UTU clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UTU1, ENABLE);

    /* config time base */
    UTU_TimeBaseStructInit(&UTU_timeBaseCfg);   //1/64us once
    UTU_timeBaseCfg.UTU_Prescaler = UTU_CKD_DIV64;    // 不分频
    //UTU_timeBaseCfg.UTU_PeriodMode = UTU_CounterMode_Up;
    //最大计时20.5ms 超时溢出
    UTU_timeBaseCfg.UTU_Period = 20500; //0xFFFFFF; // 最大计数1/64
    UTU_TimeBaseInit(UTU1, &UTU_timeBaseCfg);

    /* config IC struct */
    UTU_ICStructInit(&UTU_ICCfg);

//#if (TEST_UTU_IO == TEST_UTU_IOA)
//#else
//    UTU_ICCfg.UTU_ICSelection = UTU_ICSelection_IO_OTHER; /* select IOB as input */
//#endif

    //UTU_ICCfg.UTU_Channel = UTU_CHANNEL_A;
    /* config for ICA capture rising-edge */
   // UTU_ICCfg.UTU_ClrCnt = UTU_IC_CNT_Clr;
   // UTU_ICCfg.UTU_ICPolarity = UTU_ICPolarity_Rising;
   // UTU_ICInit(UTU1, &UTU_ICCfg);

    /* set filter */
    UTU_ICCfg.UTU_ICFilter = 0x04;

    /* config for ICB  capture falling-edge */
    //UTU_ICCfg.UTU_ICSelection = 0;
    UTU_ICCfg.UTU_Channel = UTU_CHANNEL_A;
    UTU_ICCfg.UTU_ICPolarity = UTU_ICPolarity_BothEdge; //UTU_ICPolarity_Falling;

//#if (TEST_UTU_IO == TEST_UTU_IOA)
//    UTU_ICCfg.UTU_ICSelection = UTU_ICSelection_IO_OTHER; /* select IOB as input */
//#else
    UTU_ICCfg.UTU_ICSelection = UTU_ICSelection_IO_SELF; /* select IOA as input */
//#endif

    UTU_ICCfg.UTU_ClrCnt = UTU_IC_CNT_Clr; /* 0 */
    UTU_ICInit(UTU1, &UTU_ICCfg);

    /* UTU NVIC configuration*/
    //NVIC_Config();


    /* config UTU interrupt */
    //UTU_ITConfig(TARGET_UTU, (UTU_IT_UCRA | UTU_IT_UCFB), ENABLE);
    UTU_ITConfig(UTU1, (UTU_IT_UCRA), ENABLE);

    /* Enable Timer */
    //UTU_Cmd(UTU2, ENABLE);

}
/**
 * @brief GPIO config
 * 
 * 
 */
void MX_GPIO_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable GPIO clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = HW_DIR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    //开启上拉
    GPIO_Init(HW_En_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = HW_En_PIN;
    GPIO_Init(HW_DIR_PORT, &GPIO_InitStructure);
}
/**
  * @brief NVIC Configuration.
  * @retval None
  */
void MX_NVIC_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = HTU_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStructure.NVIC_IRQChannel = UTU1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_Init(&NVIC_InitStruct);
    /* TIM1_BRK_UP_TRG_COM_IRQn interrupt configuration */
    //NVIC_InitStruct.NVIC_IRQChannel=TIM1_IRQn;
    //NVIC_InitStruct.NVIC_IRQChannelPriority = 1;  //0;
    //NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
    //NVIC_Init(&NVIC_InitStruct);
    /* TIM2_IRQn interrupt configuration */
    //NVIC_InitStruct.NVIC_IRQChannel=TIM6_IRQn;
    //NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
    //NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
    //NVIC_Init(&NVIC_InitStruct);	
    /* ADC interrupt configuration */
    //NVIC_InitStruct.NVIC_IRQChannel=ADC1_IRQn;
    //NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    //NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;		
    //NVIC_Init(&NVIC_InitStruct);
    //NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
    //NVIC_InitStruct.NVIC_IRQChannelPriority = 1;  //0;
    //NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
    //NVIC_Init(&NVIC_InitStruct);
    /* UART1 IRQ Channel configuration */
    //#ifdef cUartDebugEn
    NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    NVIC_Init(&NVIC_InitStruct);
    //#endif

    /* UART1_IRQn interrupt configuration */
    //NVIC_InitStruct.NVIC_IRQChannel=UART1_IRQn;
    //NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    //NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
    //NVIC_Init(&NVIC_InitStruct);

    /* EXTI2_3_IRQn interrupt configuration */
    //NVIC_InitStruct.NVIC_IRQChannel=PIN_plus_EXTI_IRQn;  //EXTI4_15_IRQn;
    //NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    //NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    //NVIC_Init(&NVIC_InitStruct);
	
}
