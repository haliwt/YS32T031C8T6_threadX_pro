#include "bsp.h"
#define UID_ADDR   (0x1FFF1E00UL)
#define YS32_UID_BASE    0x1FFF1E00


uint8_t time_3s_flag;
uint8_t time_200ms_run_flag;
uint8_t counter_1m;



static void task_1s_run_handler(void);
uint8_t counter_2s,tim_2s_f;



/**************************************************************************************
*
*Function Name:  void bsp_init(void)
*Function: initial of base reference 
*
*
**************************************************************************************/
  void bsp_init(void)
  {
   
   discharge_f =0;
  //SET_DHT11_DATA_OUT_MODE();//SET_DHT11_DATA_IN_MODE();
   //SET_DHT11_DATA_IN_MODE();
#if(Enable_EventRecorder == 1)  
	/* 0…60‹10‡80†40†30…4EventRecorder0…50„40†70„90‡40‹0 */
	EventRecorderInitialize(EventRecordAll, 1U);
	EventRecorderStart();
#endif
	
  
}





//void tx_thread_sleep(uint16_t ms)
//{
//    // 假设你在 TIM6 中断里已经有 time_10ms_f 标志
//    // 如果是 100ms/200ms 这种大延时，直接判断标志位是最安全的
//    uint16_t target_10ms_units = ms / 10;
//    uint8_t count = 0;
//    
//    while (count < target_10ms_units)
//    {
//        if (time_wifi_10ms_f)
//        {
//            time_wifi_10ms_f = 0;
//            count++;
//        }
//        // 裸机运行，这里可以顺便喂狗
//        // IWDG_ReloadCounter(); 
//    }
//}



/**
*@brief:  totall task
*@param:
#@notice

**/
void task_scheduler(void)
{
   uint8_t temp_counter_flag,tim_200ms_counter;
   

	if(time_5ms_f && temp_counter_flag < 100){
		time_5ms_f = 0;
		
	     temp_counter_flag ++;

		// Read_DHT11_Data(); 
		
	}

	

	if(time_10ms_f){
		


		Key_Scan();
		

		Task_beep_called_100ms();

		if(wifi_linking_tencent_f==1 &&  wifi_read_net_data_f==1){
			wifi_read_net_data_f++;

		    Wifi_Rx_InputInfo_Handler();

		}

	   
		time_10ms_f = 0;
	}

	if(time_100ms_f)
	{
	    time_100ms_f = 0;
		
		tim_200ms_counter++;
		if(tim_200ms_counter ==5)time_200ms_run_flag=1;
		Update_onLED_Display();
		//wifi_fast_led_state();
		

		
	     wifi_rx_run_handler();

		
	}

	if(time_300ms_f==1){
		 time_300ms_f =0;  
		// wifi_normal_led_state();
	     set_temp_compare();
		 peripheral_fun_handler();
	     Fan_Ctrl_Process();
		 Fan_Current_Det();  
	
     }

	


	if(time_200ms_run_flag ==1){
      
       wifi_default_handler();
	   // printf("time_200ms_f = 1\n");
	   time_200ms_run_flag=0;
	  
   }

 
	
	/*timer 1s*/
	if(time_1s_f == 1){
	 
	   counter_2s ++;
	   time_set_temp_counter++;
	   time_3s_flag++;

	  Heat_Process();	
	
	 if(counter_2s > 3){
	 	 counter_2s=0;
	 	
	    if(discharge_f==0){
	    switch(counter_1m){

		  case 0:
		  
            if(wifi_connected_success_f ==1){
	          MqttData_Publish_SetOpen(0); 
			  
			 counter_1m =1;
		
             }
		  
		    
          break;

		  case 1:
		   if(wifi_connected_success_f ==1){
            MqttData_Publish_PowerOff_Ref(); 
			
		    counter_1m =2;
		  	}
		  
		  break;

		 case 2:
		 	if(wifi_connected_success_f ==1){
			   Subscriber_Data_FromCloud_Handler();
		      counter_1m =3;
		 	}

		  case 3:
		  	if(wifi_connected_success_f ==1){
			
		       Publish_Data_fan_Warning(0); //fan warning .
		  
             
		       counter_1m =0;
		  	}
			


		  break;
			
	    	}
			
		  }
	   // printf("tim_2s_f = 1\n");
	   
	   }
       wifi_default_handler();
	 	
	   if(time_3s_flag > 3 && discharge_f ==1){
			//Read_DHT11_Data(); 
			AD_Filter();
			
		    Adc_Channel_Sample();
			time_3s_flag =0;
		}
	  task_1s_run_handler();
	   time_1s_f = 0;

	 
	} //end 1s task

	


	 if(time_1minute_f==1){//1s * 60 =60s = 1 minute
      
	     switch(discharge_f){

		  case 1: //power on
   
			if(device_rest_f)
			{
				device_rest_time++;
				if(device_rest_time>=10){
					device_rest_time = 0;
					device_rest_f = 0;

					work_time = 0;
				}
			}
			else{
				work_time++;
				if(work_time>119){ // 2 hours
					work_time = 0;

					device_rest_f = 1;
					device_rest_time = 0;

					fan_delay_time_off = 600;
			   }
			}
		

		break;

		case 0: //power off
		
        

		  break;

		
	}

	 time_1minute_f = 0;

	
     }
}
/**
 *
 * @brief 
 * @param 
 * @retrval 
 *
 **/
static void task_1s_run_handler(void)
{
	if(((discharge_f)&&(AI_timing_open_f))){
		if(!device_rest_f){
			if(++Cacl_time_sec>=60){
				Cacl_time_sec = 0;

				timing_min_cnt++;
				if(timing_min_cnt>=60)
				{
					timing_min_cnt = 0;

					timing_hour_cnt++;
					if(timing_hour_cnt>=setting_timing_hour){

						timing_hour_cnt = 0;

						discharge_f = 0;
						AI_timing_open_f = 0;
						PTC_heat_open_f = 0;
						first_temp_compare_f =0;
						ptc_prohibit_off_f =0;
						Ultra_Sound_open_f = 0;
						led_strip_open_f = 0;
						plasma_open_f = 0;
						fan_open_f = 0;

						Is_time_setting_f = 0;
						Is_temp_setting_f = 0;
						Is_timing_hour_disp_f = 0;

						timing_min_cnt = 0;
						timing_hour_cnt = 0;	

						no_fan_load_f = 0;	
						//wifi
						wifi_run_step=0;

						fan_delay_time_off = 600;

						device_rest_f = 0;
						device_rest_time = 0;
					}
				}
		   }
		}
	}
	else
	{
		timing_min_cnt = 0;
	}

	if(key_net_config_f)
	{
		key_net_config_time++;
		if(key_net_config_time>=130)
		{
			key_net_config_time = 0;

			key_net_config_f = 0;
		}
		else{ //conneting to wifi net 
		// IWDG->KR = 0xAAAA;
			link_wifi_net_handler();
		}
	}
	else
	{
		key_net_config_time = 0;
	}

	  
		
}

/**
 *
 * @brief 
 * @param 
 * @retrval 
 *
 **/


/**
 *
 * @brief 
 * @param 
 * @retrval 
 *
 **/

void wifi_rx_run_handler(void)
{
      
    if(key_net_config_f==0 ){
         wifi_communication_tnecent_handler();//
    
         //getBeijingTime_cofirmLinkNetState_handler();

         wifi_auto_detected_link_state();
		
    }


}

/**
*@brief:  totall task
*@param:
#@notice

**/
/*---------------------------------------------------------------------------------
---�� �� ����-Task_beep_called_100ms
---��    �ܣ�- 
---��    ����- 
---�� �� ֵ��-
---��    �ã�- �ڴ�ѭ������100ms����һ��
---ǰ��������-
---˵    ����- 
---------------------------------------------------------------------------------*/
void Task_beep_called_100ms(void)
{
    if(beep_times)
	  {
		    if(beep_times&0x01)
		    {
			      BEEP_ON();
			
			      if(beep_lenght>1)
			      {
				        beep_lenght--;
			      }
			      else 
			      {
				        if(beep_times>0)
				        {
					          beep_times--;
				        }
			      }		
	      }
		    else 
		    {
			      BEEP_OFF();
			
			      if(non_beep_length>1)
			      {
				        non_beep_length--;
			      }
			      else 
			      {
				        if(beep_times>1)//����һ��
				        {
					          beep_times--;
					          non_beep_length=NON_BEEP_LENGTH_DEFAULT;
					          beep_lenght=2;
				        }
				        else 
				        {
					          beep_times=0;//����
					          non_beep_length=0;
					          beep_lenght=0;
				        }
			      }
		    }	
		}
		else 
	  {
	      non_beep_length=0;
		    beep_lenght=0;
		    BEEP_OFF();
	  }
} 






#if 0
/**
 * @brief  读取 YS32 芯片的前 12 字节 (96位) 唯一 ID
 * @param  uid_buf: 外部传入的缓冲区，长度必须 >= 12
 */
void Read_YS32_UID_12Bytes(uint8_t *uid_buf)
{
    // 1. 定义 UID 基地址指针 (根据手册为 0x1FFF1E00)
    // 使用 volatile 确保每次都从硬件地址读取，而不是从寄存器缓存读取
    volatile uint32_t *p_uid = (uint32_t *)0x1FFF1E00;

    // 2. 分三次读取 32 位数据（4字节 * 3 = 12字节）
    uint32_t temp_id[3];
    temp_id[0] = p_uid[0];
    temp_id[1] = p_uid[1];
    temp_id[2] = p_uid[2];

    // 3. 将 32 位数据拆分到 8 位字节数组中 (小端模式)
    for (int i = 0; i < 3; i++)
    {
        uid_buf[i * 4 + 0] = (uint8_t)(temp_id[i] & 0xFF);         // 低字节
        uid_buf[i * 4 + 1] = (uint8_t)((temp_id[i] >> 8) & 0xFF);
        uid_buf[i * 4 + 2] = (uint8_t)((temp_id[i] >> 16) & 0xFF);
        uid_buf[i * 4 + 3] = (uint8_t)((temp_id[i] >> 24) & 0xFF); // 高字节
    }
}

#endif 



// 模仿 HAL 库读取第一个字
uint32_t YS32_GetUIDw0(void)
{
    return (*(volatile uint32_t *)(YS32_UID_BASE));
}

// 读取全部 96 位并合成一个 uint32_t (异或方式)
uint32_t Get_Unique_ID_32bit(void)
{
    uint32_t w0 = *(volatile uint32_t *)(YS32_UID_BASE);
    uint32_t w1 = *(volatile uint32_t *)(YS32_UID_BASE + 0x04);
    uint32_t w2 = *(volatile uint32_t *)(YS32_UID_BASE + 0x08);

	uint32_t mix = (w0 ^ w1 ^ w2);
    
    //return (w0 ^ w1 ^ w2); // 异或合成，最大程度保留唯一性
    return (mix % 10000000);         // 压缩成 7 位十进制（0~9999999）
}

/**********************************************************************
    *
    *Function Name:uint8_t bcc_check(const unsigned char *data, int len) 
    *Function: BCC????
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
uint8_t bcc_check(const unsigned char *data, int len) 
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}




