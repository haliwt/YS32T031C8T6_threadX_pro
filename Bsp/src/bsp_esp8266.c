#include "bsp.h"


 uint8_t dc_connect_net_step	;
 uint8_t link_counter_times;
 /**
 *
 *@brief 
 *@param  
 *
 **/
static void auto_connect_wifi_handler(void);
static void smartphone_timer_power_on_handler(void);

static void Update_Dht11_Totencent_Value(void);


/**********************************************************************
	*
	*Functin Name: void wifi_auto_detected_link_state(void)
	*Function :
	*Input Ref:  NO
	*Return Ref: NO
	*
**********************************************************************/
void wifi_auto_detected_link_state(void)
{
	if(wifi_connected_success_f ==0){
		
       auto_connect_wifi_handler();//InitWifiModule();
	
	 
    }
	
    if(wifi_connected_success_f==1 && link_counter_times ==0 ){
              
           link_counter_times ++ ;
		   link_counter_times =10;
           wifi_linking_tencent_f =0;//wifi_t.linking_tencent_cloud_doing = 0;
           time_link_net_counter =0;
        
          if(discharge_f){
		     MqttData_Publish_PowerOff_Ref();
               delay_ms(200);//HAL_Delay(200);

          }
          
          Subscriber_Data_FromCloud_Handler();
          delay_ms(200);//HAL_Delay(200);
         

          SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
          delay_ms(100);
		  time_link_net_counter =0;
   }
   
   if(time_link_net_counter > 4 && link_counter_times ==1){
	  time_link_net_counter =0;

      link_counter_times =5;
      if(wifi_connected_success_f==0){
         SendData_Set_Command(0x1F,0);//SendWifiData_To_Data(0x1F,0x0); //WT.EDIT 2025.04.02 0x1F: wifi link net is succes 
         delay_ms(100);

	  }
	  else{
	     SendData_Set_Command(0x1F,1);//SendWifiData_To_Data(0x1F,0x0); //WT.EDIT 2025.04.02 0x1F: wifi link net is succes 
		 delay_ms(100);


	  }

   }
   
   
}
/**********************************************************************
	**
	*Function Name
	*Function: 
	*Input Ref: 
	*Return Ref:NO
	*
************************************************************************/
static void auto_connect_wifi_handler(void)
{

  switch(dc_connect_net_step){

	case 0:

	    
	    wifi_rx_numbers = 0;
        time_link_net_counter=0;
        wifi_linking_tencent_f =1;//linking_tencent_cloud_doing = 1;
	    wifi_cofig_success_f=0;
		
	   send_usart2_data("AT+RST\r\n", strlen("AT+RST\r\n"));
	   delay_ms(1000);//HAL_Delay(1000);
	   time_link_net_counter=0;

	    dc_connect_net_step=1;

	break;

	case 1:
	 if(time_link_net_counter > 2 ){
	   time_link_net_counter=0; 
	    dc_connect_net_step=2;
	   
	  }
	break;

	case 2:

     if(time_link_net_counter >3){
	   
		 wifi_linking_tencent_f = 1;//net_t.linking_tencent_cloud_doing  = 1;
	     wifi_rx_numbers = 0; 
		 //wifi_t.soft_ap_config_flag =0;
		 dc_connect_net_step = 3;
     }

	break;

	case 3:
  
        time_link_net_counter=0;//gpro_t.gTimer_power_on_first_link_tencent=0;
      
	 // HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//??
	       send_usart2_data("AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"));
		   delay_ms(1000);//HAL_Delay(1000);
	       time_link_net_counter=0;
	        
	 dc_connect_net_step=4;
	  
     break;

	 case 4:
   
	   if(wifi_connected_success_f==1 && time_link_net_counter > 1){
				//wifi_t.linking_tencent_cloud_doing =0;

				time_link_net_counter=0;
				wifi_linking_tencent_f =0;
	            dc_connect_net_step =5;
	            SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
	            delay_ms(100);
		}
	    else if(wifi_connected_success_f==0 && time_link_net_counter > 1){
			time_link_net_counter=0;
	       dc_connect_net_step=5;
	        SendWifiData_To_Cmd(0x1F,0x00);
		    delay_ms(100);
	    }

	break;

	default:

	break;

	}
}

/**********************************************************************
	**
	*Function Name : void wifi_default_handler(void)
	*Function: 
	*Input Ref: 
	*Return Ref:NO
	*
************************************************************************/
void wifi_default_handler(void)
{
   static uint8_t sw_flag=0,send_times,sub_times,counter;
	static uint8_t counter_1;

	if(key_net_config_f == 1 || discharge_f == 0) return ;
		
     switch(wifi_run_step){

	 case 0:
	   if(wifi_connected_success_f ==1 && wifi_app_timer_power_on_f ==0){
    
		     if(discharge_f){
		         MqttData_Publish_SetOpen(1);  
				 delay_ms(100);
		     }
			 else{
			    MqttData_Publish_SetOpen(0);  
				delay_ms(100);
			 	Publish_Data_fan_Warning(0); //fan warning .
			    ///delay_ms(200);
			    delay_ms(100);
			


			 }
		
		 }
		 else if(wifi_app_timer_power_on_f ==1){
		     	smartphone_timer_power_on_handler();
				Publish_Data_fan_Warning(2);//fan warning 
			   delay_ms(100);
			 	
		 }

		 wifi_run_step = 1;
	 break;

	 case 1:
	 	 if(wifi_connected_success_f ==1 &&  wifi_app_timer_power_on_f ==0){
            if(discharge_f){
			  	MqttData_Publish_Init();
			    delay_ms(100);
            }
			else{
				
                MqttData_Publish_PowerOff_Ref(); 
				delay_ms(100);

			}
          }
		  else if(wifi_app_timer_power_on_f == 1){

           	fan_speed_level = 100;//gctl_t.set_wind_speed_value=100;
            MqttData_Publis_SetFan(fan_speed_level);//WT.EDIT 2025.12.19
            delay_ms(100);
            //gctl_t.set_temperature_value=40;
            MqttData_Publis_SetTemp(temperature);
			delay_ms(100);

	      }

	      counter_1 =6;
	      wifi_run_step = 2;

	 break;


	 case 2: 

	   counter_1 ++;

        if(wifi_connected_success_f ==1 &&  wifi_app_timer_power_on_f ==0 &&  counter_1 > 3){
      
	     counter_1 =0;
		   Subscriber_Data_FromCloud_Handler();
    	   delay_ms(100);
	    
		}
    
      wifi_run_step = 3;
     break; 

	 case 3:
		counter++;
         if(wifi_connected_success_f ==1 &&  wifi_app_timer_power_on_f ==0 && wifi_first_connectoed_cloud_f==0){
          
	      if(discharge_f){
		         MqttData_Publish_SetOpen(1);  
				 delay_ms(100);
		     }
			 else{
			    MqttData_Publish_SetOpen(0);  
				delay_ms(100);
			 	Publish_Data_fan_Warning(0); //fan warning .
			    delay_ms(100);
			


			 }
			 wifi_first_connectoed_cloud_f++;
         }
			 
		if(wifi_connected_success_f ==1  && soft_version == 0){ //WT.EDIT 2026.02.27

			sw_flag = sw_flag ^ 0x01;
			if(sw_flag == 1){
				SendWifiData_olderCmd(0x1F,0x01);//SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
				delay_ms(100);
			}
			else{
				SendWifiData_To_Data(0x1F,0x01);
				delay_ms(100);
			}

		}
		else if(wifi_connected_success_f ==0 && counter > 1 && soft_version ==0){ //WT.EDIT 2026.02.27
			counter =0;
			sw_flag = sw_flag ^ 0x01;
			if(sw_flag == 1){
				SendWifiData_olderCmd(0x1F,0x0);//SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
				delay_ms(100);
			}
			else{
				SendWifiData_To_Data(0x1F,0x0);
				delay_ms(100);
			}
		}

	 wifi_run_step = 4;

	 break;

	 case 4://3mm run once 

	   sub_times++;

	 if(key_net_config_f ==0 && wifi_connected_success_f ==1 && wifi_app_timer_power_on_f ==0 && wifi_first_connectoed_cloud_f ==1){
    

		   SendData_Set_Command(0x1F,0x01);//SendWifiData_To_Data(0x1F,0x01);
           delay_ms(100);
	    }

	    wifi_run_step =5;

	 break;



	  case 5:
	  	  send_times ++ ;
	  	  if(wifi_connected_success_f ==1 &&  send_times > 4){
		  	    send_times=0;
			
				Update_Dht11_Totencent_Value();
               // delay_ms(200);
        }

	    wifi_run_step =2 ;

	 break;


     	}

}
/**
*@breif :
*@note:
*@param:
*@return:
*
*/
static void Update_Dht11_Totencent_Value(void)
{

  
	//Dht11_Read_TempHumidity_Handler(&DHT11);
	
	// dht11_read_data(&gctl_t.gDht11_temperature, &gctl_t.gDht11_humidity);


	MqttData_Publis_ReadTempHum(temperature,humidity);
    //delay_ms(200);//HAL_Delay(100);

}



/************************************************************************************
*
*Function Name:
*
*
*
************************************************************************************/
static void smartphone_timer_power_on_handler(void)
{

	Parse_Json_Statement();

	if(plasma_open_f==1){ //Anion


	SendWifiData_To_Cmd(0x03,0x01);
	delay_ms(100);

	}
	else{
	plasma_open_f =0;
	SendWifiData_To_Cmd(0x03,0x0);
	delay_ms(100);
	}


	if(Ultra_Sound_open_f==1){

	SendWifiData_To_Cmd(0x04,0x01);
	delay_ms(100);
	}
	else {
	Ultra_Sound_open_f=0;
	SendWifiData_To_Cmd(0x04,0x0);
	delay_ms(100);
	}



	if(PTC_heat_open_f==1){

	SendWifiData_To_Cmd(0x02,0x01);
	delay_ms(100);
	}
	else if(PTC_heat_open_f  ==0){
		ptc_prohibit_off_f =1;
		RELAY_OFF();
		SendWifiData_To_Cmd(0x02,0x0);
		delay_ms(100);

	}

	fan_speed_level =100;

	MqttData_Publish_Update_Data();
	delay_ms(200);

}
			







