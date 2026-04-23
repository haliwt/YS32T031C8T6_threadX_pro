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



//300ms
void wifi_fast_led_state(void)
{

   if(discharge_f ==1){
	if(key_net_config_f){
	
		if(gpro_t.time_100ms_fast_led_f> 1){
			gpro_t.time_100ms_fast_led_f =0;
	       LED_WIFI_TOGGLE();
		}
	}
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


