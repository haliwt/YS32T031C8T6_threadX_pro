/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    user.C
  * @author  Yspring Firmware Team  
  * @brief   user Source Code.
  ******************************************************************************      
*/


#include "delay.h"
#include "ys32t031.h"
#include "ys32t031_it.h"
#include <stdint.h>
#include "dht11.h"
#include "adc.h"
#include "key.h"
#include "display.h"
#include "system_init.h"
#include "tim.h"
#include "tm1639.h"
#include "ys32t031_tsc_lib.h"
#include "bsp.h"





static void power_on_fan_normal_handler(void);
static void power_on_fan_error_handler(void);



volatile uint8_t Times5msCnt;
uint8_t Times10msCnt;
//uint8_t Times100msCnt;
uint8_t Times1minute;
uint16_t Times1minCnt;
uint8_t Cacl_time_sec;

volatile uint8_t time_5ms_f;
volatile uint8_t time_10ms_f;
uint8_t time_100ms_f;
uint8_t time_300ms_f;
uint8_t time_wifi_10ms_f;

uint8_t time_switch_temp_hum_counter;
uint8_t  key_be_pressed_f;
uint8_t  time_set_hours_counter;



uint8_t time_1s_f;
uint8_t time_1minute_f;

uint16_t ad_value[1];
uint16_t fan_current;

//uint8_t key_worked_f;
//uint8_t key_long_f;
//uint16_t key_data;
//uint16_t key_time;

uint8_t discharge_f;
uint16_t work_time;

uint16_t current_temperature;
uint16_t setting_temperature;
uint16_t disp_temperature;
uint16_t disp_timing_time;
uint16_t disp_humidity;

uint8_t AI_timing_open_f;
uint8_t PTC_heat_open_f;
uint8_t first_temp_compare_f;

uint8_t ptc_prohibit_off_f;

uint8_t Ultra_Sound_open_f;
uint8_t plasma_open_f;

uint16_t timing_is_reach_disptime;

uint16_t setting_timing_hour;

uint8_t Is_time_setting_f;
uint8_t Is_temp_setting_f;
uint8_t Is_timing_hour_disp_f;
uint8_t set_temperature_value_f;
uint8_t time_set_temp_counter;
uint8_t disp_set_hours_time_f;




uint16_t timing_hour_disp_time;
uint16_t led_scan_time;

uint16_t key_flash_time;
uint8_t key_net_config_f;
uint16_t key_net_config_time;
uint8_t led_strip_open_f;

uint8_t flash_f;

uint8_t device_rest_f;
uint16_t device_rest_time;

uint8_t timing_min_cnt;
uint8_t timing_hour_cnt;

uint8_t fan_open_f;
uint8_t fan_speed_level;
uint16_t fan_delay_time_off;

volatile uint8_t beep_times;				//次数
volatile uint8_t beep_lenght;			  //响的长度 *100ms
volatile uint8_t non_beep_length;		//间隔时间
uint16_t beep_interval_time;




uint16_t reach_SetTemperature_time;
uint16_t lower_SetTemperature_time;

uint16_t temperature_det_more_time;
uint16_t temperature_det_less_time;

uint16_t fan_current_det_time;
uint8_t no_fan_load_f;

uint16_t disp_switch_time;
//
uint8_t soft_version;

//wifi 

uint8_t  link_net_step;
uint8_t  time_link_net_counter;
uint8_t  wifi_linking_tencent_f;
uint8_t  wifi_connected_success_f;
uint8_t  wifi_rx_numbers;
uint8_t  wifi_cofig_success_f;
uint8_t  wifi_app_timer_power_on_f;
uint8_t  wifi_run_step ;
uint8_t  wifi_first_connectoed_cloud_f;
uint8_t  wifi_read_net_data_f;





uint8_t com_data_temp[8];
uint8_t com_data_buf[16];


const uint8_t LED_TAB[11]={ 
    _SMA|_SMB|_SMC|_SMD|_SME|_SMF,        //0
    _SMB|_SMC,                            //1
    _SMA|_SMB|_SMD|_SME|_SMG,             //2
    _SMA|_SMB|_SMC|_SMD|_SMG,             //3
    _SMB|_SMC|_SMF|_SMG,                  //4
    _SMA|_SMC|_SMD|_SMF|_SMG,             //5
    _SMA|_SMC|_SMD|_SME|_SMF|_SMG,        //6
    _SMA|_SMB|_SMC,                       //7
    _SMA|_SMB|_SMC|_SMD|_SME|_SMF|_SMG,   //8
    _SMA|_SMB|_SMC|_SMD|_SMF|_SMG,        //9
    0,                                    //清屏
};



#if 0

void Clear_Ram(void);
void Real_Time(void);
void Adc_Channel_Sample(void);
void AD_Filter(void);
void Key_Scan(void);
void LED_Strip_Ctrl(void);
void Plasma_Ctrl(void);
void Fan_Ctrl_Process(void);
void Beep(Beep_TypeDef music);   
void Task_beep_called_100ms(void);
void Ultra_Sound_Ctrl(void);
void Relay_Ctrl(void);
void Heat_Process(void);
void Update_LED_Display(void);
#endif 


//变量初始化
void Clear_Ram(void)
{
    time_5ms_f = 0;
	  time_10ms_f = 0;
	  time_100ms_f = 0;
	  time_300ms_f =0;
	  time_1s_f = 0;
	  time_1minute_f=0;
	
	 // Times5msCnt = 0;
	  Times10msCnt = 0;
	 // Times100msCnt = 0;
	  Times1minute = 0;
	  Times1minCnt = 0;
	  Cacl_time_sec = 0;
	
	  Data_Read_Finish_f = 1;
	  data_read_interval = 0;
	
	  key_worked_f = 0;
	  key_long_f = 0;
	  key_data = 0;
	  key_time = 0;
	
	  discharge_f = 0;
		work_time = 0;
		
		device_rest_f = 0;
		device_rest_time = 0;
		
		fan_speed_level = 3;
		fan_open_f = 0;
		fan_delay_time_off = 0;
		
		AI_timing_open_f = 0;
		PTC_heat_open_f = 0;
		first_temp_compare_f=0;
		Ultra_Sound_open_f = 0;
		plasma_open_f = 0;
		led_strip_open_f = 0;
		
		timing_is_reach_disptime = 0;
		
		Is_time_setting_f = 0;
		Is_temp_setting_f = 0;
		Is_timing_hour_disp_f = 0;
		
		key_flash_time = 0;
		flash_f = 0;
		led_scan_time = 0;
		
		timing_min_cnt = 0;
		timing_hour_cnt = 0;
		
	
		
		reach_SetTemperature_time = 0;
		lower_SetTemperature_time = 0;
		
		temperature_det_more_time = 0;
		temperature_det_less_time = 0;
		
		no_fan_load_f = 0;
		fan_current_det_time = 0;
		
		disp_switch_time = 0;
		beep_interval_time = 0;
		//wifi 
		wifi_linking_tencent_f=0;
		
		
		//wifi_connected_f = 0;
		
		com_data_temp[0]=0;
	  com_data_temp[1]=0;
	  com_data_temp[2]=0;
	  com_data_temp[3]=0;
		com_data_temp[4]=0;
	  com_data_temp[5]=0;
	  com_data_temp[6]=0;
	  com_data_temp[7]=0;
		
		com_data_buf[0]=0;
	  com_data_buf[1]=0;
	  com_data_buf[2]=0;
	  com_data_buf[3]=0;
		com_data_buf[4]=0;
	  com_data_buf[5]=0;
	  com_data_buf[6]=0;
	  com_data_buf[7]=0;
	  com_data_buf[8]=0;
	  com_data_buf[9]=0;
	  com_data_buf[10]=0;
	  com_data_buf[11]=0;
		com_data_buf[12]=0;
	  com_data_buf[13]=0;
	  com_data_buf[14]=0;
	  com_data_buf[15]=0;
	  //TM1639_Write_Display_Data(com_data_buf,16);
		
}




//ADC通道采样
void Adc_Channel_Sample(void)
{
    volatile uint16_t ad_temp;
	
    ad_temp = ADC_GetValue(_FCUR_CH,VREFBUF_ADC_VCC);
	
    ad_value[_AD_FCUR] = ad_temp;
}



//AD数据一阶滑动滤波
void AD_Filter(void)
{
    //FAN_CURRENT
	  fan_current=(ad_value[_AD_FCUR]*2+fan_current*18)/20;
}





uint8_t counter;
uint8_t power_Led_switch;	

 volatile uint16_t i;
 volatile uint16_t bw_i=0;
volatile uint16_t sw_i=0;
volatile uint16_t gw_i=0;
 volatile uint16_t disp_timing_time_temp;
 volatile uint16_t timing_diff_value_hour;
 volatile uint16_t timing_diff_value_min;

//更新LED显示
void Update_onLED_Display(void)
{

  for(i=0;i<8;i++)
	{
		com_data_temp[i]=0x00;
	}

	//LED_AI_OFF();LED_PTC_OFF();LED_PLASMA_OFF();LED_MOUSE_OFF();
	//LED_WIFI_OFF();LED_TEMP_OFF();LED_HUMI_OFF();//LED_WIFI_OFF();

	switch(discharge_f)//if(discharge_f)//power on 
	{

	case 1:

	switch(no_fan_load_f){

		case 1:

	      power_on_fan_error_handler();
		break;

		case 0:


		  power_on_fan_normal_handler();

	    break;

	}
	break;

	
	}




	com_data_buf[0]=(com_data_temp[0]&0x0f);
	com_data_buf[1]=((com_data_temp[0]>>4)&0x0f);
	com_data_buf[2]=(com_data_temp[1]&0x0f);
	com_data_buf[3]=((com_data_temp[1]>>4)&0x0f);
	com_data_buf[4]=(com_data_temp[2]&0x0f);
	com_data_buf[5]=((com_data_temp[2]>>4)&0x0f);
	com_data_buf[6]=(com_data_temp[3]&0x0f);
	com_data_buf[7]=((com_data_temp[3]>>4)&0x0f);
	com_data_buf[8]=(com_data_temp[4]&0x0f);
	com_data_buf[9]=((com_data_temp[4]>>4)&0x0f);
	com_data_buf[10]=(com_data_temp[5]&0x0f);
	com_data_buf[11]=((com_data_temp[5]>>4)&0x0f);
	com_data_buf[12]=(com_data_temp[6]&0x0f);
	com_data_buf[13]=((com_data_temp[6]>>4)&0x0f);
	com_data_buf[14]=(com_data_temp[7]&0x0f);
	com_data_buf[15]=((com_data_temp[7]>>4)&0x0f);


	TM1639_Write_Display_Data(com_data_buf,16);

}


void Update_offLED_Display(void)
{

  for(i=0;i<8;i++)
	{
		com_data_temp[i]=0x00;
	}

	//LED_AI_OFF();LED_PTC_OFF();LED_PLASMA_OFF();LED_MOUSE_OFF();
	//LED_WIFI_OFF();LED_TEMP_OFF();LED_HUMI_OFF();//LED_WIFI_OFF();

	

	




	com_data_buf[0]=(com_data_temp[0]&0x0f);
	com_data_buf[1]=((com_data_temp[0]>>4)&0x0f);
	com_data_buf[2]=(com_data_temp[1]&0x0f);
	com_data_buf[3]=((com_data_temp[1]>>4)&0x0f);
	com_data_buf[4]=(com_data_temp[2]&0x0f);
	com_data_buf[5]=((com_data_temp[2]>>4)&0x0f);
	com_data_buf[6]=(com_data_temp[3]&0x0f);
	com_data_buf[7]=((com_data_temp[3]>>4)&0x0f);
	com_data_buf[8]=(com_data_temp[4]&0x0f);
	com_data_buf[9]=((com_data_temp[4]>>4)&0x0f);
	com_data_buf[10]=(com_data_temp[5]&0x0f);
	com_data_buf[11]=((com_data_temp[5]>>4)&0x0f);
	com_data_buf[12]=(com_data_temp[6]&0x0f);
	com_data_buf[13]=((com_data_temp[6]>>4)&0x0f);
	com_data_buf[14]=(com_data_temp[7]&0x0f);
	com_data_buf[15]=((com_data_temp[7]>>4)&0x0f);


	TM1639_Write_Display_Data(com_data_buf,16);

}


/**
  * @brief  fan run is error
  * @note  
  * @param: no_fan_load_f =1
  *
**/
static void power_on_fan_error_handler(void)
{
	
	com_data_temp[0] |= Lcdch_E;  //E
	com_data_temp[1] |= Lcdch_r;  //r
	com_data_temp[1] |= _DP1;     //DP1
	com_data_temp[2] |= _DP2;     //DP2
	com_data_temp[2] |= Lcdch_2;

	if(!AI_timing_open_f) {LED_AI_ON();}
	if(PTC_heat_open_f) {LED_PTC_ON();}
	if(plasma_open_f) {LED_PLASMA_ON();}
	if(Ultra_Sound_open_f) {LED_MOUSE_ON();}

	LED_POWER_ON();

	if(key_net_config_f)
	{
		 led_scan_time++;
		if(led_scan_time>=33)
		{
			led_scan_time = 0;
		}

		if(led_scan_time<16)
		{
			LED_WIFI_ON();
		}
	}
	else
	{
		if(wifi_connected_success_f)
		{
			LED_WIFI_ON();
		}
		else
		{
			led_scan_time++;
			if(led_scan_time>=150)
			{
			led_scan_time = 0;
			}											

			if(led_scan_time<75)
			{
			LED_WIFI_ON();
			}
		}
	}

	com_data_temp[3] |= _A5|_B5|_CC5|_DD5|_E5|_F5|_G5|_H5;
	com_data_temp[4] |= _A1|_B1|_CC1|_DD1|_E1|_F1|_G1|_H1;
	com_data_temp[5] |= _A2|_B2|_CC2|_DD2|_E2|_F2|_G2|_H2;
	com_data_temp[6] |= _A3|_B3|_CC3|_DD3|_E3|_F3|_G3|_H3;
	com_data_temp[7] |= _A4|_B4|_CC4|_DD4|_E4|_F4|_G4|_H4;

	beep_interval_time++;
	if(beep_interval_time>=1000)
	{
	beep_interval_time = 0;

	Beep(BEEP_THREE);
	}


}

/**
  * @brief  fan run is ok
  * @note  
  *
  *
**/
static void power_on_fan_normal_handler(void)
{

	if(Is_timing_hour_disp_f){
		if(setting_timing_hour==0)
		{
			disp_timing_time_temp = 0;
			gw_i = Lcdch_H;
			LED_TEMP_OFF();
		    LED_HUMI_OFF();
		}
		else
		{
			if(setting_timing_hour>timing_hour_cnt)
			{
				timing_diff_value_hour = setting_timing_hour-timing_hour_cnt;

				if(timing_diff_value_hour>1)
				{
					disp_timing_time_temp = timing_diff_value_hour;
					gw_i = Lcdch_H;
					LED_TEMP_OFF();
		            LED_HUMI_OFF();
				}
				else
				{
					timing_diff_value_min = 60-timing_min_cnt;

					if(timing_diff_value_min>=60)
					{
						disp_timing_time_temp=timing_diff_value_hour;
						gw_i = Lcdch_H;
						LED_TEMP_OFF();
		                LED_HUMI_OFF();
					}
					else
					{
						disp_timing_time_temp = timing_diff_value_min;
						gw_i = 0;													
					}													
				}
			}
			else
			{
				disp_timing_time_temp = 0; 
				gw_i = Lcdch_H;
				LED_TEMP_OFF();
		        LED_HUMI_OFF();
			}
		}	

		disp_timing_time = disp_timing_time_temp;

		bw_i = LED_TAB[disp_timing_time/10];
		sw_i = LED_TAB[disp_timing_time%10];

		if(key_flash_time!=0)
		{
			key_flash_time--;

			if(key_flash_time==0)
			{
				Is_timing_hour_disp_f = 0;
			}
		}
	}
	else if(Is_time_setting_f)
	{
		disp_timing_time = setting_timing_hour;

		bw_i = LED_TAB[disp_timing_time/10];
		sw_i = LED_TAB[disp_timing_time%10];
		gw_i = Lcdch_H;
		LED_TEMP_OFF();
		LED_HUMI_OFF();

		if(key_flash_time!=0)
		{
		key_flash_time--;

		if(key_flash_time==0)
		{
		Is_time_setting_f = 0;
		}
		}
	}
	else if(Is_temp_setting_f)
	{
		disp_temperature = setting_temperature*10;

		bw_i = LED_TAB[disp_temperature/100];
		sw_i = LED_TAB[disp_temperature%100/10];
		sw_i |= _DP1;
		gw_i = LED_TAB[disp_temperature%10];
		
		LED_HUMI_OFF();
        LED_TEMP_ON();
		

		if(key_flash_time!=0)
		{
		key_flash_time--;

		if(key_flash_time==0)
		{
		Is_temp_setting_f = 0;
		}
		}
	}
	else
	{
		disp_temperature = temperature*10;
		disp_humidity = humidity*10;
        #if 0
		disp_switch_time++;
		if(disp_switch_time>=60)//600
		{
			disp_switch_time = 0;
		}
		#endif 

		if(disp_switch_time < 3)//300
		{
			bw_i = LED_TAB[disp_temperature/100];
			sw_i = LED_TAB[disp_temperature%100/10];
			sw_i |= _DP1;
			gw_i = LED_TAB[disp_temperature%10];
			LED_HUMI_OFF();
            LED_TEMP_ON();
			
		}
		else if(disp_switch_time > 2 && disp_switch_time < 5)
		{
			bw_i = LED_TAB[disp_humidity/100];
			sw_i = LED_TAB[disp_humidity%100/10];
			sw_i |= _DP1;
			gw_i = LED_TAB[disp_humidity%10];
			LED_TEMP_OFF();

			LED_HUMI_ON();
			
		}
		else{
          disp_switch_time =0;
		}
	}	

	com_data_temp[0] |= bw_i; 
	com_data_temp[1] |= sw_i; 
	com_data_temp[2] |= gw_i;

	LED_POWER_ON();
#if 0
	//if(!AI_timing_open_f) {LED_AI_ON();}
	///if(PTC_heat_open_f) {LED_PTC_ON();}
	///if(plasma_open_f) {LED_PLASMA_ON();}
	//if(Ultra_Sound_open_f) {LED_MOUSE_ON();}

	LED_POWER_ON();

	if(key_net_config_f)
	{
		led_scan_time++;
		if(led_scan_time>=33)
		{
			led_scan_time = 0;
		}

		if(led_scan_time<16)
		{
			LED_WIFI_ON();
		}
	}
	else
	{
		if(wifi_connected_success_f)
		{
			LED_WIFI_ON();
		}
		else
		{
			led_scan_time++;
			if(led_scan_time>=150)
			{
				led_scan_time = 0;
			}											

			if(led_scan_time<75)
			{
				LED_WIFI_ON();
			}
		}
	}
#endif 
	com_data_temp[3] |= _A5|_B5|_CC5|_DD5|_E5|_F5|_G5|_H5;
	com_data_temp[4] |= _A1|_B1|_CC1|_DD1|_E1|_F1|_G1|_H1;
	com_data_temp[5] |= _A2|_B2|_CC2|_DD2|_E2|_F2|_G2|_H2;
	com_data_temp[6] |= _A3|_B3|_CC3|_DD3|_E3|_F3|_G3|_H3;
	com_data_temp[7] |= _A4|_B4|_CC4|_DD4|_E4|_F4|_G4|_H4;

}






























