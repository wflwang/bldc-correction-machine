/****
 * @file peripherals.c
 * @date Created on: July. 28, 2025
 * @author MaxwellWang
 */

#include "peripherals.h"


static __IO uint32_t msTick=0;
#ifdef HallfilterFirstEn
static uint8_t filterInit=0;   //一阶滤波是否初始化完成
#endif
UartRxBufDef Uart_t;
filter_t hallXft={0};
filter_t hallYft={0};
Avfilter_t avfilterHallX={0};
Avfilter_t avfilterHallY={0};
//static uint8_t instNum = 0;
//static uint8_t debugOutCount=0; //输出调试计次
//static uint8_t debugOutInterval=0;  //调试输出间隔时间   
//霍尔滤波主要是过滤掉低频杂波和瞬间脉冲
//hall 滤波表格
const int16_t hallFilter[] = {
    40,80,120,200,400,900
};
const int16_t hallFilterV[] = {
    40,60,90,150,300,500,700
};
#define hallX_alp_raw   1000    //当前滤波系数
#define hallX_alp_min   100    //当前滤波系数
#define hallX_alp_max   65535    //当前滤波系数
#define hallY_alp_raw   1000    //当前滤波系数
#define hallY_alp_min   100    //当前滤波系数
#define hallY_alp_max   65535    //当前滤波系数
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
    //设置tick time
    SysTick_Config(SystemCoreClock /SYS_TICK_FREQUENCY);
    //EE_Read();
    MX_GPIO_Init();
	EE_ReadFOC(FOC_Component_M1.lc);   //读取存储的FOC 参数
    //qmi8658x_init(GYPO_SDA_GPIO_PORT,GYPO_SDA_GPIO_PIN,GYPO_SCL_GPIO_PORT,GYPO_SCL_GPIO_PIN);

    while(GetONOFF()==0){
        //LEDG_Xor();
        Delay_ms(1);
		//readQmi8658b();	//读出参数	
        fScanButton();   //扫描按键功能
    }
    //PowerEn_Write(0);
		//EE_WriteFOC(&FOC_Component_M1.lc);
    #ifdef cUartDebugEn
    SWD_Pin_To_PB5_PD5_Configuration();
    MX_Uart_Init();
    #endif
    MX_ADC_Init();
    MX_TIM_Init();
    MX_NVIC_init();
    //qmi8658x_init(GYPO_SDA_GPIO_PORT,GYPO_SDA_GPIO_PIN,GYPO_SCL_GPIO_PORT,GYPO_SCL_GPIO_PIN);
    //SysTick_Config(SystemCoreClock /SYS_TICK_FREQUENCY);
}
/**
  * @brief  SYSCFG configuration
  * @param  None
  * @retval None
  */
static void SYSCFG_Config(void)
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
static void MY_PGA_Init(void)
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
    PGA_SW_Calibration(PGA1, PGA1_PGA_GAIN_12);
    //PGA_SW_Calibration(PGA2, PGA1_PGA_GAIN_12);
    
    /* PGAx configured as follow*/
    PGA1_InitStruct.PGA1_NPLockMode = PGA1_NPLock_N0; //PGA1_N NOLock
    PGA1_InitStruct.PGA1_NInputSelect = PGA1_NInputSelectIO; //PGA1_N is selected from an external pin input
    PGA1_InitStruct.PGA1_PAInputSelect = PGA1_PA1; //PGA1_PA Select input from external pin PA1
    PGA1_InitStruct.PGA1_PBInputSelect = PGA1_PB1; //PGA1_PA Select input from external pin PB1
    PGA1_InitStruct.PGA1_CHInputSelect = PGA1_InputSelectPANA; //PGA1 Input Select Group A(Handmode takes effect)
    PGA1_InitStruct.PSAMSelect = PGA1_PSAMSelectAuto; //Select the channel by ADC(Auto)
    PGA1_InitStruct.PGA1_GAIN = PGA1_PGA_GAIN_12; //PGA_GAIN = 4
    PGA2_InitStruct.PGA2_NInputSelect = PGA2_NInputSelectAVSS;  //PGA2_NInputSelectN1; //PGA_N Select N1
    PGA2_InitStruct.PGA2_PInputSelect = PGA2_PInputSelect_PGA1OUT;  //PGA2_PInputSelect_P1; //PGA_P Select P1
    PGA2_InitStruct.PGA2_GAIN = PGA2_PGA_GAIN_4; //PGA_GAIN = 4
    PGA_Init(&PGA1_InitStruct, &PGA2_InitStruct);
    
    /* PGA user mode configured*/
    PGA_VIM_USER(PGA1);
    //PGA_VIM_USER(PGA2);
    
    /* Enable PGA*/
    PGA_Enable(PGA1);
    //PGA_Enable(PGA2);
    
    /* Configure the bias voltage*/
    PGA_SetBias( ENABLE, DISABLE, VrefDiv1); //1/2Vref
    PGA_Lock();
}
/**
  * @brief  ADC channel configuration
  * @param  None
  * @retval None
  */
static void ADC_Init_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    Group_InitTypeDef Group_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
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
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_9;
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
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable ADC */
    ADC_Cmd(ADC, ENABLE);
}
/**
  * @brief  ATU configuration
  * @param  None
  * @retval None
  */
static void ATU_Init_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    ATU_TimeBaseInitTypeDef ATU_TimeBaseInitStruct;
    ATU_OutputInitTypeDef ATU_OutputInitStruct;
    ATU_ProtectIFInitTypeDef ATU_ProtectIFInitStruct;
    ATU_ProtectOutputInitTypeDef ATU_ProtectOutputInitStruct;
    
    /* ATU Peripheral clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_ATU1, ENABLE);
    
    /* GPIO Peripheral clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
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
    ATU_TimeBaseInitStruct.ATU_Period = 2666;//12KHZ
    ATU_TimeBaseInitStruct.ATU_PeriodAutoReload = ATU_PERIOD_AUTO_RELOAD;
    ATU_TimeBaseInit(&ATU_TimeBaseInitStruct);
    
    /* Configure PWM duty cycle and dead time */
    ATU_OutputStructInit(&ATU_OutputInitStruct);
    ATU_OutputInitStruct.ATU_OutputMode = ATU_OUTPUT_COMPLEMENTARY_PWM;
    ATU_OutputInitStruct.ATU_CompareValuexA = 0;
    ATU_OutputInitStruct.ATU_CompareValuexB = 0;
    ATU_OutputInitStruct.ATU_DeadBandTimeA = 0x40;//1us,ATUclk=64M
    ATU_OutputInitStruct.ATU_DeadBandTimeB = 0x40;//1us,ATUclk=64M
    ATU_OutputInit(ATU_PWMCHANNEL_0, &ATU_OutputInitStruct);
    ATU_OutputInitStruct.ATU_CompareValuexA = 0;
    ATU_OutputInitStruct.ATU_CompareValuexB = 0;
    ATU_OutputInit(ATU_PWMCHANNEL_1, &ATU_OutputInitStruct);
    ATU_OutputInitStruct.ATU_CompareValuexA = 0;
    ATU_OutputInitStruct.ATU_CompareValuexB = 0;
    ATU_OutputInit(ATU_PWMCHANNEL_2, &ATU_OutputInitStruct);
    
    /* Configure Auto Reload */
    ATU_TroughReloadTimingPoint(ATU_TROUGH_RELOAD_EVERY_1_EVENT, ENABLE); //Update interval
    ATU_SetReloadModeCompare0(ATU_CRxA_AUTO_RELOAD, ATU_CRxB_AUTO_RELOAD);
    ATU_SetReloadModeCompare1(ATU_CRxA_AUTO_RELOAD, ATU_CRxB_AUTO_RELOAD);
    ATU_SetReloadModeCompare2(ATU_CRxA_AUTO_RELOAD, ATU_CRxB_AUTO_RELOAD);
    
    /* Configure the protected output polarity */
    //ATU_ProtectOutputStructInit(&ATU_ProtectOutputInitStruct);
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput0DefaultLevelA = ATU_PROTECT_TIO0A_DEFAULT_LEVEL_LOW;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput0DefaultLevelB = ATU_PROTECT_TIO0B_DEFAULT_LEVEL_LOW;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput0SelectA = ATU_PROTECT_TIO0A_OUTPUT_SELECT_DEFAULT_LEVLE;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput0SelectB = ATU_PROTECT_TIO0B_OUTPUT_SELECT_DEFAULT_LEVLE;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput1DefaultLevelA = ATU_PROTECT_TIO1A_DEFAULT_LEVEL_LOW;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput1DefaultLevelB = ATU_PROTECT_TIO1B_DEFAULT_LEVEL_LOW;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput1SelectA = ATU_PROTECT_TIO1A_OUTPUT_SELECT_DEFAULT_LEVLE;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput1SelectB = ATU_PROTECT_TIO1B_OUTPUT_SELECT_DEFAULT_LEVLE;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput2DefaultLevelA = ATU_PROTECT_TIO2A_DEFAULT_LEVEL_LOW;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput2DefaultLevelB = ATU_PROTECT_TIO2B_DEFAULT_LEVEL_LOW;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput2SelectA = ATU_PROTECT_TIO2A_OUTPUT_SELECT_DEFAULT_LEVLE;
    //ATU_ProtectOutputInitStruct.ATU_ProtectOutput2SelectB = ATU_PROTECT_TIO2B_OUTPUT_SELECT_DEFAULT_LEVLE;
    //ATU_ProtectOutputConfig(&ATU_ProtectOutputInitStruct);
    
    /* Configuration trigger point */
    ATU_SetTrigger0(ATU_TRG0_POINT_UP, ATU_TRG0_AUTO_RELOAD, 1); //Trg0
    ATU_SetTrigger1(ATU_TRG1_POINT_UP, ATU_TRG1_AUTO_RELOAD, 1); //Trg1
    ATU_SetTriggerDoubleDataB(ATU_TRGDB_POINT_UP, ATU_TRIGGER_DOUBLE_DATAB_AUTO_RELOAD, 2666 - 2); //TrgB
    
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
static void HTU_Init_Config(void)
{
    HTU_TimeBaseInitTypeDef HTU_TimeBaseInitStruct;
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;
    
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

    //HTU_ITConfig(HTU_IT_SW | HTU_IT_ERR | HTU_IT_OVF, ENABLE);

    /* Initializes the NVIC peripheral */
    NVIC_InitStruct.NVIC_IRQChannel = HTU_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
    NVIC_Init(&NVIC_InitStruct);

    /* Enable PWM output and Start Counter */
    //HTU_CounterCmd(ENABLE);    
					
}
/**
  * @brief  Configure the UART Device
  * @param  None
  * @retval None
  */
static void UART2_Config(void)
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
/**
 * GPIO inital
 * IO口配置 ->power set..
*/
void MX_GPIO_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(Button_PWR_GPIO_CLK|Button_LR_GPIO_CLK|Button_RR_GPIO_CLK|PowerEn_GPIO_CLK|LEDG_GPIO_CLK|LEDR_GPIO_CLK, ENABLE);
    /* Configure Button pin as input */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = PowerEn_GPIO_PIN;
	GPIO_Init(PowerEn_GPIO_PORT, &GPIO_InitStructure);
    PowerEn_Write(0);
    GPIO_InitStructure.GPIO_Pin = LEDR_GPIO_PIN;
	GPIO_Init(LEDR_GPIO_PORT, &GPIO_InitStructure);
    LEDR_Write(0);
    GPIO_InitStructure.GPIO_Pin = LEDG_GPIO_PIN;
	GPIO_Init(LEDG_GPIO_PORT, &GPIO_InitStructure);
    LEDR_Write(0);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = Button_PWR_GPIO_PIN;
    GPIO_Init(Button_PWR_GPIO_PORT, &GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = Button_LR_GPIO_PIN;
    GPIO_Init(Button_LR_GPIO_PORT, &GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = Button_RR_GPIO_PIN;
    GPIO_Init(Button_RR_GPIO_PORT, &GPIO_InitStructure); 
}

/**
 * @brief TIM Initialization Function
 * @param None
 * @retval None
 */
void MX_TIM_Init(void)
{
    /* USER CODE BEGIN TIM1_Init 0 */

    /* USER CODE END TIM1_Init 0 */

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef  TIM_OCInitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};
    /* Peripheral clock enable */
    //LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	RCC_AHBPeriphClockCmd(PHASE_A_GPIO_CLK|PHASE_B_GPIO_CLK|PHASE_C_GPIO_CLK, ENABLE);	
    /* USER CODE BEGIN TIM1_Init 1 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_Pin = PHASE_A_GPIO_PIN;
	GPIO_Init(PHASE_A_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PHASE_B_GPIO_PIN;
	GPIO_Init(PHASE_B_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PHASE_C_GPIO_PIN;
	GPIO_Init(PHASE_C_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(PHASE_A_GPIO_PORT, PHASE_A_GPIO_SOURCE, PHASE_A_GPIO_AF);
	GPIO_PinAFConfig(PHASE_B_GPIO_PORT, PHASE_B_GPIO_SOURCE, PHASE_B_GPIO_AF);
	GPIO_PinAFConfig(PHASE_C_GPIO_PORT, PHASE_C_GPIO_SOURCE, PHASE_C_GPIO_AF);
    /* USER CODE END TIM1_Init 1 */
    /* TIM1 clock enable */
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_TIM1, ENABLE);
    //TIM_DeInit(TIM1);
    //TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    TIM_TimeBaseStructure.TIM_Period = ((PWM_PERIOD_CYCLES) >> 1);
    //TIM_TimeBaseStructure.Autoreload = 3000;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV2;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = (REP_COUNTER);
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_OCStructInit(&TIM_OCInitStructure);
	/* Channel 1, 2,3 in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;                  
	TIM_OCInitStructure.TIM_Pulse = 0; //dummy value
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;         
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState =TIM_OCNIdleState_Reset;  // LOW_SIDE_POLARITY;          

	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	//TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC4Ref);
	TIM_OCStructInit(&TIM_OCInitStructure);
	/* Channel 4 Configuration in OC */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;  //TIM_OutputState_Disable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;                  
	TIM_OCInitStructure.TIM_Pulse = (((PWM_PERIOD_CYCLES) >> 1) - (HTMIN));

	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    /* CC4 ENABLE */

    TIM_CCxCmd(TIM1,ADC_TrigTIMCH,TIM_CCx_Enable);
	/* CC4_TO_ADC_SELConfig  */
    TIM_CC_TRIGADC(TIM1,ADC_TrigTIMCH, CC_TRIGADC_OCREF);
    //TIM_BrakInputRemap(TIM1, TIM_Break_Remap_COMP1OUT);
    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

    RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Prescaler = 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = (SystemCoreClock/2/500)-1;   //500Hz freq 2ms once
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* TIM Interrupts enable */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}
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

/**
 * @brief ADC initial
 * @param   None
 * @retval  None
 * 采样电压 电流 两路霍尔的AD
*/
void MX_ADC_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    ADC_InitTypeDef ADC_InitStructure;
	RCC_AHBPeriphClockCmd(Voltage_ADC_CLK|Hall_x_GPIO_CLK|Hall_y_GPIO_CLK, ENABLE);	
      /* ADC Periph clock enable */
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_ADC, ENABLE);
    /* USER CODE BEGIN TIM1_Init 1 */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = Voltage_ADC_PIN;
	GPIO_Init(Voltage_ADC_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = Hall_x_GPIO_PIN;
	GPIO_Init(Hall_x_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = Hall_y_GPIO_PIN;
	GPIO_Init(Hall_y_GPIO_PORT, &GPIO_InitStruct);
    //ADC_DeInit(ADC);
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExtTrig;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_Init(ADC, &ADC_InitStructure);
    /* Convert the ADC1 Channel4 with 239.5 Cycles as sampling time */
    // (sampletime+12.5)*ADC CLK
    //ADC_ChannelConfig(ADC, Voltage_ADC_CH, ADC_SampleTime_1_5Cycles);
    //ADC_ChannelConfig(ADC, Hall_x_CH, ADC_SampleTime_1_5Cycles);
    ADC_ChannelConfig(ADC, Hall_y_CH|Hall_x_CH|Voltage_ADC_CH, ADC_SampleTime_1_5Cycles);   //ADC_SampleTime_1_5Cycles
    /* Enable the ADC peripheral */
    //ADC_ClearITPendingBit(ADC, ADC_IT_EOC);
    //ADC_Cmd(ADC, ENABLE);
}
/**
 * @brief Hall initial
 * @param   None
 * @retval  None
 * 初始化霍尔? 初始化hall变量 XY 霍尔一阶值
*/
#ifdef HallfilterFirstEn
void MX_Hall_init(int16_t xRaw,int16_t yRaw){
    hallXft.alpha_diff = hallFilter;
    hallXft.alpha_diff_addV = hallFilterV;
    hallXft.alpha_diff_len = sizeof(hallFilter)/sizeof(int16_t);
    hallXft.alpha_raw = hallX_alp_raw;
    hallXft.alpha_min = hallX_alp_min;
    hallXft.alpha_max = hallX_alp_max;
    hallXft.filter = xRaw;
    hallYft.alpha_diff = hallFilter;
    hallYft.alpha_diff_addV = hallFilterV;
    hallYft.alpha_diff_len = sizeof(hallFilter)/sizeof(int16_t);
    hallYft.alpha_raw = hallY_alp_raw;
    hallYft.alpha_min = hallY_alp_min;
    hallYft.alpha_max = hallY_alp_max;
    hallYft.filter = yRaw;
    filterInit = 1;
}
#endif
/**
 * @brief hall sample  Out filter value
 * @param xRaw 本次采样的Xhall
 * @param yRaw 本次采样的Yhall
 * 改为采样时候做滑动平均滤波
 * 
*/
HallXYs MX_Hall_Sample(uint16_t xRaw,uint16_t yRaw){
    HallXYs hxy_t;
    #ifdef HallfilterFirstEn
    //#ifdef filterFirstOrder
    //if(filterInit==0){
    //    MX_Hall_init(xRaw,yRaw);
    //    hxy_t.Hallx = hallXft.filter;
    //    hxy_t.Hally = hallYft.filter;
    //}else{
    //    hxy_t.Hallx = firstOrderFilter(&hallXft,xRaw);
    //    hxy_t.Hally = firstOrderFilter(&hallYft,yRaw);
    //}
    //#else
    //hxy_t.Hallx = xRaw;
    //hxy_t.Hally = yRaw;
    //#endif
    #else   //滑动平均滤波
    hxy_t.Hallx = AvFilter(&avfilterHallX,xRaw);
    hxy_t.Hally = AvFilter(&avfilterHallY,yRaw);
    #endif
	return hxy_t;
}
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

    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1, ENABLE);
    UART_InitStructure.UART_BaudRate = bps_rate;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(UART1, &UART_InitStructure);
    UART_ITConfig(UART1, UART_IT_RXNE, ENABLE);
    UART_ITConfig(UART1, UART_IT_IDLE, DISABLE);
    Uart_t.Index = 0;
    Uart_t.Len = 0;
    Uart_t.FinishedFlag = RESET;
    //Uart1Rx.Index = 0;
    //Uart1Rx.Len = 0;
    //Uart1Rx.FinishedFlag = RESET;
    /* Enable the UART1 */
    UART_Cmd(UART1, ENABLE);
}
/**
 * @brief tim2 int
 * 
 * 
*/
void TIM2_IRQHandler(void)
{
    //if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    //{
    //    // 处理比较中断
    //    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    //}  
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // 处理更新中断
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        getOrientation_1ms();   //读取一次陀螺仪值
    }
}
/**
  * @brief  This function handles UART1 global interrupt request.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
	if( UART_GetITStatus(UART1, UART_IT_RXNE) != RESET )
	{
		UART_ClearITPendingBit(UART1, UART_IT_RXNE);
        //if(Uart_t.FinishedFlag != SET){
            /* receive data */
            Uart_t.Data[Uart_t.Index] = UART_ReceiveData(UART1);
            Uart_t.Index = (Uart_t.Index+1)&(RX_BUFFER_SIZE-1);
            //LEDG_Xor();
    
		    if(Uart_t.Index == 1)
		    {
		    	UART_ITConfig(UART1, UART_IT_IDLE, ENABLE);
		    }
        //}
	}
	
	if(UART_GetITStatus(UART1, UART_IT_IDLE) != RESET)
	{
		UART_ClearITPendingBit(UART1, UART_IT_IDLE);
		UART_ITConfig(UART1, UART_IT_IDLE, DISABLE);
		Uart_t.Len = Uart_t.Index;
		Uart_t.Index = 0;
		Uart_t.FinishedFlag = SET;
	}
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
    UART_SendData(UART1, *p++);

    while( UART_GetFlagStatus(UART1, UART_FLAG_TC) == RESET )
    {
    }
  }
}
/**
 * @brief debug output uartdata
 * 
 * 
*/
//void SendUartDebug(void){
    //static uint8_t sendCountTime=0;
    //uint8_t data[32];
    //sendCountTime++;
    //if(debugOutCount!=0){
    //     if(debugOutCount<sendCountTime){
    //        sendCountTime = 0;
            //间隔时间到了可以正常发码
    //UartSendDatas(data,instNum);    //发送数据

    //    }
    //    if(debugOutCount!=0xff)
    //    debugOutCount--;
    //}
//}
/***
 * @brief 获取串口调试信息 P/I/D
 * 
 * 
*/
void GetUartDebug(void){
    uint8_t len = 0;
    uint8_t data[32];
    //if(Uart_t.FinishedFlag == SET){
    //    Uart_t.FinishedFlag = RESET;
    //    UartSendDatas(Uart_t.Data, Uart_t.Len);
    //}
    if((Uart_t.FinishedFlag == SET)&&(Uart_t.Data[0]==0xaa)&&(Uart_t.Data[Uart_t.Len-1]==0x55)){
        //表示收到一帧完整信号解码调试内容
            //
            len = Uart_t.Data[1];   //指令数量  获取/设置 获取可以多个一次输出 设置设置一个?
            //data[0] = 0xbb;
            //data[1] = len;
            uint8_t index = 0;
            int16_t param = 0;
            for(uint8_t i=0;i<len;i++){
                switch(Uart_t.Data[i+2]){
                    case GetGyroAngle:  //获取陀螺仪物理角度
                        data[index++] = 0xbb;
                        param = GetOriGyroA();
                    break;
                    case GetMecAngle:
                        data[index++] = 0xaa;
                        param = GetMecA();
                    break;
                    case GetElAngle:
                        data[index++] = 0xcc;
                        param = GetElA();
                    break;
                    case GetMecAngleAcc:
                        data[index++] = 0xa1;
                        param = GetAccA();
                    break;
                    case GetMecAngleGyro:
                        data[index++] = 0xa2;
                        param = GetGyroA();
                    break;
                    case GetMotorSpeed:
                        data[index++] = 0xa0;
                        param = GetSpeedRun();
                    break;
                    case GetTorq:
                        data[index++] = 0xa3;
                        param = GetTorque();
                    break;
                    case GetMHdir:  //获取马达和霍尔方向关系=1 同向 =0 反向
                        data[index++] = 0xa4;
                        param = fGetMHdir();
                    break;
                    case GetGyroMid:  //获取陀螺仪中点
                        data[index++] = 0xa5;
                        param = GetGyroZero();
                    break;
                    case SetPosPID_P:
                        param = ((int16_t)Uart_t.Data[i+3]<<8)|((int16_t)Uart_t.Data[i+4]);
                        SetPosPIDKp(param);
                        i += 2;
                    break;
                    case SetPosPID_I:
                        param = ((int16_t)Uart_t.Data[i+3]<<8)|((int16_t)Uart_t.Data[i+4]);
                        SetPosPIDKi(param);
                        i += 2;
                    break;
                    case SetPosPID_D:
                        param = ((int16_t)Uart_t.Data[i+3]<<8)|((int16_t)Uart_t.Data[i+4]);
                        SetPosPIDKd(param);
                        i += 2;
                    break;
                    case SetSpeedPID_P:
                        param = ((int16_t)Uart_t.Data[i+3]<<8)|((int16_t)Uart_t.Data[i+4]);
                        SetSpeedPIDKp(param);
                        i += 2;
                    break;
                    case SetSpeedPID_I:
                        param = ((int16_t)Uart_t.Data[i+3]<<8)|((int16_t)Uart_t.Data[i+4]);
                        SetSpeedPIDKi(param);
                        i += 2;
                    break;
                    case SetSpeedPID_D:
                        param = ((int16_t)Uart_t.Data[i+3]<<8)|((int16_t)Uart_t.Data[i+4]);
                        SetSpeedPIDKd(param);
                        i += 2;
                    break;
                    case SetPIDInt: //设置PID调整间隔
                        param = (int8_t)Uart_t.Data[i+3];
                        SetSPIDInterval(param);
                        i += 1;
                    break;
                    case SetDead_Err:
                        param = ((int16_t)Uart_t.Data[i+3]<<8)|((int16_t)Uart_t.Data[i+4]);
                        SetDeadErr(param);
                        i += 2;
                    break;
                    case SetPosLoopCount:
                        param = ((int16_t)Uart_t.Data[i+3]<<8)|((int16_t)Uart_t.Data[i+4]);
                        SetPosLoopInv(param);
                        i += 2;
                    break;
                    case SetGyroMid:    //设置陀螺仪中点
                        param = ((int16_t)Uart_t.Data[i+3]<<8)|((int16_t)Uart_t.Data[i+4]);
                        fSetGyroInitMid(param);
                        i += 2;
                    break;
                }
                data[index++] = (uint8_t)((param>>8)&0xff);
                if(index>31)
                    break;
                data[index++] = (uint8_t)((param)&0xff);
                if(index>31)
                    break;
            }
            //index=0;
            //data[index++] = 0xaa;
            //param = GetSpeedRun();
            //data[index++] = (uint8_t)((param>>8)&0xff);
            //data[index++] = (uint8_t)((param)&0xff);
            //param = GetTorque();
            //data[index++] = (uint8_t)((param>>8)&0xff);
            //data[index++] = (uint8_t)((param)&0xff);
            UartSendDatas(data,index);
        Uart_t.FinishedFlag = RESET;
    }
}
/**
 * 发送串口启动
 * 
*/
void sendstart(void){
    uint8_t data[3];
    uint8_t index = 3;
    data[0] = 0xaa;
    data[1] = 0x0;
    data[2] = 0x55;
    UartSendDatas(data,index);
}
/**
  * @brief  Configure SWDIO pin to GPIO function
  * @param  None
  * @retval None
  */
void SWD_Pin_To_PB5_PD5_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable IOMUX clock */
  RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_IOMUX, ENABLE);

  /* Enable GPIOB adn GPIOD clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

  /* Configure SWDIO to PD5 */
#if defined (HK32F0301MJ4M7C) /* for SOP8 - PIN8*/
  GPIO_IOMUX_ChangePin(IOMUX_PIN8, IOMUX_PD5_SEL_PD5);
#elif defined (HK32F0301MD4P7C) /*for TSSOP16 - PIN 15*/
  GPIO_IOMUX_ChangePin(IOMUX_PIN15, IOMUX_PD5_SEL_PD5);
#elif defined (HK32G003F4P7) /* for TSSOP20 - PIN 2 */
  GPIO_IOMUX_ChangePin(IOMUX_PIN2, IOMUX_PD5_SEL_PD5);
#elif defined (HK32F0301MF4N7C)  /* for QFN20 - PIN19*/
  GPIO_IOMUX_ChangePin(IOMUX_PIN19, IOMUX_PD5_SEL_PD5);
#endif

  /* Configure SWCLK to PB5 */
#if defined (HK32F0301MJ4M7C) /* for SOP8 - PIN5*/
  GPIO_IOMUX_ChangePin(IOMUX_PIN5, IOMUX_PB5_SEL_PB5);
#elif defined (HK32F0301MD4P7C) /*for TSSOP16 - PIN 9*/
  GPIO_IOMUX_ChangePin(IOMUX_PIN9, IOMUX_PB5_SEL_PB5);
#elif defined (HK32G003F4P7) /* for TSSOP20 - PIN 11 */
  GPIO_IOMUX_ChangePin(IOMUX_PIN11, IOMUX_PB5_SEL_PB5);
#elif defined (HK32F0301MF4N7C)  /* for QFN20 - PIN8*/
  GPIO_IOMUX_ChangePin(IOMUX_PIN8, IOMUX_PB5_SEL_PB5);
#endif

  GPIO_SetBits(UartTX_PORT,UartTX_PIN);
  GPIO_InitStructure.GPIO_Pin = UartTX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(UartTX_PORT, &GPIO_InitStructure);
  GPIO_SetBits(UartTX_PORT,UartTX_PIN);

  GPIO_InitStructure.GPIO_Pin = UartRX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(UartRX_PORT, &GPIO_InitStructure);
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
  * @brief NVIC Configuration.
  * @retval None
  */
void MX_NVIC_init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
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
    NVIC_InitStruct.NVIC_IRQChannel=ADC1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;		
    NVIC_Init(&NVIC_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 1;  //0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
    NVIC_Init(&NVIC_InitStruct);
    /* UART1 IRQ Channel configuration */
    #ifdef cUartDebugEn
    NVIC_InitStruct.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;		
    NVIC_Init(&NVIC_InitStruct);
    #endif

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
