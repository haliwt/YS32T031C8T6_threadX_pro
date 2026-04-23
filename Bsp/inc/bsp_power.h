/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    user.H
  * @version V1.0.0
  * @date    2024
  * @author  Yspring Firmware Team  
  * @brief   user Header Code.
  ******************************************************************************      
*/
#ifndef __BSP_POWER_H
#define __BSP_POWER_H

#ifdef __cplusplus
extern "C" {
#endif
   
#include "ys32t031.h"
#include "system_ys32t031.h"  
#include <stdint.h> 

#define _AD_FCUR      0

#define _FCUR_CH      2


#define _DEVICE_WORK_TIME         120

#define _POWER_KEY_DOWN          (1<<0)     
#define _UP_KEY_DOWN             (1<<1)
#define _DOWN_KEY_DOWN           (1<<2)
#define _MODE_KEY_DOWN           (1<<3)

#define _FAN_SPEED_LEVEL_1          0
#define _FAN_SPEED_LEVEL_2          1
#define _FAN_SPEED_LEVEL_3          2

#define _DEVICE_REST_TIME            10

#define BEEP_LENGTH_DEFAULT 49		//4*100ms
#define NON_BEEP_LENGTH_DEFAULT 49	//4*100ms 

typedef enum {BEEP_ONCE,BEEP_TWO,BEEP_THREE,BEEP_1SECONDS,BEEP_TIME_OVER}Beep_TypeDef;






//extern volatile uint8_t Times5msCnt;
extern uint8_t Times10msCnt;
//extern uint8_t Times100msCnt;
extern uint8_t Times1minute;
extern uint16_t Times1minCnt;
extern uint8_t Cacl_time_sec;

extern volatile uint8_t time_5ms_f;

extern uint8_t disp_second_f ;


//WIFI TIMER
extern uint8_t time_wifi_10ms_f;

extern uint16_t ad_value[1];

extern uint16_t fan_current;

extern uint8_t discharge_f;



extern uint16_t current_temperature;
extern uint16_t setting_temperature;
extern uint16_t disp_temperature;
extern uint16_t disp_timing_time;
extern uint16_t disp_humidity;

extern uint8_t AI_timing_open_f;
extern uint8_t PTC_heat_open_f;
extern uint8_t first_temp_compare_f;

extern uint8_t ptc_prohibit_off_f;

extern uint8_t Ultra_Sound_open_f;
extern uint8_t plasma_open_f;

extern uint16_t timing_is_reach_disptime;
/*countdown timer  */
extern int8_t setting_timing_hour;
extern int8_t setting_timing_second;
extern int8_t timing_min_cnt;
extern uint8_t real_hours_counter;
extern int8_t temporary_timer_hours;
extern uint8_t time_beep_counter;


//

extern uint8_t Is_time_setting_f;

extern uint8_t Is_countdown_timer_f;
extern uint8_t set_temperature_value_f;
extern uint8_t time_1s_counter;



extern uint16_t led_scan_time;


extern uint8_t key_net_config_f;
extern uint16_t key_net_config_time;
extern uint8_t led_strip_open_f;

extern uint8_t flash_f;


extern uint16_t device_rest_time;





extern uint8_t fan_open_f;
extern uint8_t fan_speed_level;





extern volatile uint8_t beep_times;				  //次数
extern volatile uint8_t beep_lenght;			  //响的长度 *100ms
extern volatile uint8_t non_beep_length;		//间隔时间
extern uint16_t beep_interval_time;

//
extern uint8_t soft_version;


extern uint8_t temperature;
extern uint8_t humidity;




extern uint8_t no_fan_load_f;
extern uint16_t fan_current_det_time;
#define _NO_FAN_LOAD_CURRENT       50      //0.06A*0.67*4096/3.3   

extern uint8_t disp_switch_temp_humi;
//peripheral 
extern uint8_t key_be_pressed_f;
extern uint8_t disp_set_hours_time_f;
extern uint8_t  key_input_temp_f;

//wifi ref
volatile extern  uint8_t  wifi_rx_numbers;
extern  uint8_t  link_net_step;
extern  uint8_t  wifi_cofig_success_f;

extern  uint8_t  time_link_net_counter ;
extern  uint8_t  wifi_linking_tencent_f;
extern  uint8_t  wifi_connected_success_f;
extern  uint8_t  wifi_app_timer_power_on_f;
extern  uint8_t  wifi_run_step ;
extern  uint8_t  wifi_off_step;

extern  uint8_t  wifi_first_connectoed_cloud_f;
extern  uint8_t  wifi_read_net_data_f;
extern  uint8_t  wifi_check_net_f;
extern  uint8_t dc_connect_net_step	;
volatile extern  uint8_t  rx_wifi_data_success;
volatile extern  uint8_t   rx_wifi_data_counter;
extern  uint8_t  mqtt_status;
extern  uint8_t  time_autolink_counter;

/*end*/

//fan
extern uint8_t  fan_one_minute_cuonter;



//time couter 
extern uint8_t  time_set_hours_counter;

extern uint8_t  works_interval_f;


//wifi end 

extern uint8_t com_data_temp[8];
extern uint8_t com_data_buf[16];

#define _SMA            (1<<0)
#define _SMB            (1<<1)
#define _SMC            (1<<2)
#define _SMD            (1<<3)
#define _SME            (1<<4)
#define _SMF            (1<<5)
#define _SMG            (1<<6)
#define _SMDP           (1<<7)

#define Lcdch_A         _SMA|_SMB|_SMC|_SME|_SMF|_SMG               //A
#define Lcdch_b         _SMC|_SMD|_SME|_SMF|_SMG                    //b
#define Lcdch_C         _SMA|_SMD|_SME|_SMF                         //C
#define Lcdch_d         _SMB|_SMC|_SMD|_SME|_SMG                    //d
#define Lcdch_E         _SMA|_SMD|_SME|_SMF|_SMG                    //E
#define Lcdch_F         _SMA|_SME|_SMF|_SMG                         //F
#define Lcdch_u         _SMC|_SMD|_SME                              //u
#define Lcdch_L         _SMD|_SME|_SMF                              //L
#define Lcdch_P         _SMA|_SMB|_SME|_SMF|_SMG                    //P
#define Lcdch_S         _SMA|_SMC|_SMD|_SMF|_SMG                    //S
#define Lcdch_I         _SMB|_SMC                                   //I
#define Lcdch_o         _SMC|_SMD|_SME|_SMG                         //o
#define Lcdch_          _SMG                                        //-
#define Lcdch_r         _SME|_SMG                                   //r
#define Lcdch_h         _SMC|_SME|_SMF|_SMG                         //h
#define Lcdch_H         _SMB|_SMC|_SME|_SMF|_SMG                    //H
#define Lcdch_t         _SMD|_SME|_SMF|_SMG                         //t
#define Lcdch_J         _SMB|_SMC|_SMD|_SME                         //J

#define Lcdch_0         _SMA|_SMB|_SMC|_SMD|_SME|_SMF               //0
#define Lcdch_1         _SMB|_SMC                                   //1
#define Lcdch_2         _SMA|_SMB|_SMD|_SME|_SMG                    //2
#define Lcdch_3         _SMA|_SMB|_SMC|_SMD|_SMG                    //3
#define Lcdch_4         _SMB|_SMC|_SMF|_SMG                         //4
#define Lcdch_5         _SMA|_SMC|_SMD|_SMF|_SMG                    //5
#define Lcdch_6         _SMA|_SMC|_SMD|_SME|_SMF|_SMG               //6
#define Lcdch_7         _SMA|_SMB|_SMC                              //7
#define Lcdch_8         _SMA|_SMB|_SMC|_SMD|_SME|_SMF|_SMG          //8
#define Lcdch_9         _SMA|_SMB|_SMC|_SMD|_SMF|_SMG               //9

extern const uint8_t LED_TAB[11];

//LEDBUF0
#define _DA1             (1<<0)
#define _DA2             (1<<1)
#define _DA3             (1<<2)
#define _DA4             (1<<3)
#define _DA5             (1<<4)
#define _DA6             (1<<5)
#define _DA7             (1<<6)

//LEDBUF1
#define _DB1             (1<<0)
#define _DB2             (1<<1)
#define _DB3             (1<<2)
#define _DB4             (1<<3)
#define _DB5             (1<<4)
#define _DB6             (1<<5)
#define _DB7             (1<<6)
#define _DP1             (1<<7) 

//LEDBUF2
#define _DC1             (1<<0)
#define _DC2             (1<<1)
#define _DC3             (1<<2)
#define _DC4             (1<<3)
#define _DC5             (1<<4)
#define _DC6             (1<<5)
#define _DC7             (1<<6)
#define _DP2             (1<<7) 

//LEDBUF3
#define _A5              (1<<0)
#define _B5              (1<<1)
#define _CC5             (1<<2)
#define _DD5             (1<<3)
#define _E5              (1<<4)
#define _F5              (1<<5)
#define _G5              (1<<6)
#define _H5              (1<<7) 

//LEDBUF4
#define _A1              (1<<0)
#define _B1              (1<<1)
#define _CC1             (1<<2)
#define _DD1             (1<<3)
#define _E1              (1<<4)
#define _F1              (1<<5)
#define _G1              (1<<6)
#define _H1              (1<<7) 

//LEDBUF5
#define _A2              (1<<0)
#define _B2              (1<<1)
#define _CC2             (1<<2)
#define _DD2             (1<<3)
#define _E2              (1<<4)
#define _F2              (1<<5)
#define _G2              (1<<6)
#define _H2              (1<<7) 

//LEDBUF6
#define _A3              (1<<0)
#define _B3              (1<<1)
#define _CC3             (1<<2)
#define _DD3             (1<<3)
#define _E3              (1<<4)
#define _F3              (1<<5)
#define _G3              (1<<6)
#define _H3              (1<<7) 

//LEDBUF7
#define _A4              (1<<0)
#define _B4              (1<<1)
#define _CC4             (1<<2)
#define _DD4             (1<<3)
#define _E4              (1<<4)
#define _F4              (1<<5)
#define _G4              (1<<6)
#define _H4              (1<<7) 

#define _LED_MODE        ((1<<4)|(1<<5))
#define _LED_UP          ((1<<2)|(1<<3))  
#define _LED_DOWN        ((1<<6)|(1<<7))

typedef struct  _power_state{

    uint8_t on_step;
	uint8_t  off_step;


}power_state;

extern power_state gon_t;


void Clear_Ram(void);

extern void Adc_Channel_Sample(void);
extern void AD_Filter(void);

extern void LED_Strip_Ctrl(void);



void Countdown_timer_Handler(void);


void Trigger_Simple_Beep(uint8_t ms_10) ;

void Task_Beep_Simple_10ms(void);

void works_nomal_run_time_handler(void);
void set_temp_compare(void);
void beep_power_sound(void);

void power_onoff_handler(void);

	



#ifdef __cplusplus
}
#endif

#endif /* __USER_H */
