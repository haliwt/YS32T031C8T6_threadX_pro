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
uint8_t humidity;
uint8_t temperature;



static uint8_t DHT11_Read_Byte(void);
static  uint8_t Read_DHT11_Data(void);
uint8_t DHT11_Read(uint8_t humidity_value, uint8_t temperature_value);

/**
 * @brief       复位DHT11
 * @param       data: 要写入的数据
 * @retval      �??
 */

static void Delay_US(uint32_t us) 
{
    uint32_t count = us * 32;//8 // 根据 48MHz 主频估算，具体数值需微调
    while(count--) {
        __NOP();
		__NOP();
	    __NOP();
		__NOP();
	
    }
}
#if 0
/**
 * @brief       等待DHT11的回�??
 * @param       �??
 * @retval      0, DHT11正常
 *              1, DHT11异常/不存�??
 */
uint8_t dht11_check(void)
{
    uint16_t retry = 0;
    uint8_t rval = 0;

    while (GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN) && retry < 1000)  /* DHT11会拉�??83us */
    {
        retry++;
        Delay_US(4);
    }

    if (retry >= 100)
    {
        rval = 1;
    }
    else
    {
        retry = 0;

        while (!GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN) && retry < 1000) /* DHT11拉低后会再次拉高87us */
        {
            retry++;
            Delay_US(4);
        }
        if (retry >= 1000) rval = 1;
    }
    
    return rval;
}

/**
 * @brief       从DHT11读取�??个位
 * @param       �??
 * @retval      读取到的位�??: 0 / 1
 */
uint8_t dht11_read_bit(void)
{
    uint16_t retry = 0;

    while (GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN) && retry < 1000)  /* 等待变为低电�?? */
    {
        retry++;
        DelayUS(2);
    }

    retry = 0;

    while (!GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN) && retry < 1000) /* 等待变高电平 */
    {
        retry++;
        DelayUS(2);
    }

    DelayUS(40);//       /* 等待40us */

    if(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN))    /* 根据引脚状�?�返�?? bit */
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

/**
 * @brief       从DHT11读取�??个字�??
 * @param       �??
 * @retval      读到的数�??
 */
static uint8_t dht11_read_byte(void)
{
    uint8_t i, data = 0;

    for (i = 0; i < 8; i++)         /* 循环读取8位数�?? */
    {
        data <<= 1;                 /* 高位数据先输�??, 先左移一�?? */
        data |= dht11_read_bit();   /* 读取1bit数据 */
    }

    return data;
}


/**
 * @brief       复位DHT11
 * @param       data: 要写入的数据
 * @retval      �??
 */
static void dht11_reset(void)
{
    GPIO_ResetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);//DHT11_DQ_OUT(0);    /* 拉低DQ */
    DelayMS(20);       /* 拉低至少18ms */
    GPIO_SetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);//DHT11_DQ_OUT(1);    /* DQ=1 */
    DelayUS(30);       /* 主机拉高10~35us */
}

/**
 * @brief       从DHT11读取�??次数�??
 * @param       temp: 温度�??(范围:-20~50°)
 * @param       humi: 湿度�??(范围:5%~95%)
 * @retval      0, 正常.
 *              1, 失败
 */
uint8_t dht11_read_data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;
	
     SET_DHT11_DATA_OUT_MODE();
     dht11_reset();
	 dht11_check();
    __disable_irq();
    if (dht11_check() == 0)
    {
        for (i = 0; i < 5; i++)     /* 读取40位数�?? */
        {
            buf[i] = dht11_read_byte();
        }

        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0];
            *temp = buf[2];
        }
		
    }
    else
    {
		__enable_irq();

		return 1;
    }
    __enable_irq();
    return 0;
}

/**
 * @brief  read_
 * @param  mode: 0-显示温度�??1-显示湿度
 * @retval DHT11_Status 类型的操作结�??
 */
uint8_t dht11_read_temp_humidity_value(void)
{
   uint8_t status;  
	 // 读取DHT11数据
    status = dht11_read_data(&temperature,&humidity);
    //dht11_read_data(&dht11_data.temperature,&dht11_data.humidity);
	tx_thread_sleep(20);//osDelay(200);
    if(status != 0)
    {
        LED_PLASMA_ON();
        return status;
    }
	else{
       LED_PLASMA_OFF();
	   tx_thread_sleep(20);
	   LED_PLASMA_ON();
	   tx_thread_sleep(20);
	   LED_PLASMA_OFF();
	   return 1;

	}

	return  1;

}

/**
 * @brief  在TM1639上显示DHT11的温湿度数据
 * @param  mode: 0-显示温度�??1-显示湿度
 * @retval DHT11_Status 类型的操作结�??
 */




/**
 * @brief       初始化DHT11的IO�?? DQ 同时�??测DHT11的存�??
 * @param       �??
 * @retval      0, 正常
 *              1, 不存�??/不正�??
 */
#endif 

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
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DHT11_DATA_GPIO_PORT, &GPIO_InitStructure);

	//dht11_reset();
    //dht11_check();
}



/***********************************************************************
��������static u8 DHT11_Read_Byte(void)
����˵������
����ֵ�����ش�DHT11�϶�ȡ��һ��byte����
�������ã���DHT11�϶�ȡһ��byte����
************************************************************************/
static uint8_t DHT11_Read_Byte(void)    
{        

   #if 1
	uint8_t i,dat=0;

	//UINT old_post;
   // old_post = tx_interrupt_control(TX_INT_DISABLE); // 关中断
    UINT old_post;
	   // 3. 锁定中断，读取40位数据（关键时序区）
	    old_post = tx_interrupt_control(TX_INT_DISABLE); 
	   	__disable_irq();
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
		__enable_irq();
	 tx_interrupt_control(old_post); // 读完立即释放，防止影响 WiFi 任务
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
		  else{

		     dat &= (uint8_t)~(0x01 << (7 - i));

		  }
	  }
	  return dat;




	#endif 
}

static uint8_t dht11_data_buf[5];



//��DHT11�϶�ȡ��ʪ������
uint8_t Read_DHT11_Data(void)
{
   #if 0
	volatile uint32_t timeout = 0;
    uint8_t i, temp_val;
    UINT old_post;

    // 1. 发送起始信号
    SET_DHT11_DATA_OUT_MODE();
    GPIO_ResetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
    tx_thread_sleep(2); // 20ms-30ms
    
    GPIO_SetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
    Delay_US(30);
    SET_DHT11_DATA_IN_MODE();
	Delay_US(30);

    // 2. 检测响应 (带超时)
    timeout = 0;
    if(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0) {
       // if(++timeout > 10000) return 1; // 错误1：没响应
	    
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
	        humidity = dht11_data_buf[0];
	        temperature = dht11_data_buf[2];
	      
	        return 0; // 成功
	    }
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
#if 1
	uint8_t i, checksum;
		uint32_t timeout;
		UINT old_post;
	
		// 1. 发送起始信号 (这部分可以用系统的 Delay)
		SET_DHT11_DATA_OUT_MODE();
		GPIO_ResetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
		DelayMS(20); 
		
		GPIO_SetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
		DelayUS(30);
		
		// 2. 准备接收，进入临界区 (重点！)
		// 在读取这 40bit 数据期间，不能被 ThreadX 调度打断
		//old_post = tx_interrupt_control(TX_INT_DISABLE); 

		//__disable_irq();
	
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
		//__enable_irq();//tx_interrupt_control(old_post);
	
		// 6. 校验数据
		checksum = dht11_data_buf[0] + dht11_data_buf[1] + dht11_data_buf[2] + dht11_data_buf[3];
		if(dht11_data_buf[4] == checksum && checksum != 0) {
			humidity = dht11_data_buf[0];
			temperature = dht11_data_buf[2];
			return 0;
		}
		//__enable_irq();//tx_interrupt_control(old_post);
		return 1;

#endif 	
    
#if 0//#else 

    uint8_t i,temp;
    uint16_t timeout;
  UINT old_post;
	 

  //  uint8_t temp;

    // 1. 发送起始信号
  //  __disable_irq();
    SET_DHT11_DATA_OUT_MODE();
    GPIO_ResetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
    tx_thread_sleep(2);//Delay_MS(20);             // 至少18ms，这里用20ms
    
    GPIO_SetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
    Delay_US(30);             // 主机拉高等待 20-40us
    
    // 2. 切换输入模式，准备接收响应
    SET_DHT11_DATA_IN_MODE();
	 Delay_US(30);       

    // 3. 检测 DHT11 响应信号 (80us 低电平)
    if(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0){
    
        timeout=0;
		//old_post = tx_interrupt_control(TX_INT_DISABLE); // 关中断

	   while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0);
		// 等待 80us 低电平结束
	        while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1);

	        
			  old_post = tx_interrupt_control(TX_INT_DISABLE); // 关中断

			  // 4. 开始读取 5 字节数据
	            for(i = 0; i < 5; i++) {
	             dht11_data_buf[i] = DHT11_Read_Byte(); 
	           }

			tx_interrupt_control(old_post); // 恢复中断

				
			//__enable_irq();

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





#if 1
uint8_t dht11_read_temp_humidity_value(void)
{
    uint8_t dht11_read_flag;
	
	dht11_read_flag = DHT11_Read(humidity,temperature);

	if(dht11_read_flag==0){
      
	 // humidity = dht11_data_buf[0];
	 // temperature = dht11_data_buf[2];
	   LED_PLASMA_ON();
       return 0;
	}
	else{

       LED_PLASMA_OFF();
	   tx_thread_sleep(20);
	   LED_PLASMA_ON();
	   tx_thread_sleep(20);
	   LED_PLASMA_OFF();
	  
	   return 1;
		
	}


}


#endif 



static void DHT11_SetOutput(void)
{
    SET_DHT11_DATA_OUT_MODE();//LL_GPIO_SetPinMode(DHT11_PORT, DHT11_PIN, LL_GPIO_MODE_OUTPUT);
}

static void DHT11_SetInput(void)
{
   SET_DHT11_DATA_IN_MODE(); //LL_GPIO_SetPinMode(DHT11_PORT, DHT11_PIN, LL_GPIO_MODE_INPUT);
}

static void DHT11_Write(uint8_t val)
{
    if(val)
        GPIO_SetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);//LL_GPIO_SetOutputPin(DHT11_PORT, DHT11_PIN);
    else
        GPIO_ResetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);//LL_GPIO_ResetOutputPin(DHT11_PORT, DHT11_PIN);
}

static uint8_t DHT11_ReadPin(void)
{
    return GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);//LL_GPIO_IsInputPinSet(DHT11_PORT, DHT11_PIN);
}


uint8_t DHT11_Read(uint8_t humidity_value, uint8_t temperature_value)
{
    uint8_t data[5] = {0};
    uint8_t i, j;
	uint32_t interrupt_save;

    // 1. 进入临界区：在 ThreadX 中建议在此处就关闭中断
    interrupt_save = tx_interrupt_control(TX_INT_DISABLE);
	__disable_irq();

    /* 1. MCU 拉低 18ms */
    DHT11_SetOutput();
    DHT11_Write(0);
    Delay_US(18000);

    /* 2. 拉高 20~40us */
    DHT11_Write(1);
    Delay_US(30);

    /* 3. 切换为输入，等待 DHT11 响应 */
    DHT11_SetInput();

    /* 等待 DHT11 拉低（80us） */
    uint16_t timeout = 0;
    while (DHT11_ReadPin())
    {
        if (++timeout > 200) return 1; // 超时
        Delay_US(1);
    }

    /* 等待 DHT11 拉高（80us） */
    timeout = 0;
    while (!DHT11_ReadPin())
    {
        if (++timeout > 200) return 2;
        Delay_US(1);
    }

    /* 等待 DHT11 再次拉低，开始传输数据 */
    timeout = 0;
    while (DHT11_ReadPin())
    {
        if (++timeout > 200) return 3;
        Delay_US(1);
    }
  //  __disable_irq();
    /* 4. 开始读取 40bit 数据 */
    for (j = 0; j < 5; j++)
    {
        for (i = 0; i < 8; i++)
        {
            /* 等待低电平开始 */
            timeout = 0;
            while (!DHT11_ReadPin())
            {
                if (++timeout > 200) return 4;
                Delay_US(1);
            }

            /* 记录高电平长度 */
            Delay_US(40);  // 40us 后判断是 0 还是 1

            if (DHT11_ReadPin())
            {
                data[j] |= (1 << (7 - i));

                /* 等待高电平结束 */
                timeout = 0;
                while (DHT11_ReadPin())
                {
                    if (++timeout > 200) break;
                    Delay_US(1);
                }
            }
        }
    }
	
   // 4. 恢复中断
    tx_interrupt_control(interrupt_save);
	   /* 4. 开始读取 40bit

    /* 5. 校验 */
    if (data[0] + data[1] + data[2] + data[3] != data[4]){
		__enable_irq();

		return 5;
    }

    humidity_value = data[0];
    temperature_value = data[2];
	   __enable_irq();

    return 0;   // 成功
}





