;*******************************************************************************
;* File Name          : startup_ys32t031.s
;* Author             : ys Application Team
;* Description        : ys32t031 devices vector table for MDK-ARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM0 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;********************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
__stack_limit
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x200

                IF       Heap_Size != 0                      ; Heap is provided
                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit
                ENDIF

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD      __initial_sp                        ;     Top of Stack
                DCD      Reset_Handler                       ;     Reset Handler
                DCD      NMI_Handler                         ; -14 NMI Handler
                DCD      HardFault_Handler                   ; -13 Hard Fault Handler
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      SVC_Handler                         ;  -5 SVCall Handler
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      PendSV_Handler                      ;  -2 PendSV Handler
                DCD      SysTick_Handler                     ;  -1 SysTick Handler

                ; Interrupts
                DCD    	 WWDG_IRQHandler                     ;	0	
                DCD    	 PVD_IRQHandler                      ;	1	
                DCD    	 RTC_IRQHandler                      ;	2	
                DCD    	 FLASH_IRQHandler                    ;	3	
                DCD    	 RCC_IRQHandler                      ;	4	
                DCD    	 EXTI0_1_IRQHandler                  ;	5	
                DCD    	 EXTI2_3_IRQHandler                  ;	6	
                DCD    	 EXTI4_15_IRQHandler                 ;	7	
                DCD    	 TSC_IRQHandler                      ;	8	
                DCD    	 DMA_CHANNEL1_IRQHandler             ;	9	
                DCD    	 DMA_CHANNEL2_3_IRQHandler           ;	10	
                DCD    	 DMA_CHANNEL4_5_IRQHandler           ;	11	
                DCD    	 ADC_COMP_IRQHandler                 ;	12	
                DCD    	 TIM1_BRK_UP_TRG_COM_IRQHandler      ;	13	
                DCD    	 TIM1_CC_IRQHandler                  ;	14	
                DCD    	 RESERVED0                           ;	15	RESERVED
                DCD    	 TIM3_IRQHandler                     ;	16	
                DCD    	 TIM6_LPTIM_IRQHandler               ;	17	
                DCD    	 RESERVED1                           ;	18	RESERVED
                DCD    	 TIM14_IRQHandler                    ;	19	
                DCD    	 TIM15_IRQHandler                    ;	20	
                DCD    	 TIM16_IRQHandler                    ;	21	
                DCD    	 TIM17_IRQHandler                    ;	22	
                DCD    	 I2C1_IRQHandler                     ;	23	
                DCD    	 I2C2_IRQHandler                     ;	24	
                DCD    	 SPI1_IRQHandler                     ;	25	
                DCD    	 SPI2_IRQHandler                     ;	26	
                DCD    	 UART1_IRQHandler                    ;	27	
                DCD    	 UART2_IRQHandler                    ;	28	
                DCD    	 RESERVED2                           ;	29	RESERVED
                DCD    	 LED_IRQHandler                      ;	30	
                DCD    	 RESERVED3                           ;	31	RESERVED

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler routine
Reset_Handler    PROC
                 EXPORT  Reset_Handler                 [WEAK]
		     ; --- ??????????? ---
        IMPORT  _tx_timer_interrupt          ; ?? ThreadX ????
        IMPORT  __tx_PendSVHandler           ; ?? ThreadX ???????			 
					 
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
HardFault_Handler PROC
                EXPORT  HardFault_Handler              [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                    [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                IMPORT __tx_PendSVHandler 
                LDR R0,= __tx_PendSVHandler    
				BX R0
                ENDP
SysTick_Handler PROC
                IMPORT _tx_timer_interrupt
                LDR R0,= _tx_timer_interrupt
				BX R0
                ENDP

Default_Handler PROC

                EXPORT	WWDG_IRQHandler                  [WEAK] ;	0	
                EXPORT	PVD_IRQHandler                   [WEAK] ;	1	
                EXPORT	RTC_IRQHandler                   [WEAK] ;	2	
                EXPORT	FLASH_IRQHandler                 [WEAK] ;	3	
                EXPORT	RCC_IRQHandler                   [WEAK] ;	4	
                EXPORT	EXTI0_1_IRQHandler               [WEAK] ;	5	
                EXPORT	EXTI2_3_IRQHandler               [WEAK] ;	6	
                EXPORT	EXTI4_15_IRQHandler              [WEAK] ;	7	
                EXPORT	TSC_IRQHandler                   [WEAK] ;	8	
                EXPORT	DMA_CHANNEL1_IRQHandler          [WEAK] ;	9	
                EXPORT	DMA_CHANNEL2_3_IRQHandler        [WEAK] ;	10	
                EXPORT	DMA_CHANNEL4_5_IRQHandler        [WEAK] ;	11	
                EXPORT	ADC_COMP_IRQHandler              [WEAK] ;	12	
                EXPORT	TIM1_BRK_UP_TRG_COM_IRQHandler   [WEAK] ;	13	
                EXPORT	TIM1_CC_IRQHandler               [WEAK] ;	14	
                EXPORT	RESERVED0                        [WEAK] ;	15	RESERVED
                EXPORT	TIM3_IRQHandler                  [WEAK] ;	16	
                EXPORT	TIM6_LPTIM_IRQHandler            [WEAK] ;	17	
                EXPORT	RESERVED1                        [WEAK] ;	18	RESERVED
                EXPORT	TIM14_IRQHandler                 [WEAK] ;	19	
                EXPORT	TIM15_IRQHandler                 [WEAK] ;	20	
                EXPORT	TIM16_IRQHandler                 [WEAK] ;	21	
                EXPORT	TIM17_IRQHandler                 [WEAK] ;	22	
                EXPORT	I2C1_IRQHandler                  [WEAK] ;	23	
                EXPORT	I2C2_IRQHandler                  [WEAK] ;	24	
                EXPORT	SPI1_IRQHandler                  [WEAK] ;	25	
                EXPORT	SPI2_IRQHandler                  [WEAK] ;	26	
                EXPORT	UART1_IRQHandler                 [WEAK] ;	27	
                EXPORT	UART2_IRQHandler                 [WEAK] ;	28	
                EXPORT	RESERVED2                        [WEAK] ;	29	RESERVED
                EXPORT	LED_IRQHandler                   [WEAK] ;	30	
                EXPORT	RESERVED3                        [WEAK] ;	31	RESERVED


WWDG_IRQHandler                      ;	0	
PVD_IRQHandler                       ;	1	
RTC_IRQHandler                       ;	2	
FLASH_IRQHandler                     ;	3	
RCC_IRQHandler                       ;	4	
EXTI0_1_IRQHandler                   ;	5	
EXTI2_3_IRQHandler                   ;	6	
EXTI4_15_IRQHandler                  ;	7	
TSC_IRQHandler                       ;	8	
DMA_CHANNEL1_IRQHandler              ;	9	
DMA_CHANNEL2_3_IRQHandler            ;	10	
DMA_CHANNEL4_5_IRQHandler            ;	11	
ADC_COMP_IRQHandler                  ;	12	
TIM1_BRK_UP_TRG_COM_IRQHandler       ;	13	
TIM1_CC_IRQHandler                   ;	14	
RESERVED0                            ;	15	RESERVED
TIM3_IRQHandler                      ;	16	
TIM6_LPTIM_IRQHandler                ;	17	
RESERVED1                            ;	18	RESERVED
TIM14_IRQHandler                     ;	19	
TIM15_IRQHandler                     ;	20	
TIM16_IRQHandler                     ;	21	
TIM17_IRQHandler                     ;	22	
I2C1_IRQHandler                      ;	23	
I2C2_IRQHandler                      ;	24	
SPI1_IRQHandler                      ;	25	
SPI2_IRQHandler                      ;	26	
UART1_IRQHandler                     ;	27	
UART2_IRQHandler                     ;	28	
RESERVED2                            ;	29	RESERVED
LED_IRQHandler                       ;	30	
RESERVED3                            ;	31	RESERVED

                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                IF       :LNOT::DEF:__MICROLIB
                IMPORT   __use_two_region_memory
                ENDIF

                EXPORT   __stack_limit
                EXPORT   __initial_sp
                IF       Heap_Size != 0                      ; Heap is provided
                EXPORT   __heap_base
                EXPORT   __heap_limit
                ENDIF

                 END

;*****************************END OF FILE***************************************
