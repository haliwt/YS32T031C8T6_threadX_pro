#include "bsp.h"


void all_led_off(void)
{
    LED_AI_OFF();
	LED_PTC_OFF();
	LED_PLASMA_OFF();
	LED_MOUSE_OFF();
	LED_WIFI_OFF();
	//LED_POWER_OFF();
	LED_TAPE_OFF();
	LED_TEMP_OFF();
	LED_HUMI_OFF();	


}

void power_on_led_open_handler(void)
{
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

}
//300ms
void wifi_fast_led_state(void)
{

   if(discharge_f ==1 && key_net_config_f && wifi_connected_success_f == 0){
	    LED_WIFI_TOGGLE();
		
   }
}


void  wifi_normal_led_state(void)
{
	
     if(key_net_config_f==1) return ;
		if(wifi_connected_success_f)
		{
			LED_WIFI_ON();
//			  #if DEBUG_ENABLE

//			   printf("wifi_flag = %d\n\r",wifi_connected_success_f);

//			  #endif 
		}
		else{
			if(gpro_t.time_200ms_f > 7){//if(led_scan_time>1)
			   gpro_t.time_200ms_f =0; 
			 // led_scan_time =0;
	          LED_WIFI_TOGGLE();

//			  #if DEBUG_ENABLE

//			   printf("wifi_flag = %d\n\r",wifi_connected_success_f);

//			  #endif 
			}

			
		}
	

 }


