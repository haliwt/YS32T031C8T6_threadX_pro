#include "bsp.h"

/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
#define STACK_SIZE_ONE  512//1792//3072//2048//1024//896//768
#define STATC_SIZE_TWO  512//256


static TX_THREAD thread_msg;
static TX_THREAD thread_key;
/* 定义信号量 */
TX_SEMAPHORE decoder_semaphore;
/*队列*/
//static TX_QUEUE uart1_rx_queue;
//static uint8_t uart1_rx_queue_buffer[UART1_RX_BUF_SIZE * sizeof(uint8_t)];


static UCHAR stack_msg_pro[STACK_SIZE_ONE];
static UCHAR stack_key_pro[STATC_SIZE_TWO];

static void vTaskMsgPro(ULONG thread_input);
static void vTaskKeyPro(ULONG thread_input);


/*
*********************************************************************************************************
*	函 数 名: tx_application_define
*	功能说明: ThreadX专用的任务创建，通信组件创建函数
*	形    参: first_unused_memory  未使用的地址空间
*	返 回 值: 无
*********************************************************************************************************
*/
void threadx_handler(void)
{
  
/*
	   如果实现任务CPU利用率统计的话，此函数仅用于实现启动任务，统计任务和空闲任务，其它任务在函数
	   AppTaskCreate里面创建。
	*/
	/**************创建启动任务*********************/
    tx_thread_create(&thread_key,              /* 任务控制块地址 */   
                       "KeyPro",              /* 任务名 */
                       vTaskKeyPro,                  /* 启动任务函数地址 */
                       0,                             /* 传递给任务的参数 */
                       stack_key_pro,            /* 堆栈基地址 */
                       STATC_SIZE_TWO,    /* 堆栈空间大小 */  
                       1,        /* 任务优先级*/
                       1,        /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,               /* 不开启时间片 */
                       TX_AUTO_START);                 /* 创建后立即启动 */
   	   
	/**************创建统计任务*********************/
    tx_thread_create(&thread_msg,               /* 任务控制块地址 */    
                       "MsgPro",              /* 任务名 */
                        vTaskMsgPro,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       stack_msg_pro,           /* 堆栈基地址 */
                       STACK_SIZE_ONE,    /* 堆栈空间大小 */  
                       2,        /* 任务优先级*/
                       2,        /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,             /* 不开启时间片 */
                       TX_AUTO_START);               /* 创建后立即启动 */

	#if 0
				   
	/**************创建空闲任务*********************/
    tx_thread_create(&AppTaskIdleTCB,               /* 任务控制块地址 */    
                       "App Task IDLE",              /* 任务名 */
                       AppTaskIDLE,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       &AppTaskIdleStk[0],           /* 堆栈基地址 */
                       APP_CFG_TASK_IDLE_STK_SIZE,    /* 堆栈空间大小 */  
                       APP_CFG_TASK_IDLE_PRIO,        /* 任务优先级*/
                       APP_CFG_TASK_IDLE_PRIO,        /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,             /* 不开启时间片 */
                       TX_AUTO_START);               /* 创建后立即启动 */

		#endif 
			   
}

/**
 * @brief  :  static void vTaskStart(void *pvParameters
 * @note    
 * @param   None
 * @retval  None
 */
 static void vTaskMsgPro(ULONG thread_input)
{
   (void)thread_input;  /* 消除未使用的参数警告 */
  
	while(1)
    {

//       // 阻塞等待 ISR 投递
//      if(tx_semaphore_get(&decoder_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
//      {
//              // 或者直接调用解码器
//             // if(gpro_t.decoder_success_flag==1){
//			  	 /// gpro_t.decoder_success_flag =0;
//                 decoder_handler();

//              //}
                
//       }
	    //LED_POWER_TOGGLE() ;
	    LED_POWER_ON();
	    tx_thread_sleep(500);
	    LED_POWER_OFF();
		tx_thread_sleep(500);
		LED_POWER_ON();
		tx_thread_sleep(500);
	    LED_POWER_OFF();
		tx_thread_sleep(500);//500*10ms= 5000ms = 5s 

	}
      
 }

/**
  * @brief	:  static void vTaskStart(void *pvParameters
  * @note	 
  * @param	 None
  * @retval  None
  */
 static void vTaskKeyPro(ULONG thread_input)
 {
   (void)thread_input;  /* 消除未使用的参数警告 */
  
  
   while(1){
   	
		LED_WIFI_ON() ;
	    tx_thread_sleep(100);
        LED_WIFI_OFF() ;
		tx_thread_sleep(100);
		LED_WIFI_ON() ;
	    tx_thread_sleep(100);
        LED_WIFI_OFF() ;
		tx_thread_sleep(100);
	
    } 
}

