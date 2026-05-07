#include "bsp.h"

#define KEY_MODE_SHORT   (1 << 0)
#define KEY_MODE_LONG    (1 << 1)

#define KEY_UP_SHORT     (1 << 2)
#define KEY_UP_LONG      (1 << 3)

#define KEY_DOWN_SHORT   (1 << 4)
#define KEY_DOWN_LONG    (1 << 5)

#define KEY_POWER_SHORT  (1 << 6)
#define KEY_POWER_LONG   (1 << 7)


/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
//#define DEMO_BYTE_POOL_SIZE 4096

//unsigned char free_memory[DEMO_BYTE_POOL_SIZE];



#define STACK_SIZE_KEY  256//256//512//1792//3072//2048//1024//896//768
#define STACK_SIZE_DECODER  256//512//256
#define STACK_SIZE_UI  1024//256
#define STACK_SIZE_EVENT  256


static TX_THREAD thread_decoder;
static TX_THREAD thread_key;
static TX_THREAD thread_ui;
static TX_THREAD thread_event;
/* 定义信号量 */
TX_SEMAPHORE wifi_semaphore;

TX_EVENT_FLAGS_GROUP key_event;


/*队列*/
//static TX_QUEUE uart1_rx_queue;
//static uint8_t uart1_rx_queue_buffer[UART1_RX_BUF_SIZE * sizeof(uint8_t)];


static UCHAR stack_decoder_pro[STACK_SIZE_DECODER];
static UCHAR stack_key_pro[STACK_SIZE_KEY];
static UCHAR stack_ui_pro[STACK_SIZE_UI];
static UCHAR stack_event_pro[STACK_SIZE_EVENT];



static void vTaskDecoderPro(ULONG thread_input);
static void vTaskKeyPro(ULONG thread_input);
static void vTaskUiPro(ULONG thread_input);
static void vTaskKeyEvent(ULONG thread_input);


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
   /* 创建信号量 */
       tx_semaphore_create(&wifi_semaphore, "WifiSemaphore", 0);

	   tx_event_flags_create(&key_event, "key_event");
	   
	/**************创建启动任务*********************/
    tx_thread_create(&thread_key,                     /* 任务控制块地址 */   
                       "KeyPro",                      /* 任务名 */
                       vTaskKeyPro,                   /* 启动任务函数地址 */
                       0,                             /* 传递给任务的参数 */
                       stack_key_pro,                 /* 堆栈基地址 */
                       STACK_SIZE_KEY,                /* 堆栈空间大小 */  
                       1,                             /* 任务优先级*/
                       1,                             /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,              /* 不开启时间片 */
                       TX_AUTO_START);                /* 创建后立即启动 */
   	   
	/**************创建统计任务*********************/
    tx_thread_create(&thread_decoder,                       /* 任务控制块地址 */    
                       "DecoderPro",                        /* 任务名 */
                       vTaskDecoderPro,                    /* 启动任务函数地址 */
                       0,                               /* 传递给任务的参数 */
                       stack_decoder_pro,                   /* 堆栈基地址 */
                       STACK_SIZE_DECODER,                  /* 堆栈空间大小 */  
                       0,                               /* 任务优先级*/
                       0,                               /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,                /* 不开启时间片 */
                       TX_AUTO_START);                  /* 创建后立即启动 */


				   

    tx_thread_create(&thread_ui,                    /* 任务控制块地址 */    
                       "UiPro",                     /* 任务名 */
                       vTaskUiPro,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       stack_ui_pro,                /* 堆栈基地址 */
                       STACK_SIZE_UI,            /* 堆栈空间大小 */  
                       3,                           /* 任务优先级*/
                       3,                           /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,            /* 不开启时间片 */
                       TX_AUTO_START);              /* 创建后立即启动 */

					   
    tx_thread_create(&thread_event,                    /* 任务控制块地址 */    
                       "EventPro",                     /* 任务名 */
                       vTaskKeyEvent,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       stack_event_pro,                /* 堆栈基地址 */
                       STACK_SIZE_EVENT,            /* 堆栈空间大小 */  
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
 static void vTaskDecoderPro(ULONG thread_input)
{
   (void)thread_input;  /* 消除未使用的参数警告 */
  
	while(1)
    {

       // 阻塞等待 ISR 投递
      if(tx_semaphore_get(&wifi_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
      {
          decoder_handler() ;
		    
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

    static uint16_t mode_cnt = 0;
    static uint16_t up_cnt = 0;
    static uint16_t down_cnt = 0;
    static uint16_t power_cnt = 0;

    const uint16_t LONG_PRESS_TIME = 200;   // 300 * 10ms = 3000ms
  
  
 
   while(1){
   	

	if(TSC_GetFlagStatus(TSC_Flag_TimeSlot) == SET){
		TSC_ClearFlagStatus(TSC_Flag_TimeSlot);
		TSC_StartCmd(DISABLE); //停止扫描
		TSC_Handle();		   //触摸处理
		TSC_StartCmd(ENABLE);  //开始扫描;	tk enable
	}

	//Key_Scan();

	// 物理层扫描
    if(KEY10_PIN){ //power key
		  power_cnt++;
            if(power_cnt == LONG_PRESS_TIME && discharge_f == 1){
                tx_event_flags_set(&key_event, KEY_POWER_LONG, TX_OR);
             }
    }
	else{
		 if(power_cnt > 1 && power_cnt < LONG_PRESS_TIME)
              tx_event_flags_set(&key_event, KEY_POWER_SHORT, TX_OR);

            power_cnt = 0;

	}
	
    if (KEY9_PIN){// == 1 && discharge_f ==1){ //key mode

	   if(discharge_f ==1){
		 mode_cnt++;
            if(mode_cnt == LONG_PRESS_TIME  ){
				tx_event_flags_set(&key_event, KEY_MODE_LONG, TX_OR);
               
            }
	   	}
    }
	else{
		if(mode_cnt > 1 && mode_cnt < LONG_PRESS_TIME){
                tx_event_flags_set(&key_event, KEY_MODE_SHORT, TX_OR);
		}
        mode_cnt = 0;

	}

 
    if (KEY8_PIN){ //up key
		//key_i = _UP_KEY_DOWN;
	   if(discharge_f ==1){
		up_cnt++;
        // if(up_cnt == LONG_PRESS_TIME)
              //  tx_event_flags_set(&key_event, KEY_UP_LONG, TX_OR);
	   	}

	}
	else{
	    if(up_cnt > 1 )
               tx_event_flags_set(&key_event, KEY_UP_SHORT, TX_OR);

          up_cnt = 0;
	}
	
    if (KEY7_PIN){ //dwon key
		if(discharge_f ==1){
		  down_cnt++;
          if(down_cnt == LONG_PRESS_TIME)
                tx_event_flags_set(&key_event, KEY_DOWN_LONG, TX_OR);
		}
    }
	else{
	      if(down_cnt > 1 && down_cnt < LONG_PRESS_TIME)
					tx_event_flags_set(&key_event, KEY_DOWN_SHORT, TX_OR);
	
				down_cnt = 0;

	}
  
    tx_thread_sleep(1);//10*1=10 
	
    } 
}

 /**
 * @brief  :  static void vTaskStart(void *pvParameters
 * @note    
 * @param   None
 * @retval  None
 */
 static void vTaskKeyEvent(ULONG thread_input)
{
   (void)thread_input;  /* 消除未使用的参数警告 */
    ULONG flags;
    UINT status;
	
	while(1)
    {
       status = tx_event_flags_get(&key_event,
                           0xFFFFFFFF,
                           TX_OR_CLEAR,
                           &flags,
                           TX_WAIT_FOREVER);//TX_NO_WAIT);//TX_WAIT_FOREVER);//
                           
     if(status == TX_SUCCESS){

	    if(flags & KEY_POWER_SHORT){

             key_power_short_handler();
		} 
	    else if(flags & KEY_POWER_LONG){
             key_power_long_handler();
		}  
        else if(flags & KEY_MODE_SHORT){
             key_mode_short_handler();
		} 
	    else if(flags & KEY_MODE_LONG){
             key_mode_long_handler();
		}  
        else if(flags & KEY_UP_SHORT){
			 key_up_short_handler();
		}    
	    else if(flags & KEY_DOWN_SHORT){
             key_down_short_handler();
		} 
	    else if(flags & KEY_DOWN_LONG){
			key_down_long_handler();
		}   

	   
     }
     
	   

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
    for (i = 0; i < STACK_SIZE_UI; i++)
    {
        if (stack_ui_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
	unused = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}

#endif 


