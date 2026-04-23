#ifndef __BSP_PERIPHERAL_H
#define __BSP_PERIPHERAL_H
#include "main.h"


void LED_Strip_Ctrl(void);
void Plasma_Ctrl(void);
void Fan_Ctrl_Process(void);

void Beep(Beep_TypeDef music);

void Ultra_Sound_Ctrl(void);
void Relay_Ctrl(void);

void Heat_Process(void);

void Fan_Current_Det(void);



void peripheral_fun_handler(void);
void workd_interval_time_peripheral_handle(void);
void workd_interval_turn_off_handle(void);


void power_off_peripheral_handler(void);

void power_on_peripheral_handler(void);

#endif 
