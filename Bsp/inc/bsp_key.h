#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "main.h"

extern uint8_t key_worked_f;
extern uint8_t key_long_f;
extern uint16_t key_data;
extern uint16_t key_time;



void Key_Scan(void);

void System_Status_PowerOff(void) ;

void System_Status_PowerOn(void) ;

void power_onoff_handler(void);


#endif 

