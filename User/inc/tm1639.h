/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    tm1639.H
  * @version V1.0.0
  * @date    2024
  * @author  Yspring Firmware Team  
  * @brief   tm1639 Header Code.
  ******************************************************************************      
*/
#ifndef __TM1639_H
#define __TM1639_H


#ifdef __cplusplus
extern "C" {
#endif
    
#include "ys32t031.h"		
#include <stdint.h>  


#define TM1639_STB_PIN       GPIO_Pin_14            
#define TM1639_CLK_PIN       GPIO_Pin_15
#define TM1639_DIO_PIN       GPIO_Pin_13

#define TM1639_GPIO_PORT     GPIOC



#define TM1639_STB_L()       {GPIO_ResetBits(TM1639_GPIO_PORT, TM1639_STB_PIN);}
#define TM1639_STB_H()       {GPIO_SetBits(TM1639_GPIO_PORT, TM1639_STB_PIN);}

#define TM1639_CLK_L()       {GPIO_ResetBits(TM1639_GPIO_PORT, TM1639_CLK_PIN);}
#define TM1639_CLK_H()       {GPIO_SetBits(TM1639_GPIO_PORT, TM1639_CLK_PIN);}

#define TM1639_DIO_L()       {GPIO_ResetBits(TM1639_GPIO_PORT, TM1639_DIO_PIN);}
#define TM1639_DIO_H()       {GPIO_SetBits(TM1639_GPIO_PORT, TM1639_DIO_PIN);}	
	
	
#define LED_BRIGHTNESS_H     0x8F
#define LED_BRIGHTNESS_M     0x88
#define LED_BRIGHTNESS_L     0x83



	
extern void TM1639_Write_Byte(uint8_t data);
extern void TM1639_Write_Display_Data(uint8_t *data, uint8_t len);





#ifdef __cplusplus
}
#endif

#endif /* __TM1639_H */

