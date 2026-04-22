/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    dht11.H
  * @version V1.0.0
  * @date    2024
  * @author  Yspring Firmware Team  
  * @brief   dht11 Header Code.
  ******************************************************************************      
*/
#ifndef __DHT11_H
#define __DHT11_H


#ifdef __cplusplus
extern "C" {
#endif
    
#include "ys32t031.h"		
#include <stdint.h>  



//extern uint8_t Data_Read_Finish_f;
//extern uint16_t data_read_interval;
extern uint8_t humidity;
extern uint8_t temperature;


#define DHT11_DATA_PIN              GPIO_Pin_4
#define DHT11_DATA_GPIO_PORT        GPIOA




void SET_DHT11_DATA_IN_MODE(void);
void SET_DHT11_DATA_OUT_MODE(void);



uint8_t dht11_read_temp_humidity_value(void);



#ifdef __cplusplus
}
#endif

#endif /* __DHT11_H */

