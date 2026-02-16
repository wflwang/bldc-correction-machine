;;******************** (C) COPYRIGHT 2025 HK ********************
;* file               : IAR_Startup_hk32m07x.s
;* author             : AE Team
;* Version            : V1.0.0
;* date               : 22.01.2025
;* Description        : hk32m07x devices vector table for EWARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == __iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR 
;*                        address
;*                      - Configure the system clock
;*                      - Branches to main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the Cortex-M0 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;*******************************************************************************

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
                DCD     Reset_Handler                  ; Reset Handler
                DCD     NMI_Handler                    ; NMI Handler
                DCD     HardFault_Handler              ; Hard Fault Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     SVC_Handler                    ; SVCall Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     PendSV_Handler                 ; PendSV Handler
                DCD     SysTick_Handler                ; SysTick Handler

                ; External Interrupts
		DCD     WWDG_IWDG_IRQHandler           ; Window Watchdog
                DCD     PVD_IRQHandler                 ; PVD
                DCD     0			                   ; Reserved
                DCD     FLASH_IRQHandler               ; FLASH
                DCD     RCC_IRQHandler                 ; RCC
                DCD     EXTI0_1_IRQHandler             ; EXTI Line 0 and 1
                DCD     EXTI2_3_IRQHandler             ; EXTI Line 2 and 3
                DCD     EXTI4_15_IRQHandler            ; EXTI Line 4 to 15	  
                DCD     0            				   ; Reserved
                DCD     0          					   ; Reserved
                DCD     0          					   ; Reserved
                DCD     ADC_IRQHandler                 ; ADC 
                DCD     0          					   ; Reserved					
                DCD     ATU_BKP_IRQHandler			   ; ATU_BKP
                DCD     0               			   ; Reserved
                DCD     ATU_IRQHandler                 ; ATU
                DCD     0                			   ; Reserved
                DCD     UTU1_IRQHandler                ; UTU1
                DCD     UTU2_IRQHandler                ; UTU2
                DCD     CMP1_IRQHandler                ; CMP1
                DCD     CMP2_IRQHandler                ; CMP2
                DCD     HTU_IRQHandler 	               ; HTU  
                DCD     0                			   ; Reserved
                DCD     DVSQ_IRQHandler                ; DVSQ
                DCD     I2C_IRQHandler                 ; I2C
                DCD     SPI_IRQHandler                 ; SPI
                DCD     UART1_IRQHandler               ; UART1
                DCD     UART2_IRQHandler               ; UART2		
     
        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler
        
        
        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler
       
        
        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler
       
        
        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler
        
        
        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler
        
        
        PUBWEAK WWDG_IWDG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
WWDG_IWDG_IRQHandler
        B WWDG_IWDG_IRQHandler

	
	PUBWEAK PVD_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PVD_IRQHandler
        B PVD_IRQHandler 
       
        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FLASH_IRQHandler
        B FLASH_IRQHandler
        
                
        PUBWEAK RCC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RCC_IRQHandler
        B RCC_IRQHandler
        
                
        PUBWEAK EXTI0_1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI0_1_IRQHandler
        B EXTI0_1_IRQHandler
        
                
        PUBWEAK EXTI2_3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI2_3_IRQHandler
        B EXTI2_3_IRQHandler
        
                
        PUBWEAK EXTI4_15_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI4_15_IRQHandler
        B EXTI4_15_IRQHandler                      
        
        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC_IRQHandler
        B ADC_IRQHandler   
 
        PUBWEAK ATU_BKP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ATU_BKP_IRQHandler
        B ATU_BKP_IRQHandler 
        
        PUBWEAK ATU_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ATU_IRQHandler
        B ATU_IRQHandler 
 
        PUBWEAK UTU1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UTU1_IRQHandler
        B UTU1_IRQHandler 
        
        PUBWEAK UTU2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UTU2_IRQHandler
        B UTU2_IRQHandler       
        
        PUBWEAK CMP1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
CMP1_IRQHandler
        B CMP1_IRQHandler

        PUBWEAK CMP2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
CMP2_IRQHandler
        B CMP2_IRQHandler
        

        PUBWEAK HTU_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
HTU_IRQHandler
        B HTU_IRQHandler
        
        PUBWEAK DVSQ_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DVSQ_IRQHandler
        B DVSQ_IRQHandler  
        
        PUBWEAK I2C_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C_IRQHandler
        B I2C_IRQHandler 
        
                
        PUBWEAK SPI_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI_IRQHandler
        B SPI_IRQHandler


        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART1_IRQHandler
        B UART1_IRQHandler


	PUBWEAK UART2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART2_IRQHandler
        B UART2_IRQHandler  
		
        END
;******************** (C) COPYRIGHT 2025 HK ********************
