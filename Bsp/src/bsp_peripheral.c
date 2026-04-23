#include "bsp.h"





//∑Á…»øÿ÷∆
/**
*@brief : in 100ms processing .

**/
void Fan_Ctrl_Process(void)
{
   static uint8_t  fan_stop_f;
	if(discharge_f){
	   if(works_interval_f == 0 ){
	      	
			fan_stop_f = 0;
			fan_one_minute_cuonter=0;;
	     
		if((fan_open_f)){
			if(fan_speed_level < 34)
			{
			fan_on(287);
			}
			else if(fan_speed_level > 33 && fan_speed_level < 67)
			{
			fan_on(303);
			}
			else if(fan_speed_level==100 && fan_speed_level > 66)
			{
			fan_on(319);
			}

			__NOP();__NOP();__NOP();__NOP();__NOP();

			FAN_RUN_ON();
		}
    }
	else if(works_interval_f == 1){
       
		if(fan_one_minute_cuonter < 61  && fan_stop_f ==0){
		      FAN_RUN_ON(); 
	     
		}
		else{
		  fan_stop_f =2;
		  FAN_RUN_OFF(); 
		#if DEBUG_ENABLE 

		printf("fan_stop !!! \n\r");

		#endif 
		  

		}

	}

    }
 }

#if 0
        else
			{
			if(fan_delay_time_off!=0){
				fan_delay_time_off--;

				if(fan_speed_level==1)
				{
				fan_on(287);
				}   
				else if(fan_speed_level==2)
				{
				fan_on(303);
				}
				else if(fan_speed_level==3)
				{
				fan_on(319);
				}

				__NOP();__NOP();__NOP();__NOP();__NOP();

				FAN_RUN_ON();
			}
			else
			{
			FAN_RUN_OFF();

			__NOP();__NOP();__NOP();__NOP();__NOP();

			fan_off();
			}
			}					
		}
		else
		{
        	if(fan_delay_time_off!=0)
				{
				    fan_delay_time_off--;
							
						if(fan_speed_level==1)
						{
						    fan_on(287);
						}   
						else if(fan_speed_level==2)
						{
						    fan_on(303);
						}
						else if(fan_speed_level==3)
						{
						    fan_on(319);
						}
						
						__NOP();__NOP();__NOP();__NOP();__NOP();
						
						FAN_RUN_ON();
				}
				else
				{
				    FAN_RUN_OFF();
			
			      __NOP();__NOP();__NOP();__NOP();__NOP();
			
		        fan_off();
				}
		}
}

#endif 
/**
  * @brief  
  * @note  
  * @param: 
  *
**/

#if 0
//∑‰√˘∆˜…˘“Ù¿‡±
void Beep(Beep_TypeDef music)
{
    switch (music)
	  {
		    case BEEP_ONCE:
			     beep_times=1;
			     beep_lenght=2; 
			     non_beep_length=0;
			     break;
		
		    case BEEP_TWO:
			     beep_times=4;
			     beep_lenght=BEEP_LENGTH_DEFAULT; 
			     non_beep_length=NON_BEEP_LENGTH_DEFAULT;
			     break;			
		
				case BEEP_THREE:
			     beep_times=6;
			     beep_lenght=2; 
			     non_beep_length=NON_BEEP_LENGTH_DEFAULT;
			     break;
				
		    case BEEP_1SECONDS:
			     beep_times=1;
			     beep_lenght=100; 			//10ms*100=1seconds
			     non_beep_length=0;
			     break;	
		
		    case BEEP_TIME_OVER:
			     beep_times=15;
			     beep_lenght=200; 			//10ms*200=2seconds
			     non_beep_length=0;
			     break;			
		
		    default:
			     beep_times=0;
			     beep_lenght=0; 
			     non_beep_length=0;	
           break;		
    }
} 

#endif 

/**
*
*@brief 
*@notice
*@param
*
**/
void LED_Strip_Ctrl(void)
{
	  if(discharge_f){
        if((led_strip_open_f))
        {
		        LED_TAPE_ON();
        }
        else
        {
		        LED_TAPE_OFF();   
        }
	  }
		
}



/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void Plasma_Ctrl(void)
{
	  if(discharge_f){
        if(plasma_open_f)
		    {
		        PLASMA_ON();
				LED_PLASMA_ON();
		    }
		    else
		    {
		        PLASMA_OFF();
				LED_PLASMA_OFF();
		    }
        }
		
}


/**
*
*@brief environment temperature value compare set temperater value
*@notice
*@param
*@retrval 
*
**/
void Ultra_Sound_Ctrl(void)
{
    if(discharge_f)
    {
		    if(Ultra_Sound_open_f)
				{
				    ultra_sound_on(159); 
					LED_MOUSE_ON();
				}
				else
				{
				    ultra_sound_off();
					LED_MOUSE_OFF();
				}
		}
   
}



/**
*
*@brief 
*@notice
*@param
*
**/
void Relay_Ctrl(void)
{
   
	if(discharge_f){
		    if((PTC_heat_open_f)&& ptc_prohibit_off_f == 0)
				{
                    LED_PTC_ON();
					RELAY_ON();
					
				}
				else if(PTC_heat_open_f ==0)
				{
					LED_PTC_OFF();
					RELAY_OFF();
					
				}
		}

	
		
}	


void workd_interval_time_peripheral_handle(void)
{
	RELAY_OFF();
	ultra_sound_off();
	PLASMA_OFF();
		
}

/**
*
*@brief environment temperature value compare set temperater value
*@notice
*@param
*@retrval 
*
**/
void set_temp_compare(void)
{
   if(discharge_f == 1 && (set_temperature_value_f ==1 && time_1s_counter > 1 &&  key_input_temp_f != 4)|| ( key_input_temp_f == 4 && time_1s_counter  > 2)){//1
	    set_temperature_value_f ++;

    if(temperature >= setting_temperature){
	     ptc_prohibit_off_f = 0;
	     PTC_heat_open_f = 0;   // Á´ãÂç≥ÂÖ≥Èó≠
	     RELAY_OFF();
		 LED_PTC_OFF();
		 if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0);
		 //delay_ms(20);//HAL_Delay(5);
		 if(wifi_connected_success_f ==1)MqttData_Publish_SetPtc(0);

    }
	else{
	    ptc_prohibit_off_f = 0;
		PTC_heat_open_f = 1;   // Á´ãÂç≥open
		LED_PTC_ON();
		if(works_interval_f == 0)RELAY_ON();
		 
	
		if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0x01);
		//delay_ms(20);//HAL_Delay(5);
		if(wifi_connected_success_f == 1)MqttData_Publish_SetPtc(1);
        

	}
	if(wifi_connected_success_f == 1 && key_input_temp_f == 1)MqttData_Publis_SetTemp(setting_temperature);
    if(key_input_temp_f == 4) key_input_temp_f =2;

	}

}



/**
*
*@brief environment temperature value compare set temperater value
*@notice
*@param
*@retrval 
*
**/
void Fan_Current_Det(void)
{
	if((discharge_f)&&(fan_open_f) && works_interval_f == 0)
	{
		if(fan_current<_NO_FAN_LOAD_CURRENT){
			fan_current_det_time++;
			if(fan_current_det_time>=300){
				fan_current_det_time = 0;

				if(!no_fan_load_f)
				{
					//Beep(BEEP_THREE);
					beep_interval_time = 0;

					fan_open_f = 0;
				}

				no_fan_load_f = 1;
			}
		}
		else{
		fan_current_det_time = 0;
		}
	}
	else
	{
		fan_current_det_time = 0;
	}		   
}


/**
*
*@brief 
*@notice
*@param
*
**/
void peripheral_fun_handler(void)
{
   if(discharge_f==1){

    switch(works_interval_f){

	case 0:
      LED_Strip_Ctrl();
      Plasma_Ctrl();
      Ultra_Sound_Ctrl();
	  Relay_Ctrl();

	 if(disp_set_hours_time_f == 1 || Is_time_setting_f ==1) return ;
	  
      if(AI_timing_open_f==1){
	  	LED_AI_ON();
	  }
	  else{
	     LED_AI_OFF();

	  }
    break;

	case 1: //have a rest 10 minutes 
	   LED_Strip_Ctrl();
       workd_interval_time_peripheral_handle();
	   if(disp_set_hours_time_f == 1 || Is_time_setting_f ==1) return ;
	   if(AI_timing_open_f==1){
	  	LED_AI_ON();
	   }
	   else{
	     LED_AI_OFF();
       }

	break;
	  
    }

	}

}


void power_off_peripheral_handler(void)
{

	RELAY_OFF();
	ultra_sound_off();
	PLASMA_OFF();


}


void power_on_peripheral_handler(void)
{

	RELAY_ON();
	 ultra_sound_on(159); 
	PLASMA_ON();


}


