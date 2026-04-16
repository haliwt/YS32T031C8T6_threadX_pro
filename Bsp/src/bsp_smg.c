#include "bsp.h"



#define TM1639_CHAR_H               0x76    // H: 0111 0110 (b,c,e,f,g)
//#define TM1639_CHAR_N               0x2A    //n : 0010 1010 (C,E,G)

#define SWITCH_THRESHOLD 2

uint8_t disp_temp_hum,dsip_timer_value;
uint8_t timerbuf[1];


static const uint8_t Number_Table[] = {
    0xF3, // 0: 0011 1111   （f,e,d,c,b,a）--0x3F
    0x60, // 1: 0000 0110 --0x06--写数据式从低位开始，向高位开始写
    0xB5, // 2: 0101 1011 --0x5B
    0xF4, // 3: 0100 1111 --0x4F
    0x66, // 4: 0110 0110
    0xD6, // 5: 0110 1101 --0x6D
    0xD7, // 6: 0111 1101  --0x7D 
    0x70, // 7: 0000 0111
    0xF7, // 8: 0111 1111
    0xF6  // 9: 0110 1111
};


// 字母和特殊字符显示码
//static const uint8_t TM1639_Char_Table[] = {
//    0x67, // H: 0111 0110 (b,c,e,f,g)
//    0x36, // °: 0110 0011 (b,c,g)
//    0x93, // C: 0011 1001 (a,d,e,f)
//    0x05  // RH的H部分: 0101 0000 (e,g)
//};


// 字母和特殊字符显示码
static const uint8_t TM1639_Char_Err_Table[] = {
    0x97, // E: 0111 1001 (b,c,e,f,g)
    0x05, // r: 0101 0000 (b,c,g)
    
  
};




/**
 * @brief  显示0-24小时时间
 * @param  None
 * @retval None
 * @note   前两位显示小时(00-24)，最后一位显示'H'
 */
 #if 0
void SMG_Display_Hour(void)
{
    uint8_t hour;
    
    // 计算小时数 (3600秒=1小时)
    hour = g_pro.gTimer_disp_time_second / 3600;
    
    // 限制在0-24小时范围内
    if(hour > 24) hour = 24;
    
    // 显示十位数字（始终显示，小于10时显示0）
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, 
        Number_Table[hour / 10]);
    
    // 显示个位数字
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L,
        Number_Table[hour % 10]);
    
    // 最后一位显示'H'
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, TM1639_CHAR_H);
}
#endif 

/******************************************************************************
	*
	*Function Name:void SMG_Display_Err(uint8_t idata)
	*Funcion: 
	*Input Ref: idata: 1 -ptc warning  2 - fan warning
	*Return Ref:
	*
******************************************************************************/
void SMG_Display_Err(uint8_t idata)
{

	 TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, TM1639_Char_Err_Table[0]);
        
    // 写入十位（中间）
 
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, TM1639_Char_Err_Table[1]);
        
    if(idata==1){ //ptc warning 
	// 写入个位（最右边）'H'
     TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,Number_Table[1]);

    }
	else if(idata == 2){
		
	  TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,Number_Table[2]);
    }
	else{
	   TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, TM1639_Char_Err_Table[1]);

	}


}





void display_digital_3_numbers(void)
{
    
	 static uint8_t read_error_flag;//,switch_adc;

	// If any warning is active, do nothing
    if (no_fan_load_f ==1) return;
    
	 if(Is_time_setting_f ==1 || disp_set_hours_time_f ==1){//display set timer timing hours.

      
	    if(time_set_hours_counter < 4){
    	           AI_timing_open_f=0;
    	 		   LED_AI_OFF();
		           LED_HUMI_OFF();//HUMIDITY_ICON_OFF();
		           LED_TEMP_OFF();//TEMP_ICON_OFF();//WT.EDIT 2025.04.28
		           if(key_be_pressed_f == 1){
						 TM1639_Display_setTimerHours_3_Digit(setting_timing_hour);
						 if(setting_timing_hour > 0) Is_timing_hour_disp_f = 1;
						 else Is_timing_hour_disp_f = 0;

				   }
		           else{
				   	   if(timing_hour_cnt >0)
			               TM1639_Display_setTimerHours_3_Digit(setting_timing_hour);
					   else
					   	   TM1639_Display_setTimerMinutes_3_Digit(timing_min_cnt);
		           }
				  
		          
    	 }
		 else{
             disp_set_hours_time_f = 0;
			 Is_time_setting_f=0;// g_pro.g_disp_smg_timer_or_temp_hours_item = temperature_mode; //WT.EDIT 2025.010.06
             Is_temp_setting_f = 1;//at once display "temperature_mode" //WT.EDIT 2025.10.17
            
		     if(setting_timing_hour > 0 || timing_hour_cnt > 0){ // && g_key.key_mode_long_flag != 1){
                  AI_timing_open_f=1;
    	 		  LED_AI_ON(); 
			   #if DEBUG_ENABLE
			      printf("gAI = 1 \r\n");
			   #endif 
			 }
			 else {
			    AI_timing_open_f=0;
				LED_AI_OFF(); 
			 #if DEBUG_ENABLE
			    printf("gAI = 0 \r\n");
			 #endif 

            }
		
        }
      
     }
	 else if(set_temperature_value_f == 1 && time_set_temp_counter  < 3){//set up temperature value 

        
	       TM1639_Display_Temperature(setting_temperature);
		   
	 }
	 else{ //display temperature value 

	     if(key_long_f == 1) return ;

         if( AI_timing_open_f ==1){
			         
		     LED_AI_OFF(); 
		   }
		  else{
		     LED_AI_ON(); 

		   }

          
            if (time_switch_temp_hum_counter > SWITCH_THRESHOLD ){
			  time_switch_temp_hum_counter = 0; // 重置计时�??

			  disp_temp_hum = disp_temp_hum ^ 0x01;

			    if(disp_temp_hum==1){
				
					LED_TEMP_ON();
					LED_HUMI_OFF();

					DHT11_Display_Data(DISPLAY_TEMP); // 显示温度
					
				}
				else {
                   	LED_TEMP_OFF();
					LED_HUMI_ON(); //LED_TEMP_ICON_OFF();
					
					
				    DHT11_Display_Data(DISPLAY_HUM);  // 显示湿度
					
					
                }
       

          }
      
     	}

     
	   	
  
}
#if 0
/******************************************************************************
	*
	*Function Name:void set_timer_timing_value_handler(void)
	*Function:
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void set_timer_timing_value_handler(void)
{
   
   if(g_key.key_mode_long_flag ==1 && g_pro.gTimer_switch_set_timer_times > 2 ){ // 2

   	      g_pro.gTimer_switch_set_timer_times=0;
		  g_key.key_mode_long_flag++;

		  	
          if(g_pro.key_add_dec_be_pressed_flag==1){
		  	
            g_pro.key_add_dec_be_pressed_flag ++ ;
			if(g_pro.gdisp_timer_hours_value>0){
			timerbuf[0] = g_pro.gdisp_timer_hours_value;
			AI_timing_open_f = 0;
			LED_AI_OFF();
			g_pro.switch_disp_time_or_temp_item = temperature_mode; //define UP and down key is set temperature value 
			g_pro.set_timing_or_timer_time_flag=TIMER_TIME;
			g_pro.gTimer_timer_time_second=0;
			g_pro.gdisp_timer_minutes_value =0;
			

			SendWifiData_One_Data(0x2B,g_pro.gdisp_timer_hours_value);
	        osDelay(100);

               
			}
			else if(g_pro.gdisp_timer_hours_value ==0){
			
				AI_timing_open_f = 1;
				LED_AI_ON();
				g_pro.gdisp_timer_hours_value=0;
				g_pro.gdisp_timer_minutes_value =0;

				g_pro.set_timing_or_timer_time_flag = WORKS_TIME; //WT.EDIT 2025.10.18

				g_pro.switch_disp_time_or_temp_item = temperature_mode;
				
				SendWifiData_One_Data(0x2B,g_pro.gdisp_timer_hours_value);
	            osDelay(100);
			}
		}
		else{ //times is done ,exit this process
		   g_pro.switch_disp_time_or_temp_item = temperature_mode; //WT.EDIT 2025.10.17
		   if(g_pro.set_timing_or_timer_time_flag==TIMER_TIME){ //WT.EDIT 2025.11.07 
               LED_AI_OFF();
		   }
		   else{
               LED_AI_ON();
           }
		  g_pro.gTimer_mainboard_fun_counter=10; //WT.EDIT 2025.11.07
		 
        }
   	}
    else if(g_pro.set_timing_or_timer_time_flag==TIMER_TIME && g_key.key_mode_long_flag !=1){ //has been set up timer timing value .

       if(g_pro.gTimer_timer_time_second > 59){
	       g_pro.gTimer_timer_time_second=0;
		   #if 0 //TEST_UNIT
		   	 g_pro.gdisp_timer_minutes_value =g_pro.gdisp_timer_minutes_value - 40;
		   #else
		     g_pro.gdisp_timer_minutes_value--;

		   #endif 

		   if(g_pro.gdisp_timer_minutes_value< 0){

		       g_pro.gdisp_timer_minutes_value =59;

               g_pro.g_real_hours_counter++;
		        // 只有当小时数 > 1 时才减小时2026.03.09 .WT.EDIT .
		        if (g_pro.gdisp_timer_hours_value > 1 &&  g_pro.gdisp_timer_hours_value !=1 ){

				    dsip_timer_value = timerbuf[0] - g_pro.g_real_hours_counter +1;
		            g_pro.gdisp_timer_hours_value = dsip_timer_value;
		        }
				
				if(g_pro.gdisp_timer_hours_value ==1 || g_pro.gdisp_timer_hours_value==0){

				    g_pro.gdisp_timer_hours_value--;
                }
		        
			}
		   
			

		    if(g_pro.gdisp_timer_hours_value < 0){
            	
                   buzzer_sound();
			       g_pro.gpower_on = power_off;
			       SendData_Set_Command(CMD_POWER,close);
	               osDelay(100);

			  }
			 
          }
      }
}


#endif 



/**
 * @brief  在TM1639上显示DHT11的温湿度数据
 * @param  mode: 0-显示温度�??1-显示湿度
 * @retval DHT11_Status 类型的操作结�??
 */
void  DHT11_Display_Data(uint8_t mode)
{

    static uint8_t  copy_temp_value, copy_humidity_value;
    uint8_t  status;
   
    // 读取DHT11数据
    //status = dht11_read_data(&dht11_data.temperature,&dht11_data.humidity);
   status =  dht11_read_temp_humidity_value();

	if(status !=0){
	    if(mode == 0)
	    {
	    	 LED_TEMP_ON();//LED_TEMP_ICON_ON();
	         LED_HUMI_OFF();//LED_HUM_ICON_OFF();
	           
	         TM1639_Display_Temperature(copy_temp_value);
	    }
	    else
	    {
	        // 显示湿度
	    	LED_TEMP_OFF();//LED_TEMP_ICON_OFF();
	    	LED_HUMI_ON();//LED_HUM_ICON_ON();
	        TM1639_Display_Humidity(copy_humidity_value);
		}
    }
	else if(status==0){
   
    // 根据模式显示温度或湿�??
    if(mode == 0)
    {
        // 显示温度

              LED_TEMP_ON();//LED_TEMP_ICON_ON();
	         LED_HUMI_OFF();//LED_HUM_ICON_OFF();
           
            TM1639_Display_Temperature(temperature);
		    copy_temp_value = temperature;
				
        
    }
    else
    {
        // 显示湿度
    	LED_TEMP_OFF();//LED_TEMP_ICON_OFF();
	    LED_HUMI_ON();//LED_HUM_ICON_ON();
        TM1639_Display_Humidity(humidity);
		copy_humidity_value = humidity;
    }
	}
    
    
}



