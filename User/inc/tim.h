/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    TIM.H
  * @version V1.0.0
  * @date    2024
  * @author  Yspring Firmware Team  
  * @brief   TIM Header Code.
  ******************************************************************************      
*/
#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "ys32t031.h"



extern void TIM1_Configuration(void);
extern void TIM3_Configuration(void);
extern void TIM6_Configuration(void);
extern void TIM14_Configuration(void);
extern void BEEP_ON(void);
extern void BEEP_OFF(void);
extern void fan_on(uint16_t fan_duty);
extern void fan_off(void);
extern void ultra_sound_on(uint16_t us_duty);
extern void ultra_sound_off(void);

void beep_power_sound(void);


#ifdef __cplusplus
}
#endif

#endif /* __TIM_H */
