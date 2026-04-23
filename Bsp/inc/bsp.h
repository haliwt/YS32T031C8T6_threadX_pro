#ifndef __BSP_H
#define __BSP_H
#include "main.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>



#include "ys32t031.h"
#include "main.h"
#include "delay.h"
#include "system_init.h"
#include "uart.h"  
#include "tim.h"
#include "iwdg.h"
#include "key.h"
#include "adc.h"


#include "ys32t031_tsc.h"
#include "ys32t031_tsc_lib.h"
#include "ys32t031_tsc_config.h"

#include "tx_api.h"



#include "tm1639.h"
#include "key.h"
#include "system_init.h"

//
#include "bsp_power.h"
#include "bsp_led.h"
#include "bsp_peripheral.h"
#include "bsp_key.h"
#include "bsp_cmd_link.h"
#include "bsp_xdp.h"
#include "bsp_tm1639.h"
#include "bsp_smg.h"
#include "bsp_sensor.h"
#include "bsp_threadx.h"

//wifi
#include "bsp_usart_wifi.h"
#include "bsp_mqtt_iot.h"
#include "bsp_esp8266.h"
#include "bsp_mqtt_iot.h"

#define Enable_EventRecorder  0

#define DEBUG_ENABLE    1



#if(Enable_EventRecorder == 1)
	#include "EventRecorder.h"
#endif


typedef struct _main_ref{

 
 volatile uint8_t time_10ms_f;
 volatile uint8_t time_20ms_f;
 volatile uint8_t time_100ms_f;
 volatile uint8_t time_200ms_f;
 volatile uint8_t time_300ms_f;
 volatile uint8_t time_400ms_f;
 volatile uint8_t time_500ms_f;
 volatile uint8_t time_600ms_f;
 volatile uint8_t time_900ms_f;

 volatile uint8_t time_base_1s_counter;
 volatile uint8_t time_2s_f;
 volatile uint8_t time_1s_f;
 volatile uint8_t time_3s_f;
 volatile uint8_t time_4s_f;
 volatile uint8_t time_5s_f;
 volatile uint8_t time_6s_f;
 volatile uint8_t time_7s_f;
 volatile uint8_t time_10s_f;

 volatile uint8_t time_1m_f;
 volatile uint8_t time_1m_wifi_f;
 volatile uint8_t time_2m_f;




}main_ref;

extern main_ref gpro_t;



void bsp_init(void);


void task_scheduler(void);

void Task_beep_called_100ms(void);



uint32_t Get_Unique_ID_32bit(void);
uint8_t bcc_check(const unsigned char *data, int len) ;



#endif 

