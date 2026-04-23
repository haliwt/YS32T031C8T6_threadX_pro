#ifndef __BSP_SMG_H
#define __BSP_SMG_H
#include "main.h"

typedef enum {
    DISPLAY_TEMP = 0,
    DISPLAY_HUM = 1,
	DISPLAY_TIME = 2
} DisplayMode;

//void SMG_Display_Hour(void);  // 显示0-24小时时间

void SMG_Display_Err(uint8_t idata);


void display_digital_3_numbers(void);

//void set_timer_timing_value_handler(void);

void DHT11_Display_Data(uint8_t mode);

void LED_FUN_ON(void);



#endif 

