#include "bsp.h"

uint8_t TCMQTTRCVPUB[40];

uint8_t rx_app_timer_power_on_flag;

#if 0
typedef enum _CLOUD_STATE{
   OPEN_OFF_ITEM=0x01,
   OPEN_ON_ITEM=2,
   PTC_OFF_ITEM=3,
   PTC_ON_ITEM=4,
   ANION_OFF_ITEM=5,
   ANION_ON_ITEM=6,
   SONIC_OFF_ITEM=7,
   SONIC_ON_ITEM=8,
   STATE_AI_MODEL_ITEM=9,
   STATE_TIMER_MODEL_ITEM=10,
   TEMPERATURE_ITEM=11,
   FAN_ITEM=12,
   APP_TIMER_POWER_ON_REF=13,
   APP_TIMER_POWER_ON_REF_TWO,
}cloud_state;
#else

typedef enum
{
    WIFI_EVT_NONE = 0,
    OPEN_ON_ITEM,
    OPEN_OFF_ITEM,
    PTC_ON_ITEM,
    PTC_OFF_ITEM,
    ANION_ON_ITEM,
    ANION_OFF_ITEM,
    SONIC_ON_ITEM,
    SONIC_OFF_ITEM,
    STATE_TIMER_MODEL_ITEM,
    STATE_AI_MODEL_ITEM,
    TEMPERATURE_ITEM=11,
    FAN_ITEM=12,
    WIFI_EVT_MAX
} wifi_event_t;

#endif 



uint8_t rx_wifi_counter;


typedef struct PROCESS_T{

   uint8_t  rx_data_array[256];//150
   uint8_t  rx_inputBuf[1];
 
   uint8_t  rx_check_wifi[100];
   uint8_t  once_rx_data_done;
   uint8_t  rx_data_state;
  // uint16_t rx_counter;
   uint16_t rx_recoder_counter;
   uint8_t  received_data_from_tencent_cloud ;
 //  uint8_t  rx_data_success;
   uint8_t  wifi_rx_signal_f;
   
 }process_t;

process_t wifi_t;



 uint8_t set_temperature_value,power_on_f;
 static void Wifi_Event_Process(void);




 /********************************************************************************
	*
	*Functin Name:void wifi_parse_tencennt_hadler(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
void wifi_parse_tencennt_hadler(void)
{

  //Tencent_Cloud_Rx_Handler();
   Parse_Tencent_Data() ;	
   Wifi_Event_Process();
		 
  
   
}


/*******************************************************************************
   **
   *Function Name:void usart2_rx_callback_invoke(uint8_t data)
   *Function: receive data 
   *Input Ref: module , str ->data
   *Return Ref:NO
   *
********************************************************************************/
void usart2_rx_callback_invoke(uint8_t data)
{
  
   switch(wifi_linking_tencent_f){

	 case 1:
             wifi_t.rx_inputBuf[0] =data;

			wifi_t.rx_data_array[wifi_rx_numbers] =wifi_t.rx_inputBuf[0];
			wifi_rx_numbers++;//wifi_t.rx_numbers++;

			if(*wifi_t.rx_inputBuf==0x0A) // 0x0A = "\n"
			{
				wifi_read_net_data_f = 1;
			    wifi_rx_numbers=0;//wifi_t.rx_numbers=0;
				
			}

	  
	 break;

	 case 0:
	
	 	  wifi_t.rx_inputBuf[0] =data;
	 	
		  if(wifi_check_net_f ==1){

		       wifi_t.rx_check_wifi[rx_wifi_data_counter] =wifi_t.rx_inputBuf[0];
		     rx_wifi_data_counter++;
                  
				if(rx_wifi_data_counter  > 20  && *wifi_t.rx_inputBuf==0x0A && wifi_check_net_f ==1){
                    
             
				    wifi_check_net_f =2;
				    rx_wifi_data_counter =0;

				}		

				
          }
		  else if(rx_wifi_data_success==0){
			  wifi_t.rx_data_array[rx_wifi_data_counter] =wifi_t.rx_inputBuf[0];
		      rx_wifi_data_counter++;
			  if(*wifi_t.rx_inputBuf==0x0A &&  rx_wifi_data_success==0 && rx_wifi_data_counter > 80) // 0x0A = "\n"
			  {
	             rx_wifi_data_success=1;
				 wifi_t.rx_recoder_counter=rx_wifi_data_counter;
				 rx_wifi_data_counter=0;
				 
				
			   }
	 	  }
       #if 1
		  if(rx_wifi_data_success > 2){
			rx_wifi_data_success=0;
			 rx_wifi_data_counter=0;
		   
		  }
		  if(wifi_t.received_data_from_tencent_cloud > 100)wifi_t.received_data_from_tencent_cloud=0;
		 #endif 
	 
	 break;
	 }
}

/*******************************************************************************
**
*Function Name:
*Function: 
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Parse_Tencent_Data(void) 
{
   
	char *p =  NULL;
	char *p1 = NULL;
	
	 if(rx_wifi_data_success==1){
	   rx_wifi_data_success=0;
      

		
	    //wifi_app_timer_power_on_f= 1;
	    // 寻找 "open": 之后的值
	    if(discharge_f == 0){
		     // 寻找 "ptc": 之后的值
		

            if(strstr((const char *)wifi_t.rx_data_array, "\"sonic\":1"))
		    {
		        Ultra_Sound_open_f =1;
				wifi_app_timer_power_on_f= 1;
		    }
			else if(strstr((const char *)wifi_t.rx_data_array, "\"sonic\":0")){

			     Ultra_Sound_open_f= 0;
				 wifi_app_timer_power_on_f= 1;

			}
		    // 寻找 "Anion": 之后的值
		    if(strstr((const char *)wifi_t.rx_data_array, "\"Anion\":1")){
                   plasma_open_f =1;
				   wifi_app_timer_power_on_f= 1;

			}
	        else if(strstr((const char *)wifi_t.rx_data_array, "\"Anion\":0")){
		          plasma_open_f =0;
				   wifi_app_timer_power_on_f= 1;
	        }

		  #if 0
		    char *p2 = strstr((char *)wifi_t.rx_data_array, "\"ptc\":");
			if (p2)
			{
               #if 0
				p2 = strchr(p2, ':');   // 找到真正的冒号
				if (p2)
				{
					PTC_heat_open_f = atoi(p2 + 1);	 // 冒号后面一定是数字
					set_temperature_value_f =0;
				     wifi_app_timer_power_on_f= 1;
				}
				#else 
					 // 速度比 strchr 快，兼容性比固定字符串好
				    PTC_heat_open_f = atoi(p2 + 6); 
				    //ptc_prohibit_off_f = (PTC_heat_open_f == 0) ? 1 : 0; // 自动处理联动逻辑
				    set_temperature_value_f =0;
				    wifi_app_timer_power_on_f = 1;

				#endif 
				 
			}
          #else  
		    if(strstr((const char *)wifi_t.rx_data_array, "\"ptc\":0")){
				PTC_heat_open_f =0; 
				ptc_prohibit_off_f = 1;
		        wifi_app_timer_power_on_f= 1;
			}
			else if(strstr((const char *)wifi_t.rx_data_array, "\"ptc\":1")){
				PTC_heat_open_f =1; 
				ptc_prohibit_off_f = 0;
			    wifi_app_timer_power_on_f= 1;

			}
			#endif 
		
	    }
	

	
   
	
	 if(strstr((const char *)wifi_t.rx_data_array,"\"open\":0")){
	 	
		   wifi_t.rx_data_array[0]='\0';
		   memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);
		   wifi_t.wifi_rx_signal_f= OPEN_OFF_ITEM;
	       wifi_app_timer_power_on_f= 0;

		   return ;
	 }
	 else if(strstr((const char *)wifi_t.rx_data_array,"\"open\":1")){
		
        

		 memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);
		wifi_t.wifi_rx_signal_f= OPEN_ON_ITEM;
	


	     return ;
	   
	 }
     else if(strstr((const char *)wifi_t.rx_data_array,"\"ptc\":0") && discharge_f == 1){
                
			
			 memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);
	            wifi_t.wifi_rx_signal_f= PTC_OFF_ITEM;
			   
               
				return;
    }
    else if(strstr((const char *)wifi_t.rx_data_array,"\"ptc\":1") && discharge_f == 1){
        
		
	    memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);    
		wifi_t.wifi_rx_signal_f= PTC_ON_ITEM;
		
	 

		return ;
	}
	else if(strstr((const char *)wifi_t.rx_data_array,"\"Anion\":0") && discharge_f == 1){
        
	        plasma_open_f =0; //  esp_t.gPlasma=0;
	        
			wifi_t.rx_data_array[0]='\0';
	        memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);
			wifi_t.wifi_rx_signal_f= ANION_OFF_ITEM;
	
			

		     return ;
		  
    }
    else if(strstr((const char *)wifi_t.rx_data_array,"\"Anion\":1") &&  discharge_f == 1){
     
            plasma_open_f =1;//esp_t.gPlasma=1;
            
			wifi_t.rx_data_array[0]='\0';
	        memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);
			wifi_t.wifi_rx_signal_f= ANION_ON_ITEM;
			
	
		     return ;
            
    }
	else if(strstr((const char *)wifi_t.rx_data_array,"\"sonic\":0") && discharge_f == 1){  // {//if(strstr((char *)wifi_t.rx_data_array,"sonic\":0")){
       
            Ultra_Sound_open_f =0;// esp_t.gUlransonic=0;
            
			wifi_t.rx_data_array[0]='\0';

			 memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);
			wifi_t.wifi_rx_signal_f= SONIC_OFF_ITEM;
			

        return ;
                
            
		
    }
    else if(strstr((const char *)wifi_t.rx_data_array,"\"sonic\":1") && discharge_f == 1){//else if(strstr((char *)wifi_t.rx_data_array,"sonic\":1")){ 
       
            Ultra_Sound_open_f = 1;//esp_t.gUlransonic=1;
            
			wifi_t.rx_data_array[0]='\0';
	        memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);
			wifi_t.wifi_rx_signal_f= SONIC_ON_ITEM;
	
         

		return ;
       
           
			
    }
	else if(strstr((const char *)wifi_t.rx_data_array,"\"state\":1") && discharge_f == 1){
          
            AI_timing_open_f = 1;//esp_t.gModel=1;
            
			wifi_t.rx_data_array[0]='\0';
	        memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);
			wifi_t.wifi_rx_signal_f= STATE_AI_MODEL_ITEM;

		
			return ;

        	
		  
    }
    else if(strstr((const char *)wifi_t.rx_data_array,"\"state\":2") && discharge_f == 1){
        
            AI_timing_open_f = 0; //esp_t.gModel=2;
            
			wifi_t.rx_data_array[0]='\0';
	        memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);
			wifi_t.wifi_rx_signal_f= STATE_TIMER_MODEL_ITEM;
		   
			 return ;
            
			
    }

	
	 if((p=strstr((const char *)wifi_t.rx_data_array,"\"find\":"))!=NULL && discharge_f == 1){ //&& discharge_f == 1){


		   fan_speed_level =  atoi(p + 7);
		  wifi_t.rx_data_array[0]='\0';
	        memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);
		   wifi_t.wifi_rx_signal_f= FAN_ITEM;

		
		    return ;

		} 
        else if((p1 = strstr((const char *)wifi_t.rx_data_array, "\"temperature\":")) != NULL && discharge_f == 1){
	 

       setting_temperature =  atoi(p1 + 14);
	
	   memset(wifi_t.rx_data_array, 0, wifi_t.rx_recoder_counter);

	   wifi_t.wifi_rx_signal_f= TEMPERATURE_ITEM;



	   return ;
		}

	  
	}
	 
	
	
}
/*******************************************************************************
**
*Function Name:
*Function: 
*Input Ref:  
*Return Ref:NO
*
********************************************************************************/
static void evt_open_on(void)
{
    if (wifi_connected_success_f == 1)
    {
        discharge_f = 1;
        System_Status_PowerOn();
        MqttData_Publish_SetOpen(1);
		tx_thread_sleep(200);

        if (disp_second_f == 1){
            SendWifiData_To_Cmd(0x20, 0x01);
			tx_thread_sleep(100);
        }

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_open_off(void)
{
    if (wifi_connected_success_f == 1)
    {
  
        discharge_f = 0;
        System_Status_PowerOff();

        if (disp_second_f == 1){
            SendWifiData_To_Cmd(0x20, 0x00);
            tx_thread_sleep(100);
        }

        MqttData_Publish_PowerOff_Ref();
		tx_thread_sleep(200);
       
        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_ptc_on(void)
{
    if (discharge_f == 1)
    {
        Trigger_Simple_Beep(2);
        PTC_heat_open_f = 1;
        ptc_prohibit_off_f = 0;

        LED_PTC_ON();
        RELAY_ON();

        key_input_temp_f = 1;

        if (disp_second_f == 1){
            SendWifiData_To_Cmd(0x02, 0x01);
			tx_thread_sleep(100);
        }

        MqttData_Publish_SetPtc(1);
		tx_thread_sleep(200);

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_ptc_off(void)
{
    if (discharge_f == 1)
    {
        Trigger_Simple_Beep(2);
        PTC_heat_open_f = 0;
        ptc_prohibit_off_f = 1;

        LED_PTC_OFF();
        RELAY_OFF();

        key_input_temp_f = 1;

        if (disp_second_f == 1){
            SendWifiData_To_Cmd(0x02, 0x00);
		   tx_thread_sleep(200);
        }

        MqttData_Publish_SetPtc(0);
		tx_thread_sleep(200);

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_anion_on(void)
{
    if (discharge_f == 1)
    {
        Trigger_Simple_Beep(2);
        plasma_open_f = 1;

        if (disp_second_f == 1){
            SendWifiData_To_Cmd(0x03, 0x01);
			tx_thread_sleep(100);
        }

        MqttData_Publish_SetPlasma(1);
		tx_thread_sleep(200);

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_anion_off(void)
{
    if (discharge_f == 1)
    {
        Trigger_Simple_Beep(2);
        plasma_open_f = 0;

        if (disp_second_f == 1){
            SendWifiData_To_Cmd(0x03, 0x00);

		   tx_thread_sleep(100);
       }

        MqttData_Publish_SetPlasma(0);
		tx_thread_sleep(200);

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_sonic_on(void)
{
    if (discharge_f == 1)
    {
        Trigger_Simple_Beep(2);
        Ultra_Sound_open_f = 1;

        if (disp_second_f == 1){
            SendWifiData_To_Cmd(0x04, 0x01);
			tx_thread_sleep(100);
        }

        MqttData_Publish_SetUltrasonic(1);
		tx_thread_sleep(200);

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_sonic_off(void)
{
    if (discharge_f == 1)
    {
        Trigger_Simple_Beep(2);
        Ultra_Sound_open_f = 0;

        if (disp_second_f == 1){
            SendWifiData_To_Cmd(0x04, 0x00);

		tx_thread_sleep(100);

        	}

        MqttData_Publish_SetUltrasonic(0);
		tx_thread_sleep(200);

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_timer_mode(void)
{
    if (discharge_f == 1)
    {
        Trigger_Simple_Beep(2);
        AI_timing_open_f = 0;

        LED_AI_OFF();
        disp_set_hours_time_f = 1;
        time_set_hours_counter = 0;

        if (disp_second_f == 1){
			tx_thread_sleep(100);
            SendWifiData_To_Cmd(0x27, 0x02);

        }

        MqttData_Publish_AitState(2);
		tx_thread_sleep(200);

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_ai_mode(void)
{
    if (discharge_f == 1)
    {
        Trigger_Simple_Beep(2);
        AI_timing_open_f = 1;

        LED_AI_ON();
        disp_set_hours_time_f = 1;
        time_set_hours_counter = 0;

        if (disp_second_f == 1){
            SendWifiData_To_Cmd(0x27, 0x01);
			tx_thread_sleep(100);
        }

        MqttData_Publish_AitState(1);
		tx_thread_sleep(200);

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_temperature(void)
{
    if (discharge_f == 1)
    {
        Trigger_Simple_Beep(2);

        ptc_prohibit_off_f = 0;
        set_temperature_value_f = 1;
        key_input_temp_f = 4;
        time_1s_counter = 0;

        if (disp_second_f == 1){
            SendWifiData_To_Data(0x2A, setting_temperature);
			tx_thread_sleep(100);
        }

        MqttData_Publis_SetTemp(setting_temperature);
		tx_thread_sleep(200);

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}

static void evt_fan(void)
{
    if (discharge_f == 1)
    {
        Trigger_Simple_Beep(2);
        MqttData_Publis_SetFan(fan_speed_level);
		tx_thread_sleep(200);

        wifi_t.wifi_rx_signal_f = 0xfe;
    }
}
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/

typedef void (*wifi_evt_handler_t)(void);

static const wifi_evt_handler_t wifi_evt_table[WIFI_EVT_MAX] =
{
    NULL,               // WIFI_EVT_NONE
    evt_open_on,        // OPEN_ON_ITEM
    evt_open_off,       // OPEN_OFF_ITEM
    evt_ptc_on,         // PTC_ON_ITEM
    evt_ptc_off,        // PTC_OFF_ITEM
    evt_anion_on,       // ANION_ON_ITEM
    evt_anion_off,      // ANION_OFF_ITEM
    evt_sonic_on,       // SONIC_ON_ITEM
    evt_sonic_off,      // SONIC_OFF_ITEM
    evt_timer_mode,     // STATE_TIMER_MODEL_ITEM
    evt_ai_mode,        // STATE_AI_MODEL_ITEM
    evt_temperature,    // TEMPERATURE_ITEM
    evt_fan             // FAN_ITEM
};

static void Wifi_Event_Process(void)
{
    uint8_t evt = wifi_t.wifi_rx_signal_f;

    if (evt < WIFI_EVT_MAX && wifi_evt_table[evt] != NULL)
    {
        wifi_evt_table[evt]();   // 调用事件处理函数
    }

    // 清除事件
    wifi_t.wifi_rx_signal_f = 0xf0;
}



/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_InputInfo_Handler(void)
{
    
  if(strstr((const char*)wifi_t.rx_data_array,"+TCSAP:WIFI_CONNECT_SUCCESS")){
    wifi_cofig_success_f=1;

  }

  if(strstr((const char*)wifi_t.rx_data_array,"+TCMQTTCONN:OK")){
    wifi_connected_success_f=1 ;//wifi_connected_success_f=1;
    wifi_linking_tencent_f =0;// wifi_t.linking_tencent_cloud_doing=0;
   
  }
  else if(strstr((char*)wifi_t.rx_data_array,"+CME ERROR:208")){

    wifi_linking_tencent_f =0;//wifi_t.linking_tencent_cloud_doing=0; //release this flag. usart

    wifi_connected_success_f=0;//wifi_connected_success_f=0;

  }
  else if(strstr((char*)wifi_t.rx_data_array,"+TCPRDINFOSET:1,\"EHQB1P53IH\"")){ //WT.EDIT 2024.07.22

  // wifi_t.serch_for_wifi_flag=1;
  //  wifi_t.gTime_link_time_start =0;

  }
  else  if(strstr((char*)wifi_t.rx_data_array,"+TCMQTTCONN:FAIL,202")){

    //  wifi_t.esp8266_login_cloud_success =0;
    wifi_connected_success_f=0;
    key_net_config_f =0;//key_net_config_f=0;   //WT.EDIT .2024.07.31

    //wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
    wifi_linking_tencent_f = 0;//wifi_t.linking_tencent_cloud_doing=0;
    //wifi_t.get_rx_beijing_time_enable=0;
  }
  else if(strstr((const char*)wifi_t.rx_data_array,"+TCMQTTCONN:OK")){
    wifi_connected_success_f=1;
    wifi_linking_tencent_f =0;//wifi_t.linking_tencent_cloud_doing=0;
   
   
  }
  else if(strstr((char*)wifi_t.rx_data_array,"+CME ERROR:208")){


    wifi_connected_success_f =0;//wifi_connected_success_f=0;//wifi_t.esp8266_login_cloud_success =0;
    wifi_linking_tencent_f =1;//wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
   // wifi_t.get_rx_beijing_time_enable=0;

  }
  else if(strstr((char*)wifi_t.rx_data_array,"+TCMQTTCONN:FAIL,202")){

    wifi_connected_success_f=0;//wifi_connected_success_f=0; //wifi_t.esp8266_login_cloud_success =0;
    key_net_config_f =0;//key_net_config_f=0;   //WT.EDIT .2024.07.31

    wifi_linking_tencent_f = 1 ;//wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart

    
  }
  else if(strstr((char*)wifi_t.rx_data_array,"+TCMQTTDISCON")){

    wifi_connected_success_f=0;//wifi_connected_success_f=0;//wifi_t.esp8266_login_cloud_success =0;
    //wifi_t.get_rx_beijing_time_enable=0;
    wifi_linking_tencent_f = 1;//wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
  }
  else if(strstr((char*)wifi_t.rx_data_array,"+TCSAP:WIFI_CONNECT_FAILED")){

    wifi_connected_success_f=0;//wifi_t.esp8266_login_cloud_success =0;
    //wifi_t.get_rx_beijing_time_enable=0;
    wifi_linking_tencent_f = 1;//wifi_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
    }

  wifi_rx_numbers=0;//wifi_t.rx_numbers=0;
  wifi_t.once_rx_data_done = 1;
 
         
}
/*******************************************************************************
    **
    *Function Name:void Tencent_Cloud_Rx_Handler(void)
    *Function: 
    *Input Ref: +TCMQTTCONN:OK
    *Return Ref:NO
    *
********************************************************************************/





/**
 * @brief  UART2_SendByte
 * @param  Data: 数据
 * @retval None
 */

void send_usart2_data(const uint8_t* pdata,uint8_t length)
{
  uint8_t i;
  for(i = 0; i < length; i++){
    
   while(UART_GetFlagStatus(UART2, UART_FLAG_TXE) == RESET);
   UART_SendData(UART2,pdata[i]); 

   }

 
}

/********************************************************************************
	*
	*Functin Name:void Reconnection_Wifi_Order(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
void Reconnection_Wifi_Order(void)
{
    
    wifi_check_net_f = 1;
	rx_wifi_data_counter =0;
	send_usart2_data((const uint8_t*)"AT+TCMQTTSTATE?\r\n", strlen("AT+TCMQTTSTATE?\r\n"));
    tx_thread_sleep(200);//10ms*20 = 200ms
  
}

void wifi_check_id_handler(void)
{
 
   if(wifi_check_net_f ==2){
	    wifi_check_net_f++;

	 char *p = strstr((const  char *)wifi_t.rx_check_wifi, "+TCMQTTSTATE:");
	  if (p != NULL) {
	
		// 跳过前缀
		p += strlen("+TCMQTTSTATE:");
	
		// 跳过可能的空格
		while (*p == ' ' || *p == '\t') p++;
	
		// 解析数字
		if (*p == '0' || *p == '1') {
			mqtt_status = *p - '0';
		}
	}
	rx_wifi_data_counter=0;
	memset(wifi_t.rx_check_wifi, 0, 50);

	if(mqtt_status==1){
		wifi_connected_success_f =1;
	}
	else if(mqtt_status==0){
		 dc_connect_net_step = 0;
		 wifi_off_step=0;
	     wifi_run_step = 0;
	     wifi_connected_success_f =0;
		 wifi_app_timer_power_on_f = 0;
		
    }
    #if DEBUG_ENABLE

		printf("mqtt_status = %d\n\r",mqtt_status);

	#endif 

    }

	

}
