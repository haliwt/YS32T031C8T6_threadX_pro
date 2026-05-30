#include "bsp.h"


/*================= ???????? =================*/

static void DHT11_GPIO_Output(void);
static void DHT11_GPIO_Input(void);
static void DHT11_WritePin(uint8_t val);
static uint8_t DHT11_ReadPin(void);

static void TIM17_Init_1MHz(void);
//static void Delay_US_dht11(uint16_t us);


static uint8_t DHT11_ReadByte(void);

/*================= ?????? =================*/

void delay_ms_dht11(uint16_t ms)
{
    ms = 1000*ms;
	Delay_US_dht11(ms);

}

void DHT11_Init(void)
{
    /* GPIO ?????????? */
    RCC_AHB2PeriphClockCmd(DHT11_GPIO_CLK, ENABLE);

    DHT11_GPIO_Output();
    DHT11_WritePin(1);

    /* ??? TIM14 ? 1MHz ?? */
    TIM17_Init_1MHz();
}

/**
 * @brief  ?? DHT11 ???
 * @param  humi: ??????
 * @param  temp: ??????
 * @retval 0: ??,??: ??
 */
uint8_t DHT11_ReadData(uint8_t *humi, uint8_t *temp)
{
    uint8_t data[5] = {0};
    uint32_t timeout;


    /* 2. ???? 18ms */
    DHT11_GPIO_Output();
    DHT11_WritePin(0);
    Delay_US_dht11(18000);

    /* 3. ?? 20~40us */
    DHT11_WritePin(1);
    Delay_US_dht11(30);

    /* 4. ????,?? DHT11 ?? */
    DHT11_GPIO_Input();
    Delay_US_dht11(5);

    /* ?? DHT11 ??(80us) */
    timeout = 0;
    while (DHT11_ReadPin())
    {
        if (++timeout > 1000) return 3; // ????????
        Delay_US_dht11(1);
    }

    /* ?? DHT11 ??(80us) */
    timeout = 0;
    while (!DHT11_ReadPin())
    {
        if (++timeout > 1000) return 3; // ????????
        Delay_US_dht11(1);
    }

    /* ?? DHT11 ????,?????? */
    timeout = 0;
    while (DHT11_ReadPin())
    {
        if (++timeout > 1000) return 3; // ????????
        Delay_US_dht11(1);
    }

    /* 5. ?? 5 ??(40bit) */
    for (uint8_t i = 0; i < 5; i++)
    {
        data[i] = DHT11_ReadByte();
        if (data[i] == 0xFF) return 3;
    }

    /* 7. ?? */
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
        return 2;

    *humi = data[0];
    *temp = data[2];

    return 0;
}

/*================= ?????? =================*/

static void DHT11_GPIO_Output(void)
{
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);

    gpio.GPIO_Pin   = DHT11_GPIO_PIN;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_High;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(DHT11_GPIO_PORT, &gpio);
}

static void DHT11_GPIO_Input(void)
{
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);

    gpio.GPIO_Pin  = DHT11_GPIO_PIN;
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(DHT11_GPIO_PORT, &gpio);
}

static void DHT11_WritePin(uint8_t val)
{
    if (val)
        GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    else
        GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

static uint8_t DHT11_ReadPin(void)
{
    return  (DHT11_GPIO_PORT->IDR & DHT11_GPIO_PIN) ? 1 : 0;
}

/*---------------- TIM14 1MHz ?? ----------------*/

static void TIM17_Init_1MHz(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

    TIM_TimeBaseInitTypeDef tim;
    TIM_TimeBaseStructInit(&tim);

    tim.TIM_Prescaler     = 48 - 1;   /* 48MHz / 48 = 1MHz */
    tim.TIM_Period        = 0xFFFF;
    tim.TIM_CounterMode   = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM17, &tim);
    TIM_Cmd(TIM17, ENABLE);
}

void Delay_US_dht11(uint16_t us)
{
   #if 1

	uint16_t start = TIM17->CNT;

    while ((uint16_t)(TIM17->CNT - start) < us)
    {
        /* busy wait */
    }
	#else 
	  while (us--)
    {
        // 48MHz ?,1us ?? 48 ???
        // ?? while ?????????????(? 6~9 ???)
        // ??? 40 ?????? NOP ??
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }


	#endif 
}

/*---------------- DHT11 bit/byte ?? ----------------*/
#if 0
static uint8_t DHT11_ReadByte(void)
{

    #if 0
     uint8_t i,dat=0;
	 for(i=0;i<8;i++) 
		  {
				while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==0);
			
				Delay_US_dht11(40);
				
				  if(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==1)
					{
						while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==1);
					 
						
						dat|=(uint8_t)(0x01 << (7 - i)); // ?�?
					}
					else
					{
						dat&=(uint8_t)~(0x01 << (7 - i)); // ?�?
					}
			}
		  return dat;
    #else 


	#endif 
}
#endif 
static uint8_t DHT11_ReadByte(void)
{
    uint8_t i, dat = 0;
    volatile uint32_t timeout; // 使用 volatile 防止被编译器优化

    for (i = 0; i < 8; i++) 
    {
        // ------------------ 预防卡死点 1 ------------------
        // 等待引脚变为高电平（跳过起始的低电平阶段）
        timeout = 0;
        while (GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0)
        {
            timeout++;
            if (timeout > 10000) // 门槛值，防止硬件损坏时死循环
            {
                return 0xFF; // 返回错误标志
            }
        }
        
        // 延时 40微秒 区分信号是 0 还是 1
        Delay_US_dht11(40);
        
        // 如果 40us 后依然是高电平，说明这一位是数据 "1"
        if (GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1)
        {
            // ------------------ 预防卡死点 2 ------------------
            // 数据是 1，需要等待引脚变回低电平，才能开始下一位的接收
            timeout = 0;
            while (GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1)
            {
                timeout++;
                if (timeout > 10000) 
                {
                    return 0xFF; // 返回错误标志
                }
            }
            
            dat |= (uint8_t)(0x01 << (7 - i)); // 写入 1
        }
        else
        {
            // 40us 后变回了低电平，说明这一位是数据 "0"
            //（此时引脚已经是低电平了，无需等待，直接清零对应位即可）
            dat &= (uint8_t)~(0x01 << (7 - i)); // 写入 0
        }
    }
    
    return dat;
}



uint8_t dht11_read_temp_humidity_value(void)
{
    uint8_t dht11_read_flag;
	
	dht11_read_flag = DHT11_ReadData(&humidity,&temperature);
    if(dht11_read_flag==0) 
		return dht11_read_flag;
	
}
  #if 0
	if(dht11_read_flag==0){
      
	 // humidity = dht11_data_buf[0];
	 // temperature = dht11_data_buf[2];
	    printf("humidity = %d \r\n",humidity);
	    printf("temperature = %d \r\n",temperature);
	   LED_PLASMA_ON();
       return 0;
	}
	else{

       LED_PLASMA_OFF();
	   tx_thread_sleep(20);
	   LED_PLASMA_ON();
	   tx_thread_sleep(20);
	   LED_PLASMA_OFF();


	   printf("read fail, code = %d\r\n", dht11_read_flag);
	   return 1;
		
	}

  #endif 


