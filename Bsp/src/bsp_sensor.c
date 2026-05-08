#include "bsp.h"


/*================= 内部静态函数声明 =================*/

static void DHT11_GPIO_Output(void);
static void DHT11_GPIO_Input(void);
static void DHT11_WritePin(uint8_t val);
static uint8_t DHT11_ReadPin(void);

static void TIM17_Init_1MHz(void);
static void Delay_US_dht11(uint16_t us);


static uint8_t DHT11_ReadByte(void);

/*================= 对外接口实现 =================*/

void delay_ms_dht11(uint16_t ms)
{
    ms = 1000*ms;
	Delay_US_dht11(ms);

}

void DHT11_Init(void)
{
    /* GPIO 默认配置为输出高电平 */
    RCC_AHB2PeriphClockCmd(DHT11_GPIO_CLK, ENABLE);

    DHT11_GPIO_Output();
    DHT11_WritePin(1);

    /* 初始化 TIM14 为 1MHz 计数 */
    TIM17_Init_1MHz();
}

/**
 * @brief  读取 DHT11 温湿度
 * @param  humi: 湿度输出指针
 * @param  temp: 温度输出指针
 * @retval 0: 成功，其他: 失败
 */
uint8_t DHT11_ReadData(uint8_t *humi, uint8_t *temp)
{
    uint8_t data[5] = {0};
    uint32_t timeout;
    UINT old_post;

    //if (humi == 0 || temp ==0)
        ///return 1;

    /* 1. 禁止 ThreadX 调度 + 全局中断（关键时序区） */
	
    old_post = tx_interrupt_control(TX_INT_DISABLE);

    __disable_irq();

    /* 2. 主机拉低 18ms */
    DHT11_GPIO_Output();
    DHT11_WritePin(0);
    Delay_US_dht11(18000);

    /* 3. 拉高 20~40us */
    DHT11_WritePin(1);
    Delay_US_dht11(30);

    /* 4. 切换输入，等待 DHT11 响应 */
    DHT11_GPIO_Input();
    Delay_US_dht11(5);

    /* 等待 DHT11 拉低（80us） */
    timeout = 0;
    while (DHT11_ReadPin())
    {
        if (++timeout > 300) goto error;
        Delay_US_dht11(1);
    }

    /* 等待 DHT11 拉高（80us） */
    timeout = 0;
    while (!DHT11_ReadPin())
    {
        if (++timeout > 300) goto error;
        Delay_US_dht11(1);
    }

    /* 等待 DHT11 再次拉低，开始传输数据 */
    timeout = 0;
    while (DHT11_ReadPin())
    {
        if (++timeout > 300) goto error;
        Delay_US_dht11(1);
    }

    /* 5. 读取 5 字节（40bit） */
    for (uint8_t i = 0; i < 5; i++)
    {
        data[i] = DHT11_ReadByte();
        if (data[i] == 0xFF) goto error;
    }

    /* 6. 恢复中断 & 调度 */
    __enable_irq();
    tx_interrupt_control(old_post);


    /* 7. 校验 */
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
        return 2;

	

    *humi = data[0];
    *temp = data[2];

	


    return 0;

error:
    __enable_irq();
    tx_interrupt_control(old_post);
    return 3;
}

/*================= 内部函数实现 =================*/

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

/*---------------- TIM14 1MHz 延时 ----------------*/

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

static void Delay_US_dht11(uint16_t us)
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
        // 48MHz 下，1us 约为 48 个周期
        // 除去 while 循环自身的减法、比较、跳转（约 6~9 个周期）
        // 剩余约 40 个周期左右用 NOP 填充
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }


	#endif 
}

/*---------------- DHT11 bit/byte 读取 ----------------*/



static uint8_t DHT11_ReadByte(void)
{

    #if 0
	uint8_t i, val = 0;

    for (i = 0; i < 8; i++)
    {
        uint8_t bit = DHT11_ReadBit();
        if (bit == 0xFF) return 0xFF;

        val <<= 1;
        val |= bit;
    }
    return val;

	#else
	
	uint8_t i,dat=0;
	 for(i=0;i<8;i++) 
		  {
				while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==0);
			
				Delay_US_dht11(40);
				
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
		  return dat;


	#endif 
}



uint8_t dht11_read_temp_humidity_value(void)
{
    uint8_t dht11_read_flag;
	
	dht11_read_flag = DHT11_ReadData(&humidity,&temperature);

	return dht11_read_flag;

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
}

