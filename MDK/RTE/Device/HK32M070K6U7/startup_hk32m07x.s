;***************************(C) COPYRIGHT HKMicroChip**************************
;* File Name          : startup_hk32m07x.s
;* Author             : HK AE Team
;* Description        : hk32m07x devices vector table for MDK-ARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM0 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;*******************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
; <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000600

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000300

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp                   ; Top of Stack
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
                DCD     WWDG_IWDG_IRQHandler                ; Window Watchdog
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
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler routine
Reset_Handler    PROC
                 EXPORT  Reset_Handler                 [WEAK]
        IMPORT  __main
        IMPORT  SystemInit  
                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                    [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler              [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                    [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                 [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IWDG_IRQHandler                [WEAK]
                EXPORT  PVD_IRQHandler                 [WEAK]				
                EXPORT  FLASH_IRQHandler               [WEAK]
                EXPORT  RCC_IRQHandler                 [WEAK]
                EXPORT  EXTI0_1_IRQHandler             [WEAK]
                EXPORT  EXTI2_3_IRQHandler             [WEAK]
                EXPORT  EXTI4_15_IRQHandler            [WEAK]
                EXPORT  ADC_IRQHandler            	   [WEAK]
                EXPORT  ATU_BKP_IRQHandler             [WEAK]
                EXPORT  ATU_IRQHandler          	   [WEAK]
                EXPORT  UTU1_IRQHandler                [WEAK]
                EXPORT  UTU2_IRQHandler                [WEAK]
                EXPORT  CMP1_IRQHandler                [WEAK]
                EXPORT  CMP2_IRQHandler                [WEAK]
                EXPORT  HTU_IRQHandler                 [WEAK]
                EXPORT  DVSQ_IRQHandler                [WEAK]
                EXPORT  I2C_IRQHandler                 [WEAK]
                EXPORT  SPI_IRQHandler           	   [WEAK]
                EXPORT  UART1_IRQHandler               [WEAK]
                EXPORT	UART2_IRQHandler               [WEAK]

WWDG_IWDG_IRQHandler
PVD_IRQHandler 
FLASH_IRQHandler            
RCC_IRQHandler              
EXTI0_1_IRQHandler          
EXTI2_3_IRQHandler          
EXTI4_15_IRQHandler         
ADC_IRQHandler         
ATU_BKP_IRQHandler       
ATU_IRQHandler       
UTU1_IRQHandler              
UTU2_IRQHandler                         
CMP1_IRQHandler             
CMP2_IRQHandler             
HTU_IRQHandler                    
DVSQ_IRQHandler                  
I2C_IRQHandler        
SPI_IRQHandler            
UART1_IRQHandler                          
UART2_IRQHandler             		      
                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB

                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit

                 ELSE

                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap

__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

;************************ (C) COPYRIGHT HKMicroChip *****END OF FILE*****
