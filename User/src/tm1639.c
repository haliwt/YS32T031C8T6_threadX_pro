/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    tm1639.C
  * @author  Yspring Firmware Team  
  * @brief   tm1639 Source Code.
  ******************************************************************************      
*/

#include "tm1639.h"   
#include "ys32t031.h"



void TM1639_Write_Byte(uint8_t data);
void TM1639_Write_Display_Data(uint8_t *data, uint8_t len);



//向TM1639写入一个字节数据--低位在前
void TM1639_Write_Byte(uint8_t data)
{
    uint8_t i;
	
	  for(i=0;i<8;i++)
	  {
		    TM1639_CLK_L();

        if(data&0x01)
				{
				    TM1639_DIO_H();
				}
				else
				{
				    TM1639_DIO_L();
				}
				
        data>>=1;

				TM1639_CLK_H();
		}
}



//向TM1639写入显示数据
void TM1639_Write_Display_Data(uint8_t *data, uint8_t len)
{
    uint8_t i;
	
	  TM1639_STB_L();
	  TM1639_Write_Byte(0x40);   //设置数据命令 自动地址加1
	  TM1639_STB_H();
	
	  TM1639_STB_L();
	  TM1639_Write_Byte(0xc0);   //设置显示地址
	  for(i=0;i<len;i++)
    {
		    TM1639_Write_Byte(data[i]);  //设置显示数据
		}
	  TM1639_STB_H();
		
		TM1639_STB_L();
	  TM1639_Write_Byte(0x8F);   //设置显示控制命令   0x8F--最高亮度   0x88--中等亮度   0x83--最暗亮度    
		TM1639_STB_H();
}




















