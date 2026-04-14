/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    system_init.C
  * @author  Yspring Firmware Team  
  * @brief   system_init Source Code.
  ******************************************************************************      
*/

#include "system_init.h"
//#include "bsp.h"
#include "delay.h"
#include "ys32t031.h"
#include "ys32t031_it.h"   
#include <stdint.h>



void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);


// RCC initialization configuration
void RCC_Configuration(void)
{
    FLASH_SetLatency(FLASH_Latency_3);
	
    // Enable LSI
    RCC_LSICmd(ENABLE);

    RCC_HSI_CLK(RCC_HSI_48M, RCC_HSI_CLK_Div1);
    RCC_HSICmd(ENABLE);
    while( RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

    // HCLK = 48 MHz
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    RCC_SYSCLKConfig(RCC_SOURCE_HSI);
    while (RCC_GetSYSCLKSource() != RCC_SOURCE_HSI);

    FLASH_SetLatency(FLASH_Latency_2);

    // PCLK = 48 MHz
    RCC_PCLKConfig(RCC_HCLK_Div1);

    // SYSCLK = 48 MHz
    SystemCoreClockUpdate();
}



// GPIO 初始化配置
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOF);

    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOA, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOB, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOC, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOF, ENABLE);
	
	  GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_SET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);
	
	  GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
	
	  GPIO_WriteBit(GPIOF, GPIO_Pin_6, Bit_SET);
    GPIO_WriteBit(GPIOF, GPIO_Pin_7, Bit_SET);

    // GPIO_Output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Low;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);

    // GPIO_Output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
	  GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_SET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);

    // ADC_IN2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ADC_IN3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // TIM1_CH3
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_4);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_VeryHigh;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // TIM3_CH1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_VeryHigh;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // UART1_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_VeryHigh;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // UART1_RX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_VeryHigh;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // UART2_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_VeryHigh;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // UART2_RX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_4);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_VeryHigh;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // GPIO_Output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET);
	  GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_RESET);

    // TSC_IO7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // TSC_IO8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // TSC_IO9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // TSC_IO10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // TIM14_CH1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_0);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_VeryHigh;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

		// GPIO_Output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Low;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_SET);
		
    // GPIO_Output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
		
		GPIO_WriteBit(GPIOF, GPIO_Pin_0, Bit_RESET);
    GPIO_WriteBit(GPIOF, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIOF, GPIO_Pin_3, Bit_RESET);
    GPIO_WriteBit(GPIOF, GPIO_Pin_4, Bit_RESET);
    GPIO_WriteBit(GPIOF, GPIO_Pin_6, Bit_SET);
    GPIO_WriteBit(GPIOF, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOF, GPIO_Pin_9, Bit_RESET);

    // GPIO_Input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}
	


//NVIC中断配置
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
	  NVIC_InitStructure.NVIC_IRQChannel = TIM6_LPTIM_IRQn ;   //设置中断来源
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;          //设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	  NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;         //IRQ通道:串口1
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;          //优先级 :1级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //使能IRQ通道
    NVIC_Init(&NVIC_InitStructure);
	
	  NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;         //IRQ通道:串口1
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;          //优先级 :1级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //使能IRQ通道
    NVIC_Init(&NVIC_InitStructure);
}










