/* USER CODE BEGIN header */
/**
  ******************************************************************************
  * @file    main.c  
  * @author  YSPRING Application Team
  * @version 1.0.0
  * @date    2023.3.20
  * @brief   Main program
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* USER CODE END header */
#include "ys32t031.h"
#include "main.h"
#include "delay.h"
#include "system_init.h"
#include "uart.h"  
#include "tim.h"
#include "iwdg.h"
#include "key.h"
#include "adc.h"


#include "ys32t031_tsc.h"
#include "ys32t031_tsc_lib.h"
#include "ys32t031_tsc_config.h"

#include "bsp.h"

#include "tx_api.h"

///* ThreadX 强制要求的应用定义入口 */
//void tx_application_define(void *first_unused_memory)
//{
//    /* 
//       此时硬件和内核已就绪。
//       你可以在这里调用 tx_thread_create 来创建你的 WiFi 或传感器线程。
//       暂时留空以通过编译。
//    */
//    threadx_handler();
//}


/******************************************************
函数名：main
功能：主函数入口
参数：无
返回值：int
*******************************************************/
int main(void)
{
    RCC_Configuration();           //系统时钟配置   
	
    GPIO_Configuration();          //IO口配置
	
   Clear_Ram();                   //变量初始化
	
    UART1_Configuration(9600);    //串口1 用于和外接显示板通信
	
	UART2_Configuration(115200);     //串口2 用于和WIFI模组通信
	
    TIM1_Configuration();          //TIM1-PWM输出配置
	
    TIM3_Configuration();          //TIM3-PWM输出配置
	
    TIM6_Configuration();          //TIM6基本定时配置
	
    TIM14_Configuration();         //TIM14-PWM输出配置
	
    //IWDG_Configuration();          //独立看门狗配置

	
	  TSC_Lib_Init();                //触摸初始化
	
    ADC_Configuration();           //ADC配置
		
		NVIC_Configuration();          //中断嵌套向量配置
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		TSC_StartCmd(ENABLE);          //开始扫描
		bsp_init();
		DHT11_Init();
		Beep(BEEP_ONCE);	
		// printf("YST301C8T6 OK \n");
		
		  tx_kernel_enter(); 
		
		
    while(1)
    {
        //IWDG->KR = 0xAAAA;
        #if 0
			
			  if(TSC_GetFlagStatus(TSC_Flag_TimeSlot) == SET)
				{
				    TSC_ClearFlagStatus(TSC_Flag_TimeSlot);
					  TSC_StartCmd(DISABLE); //停止扫描
					  TSC_Handle();          //触摸处理
					  TSC_StartCmd(ENABLE);  //开始扫描;  tk enable
				}

			  task_scheduler();

			#endif     // 异步通信处理（USART1显示板、USART2 WiFi）
              //USART1_xdpProcess_Received();
              //USART2_wifiProcess_Received();
			  
    }
}


			  
			
			


