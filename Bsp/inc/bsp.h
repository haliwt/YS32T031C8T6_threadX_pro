#ifndef __BSP_H
#define __BSP_H
#include "main.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>



#include "ys32t031.h"

#include "tim.h"


#include "dht11.h" 
#include "tm1639.h"
#include "key.h"
#include "system_init.h"

//
#include "bsp_power.h"
#include "bsp_led.h"
#include "bsp_peripheral.h"
#include "bsp_key.h"
#include "bsp_cmd_link.h"

//wifi
#include "bsp_usart_wifi.h"
#include "bsp_mqtt_iot.h"
#include "bsp_esp8266.h"
#include "bsp_mqtt_iot.h"

#define Enable_EventRecorder  0


#if(Enable_EventRecorder == 1)
	#include "EventRecorder.h"
#endif



void bsp_init(void);


void task_scheduler(void);

void Task_beep_called_100ms(void);

void delay_ms(uint16_t ms);

uint32_t Get_Unique_ID_32bit(void);
uint8_t bcc_check(const unsigned char *data, int len) ;



#endif 

