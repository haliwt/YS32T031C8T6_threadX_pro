#include "bsp.h"

#define TASK_COUNT (sizeof(Task_Table) / sizeof(Task_Config_t))

// 1ms 系统心跳计数器

// --- 任务函数声明 ---
void Task_Key_Scan_10ms(void);
void Task_link_wifi_20ms(void);
void Task_Logic_100ms(void);
void Task_ui_200ms(void);
void Task_Peripheral_300ms(void);
void Task_400ms(void);
void Task_500ms(void);
void Task_600ms(void);
void Task_900ms(void);
void Task_System_1s(void);
void Task_2s(void);
void Task_3s(void);
void Task_4s(void);
void Task_5s(void);
void Task_10s(void);

void Task_1minutes(void);
void Task_2minutes(void);

// 2. 任务注册表：将标志位地址与函数关联
static const Task_Config_t Task_Table[] = {
    {&gpro_t.time_10ms_f,  Task_Key_Scan_10ms},
    {&gpro_t.time_20ms_f,  Task_link_wifi_20ms},
    {&gpro_t.time_100ms_f, Task_Logic_100ms},
    {&gpro_t.time_200ms_f, Task_ui_200ms},
    {&gpro_t.time_100ms_fast_led_f, Task_Peripheral_300ms},
    {&gpro_t.time_400ms_f, Task_400ms},
    {&gpro_t.time_500ms_f, Task_500ms},
    {&gpro_t.time_600ms_f, Task_600ms},
    {&gpro_t.time_1s_f,    Task_System_1s},
    {&gpro_t.time_2s_f,    Task_2s},
    {&gpro_t.time_3s_f,    Task_3s},
    {&gpro_t.time_4s_f,    Task_4s},
    {&gpro_t.time_5s_f,    Task_5s},
    {&gpro_t.time_10s_f,    Task_10s},
    {&gpro_t.time_1m_f,    Task_1minutes},
    {&gpro_t.time_2m_f,    Task_2minutes}
};






/**
*
*@brief task hadleer
*@param // --- 1. 按键任务 (10ms) ---
*
**/
void Task_Key_Scan_10ms(void) 
{
	

	Key_Scan();               // 运行你提供的按键状态机
      // 蜂鸣器驱动
    Task_Beep_Simple_10ms();  //Task_beep_called_100ms();  
	
}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void Task_link_wifi_20ms(void)
{
 if(wifi_linking_tencent_f==1 &&  wifi_read_net_data_f==1){
	wifi_read_net_data_f++;

	Wifi_Rx_InputInfo_Handler();
	
  }
 


}
/**
*
*@brief task hadleer
*@param // --- 2. 通信与显示任务 (100ms) ---
*
**/
void Task_Logic_100ms(void) 
{
    
   if(discharge_f ==1){
   	 
   	 power_on_handler();
	 wifi_fast_led_state();
    }
    if(key_net_config_f==0 ){// 处理腾讯连连通信
         wifi_parse_tencennt_hadler();//
       
    }
}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void Task_ui_200ms(void)
{
    if(discharge_f ==1){
		set_temp_compare();
		
		
	}
	else{
	    power_off_handler();

	}
	
	
}
/**
  * @brief  // --- 3. 外设任务 (300ms) ---
  * @note  
  * @param: 
  *
**/
void Task_Peripheral_300ms(void) 
{

   if(discharge_f == 1){
   	
     	display_digital_3_numbers();
	}
}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void Task_400ms(void)
{
	
	 

}
/**
  * @brief 
  * @note  
  * @param: 
  *
**/
void Task_500ms(void)
{
  if(discharge_f ==1){
		wifi_normal_led_state();
		Fan_Ctrl_Process();	  // 风扇控制
	
	}

 }
/**
  * @brief 
  * @note  
  * @param: 
  *
**/
void Task_600ms(void)
{

    if(discharge_f ==1){
	
         if(wifi_connected_success_f==1){
		   wifi_default_handler();
         }
		 else{

		 }
	
	}
	else{

	   wifi_power_off_handler();

	}

}


/**
  * @brief  // --- 4. 系统级任务 (1s) ---
  * @note  
  * @param: 
  *
**/
void Task_System_1s(void) 
{
   if(discharge_f ==1){
	

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
	 
    }

	 if(key_net_config_f==0 ){
      wifi_auto_detected_link_state();

   	}
  
    
 }
/**
  * @brief  
  * @note  
  * @param: 
  *
**/

void Task_2s(void)
{
   
   if(discharge_f ==1){
		Heat_Process(); 
		 peripheral_fun_handler();
//	    #if DEBUG_ENABLE 

//		printf("works_time = %d \n\r",work_time);

//		#endif 
   	}
   

}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/

void Task_3s(void)
{

  if(discharge_f) dht11_read_temp_humidity_value();
//   #if DEBUG_ENABLE 
  //	printf("AI = %d \n\r",AI_timing_open_f);
//   #endif 
   
}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void Task_4s(void)
{

	if(discharge_f ==1 && Is_countdown_timer_f ==1){

	    Countdown_timer_Handler();
	  #if DEBUG_ENABLE 
	
	   //printf("disp_min = %d \n\r",timing_min_cnt);

	   ///printf("disp_min = %d \n\r",dc_connect_net_step);
	
      #endif 

	 }
	

	

	
}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void Task_5s(void)
{
   
   	wifi_check_id_handler();
    
}
/**
  * @brief 
  * @note  
  * @param: 
  *
**/
void Task_10s(void)
{
	

}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/

void Task_1minutes(void)
{
    if(discharge_f ==1){

	 works_nomal_run_time_handler();
	 Fan_Current_Det();		// 电流检测
   }

	if(key_net_config_f==0 &&  discharge_f ==1 &&  wifi_linking_tencent_f ==0){
	   	    
		   Reconnection_Wifi_Order();
		
	}

	
	 
    #if DEBUG_ENABLE 
	   
	   printf("mqtt_status = %d \n\r", mqtt_status); 

	   printf(" wifi_check_net_f  = %d \n\r",wifi_check_net_f);  
	  
	   
	 #endif 
}
/**
  * @brief  fan run is error
  * @note  
  * @param: 
  *
**/

void Task_2minutes(void)
{
	// 传感器采样
	   if (discharge_f) {
		 //  dht11_read_temp_humidity_value();
		   AD_Filter();
		   Adc_Channel_Sample();
	   }



}
/**
  * @brief  fan run is error
  * @note  
  * @param: 
  *
**/

// --- 核心调度器 (放在 main 的 while(1) 中) ---
// 3. 核心调度器：在 main 的 while(1) 中调用
void Task_Scheduler(void) 
{
    for (uint8_t i = 0; i < TASK_COUNT; i++) {
        // 检查对应的标志位是否被中断置 1
        if (*(Task_Table[i].task_f) == 1) {
            
            // 立即清除标志位（重要！防止重复执行）
            *(Task_Table[i].task_f) = 0; 
            
            // 执行任务函数
            if (Task_Table[i].task_func != NULL) {
                Task_Table[i].task_func();
            }
        }
    }
}

/**
  * @brief  fan run is error
  * @note  
  * @param: 
  *
**/



