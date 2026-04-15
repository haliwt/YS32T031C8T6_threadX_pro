#include "bsp.h"

#define KEY_TICKS_SHORT    1    // 40ms 消抖
#define KEY_TICKS_LONG_P   250  // 2.5s 电源键长按
#define KEY_TICKS_LONG_M   200  // 2s 模式/下键长按



uint8_t key_worked_f;
uint8_t key_long_f;
uint16_t key_data;
uint16_t key_time;
uint8_t  on_step,off_step ;


static void power_on_handler(void);
static void power_off_handler(void);

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

// 短按处理子函数
void Process_Short_Key(uint16_t key) 
{
    if (key == _POWER_KEY_DOWN) {
        if (discharge_f ==1){
			discharge_f =0;
			Beep(BEEP_ONCE);
			off_step = 0;
		}//System_Status_PowerOff();
        else{
		 discharge_f =1;
		 Beep(BEEP_ONCE);
		 on_step=0;
			//System_Status_PowerOn();
        }
        return;
    }

    // 仅在开机且无负载故障时允许操作
    if (!discharge_f || no_fan_load_f) return;

    switch (key) {
        case _MODE_KEY_DOWN:
            Is_timing_hour_disp_f = 1;
            Is_time_setting_f = 0;
            Is_temp_setting_f = 0;
            key_flash_time = 30;//300
            if (setting_timing_hour == 0) AI_timing_open_f = 0;
            Beep(BEEP_ONCE);
            break;

        case _UP_KEY_DOWN:
            Handle_Value_Adjustment(1);
            Beep(BEEP_ONCE);
            break;

        case _DOWN_KEY_DOWN:
            Handle_Value_Adjustment(0);
            Beep(BEEP_ONCE);
            break;
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
                    Beep(BEEP_ONCE);
                }
            }
            break;

        case _MODE_KEY_DOWN:
            if (key_time >= KEY_TICKS_LONG_M) {
                key_long_f = 1;
                if (discharge_f && !no_fan_load_f) {
                    Is_time_setting_f = 1;
                    Is_temp_setting_f = 0;
                    Is_timing_hour_disp_f = 0;
                    key_flash_time = 30;//300
                    Beep(BEEP_ONCE);
                }
            }
            break;

        case _DOWN_KEY_DOWN:
            if (key_time >= KEY_TICKS_LONG_M) {
                key_long_f = 1;
                if (discharge_f && !no_fan_load_f) {
                    led_strip_open_f = !led_strip_open_f; // 翻转灯带状态
                    Beep(BEEP_ONCE);
                }
            }
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
    if (Is_time_setting_f) 
    {
        if (is_up) {
            if (setting_timing_hour < 24) setting_timing_hour++;
        } else {
            if (setting_timing_hour > 0)  setting_timing_hour--;
        }
        
        // 逻辑关联：如果定时不为0，开启定时标志
        AI_timing_open_f = (setting_timing_hour > 0) ? 1 : 0;
        
        // 只要动了时间，秒和分计数值都要清零重新开始
        timing_min_cnt = 0;
        timing_hour_cnt = 0;
        Cacl_time_sec = 0;
    } 
    // 情况 B: 正在设置温度
    else if (Is_temp_setting_f) 
    {
        if (is_up) {
            if (setting_temperature < 40) setting_temperature++;
        } else {
            if (setting_temperature > 20) setting_temperature--;
        }
		set_temperature_value_f = 1;
		first_temp_compare_f = 0; 
		time_1s_set_temp_f =0;
    } 
    // 情况 C: 当前没在任何设置界面，按下加减键默认进入“温度设置”状态
    else 
    {
        Is_temp_setting_f = 1;
        Is_time_setting_f = 0;
        Is_timing_hour_disp_f = 0;
    }

    // 统一处理：设置闪烁倒计时（比如数码管闪烁 3 秒）
   
   // key_flash_time = 300; 
}


/**
  * @brief  系统状态初始化（开机）
  * @note   设定系统启动时的默认工作参数
  */
void System_Status_PowerOn(void) 
{
    // 1. 开启核心工作标志位
    if(wifi_app_timer_power_on_f==0){ //手机定时开机
	    discharge_f = 1;            // 总输出使能
	    PTC_heat_open_f = 1;        // 默认开启加热
	    Ultra_Sound_open_f = 1;     // 默认开启超声波
	    plasma_open_f = 1;          // 默认开启等离子
    }
	discharge_f = 1; 
    fan_open_f = 1;             // 默认开启风扇
    led_strip_open_f = 1;       // 默认开启灯带
    
    // 2. 设定启动默认参数
    setting_temperature = 40;   // 默认设定温度 40°C
    setting_timing_hour = 0;    // 默认不设置定时（常开模式）
    
    // 3. 状态显示切换
    Is_time_setting_f = 0;
    Is_temp_setting_f = 0;
    Is_timing_hour_disp_f = 0;
	
    
    // 4. 重置计数器（确保从 0 开始计时）
    timing_min_cnt = 0;
    timing_hour_cnt = 0;
    Cacl_time_sec = 0;
    work_time = 0;              // 重置工作时间累计
    device_rest_f = 0;          // 退出休息模式
    device_rest_time = 0;
	key_net_config_f =0;
    
    // 5. 清除异常标志
    no_fan_load_f = 0;          // 清除负载异常
    fan_delay_time_off = 0;     // 清除风扇延时关闭倒计时
    
    // 6. 执行开机提示音
   // Beep(BEEP_ONCE);
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
    
    // 2. 重置所有功能模式标志
    AI_timing_open_f = 0;
    Is_time_setting_f = 0;
    Is_temp_setting_f = 0;
    Is_timing_hour_disp_f = 0;
	//wifi
	wifi_run_step=0;
    
    // 3. 重置所有时间/计数器
    timing_min_cnt = 0;
    timing_hour_cnt = 0;
    Cacl_time_sec = 0;
    work_time = 0;
    device_rest_time = 0;
    device_rest_f = 0;
    
    // 4. 特殊逻辑处理
    no_fan_load_f = 0;
    fan_delay_time_off = 600; // 开启风扇延时关闭倒计时
    
    // 5. 提示音
    //Beep(BEEP_ONCE);
}




/**
*@brief:  totall task
*@param:
#@notice

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

}


static void power_on_handler(void)
{
    switch(on_step){

    case 0:
		System_Status_PowerOn();
        on_step = 1;
	break;

	case 1:
		Update_onLED_Display();
	    on_step = 2;

	break;

	case  2:
      peripheral_fun_handler();
	  wifi_led_state();
      on_step = 1;
	break;




	}


}

static void power_off_handler(void)
{
   static uint8_t counter; 
   switch(off_step){

    case 0:
       System_Status_PowerOff() ;
       off_step=1;
    break;

	case 1:
		all_led_off();
        
		if(++counter > 150){//
		 counter =0;
		 LED_POWER_TOGGLE();
		}
		Update_offLED_Display();
		off_step=1;

	break;



   }


}

