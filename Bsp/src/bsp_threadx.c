#include "bsp.h"

/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
#define STACK_SIZE_ONE  512//1792//3072//2048//1024//896//768
#define STATC_SIZE_TWO  512//256


static TX_THREAD thread_msg;
static TX_THREAD thread_key;
/* 定义信号量 */
TX_SEMAPHORE wifi_semaphore;
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
void tx_application_define(void *first_unused_memory)
{

      /* 创建信号量 */
       tx_semaphore_create(&wifi_semaphore, "WifiSemaphore", 0);
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

       // 阻塞等待 ISR 投递
      if(tx_semaphore_get(&wifi_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
      {
           if(wifi_linking_tencent_f==1 ){
			 Wifi_Rx_InputInfo_Handler();

		    }
		    else{ //if( wifi_t.rx_data_success==1){
				wifi_rx_run_handler();

			}
                
       }
	 
	   

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
   	

	if(TSC_GetFlagStatus(TSC_Flag_TimeSlot) == SET){
		TSC_ClearFlagStatus(TSC_Flag_TimeSlot);
		TSC_StartCmd(DISABLE); //停止扫描
		TSC_Handle();		   //触摸处理
		TSC_StartCmd(ENABLE);  //开始扫描;	tk enable
	}

	Key_Scan();

	tx_thread_sleep(1);//1*10ms 
	
    } 
}
/********************************************************************************
	**
	*Function Name:
	*Function :
	*Input Ref: 
	*Return Ref:NO
	*
*******************************************************************************/
void wifi_semaphore_xtask(void)
{

  tx_semaphore_put(&wifi_semaphore);
    // 投递到队列
   // tx_queue_send(&uart1_rx_queue, &data, TX_NO_WAIT);

}

