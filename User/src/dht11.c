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
#include "bsp.h"


uint8_t Data_Read_Finish_f;
uint16_t data_read_interval;
uint16_t humidity;
uint16_t temperature;


void SET_DHT11_DATA_IN_MODE(void);
void SET_DHT11_DATA_OUT_MODE(void);
static uint8_t DHT11_Read_Byte(void);

static  uint8_t Read_DHT11_Data(void);




//DHT11���ݿ���Ϊ����ģʽ
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



//DHT11���ݿ���Ϊ���ģʽ
void SET_DHT11_DATA_OUT_MODE(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);	
	
	  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOA, ENABLE);	
	
    GPIO_InitStructure.GPIO_Pin = DHT11_DATA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP ;//GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DHT11_DATA_GPIO_PORT, &GPIO_InitStructure);
}

static void Delay_US(uint32_t us) 
{
    uint32_t count = us * 32;//8 // 根据 48MHz 主频估算，具体数值需微调
    while(count--) {
        __NOP(); 
		__NOP(); 
	    __NOP(); 
		__NOP(); 
		__NOP(); 
		__NOP(); 
		__NOP(); 
		__NOP(); 
		__NOP(); 
		__NOP(); 
		__NOP(); 
    }
}


/***********************************************************************
��������static u8 DHT11_Read_Byte(void)
����˵������
����ֵ�����ش�DHT11�϶�ȡ��һ��byte����
�������ã���DHT11�϶�ȡһ��byte����
************************************************************************/
static uint8_t DHT11_Read_Byte(void)    
{        

   #if 0
	uint8_t i,dat=0;
	SET_DHT11_DATA_IN_MODE();
	//UINT old_post;
   // old_post = tx_interrupt_control(TX_INT_DISABLE); // 关中断
    for(i=0;i<8;i++) 
	  {
		    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==0);
        
				
									
			  
				Delay_US(40);
			
			  if(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==1)
				{
				    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==1);
				 
					
				    dat|=(uint8_t)(0x01 << (7 - i)); // ����1
				}
				else
				{
				    dat&=(uint8_t)~(0x01 << (7 - i)); // ����0
				}
    }	
//	tx_interrupt_control(old_post); // 恢复中断
    return dat;

	#else 
	
	uint8_t i, dat = 0;
	  volatile uint32_t timeout;
	
	  for(i = 0; i < 8; i++) {
		  // 等待低电平结束（起始位 50us）
		  timeout = 0;
		  while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0) {
			  if(++timeout > 10000) break;
		  }
		  
		  // 延时 40us 区分 0 和 1
		  Delay_US(40);
		  
		  if(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1) {
			  dat |= (uint8_t)(0x01 << (7 - i));
			  // 等待高电平结束（1的持续时间）
			  timeout = 0;
			  while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1) {
				  if(++timeout > 10000) break;
			  }
		  }
	  }
	  return dat;




	#endif 
}

static uint8_t dht11_data_buf[5];



//��DHT11�϶�ȡ��ʪ������
uint8_t Read_DHT11_Data(void)
{
   #if 1
	volatile uint32_t timeout = 0;
    uint8_t i, temp_val;
    UINT old_post;

    // 1. 发送起始信号
    SET_DHT11_DATA_OUT_MODE();
    GPIO_ResetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
    tx_thread_sleep(3); // 20ms-30ms
    
    GPIO_SetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
    Delay_US(30);
    SET_DHT11_DATA_IN_MODE();

    // 2. 检测响应 (带超时)
    timeout = 0;
    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1) {
        if(++timeout > 10000) return 1; // 错误1：没响应
    }
    timeout = 0;
    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0) {
        if(++timeout > 10000) return 2; // 错误2：响应低电平不结束
    }
    timeout = 0;
    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1) {
        if(++timeout > 10000) return 3; // 错误3：响应高电平不结束
    }

    // 3. 锁定中断，读取40位数据（关键时序区）
    old_post = tx_interrupt_control(TX_INT_DISABLE); 
    
    for(i = 0; i < 5; i++) {
        dht11_data_buf[i] = DHT11_Read_Byte(); 
    }

    tx_interrupt_control(old_post); // 读完立即释放，防止影响 WiFi 任务

    // 4. 校验数据
    temp_val = dht11_data_buf[0] + dht11_data_buf[1] + dht11_data_buf[2] + dht11_data_buf[3];
    if(dht11_data_buf[4] == temp_val && temp_val != 0) {
        humidity = 77;//dht11_data_buf[0];
        temperature = 33;//dht11_data_buf[2];
        return 0; // 成功
    }

    return 4; // 校验失败
#endif 

#if 0
	volatile uint32_t timeout = 0;

    SET_DHT11_DATA_OUT_MODE();
    GPIO_ResetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
    tx_thread_sleep(2); // 延时 20ms
    
    GPIO_SetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
    Delay_US(30);
    SET_DHT11_DATA_IN_MODE();

    // 关键检测：等待低电平响应
    timeout = 0;
    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1) {
        if(++timeout > 10000) return 1; // 如果 10000 次循环还没变低，说明 DHT11 没响应
    }

    // 关键检测：等待高电平准备
    timeout = 0;
    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0) {
        if(++timeout > 10000) return 2; // 如果一直不回高，退出
    }

    return 0; // 走到这里说明检测到了 DHT11 响应
  
#endif 
#if 0
	uint8_t i, checksum;
		uint32_t timeout;
		UINT old_post;
	
		// 1. 发送起始信号 (这部分可以用系统的 Delay)
		SET_DHT11_DATA_OUT_MODE();
		GPIO_ResetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
		Delay_MS(20); 
		
		GPIO_SetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
		Delay_US(30);
		
		// 2. 准备接收，进入临界区 (重点！)
		// 在读取这 40bit 数据期间，不能被 ThreadX 调度打断
		//old_post = tx_interrupt_control(TX_INT_DISABLE); 

		__disable_irq();
	
		SET_DHT11_DATA_IN_MODE();
	
		// 3. 等待 DHT11 响应 (低电平 80us)
		timeout = 0;
		while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1) {
			if(++timeout > 1000) { tx_interrupt_control(old_post); return 1; }
		}
		
		// 等待响应低电平结束
		timeout = 0;
		while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0) {
			if(++timeout > 1000) { tx_interrupt_control(old_post); return 1; }
		}
	
		// 等待响应高电平结束 (准备开始数据传输)
		timeout = 0;
		while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1) {
			if(++timeout > 1000) { tx_interrupt_control(old_post); return 1; }
		}
	
		// 4. 连续读取 5 字节
		for(i = 0; i < 5; i++) {
			dht11_data_buf[i] = DHT11_Read_Byte();
		}
	
		// 5. 退出临界区，恢复中断
		__enable_irq();//tx_interrupt_control(old_post);
	
		// 6. 校验数据
		checksum = dht11_data_buf[0] + dht11_data_buf[1] + dht11_data_buf[2] + dht11_data_buf[3];
		if(dht11_data_buf[4] == checksum && checksum != 0) {
			humidity = dht11_data_buf[0];
			temperature = dht11_data_buf[2];
			return 0;
		}
		__enable_irq();//tx_interrupt_control(old_post);
		return 1;

#endif 	
    
#if 0//#else 

    uint8_t i,temp;
    uint16_t timeout;
  UINT old_post;
	 

  //  uint8_t temp;

    // 1. 发送起始信号
   
    SET_DHT11_DATA_OUT_MODE();
    GPIO_ResetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
     DelayMS(20);             // 至少18ms，这里用20ms
    
    GPIO_SetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
    DelayUS(30);             // 主机拉高等待 20-40us
    
    // 2. 切换输入模式，准备接收响应
    SET_DHT11_DATA_IN_MODE();
	 DelayUS(30);       

    // 3. 检测 DHT11 响应信号 (80us 低电平)
    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1);
    
        timeout=0;
		//old_post = tx_interrupt_control(TX_INT_DISABLE); // 关中断
	// 等待 80us 低电平结束
        while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0){

               if(++timeout > 50000) return 1; // 硬件不响应时退出，防止卡死

		  // 4. 开始读取 5 字节数据
            for(i = 0; i < 5; i++) {
        dht11_data_buf[i] = DHT11_Read_Byte(); 
    }

		//tx_interrupt_control(old_post); // 恢复中断

			humidity = dht11_data_buf[0];
            temperature = dht11_data_buf[2];

        // 5. 校验
        temp = dht11_data_buf[0] + dht11_data_buf[1] + dht11_data_buf[2] + dht11_data_buf[3];
        if(dht11_data_buf[4] == temp)
        {
            humidity = dht11_data_buf[0];
            temperature = dht11_data_buf[2];
            return 0; // 成功
        }
    }
		//tx_interrupt_control(old_post); // 恢复中断

		return 1;
#endif 

		
}






void dht11_read_temp_humidity_value(void)
{
    uint8_t dht11_read_flag;
	
	dht11_read_flag = Read_DHT11_Data();

	if(dht11_read_flag==0){
      
	 // humidity = dht11_data_buf[0];
	 // temperature = dht11_data_buf[2];		

	}
	else{

       LED_PLASMA_OFF();
	   tx_thread_sleep(20);
	   LED_PLASMA_ON();
	   tx_thread_sleep(20);
	    humidity = dht11_data_buf[0];
	     temperature = dht11_data_buf[2];	
	}


}











