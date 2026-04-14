/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    uart.H
  * @version V1.0.0
  * @date    2024
  * @author  Yspring Firmware Team  
  * @brief   uart Header Code.
  ******************************************************************************      
*/
#ifndef __UART_H
#define __UART_H


#ifdef __cplusplus
extern "C" {
#endif
    
#include "ys32t031.h"		
#include <stdint.h>  


extern volatile uint8_t UART1_RX_BUF[60];
extern volatile uint8_t uart1_rx_cnt;

extern volatile uint8_t UART2_RX_BUF[60];
extern volatile uint8_t uart2_rx_cnt;



extern void UART1_Configuration(uint32_t BaudRate1);
extern void UART2_Configuration(uint32_t BaudRate2);
extern void UART1_SendByte(uint8_t Data);
extern void UART1_Send_Str(uint8_t *String,uint8_t s1);
extern void UART2_SendByte(uint8_t Data);
extern void UART2_Send_Str(uint8_t *String,uint8_t s2);


#ifdef __cplusplus
}
#endif

#endif /* __UART_H */

