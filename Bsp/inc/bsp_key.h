#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "main.h"

extern uint8_t key_worked_f;
//extern uint8_t key_long_f;
extern uint16_t key_data;
extern uint16_t key_time;



void Key_Scan(void);

void System_Status_PowerOff(void) ;

void System_Status_PowerOn(void) ;

void key_power_short_handler(void);

void key_power_long_handler(void);

void key_mode_short_handler(void);

void key_mode_long_handler(void);

void key_up_short_handler(void);


void key_down_short_handler(void);

void key_down_long_handler(void);


#endif 

