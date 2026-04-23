#include "bsp.h"

/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
//#define DEMO_BYTE_POOL_SIZE 4096

//unsigned char free_memory[DEMO_BYTE_POOL_SIZE];



#define STACK_SIZE_ONE  512//1792//3072//2048//1024//896//768
#define STACK_SIZE_TWO  512//256
#define STACK_SIZE_THREE  1024//256


static TX_THREAD thread_msg;
static TX_THREAD thread_key;
static TX_THREAD thread_ui;
/* 定义信号量 */
TX_SEMAPHORE wifi_semaphore;



/*队列*/
//static TX_QUEUE uart1_rx_queue;
//static uint8_t uart1_rx_queue_buffer[UART1_RX_BUF_SIZE * sizeof(uint8_t)];


static UCHAR stack_msg_pro[STACK_SIZE_ONE];
static UCHAR stack_key_pro[STACK_SIZE_TWO];
static UCHAR stack_ui_pro[STACK_SIZE_THREE];


static void vTaskMsgPro(ULONG thread_input);
static void vTaskKeyPro(ULONG thread_input);
static void vTaskUiPro(ULONG thread_input);

#if DEBUG_ENABLE
ULONG unused =0;

static void debug_stack_check(void);
#endif 

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
     // 创建内存池
   /// tx_byte_pool_create(&my_pool, "my_pool", free_memory, DEMO_BYTE_POOL_SIZE);

	 // 从池中分配堆栈并创建线程
    ///void *stack_ptr;

	// tx_byte_allocate(&my_pool, &stack_ptr, DEMO_STACK_SIZE, TX_NO_WAIT);


	
      /* 创建信号量 */
       tx_semaphore_create(&wifi_semaphore, "WifiSemaphore", 0);
/*
	   如果实现任务CPU利用率统计的话，此函数仅用于实现启动任务，统计任务和空闲任务，其它任务在函数
	   AppTaskCreate里面创建。
	*/
	/**************创建启动任务*********************/
    tx_thread_create(&thread_key,                     /* 任务控制块地址 */   
                       "KeyPro",                      /* 任务名 */
                       vTaskKeyPro,                   /* 启动任务函数地址 */
                       0,                             /* 传递给任务的参数 */
                       stack_key_pro,                 /* 堆栈基地址 */
                       STACK_SIZE_TWO,                /* 堆栈空间大小 */  
                       1,                             /* 任务优先级*/
                       1,                             /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,              /* 不开启时间片 */
                       TX_AUTO_START);                /* 创建后立即启动 */
   	   
	/**************创建统计任务*********************/
    tx_thread_create(&thread_msg,                       /* 任务控制块地址 */    
                       "MsgPro",                        /* 任务名 */
                        vTaskMsgPro,                    /* 启动任务函数地址 */
                       0,                               /* 传递给任务的参数 */
                       stack_msg_pro,                   /* 堆栈基地址 */
                       STACK_SIZE_ONE,                  /* 堆栈空间大小 */  
                       0,                               /* 任务优先级*/
                       0,                               /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,                /* 不开启时间片 */
                       TX_AUTO_START);                  /* 创建后立即启动 */


				   

    tx_thread_create(&thread_ui,                    /* 任务控制块地址 */    
                       "UiPro",                     /* 任务名 */
                       vTaskUiPro,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       stack_ui_pro,                /* 堆栈基地址 */
                       STACK_SIZE_THREE,            /* 堆栈空间大小 */  
                       2,                           /* 任务优先级*/
                       2,                           /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,            /* 不开启时间片 */
                       TX_AUTO_START);              /* 创建后立即启动 */

	
			   
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
           //if(wifi_linking_tencent_f==1 ){
			 Wifi_Rx_InputInfo_Handler();

		   /// }
		   
       }
	 
	   

	}
      
 }
 /**
  * @brief	:  static void vTaskStart(void *pvParameters
  * @note	 
  * @param	 None
  * @retval  None
  */
 static void vTaskUiPro(ULONG thread_input)
 {
   (void)thread_input;  /* 消除未使用的参数警告 */
  
  
   while(1){

    power_onoff_handler();
    
	IWDG_ReloadCounter();
  
	tx_thread_sleep(1);//10ms * 10 = 100ms  
	
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

    tx_thread_sleep(1);//10*1=10 
	
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

#if DEBUG_ENABLE
static void debug_stack_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计
    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_ONE; i++)
    {
        if (stack_msg_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
	unused = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}

#endif 


