/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    adc.H
  * @version V1.0.0
  * @date    2024
  * @author  Yspring Firmware Team  
  * @brief   adc Header Code.
  ******************************************************************************      
*/
#ifndef __ADC_H
#define __ADC_H


#ifdef __cplusplus
extern "C" {
#endif
    
#include "ys32t031.h"		
#include <stdint.h>  



extern void ADC_Configuration(void);
extern void ADC_Channel_Init(uint8_t CH);
extern void ADC_Vrefbuf_Select(uint32_t Mode);
extern void ADC_Channel_Select(uint8_t CH);
extern uint16_t ADC_GetValue(uint8_t CHx,uint32_t Vrefx);




#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */

