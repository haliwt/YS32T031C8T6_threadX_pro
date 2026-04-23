#include "bsp.h"

#define KEY_TICKS_SHORT    4    // 40ms 消抖
#define KEY_TICKS_LONG_P   150//250  // 2.5s 电源键长按
#define KEY_TICKS_LONG_M   100//200  // 2s 模式/下键长按



uint8_t key_worked_f;
uint8_t key_long_f;
uint16_t key_data;
uint16_t key_time;



void Process_Short_Key(uint16_t key) ;
void Process_Short_Key(uint16_t key);
void Process_Long_Key(uint16_t key);


void Handle_Value_Adjustment(uint8_t is_up);



void Key_Scan(void)
{
    uint16_t key_i = 0;
    
    // 物理层扫描
    if      (KEY10_PIN) key_i = _POWER_KEY_DOWN;
    else if (KEY9_PIN)  key_i = _MODE_KEY_DOWN;
    else if (KEY8_PIN)  key_i = _UP_KEY_DOWN;
    else if (KEY7_PIN)  key_i = _DOWN_KEY_DOWN;

    // 状态机处理
    if (key_i == 0) { // 松手或无按键
        if (!key_long_f && key_time >= KEY_TICKS_SHORT) {
            // --- 执行短按逻辑 ---
            Process_Short_Key(key_data);
        }
        key_time = 0;
        key_data = 0;
        key_long_f = 0;
    } 
    else if (key_i != key_data) { // 切换按键
        key_data = key_i;
        key_time = 0;
        key_long_f = 0;
    } 
    else { // 持续按下
        if (key_time < 0xFFF) key_time++;
        
        // --- 执行长按逻辑 ---
        Process_Long_Key(key_data);
    }
}


// 长按处理子函数
void Process_Long_Key(uint16_t key) {
    if (key_long_f) return; // 已处理过长按则退出

    switch (key) {
        case _POWER_KEY_DOWN:
            if (key_time >= KEY_TICKS_LONG_P) {
                key_long_f = 1;
                if (discharge_f && !key_net_config_f) {
                    key_net_config_f = 1;
					link_net_step=0;
				    wifi_connected_success_f =0;
					wifi_first_connectoed_cloud_f =0;
                    key_net_config_time = 0;
                    Trigger_Simple_Beep(2) ;//Beep(BEEP_ONCE);
                }
            }
            break;

        case _MODE_KEY_DOWN://LONG KEY MODE ID 
            if (key_time >= KEY_TICKS_LONG_M) {
                key_long_f = 1;
                if (discharge_f && !no_fan_load_f) {
                    Is_time_setting_f = 1;
                   
                    time_set_hours_counter =0;
                    Trigger_Simple_Beep(2) ; //Beep(BEEP_ONCE);
                }
            }
            break;

        case _DOWN_KEY_DOWN:
            if (key_time >= KEY_TICKS_LONG_M) {
                key_long_f = 1;
                if (discharge_f) {
					if(led_strip_open_f==1){
						led_strip_open_f=0;
						LED_TAPE_OFF();
					}
                    else{
						led_strip_open_f = 1;// 翻转灯带状态
                         LED_TAPE_ON();
                    }
                    
                  Trigger_Simple_Beep(2) ;  //Beep(BEEP_ONCE);
                }
            }
            break;
    }
}

// 短按处理子函数
void Process_Short_Key(uint16_t key) 
{
    if (key == _POWER_KEY_DOWN  && !KEY10_PIN) {
        if (discharge_f) System_Status_PowerOff();
        else System_Status_PowerOn();
        return;
    }

    // 仅在开机且无负载故障时允许操作
    if (!discharge_f || no_fan_load_f) return;

    switch (key) {
        case _MODE_KEY_DOWN:
			if(Is_time_setting_f == 1){

		    }
			else{
		      
	            Is_time_setting_f = 0;
	            disp_set_hours_time_f = 1;
	            LED_AI_OFF();
				
				time_set_hours_counter =0;
			}
             Trigger_Simple_Beep(2) ;//Beep(BEEP_ONCE);
            break;

        case _UP_KEY_DOWN:
		
            Handle_Value_Adjustment(1);
            Trigger_Simple_Beep(2); //Beep(BEEP_ONCE);
            break;

        case _DOWN_KEY_DOWN:
            Handle_Value_Adjustment(0);
            Trigger_Simple_Beep(2);//Beep(BEEP_ONCE);
            break;
    }
}


/**
  * @brief  数值调节处理函数
  * @param  is_up: 1 为增加(UP), 0 为减少(DOWN)
  */
void Handle_Value_Adjustment(uint8_t is_up) 
{


    // 情况 A: 正在设置时间 (定时小时)
    if (Is_time_setting_f)  //设置 :定时时间值
    {
        if (is_up) {
            if (setting_timing_hour < 24) setting_timing_hour++;
        } else {
            setting_timing_hour--;
			if(setting_timing_hour < 0 )  setting_timing_hour=0;
        }
        
        // 逻辑关联：如果定时不为0，开启定时标志
        // AI_timing_open_f = (setting_timing_hour > 0) ? 1 : 0;
        AI_timing_open_f=0;
        // 只要动了时间，秒和分计数值都要清零重新开始
        timing_min_cnt = 0;
      
        Cacl_time_sec = 0;
		key_be_pressed_f =1;
		
		time_set_hours_counter =0;
    } 
    else{  // 情况 B: 正在设置温度
		
        if (is_up) {
           if (setting_temperature < 40) setting_temperature++;
        } else {
            if (setting_temperature > 20) setting_temperature--;
        }
		disp_set_hours_time_f = 0;
		set_temperature_value_f = 1;
		first_temp_compare_f = 0; 
		time_1s_counter =0;
	    time_set_hours_counter =0;
		key_be_pressed_f =0;
		key_input_temp_f= 1;
      
    }
}


/**
  * @brief  系统状态初始化（开机）
  * @note   设定系统启动时的默认工作参数
  */
void System_Status_PowerOn(void) 
{
    // 1. 开启核心工作标志位
    discharge_f = 1; 
    if(wifi_app_timer_power_on_f==0){ //手机定时开机
	    discharge_f = 1;            // 总输出使能
	    PTC_heat_open_f = 1;        // 默认开启加热
	    Ultra_Sound_open_f = 1;     // 默认开启超声波
	    plasma_open_f = 1;          // 默认开启等离子
	    power_on_peripheral_handler();
    }
	
    fan_open_f = 1;             // 默认开启风扇
    led_strip_open_f = 1;       // 默认开启灯带
    AI_timing_open_f = 1;       // 默认AI 开启
    fan_speed_level =100;       // 默认风扇最大风速 
    set_temperature_value_f =0; 
	//wifi
	wifi_run_step=0;
	wifi_off_step=0;
	
    // 2. 设定启动默认参数
    setting_temperature = 40;   // 默认设定温度 40°C
    setting_timing_hour = 0;    // 默认不设置定时（常开模式）
    gpro_t.time_base_1s_counter=0;  // 重置工作时间累计
    gpro_t.time_1m_f = 0;           // 重置工作时间累计
    // 3. 状态显示切换
    Is_time_setting_f = 0;
    Is_countdown_timer_f = 0;
    disp_set_hours_time_f = 0;
    
    // 4. 重置计数器（确保从 0 开始计时）
    timing_min_cnt = 0;
   
    Cacl_time_sec = 0;
            
    works_interval_f = 0 ; // device_rest_f = 0;          // 退出休息模式
    device_rest_time = 0;
	key_net_config_f =0;
    
    // 5. 清除异常标志
    no_fan_load_f = 0;          // 清除负载异常

    
    // 6. 执行开机提示音
   
    beep_power_sound();//Trigger_Simple_Beep(2) ;//Beep(BEEP_ONCE);
	LED_FUN_ON();

}
/**
  * @brief  系统状态复位（关机/重置）
  * @note   将所有业务逻辑标志位恢复至初始关闭状态
  */
void System_Status_PowerOff(void) 
{
    // 1. 关闭所有输出负载标志
    
    discharge_f = 0;
	wifi_app_timer_power_on_f =0; //smart app power on by timer timing clear .
    PTC_heat_open_f = 0;
	first_temp_compare_f=0;
    Ultra_Sound_open_f = 0;
    led_strip_open_f = 0;
    plasma_open_f = 0;
    fan_open_f = 0;
	key_net_config_f =0;

	 discharge_f = 0;            // 总输出使能
	 PTC_heat_open_f = 0;        // 默认开启加热
	 Ultra_Sound_open_f = 0;     // 默认开启超声波
	 plasma_open_f = 0;          // 默认开启等离子
     set_temperature_value_f =0; 
    // 2. 重置所有功能模式标志
    AI_timing_open_f = 0;
    Is_time_setting_f = 0;
 
    Is_countdown_timer_f = 0;
	works_interval_f =0;
	gpro_t.time_base_1s_counter=0;// 重置工作时间累计
	gpro_t.time_1m_f = 0;// 重置工作时间累计
	
	//wifi
	wifi_run_step=0;
	wifi_off_step =0;
    
    // 3. 重置所有时间/计数器
    timing_min_cnt = 0;
   
    Cacl_time_sec = 0;

    device_rest_time = 0;
    
    
    // 4. 特殊逻辑处理
    no_fan_load_f = 0;
	power_off_peripheral_handler();

    
    // 5. 提示音
    beep_power_sound();//Trigger_Simple_Beep(2) ;//Beep(BEEP_ONCE);
	all_led_off();
    TM1639_Display_ON_OFF(0);
}





