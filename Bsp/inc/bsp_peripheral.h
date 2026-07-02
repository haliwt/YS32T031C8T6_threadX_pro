#ifndef __BSP_PERIPHERAL_H
#define __BSP_PERIPHERAL_H
#include "main.h"



#define FAN_PWM_PIN             GPIO_Pin_6            
#define FAN_PWM_GPIO_PORT       GPIOA

#define FAN_PWM_GPIO_ON()            do{FAN_PWM_GPIO_PORT->BSRR = FAN_PWM_PIN;}while(0)//{GPIO_SetBits(FAN_RUN_GPIO_PORT, FAN_RUN_PIN);}
#define FAN_PWM_GPIO_OFF()           do{FAN_PWM_GPIO_PORT->BSRR =(uint32_t)FAN_PWM_PIN << 16;}while(0)//{GPIO_ResetBits(FAN_RUN_GPIO_PORT, FAN_RUN_PIN);}



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

void fan_full_fun(void);
void fan_stop(void);

void compare_set_temp_value(void);

void BEEP_OFF(void);

void BEEP_ON(void);

void beep_high_temperature_sound(void);

void beep_fan_default_sound(void);

void fan_start_power_on(void);

void direct_compare_set_temp_value(void);



#endif 
