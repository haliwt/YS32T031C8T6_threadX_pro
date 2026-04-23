/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    user.C
  * @author  Yspring Firmware Team  
  * @brief   user Source Code.
  ******************************************************************************      
*/

#include "bsp.h"

power_state gon_t;



volatile uint8_t Times5msCnt;
uint8_t Times10msCnt;
//uint8_t Times100msCnt;
uint8_t Times1minute;
uint16_t Times1minCnt;
uint8_t Cacl_time_sec;

volatile uint8_t time_5ms_f;


uint8_t time_wifi_10ms_f;





uint16_t ad_value[1];
uint16_t fan_current;


uint8_t discharge_f;


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
uint8_t time_beep_counter;



uint8_t Is_time_setting_f;

uint8_t Is_countdown_timer_f;
uint8_t set_temperature_value_f;
uint8_t time_1s_counter;

//display second board
uint8_t disp_second_f;


uint16_t led_scan_time;


uint8_t key_net_config_f;
uint16_t key_net_config_time;
uint8_t led_strip_open_f;

uint8_t flash_f;


uint16_t device_rest_time;

//countdown timer 
int8_t timing_min_cnt;
int8_t setting_timing_second;
uint8_t real_hours_counter;
int8_t temporary_timer_hours;
int8_t setting_timing_hour;

//end
//timer 
uint8_t  time_set_hours_counter;

//wroks time two hours

uint8_t  works_interval_f;



uint8_t fan_open_f;
uint8_t fan_speed_level;


volatile uint8_t beep_times;				//����
volatile uint8_t beep_lenght;			  //��ĳ��� *100ms
volatile uint8_t non_beep_length;		//���ʱ��
uint16_t beep_interval_time;

//temp ref
uint8_t temperature;
uint8_t humidity;




//peripheral ref


uint16_t fan_current_det_time;
uint8_t no_fan_load_f;

uint8_t disp_switch_temp_humi;
//
uint8_t soft_version;

//wifi 

uint8_t  link_net_step;
uint8_t  time_link_net_counter;
uint8_t  wifi_linking_tencent_f;
uint8_t  wifi_connected_success_f;
volatile uint8_t  wifi_rx_numbers;
uint8_t  wifi_cofig_success_f;
uint8_t  wifi_app_timer_power_on_f;
uint8_t  wifi_run_step ;
uint8_t  wifi_off_step;

uint8_t  wifi_first_connectoed_cloud_f;
uint8_t  wifi_read_net_data_f;

uint8_t  time_autolink_counter;
uint8_t  wifi_check_net_f;
uint8_t dc_connect_net_step	;

volatile uint8_t  rx_wifi_data_success;
volatile uint8_t   rx_wifi_data_counter;
uint8_t  mqtt_status;


//fan
uint8_t  fan_one_minute_cuonter;





uint8_t key_be_pressed_f;
uint8_t disp_set_hours_time_f;
uint8_t key_input_temp_f;






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
    0,                                    //����
};



uint8_t counter;
uint8_t power_Led_switch;	

volatile uint16_t i;
volatile uint16_t bw_i=0;
volatile uint16_t sw_i=0;
volatile uint16_t gw_i=0;
volatile uint16_t disp_timing_time_temp;
volatile uint16_t timing_diff_value_hour;
volatile uint16_t timing_diff_value_min;


volatile uint8_t static beep_sound_f =0;
static void power_on_handler(void);
static void power_off_handler(void);



/**
  * @brief  fan run is error
  * @note  
  * @param: 
  *
**/
void Clear_Ram(void)
{
    time_5ms_f = 0;
	
	  gpro_t.time_100ms_f = 0;
	  gpro_t.time_300ms_f =0;
	  gpro_t.time_400ms_f =0;
	  gpro_t.time_500ms_f =0;
	  gpro_t.time_1s_f = 0;
	  gpro_t.time_1m_f=0;
	
	 // Times5msCnt = 0;
	  Times10msCnt = 0;
	 // Times100msCnt = 0;
	  Times1minute = 0;
	  Times1minCnt = 0;
	  Cacl_time_sec = 0;
	
	
	
	  key_worked_f = 0;
	  key_long_f = 0;
	  key_data = 0;
	  key_time = 0;
	
	  discharge_f = 0;
		
		
		device_rest_time = 0;
		
		fan_speed_level = 100;
		fan_open_f = 0;
	
		
		AI_timing_open_f = 0;
		PTC_heat_open_f = 0;
		first_temp_compare_f=0;
		Ultra_Sound_open_f = 0;
		plasma_open_f = 0;
		led_strip_open_f = 0;
		
		timing_is_reach_disptime = 0;
		
		Is_time_setting_f = 0;
	
		Is_countdown_timer_f = 0;
		
	
		flash_f = 0;
		led_scan_time = 0;
		
		timing_min_cnt = 0;
		
		no_fan_load_f = 0;
		fan_current_det_time = 0;
		
		disp_switch_temp_humi = 0;
		beep_interval_time = 0;
		//wifi 
		wifi_linking_tencent_f=0;
		
		
	
		
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




/**
  * @brief  fan run is error
  * @note  
  * @param: 
  *
**/

//ADCͨ������
void Adc_Channel_Sample(void)
{
    volatile uint16_t ad_temp;
	
    ad_temp = ADC_GetValue(_FCUR_CH,VREFBUF_ADC_VCC);
	
    ad_value[_AD_FCUR] = ad_temp;
}



/**
  * @brief  fan run is error
  * @note  
  * @param: 
  *
**/

//AD����һ�׻����˲�
void AD_Filter(void)
{
    //FAN_CURRENT
	  fan_current=(ad_value[_AD_FCUR]*2+fan_current*18)/20;
}


/**
  * @brief  fan run is ok
  * @note  
  *
  *
**/

/************************************************************************
 * Function Name: LED_Power_Breathing(void)
 * 功能:
 * 参数:无
 * 返回值:无
 ************************************************************************/
static void power_on_handler(void)
{
   static uint8_t counter_10s=0;
   
   switch(gon_t.on_step){

   case 0:
   	  gon_t.off_step = 0;
      if(wifi_app_timer_power_on_f==0){

	     LED_AI_ON();
		 LED_PTC_ON();
		 LED_PLASMA_ON();
		 LED_MOUSE_ON();
		 LED_WIFI_ON();
		 LED_POWER_ON();
		 LED_TAPE_ON();
		 LED_TEMP_ON();
		 LED_HUMI_ON(); 


	  }
	  else{
		  LED_AI_ON();
		  LED_WIFI_ON();
		 LED_POWER_ON();
		 LED_TAPE_ON();
		 LED_TEMP_ON();
		 LED_HUMI_ON(); 


	  }
	  dht11_read_temp_humidity_value();
	  display_digital_3_numbers();
      gon_t.on_step =1;
	   #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 

   break;

   case 1:
    dht11_read_temp_humidity_value();
    display_digital_3_numbers();
    gon_t.on_step =2;
	 #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 

   break;

   case 2:
   	 
       dht11_read_temp_humidity_value();
	   display_digital_3_numbers();
	   gon_t.on_step =3;
	    #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 

   break;



   case 3:
        if(gpro_t.time_3s_f > 2){
		  gpro_t.time_3s_f =0;	
		  Fan_Ctrl_Process();	  // 风扇控制

        }
        gon_t.on_step =4;
       #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 
   break;

   case 4:

      if(wifi_connected_success_f==1 && gpro_t.time_4s_f > 1){
	  	   gpro_t.time_4s_f=0;
		   wifi_default_handler();
         }
        gon_t.on_step =5;
	   #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 
   break;

   case 5:
   	if(key_net_config_f)
	{
		key_net_config_time++;
		if(key_net_config_time>=130)
		{
			key_net_config_time = 0;

			key_net_config_f = 0;
			
		}
		else{ //conneting to wifi net 
	
			link_wifi_net_handler();
		}
	} 
      gon_t.on_step =6;
	 #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 

   break;


   case 6:
   	   if(gpro_t.time_5s_f > 1){
	   	  gpro_t.time_5s_f=0;
          Heat_Process(); 
	      peripheral_fun_handler();

   	   }
       gon_t.on_step =7;
	    #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 

   break;

   case 7:
   	if(gpro_t.time_6s_f > 3){
		gpro_t.time_6s_f =0;
      	dht11_read_temp_humidity_value();
   	}
   gon_t.on_step =8;
	 #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 

   break;

   case 8:
	   if(Is_countdown_timer_f ==1){
             Countdown_timer_Handler();
	   	}
      gon_t.on_step =9;
	    #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 

   break;


   case 9:

        wifi_check_id_handler();
        
	    works_nomal_run_time_handler();
		
	    gon_t.on_step =10;
    #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 

   break;

   case 10:
    if(gpro_t.time_7s_f > 6){

	   gpro_t.time_7s_f =0 ;
	   counter_10s++;
	   
       AD_Filter();
	   Adc_Channel_Sample();
    }
	gon_t.on_step =11;

	break; 

	case 11:

	   if(counter_10s > 1){
	   	   counter_10s =0;
	       Fan_Current_Det();		// 电流检测
		}
    
      gon_t.on_step =12;
	 #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 

   break;

   case 12:
     if(key_net_config_f==0 &&  discharge_f ==1 &&  wifi_linking_tencent_f ==0){
	   	   if(gpro_t.time_1m_wifi_f ==1){ 
		     	gpro_t.time_1m_wifi_f =0;
		       Reconnection_Wifi_Order();

	   	   	}
		
	}
     gon_t.on_step =3;

	  #if DEBUG_ENABLE
         printf("on_step = %d  \r\n",gon_t.on_step);

	   #endif 

   break;


   

   default :

   break;

    }
 
  
}
/************************************************************************
 *
 * Function Name: LED_Power_Breathing(void)
 * 功能:
 * 参数:无
 * 返回值:无
 *
 ************************************************************************/
static void power_off_handler(void)
{
   static uint8_t dc_on=0,fan_one_f=0;
	switch(gon_t.off_step){
	
		 case 0:
			gon_t.on_step =0;
	       
		    fan_one_f =1;
			time_1s_counter=0;
			fan_one_minute_cuonter =0;
			wifi_run_step = 0;
			wifi_off_step =0;
			power_off_peripheral_handler();
			all_led_off();
	        TM1639_Display_ON_OFF(0);
			gon_t.off_step = 1;
	
		 break;
	
		 case 1:

		    if(setting_timing_second> 1){//
		       setting_timing_second =0;
		        LED_POWER_TOGGLE();
		      }
             if(dc_on ==0){
			    gon_t.off_step = 2;

			 }
			 else{
				 if(time_1s_counter > 1){
				 	time_1s_counter =0;
				    dht11_read_temp_humidity_value();
				 }

				 if(fan_one_f == 1  && fan_one_minute_cuonter>59){
				     fan_one_f ++;
	                 FAN_RUN_OFF();

				 }
			 }
 
		break;

		 case 2 :

          if(setting_timing_second> 1){//
		       setting_timing_second =0;
		        LED_POWER_TOGGLE();
		    }
		  gon_t.off_step = 2;
		break;

   }
}

/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void Countdown_timer_Handler(void)
{
   static int8_t dsip_timer_value ;
 
   if(setting_timing_second >=60) //60s
    {
	   setting_timing_second=0;

	   #if DEBUG_ENABLE

		timing_min_cnt = timing_min_cnt - 40;
	   #else 
		 timing_min_cnt --;

	   #endif 

        if(timing_min_cnt <  0)
        {
           timing_min_cnt =59;
		   real_hours_counter++;
		   if((setting_timing_hour > 1) && setting_timing_hour !=1){
		   	
		          dsip_timer_value = temporary_timer_hours - real_hours_counter +1;
				  setting_timing_hour = dsip_timer_value;


		   }

		   if(setting_timing_hour ==1 || setting_timing_hour==0){
               
				   setting_timing_hour--;
				   
            }

        }

        if (setting_timing_hour < 0)
        {
             discharge_f = 0;
			 System_Status_PowerOff() ;

        }
    }
}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void works_nomal_run_time_handler(void)
{
 
		#if DEBUG_ENABLE 
			if(gpro_t.time_1m_f >11 && works_interval_f==0){
		#else 
			if(gpro_t.time_1m_f > 119 && works_interval_f==0){

		#endif 

			gpro_t.time_1m_f = 0;
		    gpro_t.time_base_1s_counter=0;
			works_interval_f=1;
			fan_one_minute_cuonter =0;
		#if DEBUG_ENABLE 
			printf("works_interval_f = %d \n\r",works_interval_f);
		#endif 
		}
		else if(works_interval_f==1 && gpro_t.time_1m_f >10){
				gpro_t.time_1m_f = 0;  
				works_interval_f =0;
		        gpro_t.time_base_1s_counter=0;
		#if DEBUG_ENABLE 
			printf("works_interval_f = %d \n\r",works_interval_f);
		#endif 
		}
		
 }
  

/**
  * @brief  // 按键按下时调用
  * @note  
  * @param: 
  *
**/
void beep_power_sound(void)
{
  
	BEEP_ON();
	delay_ms_dht11(20);//tx_thread_sleep(2);//2*10ms //delay_ms_dht11(20);//DelayMS(20);
    BEEP_OFF();

}

/**
  * @brief  // 按键按下时调用
  * @note  
  * @param: 
  *
**/
void Trigger_Simple_Beep(uint8_t ms_10) 
{
    //time_beep_counter = 0;
	//beep_sound_f = 1;
	BEEP_ON();
	tx_thread_sleep(2);//DelayMS(20);
    BEEP_OFF();
   // BEEP_ON();//BEEP_PWM_ON(); // 立即响
}
/**
  * @brief 
  * @note  // 10ms 任务
  * @param: 
  *
**/
void Task_Beep_Simple_10ms(void) 
{

  if (time_beep_counter > 0 && beep_sound_f == 1) {
		 beep_sound_f ++;

	     BEEP_OFF();
//        time_beep_counter--;
//        if (time_beep_counter == 0) {
//           BEEP_OFF(); //BEEP_PWM_OFF(); // 时间到，关掉
//        }
  }
  else if(beep_sound_f == 1){

	    BEEP_ON();

 }

//  if( beep_sound_f ==2){

//	   BEEP_OFF();
//	    beep_sound_f ++;

//  }
}
/**
	*
	*@brief environment temperature value compare set temperater value
	*@notice
	*@param
	*
**/
void Heat_Process(void)
{
      static uint8_t default_init = 0xff;   // 第一次比较标志
     if(discharge_f == 1){
	   if(ptc_prohibit_off_f == 1 || set_temperature_value_f ==1 ) return ;

	  uint8_t target_temp;

	  target_temp = setting_temperature;

	  if(temperature > 39){

        PTC_heat_open_f = 0;   // 立即关闭
	    first_temp_compare_f = 1; 
	    if(default_init != PTC_heat_open_f || key_input_temp_f ==1 || key_input_temp_f==2 ){
					default_init= PTC_heat_open_f;
					key_input_temp_f++;
				if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0);
		        //delay_ms(100);//HAL_Delay(5);
		        if(wifi_connected_success_f == 1)MqttData_Publish_SetPtc(0);

				}
	  
	     return ;

	  }

      // -----------------------------
    // 2. 第一次比较：必须立即决定 PTC 开关
    // -----------------------------
	  if(first_temp_compare_f == 0){

		if(temperature >= target_temp){
            PTC_heat_open_f = 0;   // 立即关闭

		       if(default_init != PTC_heat_open_f  || key_input_temp_f ==1 || key_input_temp_f==2 ){
					default_init = PTC_heat_open_f;
					key_input_temp_f ++;
				if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0);
		        //delay_ms(100);//HAL_Delay(5);
		        if(wifi_connected_success_f == 1)MqttData_Publish_SetPtc(0);

				}
		}
        else{
            PTC_heat_open_f = 1;   // 立即打开
            first_temp_compare_f = 1;         // 以后进入滞后控制
            if(default_init!= PTC_heat_open_f || key_input_temp_f ==1 || key_input_temp_f==2 ){
					default_init = PTC_heat_open_f;
					key_input_temp_f++;
				if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0x01);
		        //delay_ms(100);//HAL_Delay(5);
		        if(wifi_connected_success_f == 1)MqttData_Publish_SetPtc(0x01);

			}
        }
        return;


	  }

		// -----------------------------
		// 3. 第二次及以后：使用 -2°C 滞后控制
		// -----------------------------
		if(first_temp_compare_f == 1)
		{
			// 当前是开启状态 → 高于设定温度则关闭
			if(temperature >= target_temp){
					PTC_heat_open_f = 0;
				if(default_init != PTC_heat_open_f  || key_input_temp_f ==1 || key_input_temp_f==2 ){
					default_init = PTC_heat_open_f;
					key_input_temp_f++;
				if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0);
		       // delay_ms(100);//HAL_Delay(5);
		        if(wifi_connected_success_f == 1)MqttData_Publish_SetPtc(0);

				}
			}
			else
			{
				// 当前是关闭状态 → 低于设定温度 - 2 才重新打开
				if(temperature <  (target_temp - 2))
				PTC_heat_open_f = 1;
				if(default_init!= PTC_heat_open_f || key_input_temp_f ==1 || key_input_temp_f==2 ){
					default_init = PTC_heat_open_f;
					key_input_temp_f++;
				if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0x01);
		       // delay_ms(100);//HAL_Delay(5);
		        if(wifi_connected_success_f == 1)MqttData_Publish_SetPtc(0x01);

				}
			}
		}

       }

}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/

void power_onoff_handler(void)
{
     switch(discharge_f){

      case 1:
           power_on_handler();
	  break;

	  case 0:
	  	   power_off_handler();

	  break;
      }

	if(discharge_f == 1){

       display_digital_3_numbers();
	   set_temp_compare();
	   wifi_normal_led_state();
	 
	  
	}

	if(key_net_config_f==0 && gpro_t.time_100ms_f > 0){// 处理腾讯连连通信
	      gpro_t.time_100ms_f=0;
         wifi_parse_tencennt_hadler();//
       
    }

	
    wifi_fast_led_state();
}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/

/**
  * @brief  
  * @note  
  * @param: 
  *
**/


