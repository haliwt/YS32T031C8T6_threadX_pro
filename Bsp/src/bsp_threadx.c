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



#define STACK_SIZE_KEY  256//512//256//512//1792//3072//2048//1024//896//768
#define STACK_SIZE_DECODER  256//512//512//256
#define STACK_SIZE_UI    1536//1536//1024//256
#define STACK_SIZE_EVENT  512
//#define  TX_TIMER_THREAD_STACK_SIZE   128

static TX_THREAD s_decoder_thread;
static TX_THREAD s_key_thread;
static TX_THREAD s_ui_thread; //thread_ui
static TX_THREAD s_key_event_thread; //s-> static 
/* 定义信号量 */
TX_SEMAPHORE wifi_semaphore;

TX_EVENT_FLAGS_GROUP key_event;

//TX_TIMER beep_timer;
	
	

/*队列*/
//static TX_QUEUE uart1_rx_queue;
//static uint8_t uart1_rx_queue_buffer[UART1_RX_BUF_SIZE * sizeof(uint8_t)];


///static UCHAR s_decoder_stack[STACK_SIZE_DECODER];
///static UCHAR s_key_stack[STACK_SIZE_KEY];
///static UCHAR s_ui_stack[STACK_SIZE_UI];
//static UCHAR stack_event_pro[STACK_SIZE_EVENT];

__attribute__((aligned(8))) static UCHAR s_ui_stack[STACK_SIZE_UI];
__attribute__((aligned(8))) static UCHAR s_decoder_stack[STACK_SIZE_DECODER];
__attribute__((aligned(8))) static UCHAR s_key_stack[STACK_SIZE_KEY];
__attribute__((aligned(8))) static UCHAR s_key_event_stack[STACK_SIZE_EVENT];




static void decoder_thread_entry(ULONG thread_input);
static void key_thread_entry(ULONG thread_input);
static void ui_thread_entry(ULONG thread_input);
static void key_event_thread_entry(ULONG thread_input);
/* 定时器回调函数 */
//void my_timer_callback(ULONG input);
	
uint8_t down_cnt_long_f= 0;

#if DEBUG_ENABLE



static void debug_stack_ui_check(void);

static void debug_stack_key_check(void);

static void debug_stack_decoder_check(void);

static void debug_stack_key_event_check(void);

volatile ULONG unused_ui,unused_key,unused_decoder,unused_event ;


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

     #if DEBUG_ENABLE
    /* 2. 只有当 stack_msg_pro 是全局定义的静态数组时，这样写才有效 */
    memset(s_ui_stack, 0xEF, sizeof(stack_ui_pro));
    memset(s_key_stack, 0xEF, sizeof(stack_key_pro));
	memset(s_decoder_stack, 0xEF, sizeof(stack_decoder_pro));
	memset(s_key_event_stack, 0xEF, sizeof(stack_key_event));
    #endif 

    /* 3. 注册堆栈错误回调（推荐保持） */
 


    /* 创建信号量 */
       tx_semaphore_create(&wifi_semaphore, "WifiSemaphore", 0);

	   tx_event_flags_create(&key_event, "key_event");
	   
	/**************创建启动任务*********************/
    tx_thread_create(&s_key_thread,                     /* 任务控制块地址 */   
                       "KeyPro",                      /* 任务名 */
                       key_thread_entry,                   /* 启动任务函数地址 */
                       0,                             /* 传递给任务的参数 */
                       s_key_stack,                 /* 堆栈基地址 */
                       STACK_SIZE_KEY,                /* 堆栈空间大小 */  
                       0,                             /* 任务优先级*/
                       0,                             /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,              /* 不开启时间片 */
                       TX_AUTO_START);                /* 创建后立即启动 */
   	   
	/**************创建统计任务*********************/
    tx_thread_create(&s_decoder_thread,                       /* 任务控制块地址 */    
                       "DecoderPro",                        /* 任务名 */
                       decoder_thread_entry,                    /* 启动任务函数地址 */
                       0,                               /* 传递给任务的参数 */
                       s_decoder_stack,                   /* 堆栈基地址 */
                       STACK_SIZE_DECODER,                  /* 堆栈空间大小 */  
                       2,                               /* 任务优先级*/
                       2,                               /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,                /* 不开启时间片 */
                       TX_AUTO_START);                  /* 创建后立即启动 */


				   

    tx_thread_create(&s_ui_thread,                    /* 任务控制块地址 */    
                       "UiPro",                     /* 任务名 */
                       ui_thread_entry,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       s_ui_stack,                /* 堆栈基地址 */
                       STACK_SIZE_UI,            /* 堆栈空间大小 */  
                       3,                           /* 任务优先级*/
                       3,                           /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,            /* 不开启时间片 */
                       TX_AUTO_START);              /* 创建后立即启动 */

					   
    tx_thread_create(&s_key_event_thread,                    /* 任务控制块地址 */    
                       "EventPro",                     /* 任务名 */
                       key_event_thread_entry,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       s_key_event_stack,                /* 堆栈基地址 */
                       STACK_SIZE_EVENT,            /* 堆栈空间大小 */  
                       1,                           /* 任务优先级*/
                       1,                           /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,            /* 不开启时间片 */
                       TX_AUTO_START);              /* 创建后立即启动 */

	    /* 创建一个 20ms 周期的软件定时器 */
//		tx_timer_create(&beep_timer,				   /* 定时器控制块 */
//						"20msTimer",			   /* 名字 */
//						my_timer_callback,		   /* 回调函数 */
//						0,						   /* 回调参数 */
//						2, 					   /* initial_ticks：首次延迟 20 ticks */
//						2, 					   /* reschedule_ticks：周期 20 ticks */
//						TX_AUTO_ACTIVATE);		   /* 自动启动 */
	

}

/**
 * @brief  :  static void vTaskStart(void *pvParameters
 * @note    
 * @param   None
 * @retval  None
 */
 static void decoder_thread_entry(ULONG thread_input)
{
   (void)thread_input;  /* 消除未使用的参数警告 */
  
	while(1)
    {

       // 阻塞等待 ISR 投递
      if(tx_semaphore_get(&wifi_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
      {
          decoder_handler() ;
		    
       }
	   else{

         tx_thread_sleep(10);
	   }
	}
      
 }
 /**
  * @brief	:  static void vTaskStart(void *pvParameters
  * @note	 
  * @param	 None
  * @retval  None
  */
 static void ui_thread_entry(ULONG thread_input)
 {
   (void)thread_input;  /* 消除未使用的参数警告 */
  
  
   while(1){

    power_on_off_handler();

    IWDG_ReloadCounter();
    
	
#if DEBUG_ENABLE
	 debug_stack_ui_check();
#endif 
	tx_thread_sleep(1);//10ms * 2 = 20ms  
	
    } 
}

/**
  * @brief	:  static void vTaskStart(void *pvParameters
  * @note	 
  * @param	 None
  * @retval  None
  */
 static void key_thread_entry(ULONG thread_input)
 {
   (void)thread_input;  /* 消除未使用的参数警告 */

    static uint16_t mode_cnt = 0;
    static uint16_t up_cnt = 0;
    static uint16_t down_cnt = 0;
    static uint16_t power_cnt = 0;

    const uint16_t LONG_PRESS_TIME = 40;   // 300 * 10ms = 3000ms
  
  
 
   while(1){
   	
	// 物理层扫描
    if(KEY_POWER_VALUE() == KEY_DOWN){ //power key
		  power_cnt++;
            if(power_cnt == LONG_PRESS_TIME && discharge_f == 1){
                tx_event_flags_set(&key_event, KEY_POWER_LONG, TX_OR);
             }
    }
	else if(power_cnt > 0 && KEY_POWER_VALUE() == KEY_UP){
		    if(power_cnt > 0 && power_cnt < LONG_PRESS_TIME)
              tx_event_flags_set(&key_event, KEY_POWER_SHORT, TX_OR);

            power_cnt = 0;

	}
	else if(KEY_MODE_VALUE() == KEY_DOWN && discharge_f ==1){// == 1 && discharge_f ==1){ //key mode

	   
		 mode_cnt++;
            if(mode_cnt == LONG_PRESS_TIME  ){
				tx_event_flags_set(&key_event, KEY_MODE_LONG, TX_OR);
               
            }
	   	
    }
	else  if(mode_cnt > 0 && KEY_MODE_VALUE() == KEY_UP ){
		       if(mode_cnt > 0 && mode_cnt < LONG_PRESS_TIME)
                tx_event_flags_set(&key_event, KEY_MODE_SHORT, TX_OR);
		
        mode_cnt = 0;

	}
    else if (KEY_UP_VALUE() == KEY_DOWN && discharge_f ==1){ //up key
		//key_i = _UP_KEY_DOWN;
	  
		up_cnt++;
        if(up_cnt == LONG_PRESS_TIME)
               tx_event_flags_set(&key_event, KEY_UP_LONG, TX_OR);
	   	

	}
	else if(up_cnt > 0 && (KEY_UP_VALUE() == KEY_UP)){
	         if(up_cnt > 0 && up_cnt < LONG_PRESS_TIME)
               tx_event_flags_set(&key_event, KEY_UP_SHORT, TX_OR);

          up_cnt = 0;
	}
	else if (KEY_DOWN_VALUE() == KEY_DOWN && discharge_f ==1){ //dwon key
		
		  down_cnt++;
          if(down_cnt == LONG_PRESS_TIME && down_cnt_long_f ==0){
		  	    down_cnt_long_f =1;
                tx_event_flags_set(&key_event, KEY_DOWN_LONG, TX_OR);
          	}
		
    }
	else  if(down_cnt > 0 && KEY_DOWN_VALUE() == KEY_UP){
		 if( down_cnt > 0 && down_cnt < LONG_PRESS_TIME)
			tx_event_flags_set(&key_event, KEY_DOWN_SHORT, TX_OR);
			
	        down_cnt_long_f =0;
			down_cnt = 0;
	}
  
	
#if DEBUG_ENABLE
	 debug_stack_key_check();
#endif 
    tx_thread_sleep(6);//10ms*6=60 
	
    } 
}

 /**
 * @brief  :  static void vTaskStart(void *pvParameters
 * @note    
 * @param   None
 * @retval  None
 */
 static void key_event_thread_entry(ULONG thread_input)
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
		else if(flags & KEY_POWER_LONG && ptc_high_temperature_f ==0 && fan_warning_f ==0){
			    
             key_power_long_handler();
		} 
	    else if(flags & KEY_MODE_SHORT &&  ptc_high_temperature_f ==0 && fan_warning_f ==0){
             key_mode_short_handler();
		} 
		else if(flags & KEY_MODE_LONG && ptc_high_temperature_f ==0 && fan_warning_f ==0){
             key_mode_long_handler();
		} 
		else if(flags & KEY_UP_SHORT && ptc_high_temperature_f ==0 && fan_warning_f ==0){
			 key_up_short_handler();
		}
	    else if(flags & KEY_DOWN_SHORT && ptc_high_temperature_f ==0 && fan_warning_f ==0){
             key_down_short_handler();
		}
		
		if(flags & KEY_DOWN_LONG && ptc_high_temperature_f ==0 && fan_warning_f ==0){
			if(down_cnt_long_f ==1){
				down_cnt_long_f=2;
			 key_down_long_handler();

		   }
		}   
       
	   
	 
#if DEBUG_ENABLE
		  debug_stack_key_event_check();
#endif 
     }
	 else{

	    tx_thread_sleep(10);//WT.EDIT 2026-05-26

	 }
     
	   

	}
      
 }
 /********************************************************************************
	 **
	 *Function Name:
	 *Function : threadx software timer is callback function.
	 *Input Ref: 
	 *Return Ref:NO
	 *
 *******************************************************************************/
//void my_timer_callback(ULONG input)
// {
//    (void) input;
//	BEEP_OFF();

// }
// void open_beep_sound(void)
// {
//   tx_timer_activate(&beep_timer);
// }
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
static void debug_stack_ui_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计
    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_UI; i++)
    {
        if (s_ui_stack[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
	unused_ui = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}
static void debug_stack_key_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计


    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_KEY; i++)
    {
        if (s_key_stack[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_key = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}

static void debug_stack_decoder_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计


    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_DECODER; i++)
    {
        if (s_decoder_stack[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_decoder = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}

static void debug_stack_key_event_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计


    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_EVENT; i++)
    {
        if (stack_event_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_event = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}


#endif 


