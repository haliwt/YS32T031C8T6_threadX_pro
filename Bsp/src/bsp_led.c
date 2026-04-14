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
		led_scan_time++;
		if(led_scan_time>0){
			led_scan_time =0;
	       LED_WIFI_TOGGLE();
		}
	}
   }
}

void  wifi_normal_led_state(void)
{
	if(discharge_f ==1){

	  if(key_net_config_f==1) return ;
		if(wifi_connected_success_f)
		{
			LED_WIFI_ON();
		}
		else
		{
			led_scan_time++;
			if(led_scan_time>2)
			{
			  led_scan_time =0;
	          LED_WIFI_TOGGLE();
			}
		}
	}

 }


