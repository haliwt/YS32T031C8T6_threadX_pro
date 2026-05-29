#ifndef __BSP_NTC_H
#define __BSP_NTC_H
#include "main.h"





void Get_Ntc_Resistance_Temperature_Handler(uint16_t voltage);




uint8_t ntc_res_linear_value(uint8_t ntc_value);


void getNtc_temperatureValue_init(uint16_t voltage);




#endif 


