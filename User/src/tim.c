/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    TIM.C
  * @author  Yspring Firmware Team  
  * @brief   TIM Source Code.
  ******************************************************************************      
*/

#include "ys32t031.h"
#include "tim.h"



void TIM1_Configuration(void);
void TIM3_Configuration(void);
void TIM6_Configuration(void);
void TIM14_Configuration(void);
void BEEP_ON(void);
void BEEP_OFF(void);
void fan_on(uint16_t fan_duty);
void fan_off(void);
void ultra_sound_on(uint16_t us_duty);
void ultra_sound_off(void);



// TIM1 初始化配置
void TIM1_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // 溢出时间 = ((自动重装值319 + 1) * (分频系数 5 + 1)) / 48000000 = 40 μs, 频率= 25 kHz
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 5;
    TIM_TimeBaseStructure.TIM_Period = 319;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}



// TIM3 初始化配置
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 溢出时间 = ((自动重装值319 + 1) * (分频系数 5 + 1)) / 48000000 = 40 μs, 频率= 25 kHz
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 5;
    TIM_TimeBaseStructure.TIM_Period = 319;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_Cmd(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);  
}



// TIM6 初始化配置
void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    // 溢出时间 = ((自动重装值3332 + 1) * (分频系数 47 + 1)) / 48000000 = 5 ms, 频率= 200 Hz
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 95;
    TIM_TimeBaseStructure.TIM_Period = 2499;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	  TIM_ClearFlag(TIM6, TIM_FLAG_Update);                   // 清除计数器中断标志位  
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
    TIM_Cmd(TIM6, ENABLE);
}



// TIM14 初始化配置
void TIM14_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM14, ENABLE);

    // 溢出时间 = ((自动重装值749 + 1) * (分频系数 15 + 1)) / 48000000 = 250 μs, 频率= 4 kHz
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 15;
    TIM_TimeBaseStructure.TIM_Period = 749;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);

    TIM_Cmd(TIM14, ENABLE);
    TIM_CtrlPWMOutputs(TIM14, ENABLE);
}



//蜂鸣器开
void BEEP_ON(void)
{
    TIM_SetCompare1(TIM14,374);
	
	  TIM_Cmd(TIM14, ENABLE);
    TIM_CtrlPWMOutputs(TIM14, ENABLE);
}


//蜂鸣器关
void BEEP_OFF(void)
{
    TIM_SetCompare1(TIM14,0);
	
	  TIM_Cmd(TIM14, ENABLE);
    TIM_CtrlPWMOutputs(TIM14, ENABLE);
}



//风扇开
void fan_on(uint16_t fan_duty)
{
    TIM_SetCompare1(TIM3,fan_duty);
	
	  TIM_Cmd(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}


//风扇关
void fan_off(void)
{
    TIM_SetCompare1(TIM3,0);
	
	  TIM_Cmd(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}



//超声波开
void ultra_sound_on(uint16_t us_duty)
{
    TIM_SetCompare3(TIM1,us_duty);
	
	  TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}


//超声波关
void ultra_sound_off(void)
{
    TIM_SetCompare3(TIM1,0);
	
	  TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}










