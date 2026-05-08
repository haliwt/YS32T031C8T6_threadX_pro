#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "main.h"

#define KEY_POWER_VALUE()         ((GPIOB->IDR & GPIO_Pin_6) ? 1 : 0 )//sys_read_gpio_pin_value(GPIOD, KEY_POWER_Pin) //GPIO_1 按键按下，返回�??: 1
#define KEY_MODE_VALUE()          ((GPIOB->IDR & GPIO_Pin_5) ? 1 : 0 ) //sys_read_gpio_pin_value(GPIOD, KEY_MODE_Pin)

#define KEY_DOWN_VALUE()          ((GPIOB->IDR & GPIO_Pin_3) ? 1 : 0 )//sys_read_gpio_pin_value(GPIOD, KEY_DOWN_Pin)
#define KEY_UP_VALUE()            ((GPIOB->IDR & GPIO_Pin_4) ? 1 : 0 )



typedef enum{

    KEY_DOWN,
	KEY_UP
}key_state;

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

