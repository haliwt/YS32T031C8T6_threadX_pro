/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    dht11.C
  * @author  Yspring Firmware Team  
  * @brief   dht11 Source Code.
  ******************************************************************************      
*/

#include "dht11.h"   
#include "ys32t031.h"
#include "delay.h"



uint8_t Data_Read_Finish_f;
uint16_t data_read_interval;
uint16_t humidity;
uint16_t temperature;


void SET_DHT11_DATA_IN_MODE(void);
void SET_DHT11_DATA_OUT_MODE(void);
static uint8_t DHT11_Read_Byte(void);
void Read_DHT11_Data(void);



//DHT11数据口配为输入模式
void SET_DHT11_DATA_IN_MODE(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOA, ENABLE);	
	
    GPIO_InitStructure.GPIO_Pin = DHT11_DATA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DHT11_DATA_GPIO_PORT, &GPIO_InitStructure);
}



//DHT11数据口配为输出模式
void SET_DHT11_DATA_OUT_MODE(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);	
	
	  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOA, ENABLE);	
	
    GPIO_InitStructure.GPIO_Pin = DHT11_DATA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Low;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DHT11_DATA_GPIO_PORT, &GPIO_InitStructure);
}



/***********************************************************************
函数名：static u8 DHT11_Read_Byte(void)
参数说明：无
返回值：返回从DHT11上读取的一个byte数据
函数作用：从DHT11上读取一个byte数据
************************************************************************/
static uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat=0;
	
    for(i=0;i<8;i++) 
	  {
		    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==0)
        {
				
				}					
			  
				DelayUS(40);
			
			  if(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==1)
				{
				    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==1)
				    {
				
				    }	
					
				    dat|=(uint8_t)(1 << (7 - i)); // 数据1
				}
				else
				{
				    dat&=(uint8_t)~(1 << (7 - i)); // 数据0
				}
    }	
	
    return dat;
}



//从DHT11上读取温湿度数据
void Read_DHT11_Data(void)
{
	  uint8_t dht11_data_buf[5];
	  static uint8_t start_delay_time=4;
	
	  dht11_data_buf[0]=0;
	  dht11_data_buf[1]=0;
	  dht11_data_buf[2]=0;
	  dht11_data_buf[3]=0;
	  dht11_data_buf[4]=0;
	
    if(Data_Read_Finish_f)                                     //通信间隔：5ms*400=2000ms
    {
			  data_read_interval++;
			  if(data_read_interval>=399)
				{
				    data_read_interval = 0;
					  Data_Read_Finish_f = 0;
				}
		    return;
		}    	
	
	  //主机发送开始信号到DHT11即单片机拉低18ms之后再拉高20-40us
		if(start_delay_time==4)
		{
        SET_DHT11_DATA_OUT_MODE();							               //配置成输出模式
		}
    GPIO_ResetBits(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN);       //拉低数据线
		
		if(start_delay_time>0)                                     //数据线拉低20ms
		{
		    start_delay_time--;
			  return;
		}
		else
		{
		    start_delay_time = 4;
		}
		
    GPIO_SetBits(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN); 	       //拉高数据线 
	  DelayUS(30);     						                               //数据线拉高20~40us
	  GPIO_ResetBits(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN);
		
		SET_DHT11_DATA_IN_MODE();                                  //配置成输入模式
		
		uint8_t reply_cnt=0;
		while((GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==0)&&(reply_cnt<120))  
		{
			  DelayUS(1);
		    reply_cnt++;
		}
		if(reply_cnt>=120) 
    { 
		    Data_Read_Finish_f = 1;
		    data_read_interval = 0;
		   
		    return;
		}   
	  
		reply_cnt=0;
		while((GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==1)&&(reply_cnt<120))  
		{
			  DelayUS(1);
		    reply_cnt++;
		}
		if(reply_cnt>=120) 
    {
		    Data_Read_Finish_f = 1;
		    data_read_interval = 0;
		   
		    return;
		}
		
		dht11_data_buf[0] = DHT11_Read_Byte();  //湿度整数   
		dht11_data_buf[1] = DHT11_Read_Byte();  //湿度小数（DHT11不提供小数部分，可以忽略）   
		dht11_data_buf[2] = DHT11_Read_Byte();  //温度整数
		dht11_data_buf[3] = DHT11_Read_Byte();  //温度小数（DHT11不提供小数部分，可以忽略）
		dht11_data_buf[4] = DHT11_Read_Byte();  //校验和
		
		if(((uint8_t)(dht11_data_buf[0]+dht11_data_buf[1]+dht11_data_buf[2]+dht11_data_buf[3]))==dht11_data_buf[4])
		{
		    humidity = dht11_data_buf[0];
        temperature = dht11_data_buf[2]; 			
		}
		
		Data_Read_Finish_f = 1;
		data_read_interval = 0;
}


















