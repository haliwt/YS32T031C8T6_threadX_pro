#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp.h"

#define TOKEN_ID      "123"


static char   message[180]    = {0};
static int    message_len	 = 0;



//char *tx_data={"AT+TCMQTTPUB=\"$thing/up/property/EHQB1P53IH/UYIJIA01-a0005\",0,\"{\"method\":\"report\"\,\"clientToken\":\"up01\"\,\"params\":\"{\"open\":1\,\"temperature\":26}}\"\r\n"};
//char *tx_data={"AT+TCMQTTPUB=\"$thing/up/property/EHQB1P53IH/UYIJIA01-a0005\",0,\"{\\\"method\\\":\\\"report\\\"\\\,\\\"clientToken\\\":\\\"up01\\\"\\,\\\"params\\\":{\\\"open\\\":1\\,\\\"temperature\\\":26}}\"\r\n"};

/* MAX size of client ID */


static void Mqtt_power_off_Value(void);

static void property_report_SetOpen(uint8_t open);
static void Mqtt_Value_update_data(void);



static void property_report_state(void);


static void property_report_ReadTempHum(uint8_t tempvalue,uint8_t humvalue);


static void property_report_SetTemp(uint8_t temp);
static void property_report_SetFan(uint8_t fan);
static void property_report_SetTime(uint8_t time);
static void property_report_SetState(uint8_t dat);
static void property_report_power_off_state(void);
static void property_report_fan_warning(uint8_t warning);






typedef struct {
    
    bool     open;
	bool     sonic;
	
	bool     ptc;
	bool     anion;

	uint8_t     state ; //
    
	uint8_t  find;
	uint8_t  set_temperature;

} serviceInfo;


static serviceInfo    sg_info;

uint8_t gmode;

// led attributes, corresponding to struct LedInfo
//static char *sg_property_name[] = {"opne", "sonic", "find", "nowtemperature","state","ptc","Anion","temperature","Humidity"};
/**********************************************************************
    *
    *Function Name:
    *Function: 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void Mqtt_Value_Init(void)
{
    fan_speed_level=100;
    setting_temperature=40 ;
   	sg_info.open=1;
    sg_info.state=1;
    sg_info.ptc=1; 
    sg_info.anion=1;  //灭菌
	sg_info.sonic =1;  //驱虫
    sg_info.find=100;

	sg_info.set_temperature = 40;  //esp_t.set_temperature_value ;
	
}
static void Mqtt_Value_update_data(void)
{
    
    sg_info.open = 1;
	if(AI_timing_open_f==1)gmode =1;
	sg_info.state = gmode;
	sg_info.ptc  = PTC_heat_open_f;//esp_t.gDry;
    //sg_info.ptc = g_dry_open_flag;
	sg_info.anion = plasma_open_f;
	sg_info.sonic = Ultra_Sound_open_f;//esp_t.gUlransonic ;
    sg_info.find = fan_speed_level;//esp_t.set_wind_speed_value;
    //if(esp_t.set_temperature_value <20)esp_t.set_temperature_value = 20;
	//else if(esp_t.set_temperature_value > 40)esp_t.set_temperature_value = 40;
	sg_info.set_temperature = setting_temperature;

}

static void Mqtt_power_off_Value(void)
{
   // fan_speed_level=10;
   	sg_info.open=0;
    sg_info.state=1;
    sg_info.ptc=0; 
    sg_info.anion=0;  //灭菌
	sg_info.sonic =0;  //驱虫
    sg_info.find=0;
	//if(esp_t.set_temperature_value <20)esp_t.set_temperature_value = 20;
	//else if(esp_t.set_temperature_value > 40 )esp_t.set_temperature_value = 40;
	sg_info.set_temperature = 20; //esp_t.set_temperature_value ;
	
}


/********************************************************************************
	*
	*Function Name:static void property_report_Temp_Humidity(void)
	*Function :send data to tencent cloud only read temperature and humidity of data
	*Input Ref: only read temperature value and humidiy value
	*           
	*Return Ref:
	*
********************************************************************************/
void property_topic_publish(void)
{
    //char topic[180] = {0};
   // int  size;
   static uint32_t uid;
	message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串

    uid =Get_Unique_ID_32bit();
    message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
    message_len= snprintf(message, sizeof(message), "AT+TCMQTTPUB=\"$thing/up/property/%s/UYIJIA01-%d\",0,", PRODUCT_ID,uid);
    //at_send_data((uint8_t *)topic, size);
    //delay_ms(300);
    //USART2_DMA_Send((uint8_t *)topic, size);
    send_usart2_data((const uint8_t *)message, message_len);
   // delay_ms(300);
}
/********************************************************************************
	*
	*Function Name:static void property_report_state(void)
	*Function : publis to tencent cloud data 
	*Input Ref: 
	*           
	*Return Ref:
	*
********************************************************************************/
static void property_report_state(void)
{
   // char       message[180]    = {0};
   // int        message_len     = 0;
   message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
    Mqtt_Value_Init();
   message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up01\\\"\\,\\\"params\\\":{\\\"open\\\":%d\\,\\\"Anion\\\":%d\\,\\\"ptc\\\":%d\\,\\\"sonic\\\":%d\\,\\\"state\\\":%d\\,\\\"find\\\":%d\\,\\\"temperature\\\":%d}}\"\r\n",
                             sg_info.open,sg_info.anion,sg_info.ptc,sg_info.sonic,sg_info.state,sg_info.find,sg_info.set_temperature);
                               
 
	//at_send_data((uint8_t *)message, message_len);
	//delay_ms(100);
	//USART2_DMA_Send((uint8_t *)message, message_len);
	send_usart2_data((const uint8_t *)message, message_len);
	//delay_ms(200);
   
}


void property_report_update_data(void)
{
	///char  message[180]    = {0};
	//int   message_len	   = 0;
	 message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	 Mqtt_Value_update_data();
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up02\\\"\\,\\\"params\\\":{\\\"open\\\":%d\\,\\\"Anion\\\":%d\\,\\\"ptc\\\":%d\\,\\\"sonic\\\":%d\\,\\\"state\\\":%d\\,\\\"find\\\":%d\\,\\\"temperature\\\":%d}}\"\r\n",
								 sg_info.open,sg_info.anion,sg_info.ptc,sg_info.sonic,sg_info.state,sg_info.find,sg_info.set_temperature);
								   
	 
	//at_send_data((uint8_t *)message, message_len);
	//USART2_DMA_Send((uint8_t *)message, message_len);
	 send_usart2_data((uint8_t *)message, message_len);
	///	delay_ms(200);


}

static void property_report_power_off_state(void)
{

	//char       message[180]    = {0};// message[256]
    //int        message_len     = 0;
   message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串

   Mqtt_power_off_Value();
   message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up01\\\"\\,\\\"params\\\":{\\\"open\\\":%d\\,\\\"Anion\\\":%d\\,\\\"ptc\\\":%d\\,\\\"sonic\\\":%d\\,\\\"state\\\":%d\\,\\\"find\\\":%d\\,\\\"temperature\\\":%d}}\"\r\n",
                             sg_info.open,sg_info.anion,sg_info.ptc,sg_info.sonic,sg_info.state,sg_info.find,sg_info.set_temperature);
                               
 
  //at_send_data((uint8_t *)message, message_len);
  
	//USART2_DMA_Send((uint8_t *)message, message_len);
	 send_usart2_data((uint8_t *)message, message_len);
	///	delay_ms(200);


}


/********************************************************************************
	*
	*Function Name:static void property_report_Temp_Humidity(void)
	*Function :send data to tencent cloud only read temperature and humidity of data
	*Input Ref: only read temperature value and humidiy value
	*           
	*Return Ref:
	*
********************************************************************************/
static void property_report_ReadTempHum(uint8_t tempvalue,uint8_t humvalue)
{

	  message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	  message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up00\\\"\\,\\\"params\\\":{\\\"nowtemperature\\\":%d\\,\\\"Humidity\\\":%d}}\"\r\n"
								,tempvalue,humvalue);
								  
		//at_send_data((uint8_t *)message, message_len);
		//USART2_DMA_Send((uint8_t *)message, message_len);
		 send_usart2_data((uint8_t *)message, message_len);
		//delay_ms(200);

}

static void property_report_SetState(uint8_t dat)
{
     message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"state\\\":%d}}\"\r\n",dat);
								  
	//at_send_data((uint8_t *)message, message_len);

	//USART2_DMA_Send((uint8_t *)message, message_len);
	 send_usart2_data((uint8_t *)message, message_len);
		//delay_ms(200);

}
/********************************************************************************
	*
	*Function Name:static void property_report_SetTempFan(void)
	*Function : sensor of data to tencent cloud  temperature and humidity of data
	*Input Ref: write temperature value          
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetTemp(uint8_t temp)
{
     message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up03\\\"\\,\\\"params\\\":{\\\"temperature\\\":%d}}\"\r\n",temp);
								  
	//at_send_data((uint8_t *)message, message_len);
	//USART2_DMA_Send((uint8_t *)message, message_len);
	 send_usart2_data((uint8_t *)message, message_len);
	//delay_ms(200);


}
static void property_report_SetOpen(uint8_t open)
{
     message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"open\\\":%d}}\"\r\n",open);
								  
	//at_send_data((uint8_t *)message, message_len);
	//delay_ms(200);
	///USART2_DMA_Send((uint8_t *)message, message_len);

    send_usart2_data((uint8_t *)message, message_len);
	
	//delay_ms(200);

}
/********************************************************************************
	*
	*Function Name:static void property_report_SetSonic(uint8_t datsonic)
	*Function : "驱虫"
	*Input Ref: bool-> 1 ->on ,0 -> off
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetSonic(uint8_t datsonic)
{
   message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	
	
	message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up02\\\"\\,\\\"params\\\":{\\\"sonic\\\":%d}}\"\r\n"
								,datsonic);
								  
	//at_send_data((uint8_t *)message, message_len);
	//USART2_DMA_Send((uint8_t *)message, message_len);
	send_usart2_data((uint8_t *)message, message_len);
		//delay_ms(200);

}
/********************************************************************************
	*
	*Function Name:static void property_report_SetAnion(uint8_t datsonic)
	*Function : "灭菌"
	*Input Ref: bool-> 1 ->on ,0 -> off
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetAnion(uint8_t datanion)
{
     message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up03\\\"\\,\\\"params\\\":{\\\"Anion\\\":%d}}\"\r\n"
								,datanion);
								  
	//at_send_data((uint8_t *)message, message_len);
	//USART2_DMA_Send((uint8_t *)message, message_len);
	 send_usart2_data((uint8_t *)message, message_len);
	//	delay_ms(200);

}
/********************************************************************************
	*
	*Function Name:static void property_report_SetPtc(uint8_t datsonic)
	*Function : "驱虫"
	*Input Ref: bool-> 1 ->on ,0 -> off
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetPtc(uint8_t datptc)
{
     message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"ptc\\\":%d}}\"\r\n"
								,datptc);
								  
	//at_send_data((uint8_t *)message, message_len);
	//USART2_DMA_Send((uint8_t *)message, message_len);
	send_usart2_data((uint8_t *)message, message_len);
	//delay_ms(200);

}


/********************************************************************************
	*
	*Function Name:static void property_report_SetTempFan(void)
	*Function : sensor of data to tencent cloud  temperature and humidity of data
	*Input Ref: only read temperature value and humidiy value
	*           
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetFan(uint8_t fan)
{

     message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up05\\\"\\,\\\"params\\\":{\\\"find\\\":%d}}\"\r\n",fan);
	//at_send_data((uint8_t *)message, message_len);
	//USART2_DMA_Send((uint8_t *)message, message_len);
	 send_usart2_data((uint8_t *)message, message_len);
		///delay_ms(200);

}
/********************************************************************************
	*
	*Function Name:static void property_report_SetTempFan(void)
	*Function : sensor of data to tencent cloud  temperature and humidity of data
	*Input Ref: only read temperature value and humidiy value
	*           
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetTime(uint8_t time)
{

   
	message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	   
	message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up05\\\"\\,\\\"params\\\":{\\\"find\\\":%d}}\"\r\n",
								   time);
	 // at_send_data((uint8_t *)message, message_len);
	 //USART2_DMA_Send((uint8_t *)message, message_len);
	  send_usart2_data((uint8_t *)message, message_len);
	 //	delay_ms(200);
}


/*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscriber_Data_FromCloud_Handler(void)
{
  // uint8_t *device_massage;
        
         // uint8_t  device_massage[128] ={0} ;//(uint8_t *)malloc(128);
         
        static uint32_t uid;
  	    message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
        uid =Get_Unique_ID_32bit();
      
        message_len = sprintf((char *)message,"AT+TCMQTTSUB=\"$thing/down/property/%s/UYIJIA01-%d\",0\r\n", PRODUCT_ID,uid);

         //delay_ms(50);
         tx_thread_sleep(1);//10ms.
        // free(device_massage);
         send_usart2_data((const uint8_t *)message,message_len);
       //  delay_ms(300);
}
/*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Publish_Data_ToTencent_Initial_Data(void)
{
    
          MqttData_Publish_Init();
	         
}

/********************************************************************************
	*
	*Function Name:
	*Function :
	*Input Ref: 
	*Return Ref:
	*
********************************************************************************/
static void property_report_fan_warning(uint8_t warning)
{
    // char	message[128]    = {0};
	 //int	message_len	  = 0;
	message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"fan_warning\\\":%d}}\"\r\n",warning);
								  
	 send_usart2_data((const uint8_t *)message, message_len);
	///delay_ms(200);

}



/********************************************************************************
	*
	*Function Name:void IOT_MQTT_Publish(pClient, topic, &pubParams)
	*Function :send data to tencent cloud 
	*Input Ref: pClient ->client data array ,topic->tencent register topic name,
	*           &pubParams -> topic of reference
	*Return Ref:
	*
********************************************************************************/
void MqttData_Publish_Init(void)
{
	 property_topic_publish();

	 property_report_state();

}

void MqttData_Publish_Update_Data(void)
{
    property_topic_publish();
	property_report_update_data();

}

void MqttData_Publish_SetOpen(uint8_t dop)
{
   property_topic_publish();
   property_report_SetOpen(dop);
}

void MqttData_Publish_SetPtc(uint8_t dptc)
{
   property_topic_publish();
    property_report_SetPtc(dptc);

}

void MqttData_Publish_SetPlasma(uint8_t pla) //�?�?
{
   property_topic_publish();
   property_report_SetAnion(pla);

}

void MqttData_Publish_SetUltrasonic(uint8_t datsonic) //超声�?
{
   property_topic_publish();
   property_report_SetSonic(datsonic);

}

void MqttData_Publish_AitState(uint8_t sdat) //Ai model
{
   property_topic_publish();
   property_report_SetState(sdat);

}


void MqttData_Publish_PowerOff_Ref(void) //
{
   property_topic_publish();

   property_report_power_off_state();

}



void Publish_Data_fan_Warning(uint8_t warning)
{
	  property_topic_publish();
	  property_report_fan_warning(warning);

}


/********************************************************************************
	*
	*Function Name:void MqttData_Publis_TempHumidity(void)
	*Function : publis data to tencent cloud 
	*Input Ref: pClient ->client data array ,topic->tencent register topic name,
	*           &pubParams -> topic of reference
	*Return Ref:
	*
********************************************************************************/
void MqttData_Publis_ReadTempHum(uint8_t tempvalue,uint8_t humvalue)
{

	property_topic_publish(); 
	property_report_ReadTempHum(tempvalue,humvalue);


}

void MqttData_Publis_SetFan(uint8_t fan)
{
	property_topic_publish(); 
    property_report_SetFan(fan);

}
void MqttData_Publis_SetTemp(uint8_t temp)
{
	property_topic_publish(); 
    property_report_SetTemp(temp);


}


void MqttData_Publis_SetTime(uint8_t time)
{
	property_topic_publish(); 
    property_report_SetTime(time);


}



/*****************************************************************************
    *
    *Function Name: void link_wifi_net_handler(void)
    *Function: 
    *Input Ref: NO
    *Return Ref:NO
    *
*****************************************************************************/	 	
void link_wifi_net_handler(void)
{
    static uint32_t uid;
//	uint8_t wifi_step_f=0;
	message[0] = '\0'; // 只需将第一个字符设为结束符，逻辑上就成了空字符串
    switch(link_net_step){



	        case 0:

              send_usart2_data((const uint8_t *)"AT+RST\r\n", strlen("AT+RST\r\n"));
        
              //delay_ms(200);//delay_ms(1000);
               tx_thread_sleep(300);//10ms * 100 = 1000s 
			  	link_net_step  = 1;
			  
			break;

            case 1:
               // WIFI_IC_ENABLE();
              
                send_usart2_data((const uint8_t *)"AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"));
                //delay_ms(100);
                tx_thread_sleep(200);
                uid =Get_Unique_ID_32bit();
			  
                 link_net_step  = 2;
                
                time_link_net_counter=0;

            break;

            case 2:
		
                 if(time_link_net_counter  > 6){
                    
					time_link_net_counter=0;

                        // WIFI_IC_ENABLE();
            			
                       message_len = sprintf((char *)message, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"UYIJIA01-%d\"\r\n", PRODUCT_ID, DEVICE_SECRET,uid);
            		   send_usart2_data((const uint8_t *)message,message_len);
            	  	   //delay_ms(1000);
                      
                        tx_thread_sleep(300);//10ms * 100 

						link_net_step  = 3;

                 }   
		

            break;


            case 3:
            if(time_link_net_counter   > 6){
                      time_link_net_counter  = 0;
                  
                send_usart2_data((const uint8_t *)"AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"));

			   // delay_ms(1000);
			     tx_thread_sleep(100);//10ms * 100 

				 link_net_step  = 4;
                 time_link_net_counter  =0;
                
            }
	

            break;


            case 4:
		
                 if(time_link_net_counter   > 8){
                   time_link_net_counter  = 0;

                   wifi_linking_tencent_f =1;
				   

					link_net_step  = 5;
          
                 // link_net_step = 5;
                 }
            
            break;

            case 5:
             
            //  uid =Get_Unique_ID_32bit();
	          message_len =  sprintf((char *)message, "AT+TCSAP=\"UYIJIA01-%d\"\r\n",uid);
              send_usart2_data((const uint8_t *)message,message_len);
	            tx_thread_sleep(300);//10ms * 100 
				link_net_step  = 6;
			
             

                    

            break;


            case 6:
                

            if(wifi_cofig_success_f==1){

              wifi_connected_success_f=0;
 //           HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//�?始连�?
             send_usart2_data((const uint8_t *)"AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"));
			 //delay_ms(1000);
			   tx_thread_sleep(200);//10ms * 100 

			  link_net_step  = 7;

                 time_link_net_counter  = 0;
            }
            
                   
            break;

            case 7:
	

            if( time_link_net_counter   > 7){

             if(wifi_connected_success_f==1){
			
	
                
               if(disp_second_f ==1)SendData_Set_Command(0x1F,0x01);//SendWifiData_To_Data(0x1F,0x01); //link wifi order 1 --link wifi net is success.
               //delay_ms(100);
			
			    link_net_step= 8;
               
				
		     }
		     else{
                
                  key_net_config_f =0;
                  link_net_step = 11;
                  if(disp_second_f == 1)SendData_Set_Command(0x1F,0);//SendWifiData_To_Data(0x1F,0x00) ;	 //Link wifi net is fail .WT.EDTI .2024.08.31
                 // delay_ms(100);
                  
                }
                
               }

            break;

            case 8:

              key_net_config_f =0;
             
			 
				MqttData_Publish_SetOpen(0x01);
		      
		        tx_thread_sleep(10);//10ms*10 =100ms
		        
				
			  link_net_step = 9; // this is flag: link wifi times 119s is over.
		    break;
				 

			 case 9: 
			 
			    link_net_step = 10;


			break;

			case 10:

			Subscriber_Data_FromCloud_Handler();
		
	           tx_thread_sleep(10);//  delay_ms(200);
        
			 link_net_step = 0xfe;

                   
            break;


            case 11:

			  key_net_config_f =0;

              link_net_step = 0xff;

            break;


            default:


            break;


        }

}






