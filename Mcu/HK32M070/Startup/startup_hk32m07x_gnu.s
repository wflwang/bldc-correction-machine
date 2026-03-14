/*
 * startup_hk32m07x_gnu.s
 * GNU assembler startup file for HK32M070
 */

    .syntax unified
    .cpu cortex-m0
    .fpu softvfp
    .thumb

/* Stack Configuration */
    .equ    Stack_Size, 0x400
    .section .stack
    .align 3
stack_bottom:
    .space  Stack_Size
stack_top:

/* Heap Configuration */
    .equ    Heap_Size, 0x200
    .section .heap
    .align 3
heap_bottom:
    .space  Heap_Size
heap_top:

/* Vector Table */
    .section .isr_vector, "a", %progbits
    .align 2
    .globl  __Vectors
__Vectors:
    .long   stack_top             /* Top of Stack */
    .long   Reset_Handler         /* Reset Handler */
    .long   NMI_Handler           /* NMI Handler */
    .long   HardFault_Handler     /* Hard Fault Handler */
    .long   0                     /* Reserved */
    .long   0                     /* Reserved */
    .long   0                     /* Reserved */
    .long   0                     /* Reserved */
    .long   0                     /* Reserved */
    .long   0                     /* Reserved */
    .long   0                     /* Reserved */
    .long   SVC_Handler           /* SVCall Handler */
    .long   0                     /* Reserved */
    .long   0                     /* Reserved */
    .long   PendSV_Handler        /* PendSV Handler */
    .long   SysTick_Handler       /* SysTick Handler */

    /* External Interrupts */
    .long   WWDG_IWDG_IRQHandler  /* Window Watchdog */
    .long   PVD_IRQHandler        /* PVD */
    .long   0                     /* Reserved */
    .long   FLASH_IRQHandler      /* FLASH */
    .long   RCC_IRQHandler        /* RCC */
    .long   EXTI0_1_IRQHandler    /* EXTI Line 0 and 1 */
    .long   EXTI2_3_IRQHandler    /* EXTI Line 2 and 3 */
    .long   EXTI4_15_IRQHandler   /* EXTI Line 4 to 15 */
    .long   0                     /* Reserved */
    .long   0                     /* Reserved */
    .long   0                     /* Reserved */
    .long   ADC_IRQHandler        /* ADC */
    .long   0                     /* Reserved */
    .long   ATU_BKP_IRQHandler    /* ATU_BKP */
    .long   0                     /* Reserved */
    .long   ATU_IRQHandler        /* ATU */
    .long   0                     /* Reserved */
    .long   UTU1_IRQHandler       /* UTU1 */
    .long   UTU2_IRQHandler       /* UTU2 */
    .long   CMP1_IRQHandler       /* CMP1 */
    .long   CMP2_IRQHandler       /* CMP2 */
    .long   HTU_IRQHandler        /* HTU */
    .long   0                     /* Reserved */
    .long   DVSQ_IRQHandler       /* DVSQ */
    .long   I2C_IRQHandler        /* I2C */
    .long   SPI_IRQHandler        /* SPI */
    .long   UART1_IRQHandler      /* UART1 */
    .long   UART2_IRQHandler      /* UART2 */

    .size   __Vectors, . - __Vectors

/* Reset Handler */
    .section .text
    .align 2
    .thumb_func
    .globl  Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
    /* Initialize stack pointer */
    ldr     r0, =stack_top
    mov     sp, r0

    /* Call SystemInit */
    bl      SystemInit

    /* Call main */
    bl      main

    /* Infinite loop */
    b       .
    .size   Reset_Handler, . - Reset_Handler

/* Dummy Exception Handlers */
    .section .text
    .align 2
    .thumb_func
    .globl  NMI_Handler
    .type   NMI_Handler, %function
NMI_Handler:
    b       .
    .size   NMI_Handler, . - NMI_Handler

    .section .text
    .align 2
    .thumb_func
    .globl  HardFault_Handler
    .type   HardFault_Handler, %function
HardFault_Handler:
    b       .
    .size   HardFault_Handler, . - HardFault_Handler

    .section .text
    .align 2
    .thumb_func
    .globl  SVC_Handler
    .type   SVC_Handler, %function
SVC_Handler:
    b       .
    .size   SVC_Handler, . - SVC_Handler

    .section .text
    .align 2
    .thumb_func
    .globl  PendSV_Handler
    .type   PendSV_Handler, %function
PendSV_Handler:
    b       .
    .size   PendSV_Handler, . - PendSV_Handler

    .section .text
    .align 2
    .thumb_func
    .globl  SysTick_Handler
    .type   SysTick_Handler, %function
SysTick_Handler:
    b       .
    .size   SysTick_Handler, . - SysTick_Handler

    .section .text
    .align 2
    .thumb_func
    .globl  WWDG_IWDG_IRQHandler
    .type   WWDG_IWDG_IRQHandler, %function
WWDG_IWDG_IRQHandler:
    b       .
    .size   WWDG_IWDG_IRQHandler, . - WWDG_IWDG_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  PVD_IRQHandler
    .type   PVD_IRQHandler, %function
PVD_IRQHandler:
    b       .
    .size   PVD_IRQHandler, . - PVD_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  FLASH_IRQHandler
    .type   FLASH_IRQHandler, %function
FLASH_IRQHandler:
    b       .
    .size   FLASH_IRQHandler, . - FLASH_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  RCC_IRQHandler
    .type   RCC_IRQHandler, %function
RCC_IRQHandler:
    b       .
    .size   RCC_IRQHandler, . - RCC_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  EXTI0_1_IRQHandler
    .type   EXTI0_1_IRQHandler, %function
EXTI0_1_IRQHandler:
    b       .
    .size   EXTI0_1_IRQHandler, . - EXTI0_1_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  EXTI2_3_IRQHandler
    .type   EXTI2_3_IRQHandler, %function
EXTI2_3_IRQHandler:
    b       .
    .size   EXTI2_3_IRQHandler, . - EXTI2_3_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  EXTI4_15_IRQHandler
    .type   EXTI4_15_IRQHandler, %function
EXTI4_15_IRQHandler:
    b       .
    .size   EXTI4_15_IRQHandler, . - EXTI4_15_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  ADC_IRQHandler
    .type   ADC_IRQHandler, %function
ADC_IRQHandler:
    b       .
    .size   ADC_IRQHandler, . - ADC_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  ATU_BKP_IRQHandler
    .type   ATU_BKP_IRQHandler, %function
ATU_BKP_IRQHandler:
    b       .
    .size   ATU_BKP_IRQHandler, . - ATU_BKP_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  ATU_IRQHandler
    .type   ATU_IRQHandler, %function
ATU_IRQHandler:
    b       .
    .size   ATU_IRQHandler, . - ATU_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  UTU1_IRQHandler
    .type   UTU1_IRQHandler, %function
UTU1_IRQHandler:
    b       .
    .size   UTU1_IRQHandler, . - UTU1_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  UTU2_IRQHandler
    .type   UTU2_IRQHandler, %function
UTU2_IRQHandler:
    b       .
    .size   UTU2_IRQHandler, . - UTU2_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  CMP1_IRQHandler
    .type   CMP1_IRQHandler, %function
CMP1_IRQHandler:
    b       .
    .size   CMP1_IRQHandler, . - CMP1_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  CMP2_IRQHandler
    .type   CMP2_IRQHandler, %function
CMP2_IRQHandler:
    b       .
    .size   CMP2_IRQHandler, . - CMP2_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  HTU_IRQHandler
    .type   HTU_IRQHandler, %function
HTU_IRQHandler:
    b       .
    .size   HTU_IRQHandler, . - HTU_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  DVSQ_IRQHandler
    .type   DVSQ_IRQHandler, %function
DVSQ_IRQHandler:
    b       .
    .size   DVSQ_IRQHandler, . - DVSQ_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  I2C_IRQHandler
    .type   I2C_IRQHandler, %function
I2C_IRQHandler:
    b       .
    .size   I2C_IRQHandler, . - I2C_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  SPI_IRQHandler
    .type   SPI_IRQHandler, %function
SPI_IRQHandler:
    b       .
    .size   SPI_IRQHandler, . - SPI_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  UART1_IRQHandler
    .type   UART1_IRQHandler, %function
UART1_IRQHandler:
    b       .
    .size   UART1_IRQHandler, . - UART1_IRQHandler

    .section .text
    .align 2
    .thumb_func
    .globl  UART2_IRQHandler
    .type   UART2_IRQHandler, %function
UART2_IRQHandler:
    b       .
    .size   UART2_IRQHandler, . - UART2_IRQHandler

/* User Stack and Heap initialization */
    .section .text
    .align 2
    .thumb_func
    .globl  __user_initial_stackheap
    .type   __user_initial_stackheap, %function
__user_initial_stackheap:
    ldr     r0, =heap_bottom
    ldr     r1, =stack_top
    ldr     r2, =heap_top
    ldr     r3, =stack_bottom
    bx      lr
    .size   __user_initial_stackheap, . - __user_initial_stackheap

    .end
