/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    SYSTEM_INIT.H
  * @version V1.0.0
  * @date    2024
  * @author  Yspring Firmware Team  
  * @brief   system_init Header Code.
  ******************************************************************************      
*/
#ifndef __SYSTEM_INIT_H
#define __SYSTEM_INIT_H

#ifdef __cplusplus
extern "C" {
#endif
   
#include "ys32t031.h"
#include "system_ys32t031.h"
#include <stdint.h> 



#define LED_TAPE_PIN            GPIO_Pin_11
#define LED_TAPE_GPIO_PORT      GPIOB

#define LED_TAPE_ON()           {GPIO_SetBits(LED_TAPE_GPIO_PORT,LED_TAPE_PIN);}
#define LED_TAPE_OFF()          {GPIO_ResetBits(LED_TAPE_GPIO_PORT,LED_TAPE_PIN);}


#define PLASMA_PIN              GPIO_Pin_2
#define PLASMA_GPIO_PORT        GPIOB

#define PLASMA_ON()             {GPIO_SetBits(PLASMA_GPIO_PORT, PLASMA_PIN);}
#define PLASMA_OFF()            {GPIO_ResetBits(LED_TAPE_GPIO_PORT, LED_TAPE_PIN);}


#define FAN_RUN_PIN             GPIO_Pin_5            
#define FAN_RUN_GPIO_PORT       GPIOA

#define FAN_RUN_ON()            {GPIO_SetBits(FAN_RUN_GPIO_PORT, FAN_RUN_PIN);}
#define FAN_RUN_OFF()           {GPIO_ResetBits(FAN_RUN_GPIO_PORT, FAN_RUN_PIN);}


#define RELAY_PIN               GPIO_Pin_10
#define RELAY_GPIO_PORT         GPIOB

#define RELAY_ON()              {GPIO_SetBits(RELAY_GPIO_PORT, RELAY_PIN);}
#define RELAY_OFF()             {GPIO_ResetBits(RELAY_GPIO_PORT, RELAY_PIN);}


#define LED_AI_PIN              GPIO_Pin_8
#define LED_AI_GPIO_PORT        GPIOB

#define LED_AI_ON()             {GPIO_ResetBits(LED_AI_GPIO_PORT, LED_AI_PIN);}
#define LED_AI_OFF()            {GPIO_SetBits(LED_AI_GPIO_PORT, LED_AI_PIN);}


#define LED_PTC_PIN             GPIO_Pin_7
#define LED_PTC_GPIO_PORT       GPIOB

#define LED_PTC_ON()            {GPIO_ResetBits(LED_PTC_GPIO_PORT, LED_PTC_PIN);}
#define LED_PTC_OFF()           {GPIO_SetBits(LED_PTC_GPIO_PORT, LED_PTC_PIN);}


#define LED_PLASMA_PIN          GPIO_Pin_7
#define LED_PLASMA_GPIO_PORT    GPIOF

#define LED_PLASMA_ON()         {GPIO_ResetBits(LED_PLASMA_GPIO_PORT, LED_PLASMA_PIN);}
#define LED_PLASMA_OFF()        {GPIO_SetBits(LED_PLASMA_GPIO_PORT, LED_PLASMA_PIN);}


#define LED_MOUSE_PIN           GPIO_Pin_6
#define LED_MOUSE_GPIO_PORT     GPIOF

#define LED_MOUSE_ON()          {GPIO_ResetBits(LED_MOUSE_GPIO_PORT, LED_MOUSE_PIN);}
#define LED_MOUSE_OFF()         {GPIO_SetBits(LED_MOUSE_GPIO_PORT, LED_MOUSE_PIN);}


#define LED_WIFI_PIN            GPIO_Pin_9
#define LED_WIFI_GPIO_PORT      GPIOB

#define LED_WIFI_ON()           {GPIO_ResetBits(LED_WIFI_GPIO_PORT, LED_WIFI_PIN);}
#define LED_WIFI_OFF()          {GPIO_SetBits(LED_WIFI_GPIO_PORT, LED_WIFI_PIN);}
#define LED_WIFI_TOGGLE()       GPIO_TogglePin(LED_WIFI_GPIO_PORT, LED_WIFI_PIN)

#define LED_POWER_PIN           GPIO_Pin_15
#define LED_POWER_GPIO_PORT     GPIOA

#define LED_POWER_ON()          do{LED_POWER_GPIO_PORT->BSRR =(uint32_t)LED_POWER_PIN <<16 ;}while(0)//{GPIO_ResetBits(LED_POWER_GPIO_PORT, LED_POWER_PIN);}
#define LED_POWER_OFF()         do{LED_POWER_GPIO_PORT->BSRR = LED_POWER_PIN;}while(0)//GPIO_SetBits(LED_POWER_GPIO_PORT, LED_POWER_PIN);}
#define LED_POWER_TOGGLE()      GPIO_TogglePin(LED_POWER_GPIO_PORT, LED_POWER_PIN)



#define LED_TEMP_PIN            GPIO_Pin_12
#define LED_TEMP_GPIO_PORT      GPIOA

#define LED_TEMP_ON()           {GPIO_ResetBits(LED_TEMP_GPIO_PORT, LED_TEMP_PIN);}
#define LED_TEMP_OFF()          {GPIO_SetBits(LED_TEMP_GPIO_PORT, LED_TEMP_PIN);}


#define LED_HUMI_PIN            GPIO_Pin_11
#define LED_HUMI_GPIO_PORT      GPIOA

#define LED_HUMI_ON()           {GPIO_ResetBits(LED_HUMI_GPIO_PORT, LED_HUMI_PIN);}
#define LED_HUMI_OFF()          {GPIO_SetBits(LED_HUMI_GPIO_PORT, LED_HUMI_PIN);}



extern void RCC_Configuration(void);
extern void GPIO_Configuration(void);
extern void NVIC_Configuration(void);



#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_INIT_H */
