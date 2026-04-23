/* USER CODE BEGIN header */
/**
  ******************************************************************************
  * @file    ys32t031_it.c 
  * @author  YSPRING Application Team
  * @version V1.0.0
  * @date    
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* USER CODE END header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ys32t031_it.h"
#include "uart.h"
#include "bsp.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN includes */

/* USER CODE END includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN typedef */

/* USER CODE END typedef */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN define */

/* USER CODE END define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN macro */

/* USER CODE END macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN variables */

/* USER CODE END variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN prototypes */

/* USER CODE END prototypes */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN External variables */

/* USER CODE END External variables */

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NMI_Handler */

  /* USER CODE END NMI_Handler */
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  /* USER CODE BEGIN HardFault_Handler 0 */

  /* USER CODE END HardFault_Handler 0 */
  while (1)
  {
    /* USER CODE BEGIN HardFault_Handler 1 */
    
    /* USER CODE END HardFault_Handler 1 */
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */

void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_Handler */

  /* USER CODE END SVC_Handler */
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#if 0
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_Handler */
    _tx_thread_context_switch();
  /* USER CODE END PendSV_Handler */
}
#endif 
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
#if 0
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_Handler */
    _tx_timer_interrupt();
  /* USER CODE END SysTick_Handler */
}
#endif 


/**
  * @brief  This function handles TIM6_LPTIM_IRQHandler.
  * @param  None
  * @retval None
  */
void TIM6_LPTIM_IRQHandler (void)
{

    volatile static uint8_t cnt10 =0,cnt100 =0,cnt1000,cnt1m=0,cnt20ms=0;

	volatile static uint8_t c100ms;
	
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET ) 
	{
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);

		time_5ms_f = 1;
		cnt10++;  

		if(cnt10 > 1){//5ms*2 =10ms

			cnt10 =0; 
			time_beep_counter++;
			gpro_t.time_10ms_f = 1;
			time_wifi_10ms_f = 1;
			
            if(++cnt20ms >=2){cnt20ms =0; gpro_t.time_20ms_f =1;}

			if(++cnt100 >=10){ //10* 10 = 100ms .
				cnt100 =0;
				
				gpro_t.time_100ms_f ++;

				gpro_t.time_200ms_f++;

			

				if(++cnt1000> 9){ // 100ms *10 =1000ms=1s 
					cnt1000 = 0;
					time_1s_counter ++ ;

				    gpro_t.time_1s_f =1;
				
					time_link_net_counter++;

					disp_switch_temp_humi++;
					time_set_hours_counter++;
					setting_timing_second ++;
					time_autolink_counter++;
					fan_one_minute_cuonter++;
				
					gpro_t.time_2s_f++;
					gpro_t.time_3s_f++;
					gpro_t.time_4s_f++;
					gpro_t.time_5s_f++;
					gpro_t.time_6s_f++;
				    gpro_t.time_7s_f++;
					
				     if(++gpro_t.time_base_1s_counter > 59){//1s *60 =60s 
					     gpro_t.time_base_1s_counter = 0;
					    
						gpro_t.time_1m_f++;
						gpro_t.time_1m_wifi_f = 1;
						
						
						
					}

				} 

			}

		}
	}
}



/**
  * @brief  This function handles Uart1 Handler.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
    uint8_t res;
	
    if(UART_GetFlagStatus(UART1, UART_FLAG_RXNE) == SET)
    {
		    UART_ClearFlag(UART1, UART_FLAG_RXNE);
			
			  res = UART1->RDR;
			
			  if(uart1_rx_cnt<sizeof(UART1_RX_BUF))
				{
				    UART1_RX_BUF[uart1_rx_cnt++] = res;
				}
				else
				{
				    uart1_rx_cnt = 0;
				}
		}	

    if(UART_GetFlagStatus(UART1, UART_FLAG_TC) == SET)
    {
        UART_ClearFlag(UART1, UART_FLAG_TC);
    }
    
    UART1->ICR = 0xFF;  //清除所有中断请求标志
    UART_ClearFlag(UART1, UART_FLAG_ORE);
}



/**
  * @brief  This function handles Uart2 Handler.
  * @param  wifi receive 
  * @retval None
  */
void UART2_IRQHandler(void)
{
	  uint8_t res;
	
    if(UART_GetFlagStatus(UART2, UART_FLAG_RXNE) == SET)
    {
		    UART_ClearFlag(UART2, UART_FLAG_RXNE);

			  res = UART2->RDR;
			  usart2_rx_callback_invoke(res);

		#if 0
			
	      if(uart2_rx_cnt<sizeof(UART2_RX_BUF))
				{
				    UART2_RX_BUF[uart2_rx_cnt++] = res;
				}
				else
				{
				    uart2_rx_cnt = 0;
				}
		#endif 
	}	

    if(UART_GetFlagStatus(UART2, UART_FLAG_TC) == SET)
    {
        UART_ClearFlag(UART2, UART_FLAG_TC);
    }
	 UART_ClearFlag(UART2, UART_FLAG_ORE);
    
    UART2->ICR = 0xFF;  //清除所有中断请求标志				
}











