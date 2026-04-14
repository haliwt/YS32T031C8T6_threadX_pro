#include "ys32t031_tsc.h"
#include "ys32t031_tsc_lib.h"
#include "ys32t031_tsc_config.h"
#if !defined(USE_FULL_LL_DRIVER)
#include "delay.h"
#else
#include "ys32f032_ll_utils.h"
#endif

uint8_t CapDif[12] = 
{
	TOUCH0_DIF, TOUCH1_DIF, TOUCH2_DIF, TOUCH3_DIF,
	TOUCH4_DIF, TOUCH5_DIF, TOUCH6_DIF, TOUCH7_DIF,
	TOUCH8_DIF, TOUCH9_DIF, TOUCH10_DIF,TOUCH11_DIF, 
};

CapParaStruct CapThSpinnerCnt[TOUCH_NUM] = {0};

 uint8_t  TouchNum = TOUCH_NUM;
const uint8_t SOF_HOPE_SEL[] ={0,2,5,7};  //cap osc 附近频点 
uint8_t SetFilterOn = SET_FILTER_ON;
uint8_t SetFilterOff = SET_FILTER_OFF;
uint8_t CapDifCoeff = CAP_DIFF_COEFF;
uint8_t CapDiffApproxMax = CAP_DIFF_APPROX_MAX;
uint8_t CapReferCalibDebNum = CAP_REFER_CALIB_DEB_NUM;
uint16_t ReferCapCalibRange = REFER_CAP_CALIB_RANGE;
uint16_t BaseLineUpdateTime = BASE_LINE_UPDATE_TIME;
uint16_t TouchBaseLine = TK_BASE_LINE;

uint32_t  PersKeyLockTime = PERS_LOCK_KEY_TIME_TH;
uint32_t TouchChannel = TOUCH_CHANNEL;  
uint32_t TouchSetChannel = TOUCH_SETCHANNEL;
uint8_t filterDif = 0;
uint8_t filterCnt = 0;
uint8_t filterK = 0;
uint8_t capFilterStatus[TOUCH_NUM] = {0};
uint8_t capFilterDataCnt[TOUCH_NUM] = {0};
uint8_t filterN = 0;

TkDebounceStruct TkDebounce[TOUCH_NUM];
uint8_t CapDifLib[TOUCH_NUM];
uint8_t CapReferCalibDebCnt[TOUCH_NUM];
uint16_t CapRCC[TOUCH_NUM];
uint16_t CapReferMax[TOUCH_NUM];
uint16_t CapReferMin[TOUCH_NUM];
uint16_t CapTh[TOUCH_NUM]; 
uint16_t CapReSetCnt[TOUCH_NUM];
uint16_t Cap[TOUCH_NUM];
uint16_t LpButFilterBuff[TOUCH_NUM];
uint16_t CapRawCache[TOUCH_NUM][RAW_DATA_SIZE];
uint16_t SleepTimeCnt;
uint32_t CapThSum[TOUCH_NUM];
uint8_t TouchTrigCnt = TOUCH_TRIG_CNT;						// 触摸值突然改变增加，但不超过触摸阈值计数值，超过该值从新更新环境值


uint8_t TkRefOscNearFreqIndex; 
uint8_t bPowerOnCapThUpdate;
uint8_t CapThSampleCntH = 0;
uint8_t CapThSampleCntL = 0;
uint8_t RawDataCnt[TOUCH_NUM];
uint16_t CapReferCalibPersTime = 0;
uint16_t CapRaw[4][TOUCH_NUM];
uint16_t CapDynMinTh[TOUCH_NUM];
//uint16_t CapDynMaxTh[TOUCH_NUM];
uint16_t CapPeakMin[TOUCH_NUM];
//uint16_t CapPeakMax[TOUCH_NUM];
uint32_t CapReferCalibStatus = 0;   //平衡状态标志位
uint32_t CapReferCalibPreStatus = 0;  

//const uint16_t TK_REFER_CAP[] ={451,379,323};  
//const uint16_t TK_REFER_CAP[] ={288,255,215};  
//const uint8_t TK_REFER_FREQ[] = {0,0x08,0x10}; //3M , 7M, 11M


uint16_t get_elap(uint16_t CurCnt)
{
    uint16_t GetTimer = 0;

    if(CurCnt <= TIM6->CNT)
  	    GetTimer = TIM6->CNT - CurCnt;
    else
  	{
        GetTimer = 65535 - CurCnt;
        GetTimer += TIM6->CNT;
  	}
  	return GetTimer;
}



void TSC_Lib_GPIO_Config(void)
{
    static uint8_t i = 0;

    if(TouchChannel & 0x000001)   //TCS_IO0
        RCC->IOPENR |= 0x20UL;    //RCC_AHB2Periph_GPIOF
    if(TouchChannel & 0x0007E)
        RCC->IOPENR |= 0x1UL ;    //RCC_AHB2Periph_GPIOA
    if(TouchChannel & 0x00F80)
        RCC->IOPENR |= 0x2UL;     //RCC_AHB2Periph_GPIOB;    
    
    for(i = 0; i < 12; i++)
    {
        switch(TouchChannel & (1UL << i))
        {
            case 0x000001:  //PF9 -> TCS_IO0  
                GPIOF->MODER |= (0x03UL << 18);     //set AN input
                GPIOF->PUPDR &= ~(GPIO_PUPDR_PUPD0 << 18);          //no pull up & pull down       
            break;
          
            case 0x000002:  //PA8 -> TCS_IO1 
            case 0x000004:  //PA9 -> TCS_IO2 
            case 0x000008:  //PA10 -> TCS_IO3 
            case 0x000010:  //PA11 -> TCS_IO4 
            case 0x000020:  //PA12 -> TCS_IO5 
                GPIOA->MODER |= (0x03UL << (i+7)*2);     //set AN input
                GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0 << (i+7)*2);          //no pull up & pull down       
            break;

            case 0x000040:  //PA15 -> TCS_IO6
            GPIOA->MODER |= (0x03UL << (15)*2);     //set AN input
            GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0 << (15)*2);          //no pull up & pull down
            break;
            case 0x000080:  //PB3 -> TCS_IO7 
            case 0x000100:  //PB4 -> TCS_IO8 
            case 0x000200:  //PB5 -> TCS_IO9 
            case 0x000400:  //PB6 -> TCS_IO10 
            case 0x000800:  //PB7 -> TCS_IO11 
                GPIOB->MODER |= (0x03UL << (i-4)*2);     //set AN input
                GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD0 << (i-4)*2);          //no pull up & pull down        
            break;
            
            default:
            break;                                                               
        }
    }
}
/**
  * @brief  TSC Config
  * @param  None
  * @retval None
  */
void TSC_Lib_SleepConfig(void)
{
    uint8_t i;

    TSC_InitTypeDef TSC_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_TSC, ENABLE);

    //TSC_DeInit();
    TSC_StructInit(&TSC_InitStruct);

    TSC_InitStruct.TSC_ScanMode = TSC_ScanMode_PeriodicAuto1;
    TSC_InitStruct.TSC_ClkSrc = TSC_ClkSrc_LSI;
    TSC_InitStruct.TSC_T16Src = TSC_T16Src_Div16;
    TSC_InitStruct.TSC_TimeSlotSrc = TSC_TimeSlotSrc_RefOSC;
    TSC_InitStruct.TSC_RefOSCFreq = TSC_RefOSCFrequency;
    TSC_InitStruct.TSC_AutoScanModeTime = TSC_AutoScanModeTime_2n15;
    TSC_InitStruct.TSC_AutoScanModeTimeDiv = TSC_AutoScanModeTime_Div4;
    TSC_InitStruct.TSC_OSCHopSel = TSC_OSCHopSel_Sofe;
    TSC_InitStruct.TSC_OSCHopFreq = TSC_OSCHopFreq_1_429M;
    TSC_InitStruct.TSC_TouchKey = TouchSetChannel;
    TSC_Init(&TSC_InitStruct);

//    TSC_RefCAP_Config(50);     //参考振荡器电容值
//    TSC_SetTimeSlotCount(100); //时隙计数器预载值
//    for (i = 0; i < 12; i++)
//    {
//        TSC_SetKeyRefCAP(i, 250);
//    }

    TSC_FilterCmd(ENABLE); //使能滤波

    //TSC_ITConfig(TSC_IT_TimeSlot_WakeUp, ENABLE);
    TSC_ITConfig(TSC_IT_ThresholdMatches_WakeUp, ENABLE);
    //TSC_ITConfig(TSC_IT_TimeSlot, ENABLE);
    TSC_ITConfig(TSC_IT_ThresholdMatches, ENABLE);

    //根据实际情况设置匹配值(TSC TimeSlot[0_3])
    for(i = 0; i < 12; i++)
    {
        TSC_SetKeyThreshold(i, CapTh[i]-CapDifLib[i]);
    }

    TSC_KeyThresholdConfig(TouchSetChannel,TSC_LowerThreshold);
    TSC_AutoCompareThresholdCmd(ENABLE);
}

/*
void Tsc_Lib_DeInit(void)
{
    uint8_t i;

    TSC->TKCR1 = 0x00000902;    
    TSC->TKCR2 = 0x00;    
    TSC->TKTSCRR = 0x00;  
    TSC->TKIER = 0x00;    
    TSC->TKSR1 = 0x00000020;    
    TSC->TKSR2 = 0x00;    
    TSC->TKENCFGR = 0x00; 
    TSC->TKTHSCFGR = 0x00;
    TSC->TKROCPR = 0x00;  
    for(i=0; i<16; i++)
    {
        *(uint32_t *)(&TSC->TKK0CPR+i) = 0x00;
        *(uint32_t *)(&TSC->TKK0THR+i) = 0x00;
    }
}
*/

void TSC_Reg_ReInit(void)
{
    uint8_t i,j;
    
    CapReferCalibStatus = 0;          //平衡状态为0，表示正在平衡
    bPowerOnCapThUpdate = 0;
    CapReferCalibPersTime = 0;
    TkRefOscNearFreqIndex = 0;

    if(filterCnt > FILTERCNT)
    {
      filterCnt = FILTERCNT;
    }

    filterDif = FILTERDIF;
    filterK = FILTERK;      //最小K值
		filterN = FILTERN;
    for(i = 0, j = 0; i < 12; i++)   //阈值赋值
		{ 
				if(TouchChannel & (1UL << i))
					CapDifLib[j++] = CapDif[i];
		}     

    for(i = 0; i < TouchNum; i++)
    {
        RawDataCnt[i] = 0;
        CapRCC[i] = 512;
        CapReferMax[i] = 1024;
        CapReferMin[i] = 0;
        CapReSetCnt[i] = 0;
        CapReferCalibDebCnt[i] = CapReferCalibDebNum;
        CapTh[i] = TK_BASE_LINE;
        Cap[i] = TK_BASE_LINE;
        CapDynMinTh[i] = CapTh[i] - CapDifLib[i];
        //CapDynMaxTh[i] = CapTh[i] + CapDifLib[i];
        CapThSum[i] = 0;
        CapThSampleCntH = 0;
        CapThSampleCntL = 0; 
        CapReferCalibPreStatus |= (1UL << i);

        CapRaw[0][i] =  TK_BASE_LINE; 
        CapRaw[1][i] =  TK_BASE_LINE; 
        CapRaw[2][i] =  TK_BASE_LINE; 
        CapRaw[3][i] =  TK_BASE_LINE; 
        CapPeakMin[i] =  TK_BASE_LINE;
        //CapPeakMax[i] =  TK_BASE_LINE;
        LpButFilterBuff[i] = TK_BASE_LINE;; 
        for(j = 0; j < RAW_DATA_SIZE; j++)
            CapRawCache[i][j] = TK_BASE_LINE;        
    }


    
    for(i=0; i < TouchNum; i++)
    {
       *(uint32_t *)(&TSC->TKK0CPR+i) = 512; //auto mode ref cap set
       *(uint32_t *)(&TSC->TKK0THR+i) = CapTh[i] - CapDifLib[i];  //tk
    }
        
    TSC->TKROCPR = 512;                         //ref cap set 15pF, MAX = 1024  
    TSC->TKTSCRR = (256 - (TK_BASE_LINE >> 5));  //slot time count , MAX = 255;  
}


void TSC_Lib_Config(void)
{
    TSC_InitTypeDef TSC_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_TSC, ENABLE);
    TSC_StructInit(&TSC_InitStruct);

    //TSC_InitStruct.TSC_ScanMode = TSC_Lib_ScanMode_PeriodicAuto;
    TSC_InitStruct.TSC_ScanMode = TSC_Lib_ScanMode_Auto; 
    //TSC_InitStruct.TSC_ScanMode = TSC_Lib_ScanMode_Manual;
    TSC_InitStruct.TSC_ClkSrc = TSC_Lib_ClkSrc_HCLK;
    TSC_InitStruct.TSC_T16Src = TSC_Lib_T16Src_Div16; //calib reg
		TSC_InitStruct.TSC_TimeSlotSrc = TSC_TimeSlotSrc_RefOSC;
    TSC_InitStruct.TSC_TimeSlotSrc = TSC_Lib_TimeSlotSrc_Div1;
    TSC_InitStruct.TSC_RefOSCFreq = TSC_RefOSCFrequency;
    TSC_InitStruct.TSC_OSCHopSel = TSC_OSCHopSel_Sofe;//TSC_Lib_OSCHopSel_Sofe;// TSC_Lib_OSCHopSel_Hardware;
    TSC_InitStruct.TSC_OSCHopFreq = TSC_OSCHopFreq_1_429M;
    TSC_InitStruct.TSC_TouchKey = TouchSetChannel;
    
    #if(TSC_WAKE_CYCLE == 0)
        TSC_InitStruct.TSC_AutoScanModeTime = TSC_Lib_AutoScanModeTime_2n13;    //period autoscan
    #elif(TSC_WAKE_CYCLE == 1)
        TSC_InitStruct.TSC_AutoScanModeTime = TSC_Lib_AutoScanModeTime_2n14;    
    #elif(TSC_WAKE_CYCLE == 2)
        TSC_InitStruct.TSC_AutoScanModeTime = TSC_Lib_AutoScanModeTime_2n15; 
    #elif(TSC_WAKE_CYCLE == 3)
        TSC_InitStruct.TSC_AutoScanModeTime = TSC_Lib_AutoScanModeTime_2n16; 
    #elif(TSC_WAKE_CYCLE == 4)
        TSC_InitStruct.TSC_AutoScanModeTime = TSC_Lib_AutoScanModeTime_2n17; 
    #else   
        TSC_InitStruct.TSC_AutoScanModeTime = TSC_Lib_AutoScanModeTime_2n18;                      
    #endif
    
    #if(TSC_CYCLE_SCAN_TIMES == 0)
        TSC_InitStruct.TSC_AutoScanModeTimeDiv = TSC_Lib_AutoScanModeTime_Div2;
    #elif(TSC_CYCLE_SCAN_TIMES == 1)
        TSC_InitStruct.TSC_AutoScanModeTimeDiv = TSC_Lib_AutoScanModeTime_Div4;
    #elif(TSC_CYCLE_SCAN_TIMES == 2)
        TSC_InitStruct.TSC_AutoScanModeTimeDiv = TSC_Lib_AutoScanModeTime_Div8;
    #else                
        TSC_InitStruct.TSC_AutoScanModeTimeDiv = TSC_Lib_AutoScanModeTime_Div16;
    #endif
    
    TSC_Init(&TSC_InitStruct);
    TSC_FilterCmd(ENABLE); //使能滤波
}


//注： 在自动扫描模式下，关闭阈值匹配功能，否则 读取触摸值时 需要
// TSC->TKSR1 & TSC_TKSR1_TKRCOV == 1且  TSC->TKSR1 & TSC_TKSR1_TKBUSY  == 0  
void TSC_Lib_Init(void)
{
    TSC_Lib_GPIO_Config();
    TSC_Lib_Config();
    TSC_Reg_ReInit();
    
//	  __NOP();__NOP();
//	
//    TSC_StartCmd(ENABLE); //开始扫描
    
    // Timer6_Init();                //用于相关时基计数
    // NVIC_Redefine_Config();
}









void NVIC_Redefine_Init(NVIC_StructDef* NVIC_InitStruct)
{
  uint32_t tmppriority = 0x00;
  
  /* Check the parameters */
//  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
//  assert_param(IS_NVIC_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPriority));  
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
  {
    /* Compute the Corresponding IRQ Priority --------------------------------*/    
    tmppriority = NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel >> 0x02];
    tmppriority &= (uint32_t)(~(((uint32_t)0xFF) << ((NVIC_InitStruct->NVIC_IRQChannel & 0x03) * 8)));
    tmppriority |= (uint32_t)((((uint32_t)NVIC_InitStruct->NVIC_IRQChannelPriority << 6) & 0xFF) << ((NVIC_InitStruct->NVIC_IRQChannel & 0x03) * 8));    
    
    NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel >> 0x02] = tmppriority;
    
    /* Enable the Selected IRQ Channels --------------------------------------*/
    NVIC->ISER[0] = (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
  else
  {
    /* Disable the Selected IRQ Channels -------------------------------------*/
    NVIC->ICER[0] = (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
}




/**
  * @brief  Initializes the EXTI peripheral according to the specified
  *         parameters in the EXTI_InitStruct.
  * @param  EXTI_InitStruct: pointer to a EXTI_InitTypeDef structure that 
  *         contains the configuration information for the EXTI peripheral.
  * @retval None
  */
void EXTI_Redefine_Init(EXTI_StructDef* EXTI_InitStruct)
{
  uint32_t tmp = 0;

  /* Check the parameters */
//  assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
//  assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
//  assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));
//  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

  tmp = (uint32_t)EXTI_BASE;

  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
  {
    /* Clear EXTI line configuration */
    EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;

    tmp += EXTI_InitStruct->EXTI_Mode;

    *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;

    /* Clear Rising Falling edge configuration */
    EXTI->RTSR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->FTSR &= ~EXTI_InitStruct->EXTI_Line;

    /* Select the trigger for the selected interrupts */
    if (EXTI_InitStruct->EXTI_Trigger == 0x10)
    {
      /* Rising Falling edge */
      EXTI->RTSR |= EXTI_InitStruct->EXTI_Line;
      EXTI->FTSR |= EXTI_InitStruct->EXTI_Line;
    }
    else
    {
      tmp = (uint32_t)EXTI_BASE;
      tmp += EXTI_InitStruct->EXTI_Trigger;

      *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;
    }
  }
  else
  {
    tmp += EXTI_InitStruct->EXTI_Mode;

    /* Disable the selected external lines */
    *(__IO uint32_t *) tmp &= ~EXTI_InitStruct->EXTI_Line;
  }
}




//PWR_Regulator(PWR_Regulator_ON) = 0;
//PWR_Regulator(PWR_Regulator_LowPower) = 1,
void PWR_Redef_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry)
{
  /* Check the parameters */
//  assert_param(IS_PWR_REGULATOR(PWR_Regulator));
//  assert_param(IS_PWR_STOP_ENTRY(PWR_STOPEntry));
  if (PWR_Regulator != PWR_Redef_Regulator_ON)
  {
    /* Stop mode with Low-Power Regulator */
    PWR->CR1 |= PWR_CR1_LPR;
  }
  else
  {
    /* Stop mode with Main Regulator */
    PWR->CR1 &= (~PWR_CR1_LPR);
  }
  
  /* Set SLEEPDEEP bit of Cortex-M0 System Control Register */
  SCB->SCR |= (1UL << 2);
  
  /* Select STOP mode entry --------------------------------------------------*/
  if(PWR_STOPEntry == PWR_STOPEntry_WFI)
  {
    /* Request Wait For Interrupt */
    __WFI();
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)(1UL << 2)); 
  }
  else if (PWR_STOPEntry == PWR_STOPEntry_WFE)
  {
    /* Request Wait For Event */
    __SEV();  
    __WFE();
    __WFE();  
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)(1<<2));   
  }
  else
  {
    /* Set SLEEP on exit bit of Cortex-M0 System Control Register */
    SCB->SCR |= (1UL << 1);
  }
}





void NVIC_Redefine_Config(void)
{
   EXTI_StructDef EXTI_StructInf;
    
   /* EXTI外部中断线配置 */
   EXTI_StructInf.EXTI_Line = ((uint32_t)0x00100000);    //!< Internal interrupt line 20 //配置外部中断线20(TSC)
   EXTI_StructInf.EXTI_Mode = 0x84;                     //配置为事件模式 = 0x84 ,//配置为中断模式 = 0x80,
   EXTI_StructInf.EXTI_Trigger = 0x04;                  //配置中断的触发边沿为下降沿 EXTI_Trigger_Falling = 0x04 //EXTI_Trigger_Rising = 0
   EXTI_StructInf.EXTI_LineCmd = ENABLE;                //使能中断线
   EXTI_Redefine_Init(&EXTI_StructInf);
}




void TSC_Sleep_Proc(void)
{
   uint32_t TSC_TkCR1RegBak;    

    if(TouchResult)
    {
        SleepTimeCnt = 0;
        return;
    }   
   
    if(++SleepTimeCnt >= SLEEP_TIME)  //
    {
      SleepTimeCnt = 0;
			TSC_TkCR1RegBak = TSC->TKCR1;
   
				TSC_Lib_SleepConfig();
				TSC_StartCmd(ENABLE); //开始扫描
        DelayUS(100);
        while (TSC_GetFlagStatus(TSC_Flag_ScanKey_Busy) != SET)
            ;

        PWR->CR1 |= PWR_CR1_VOS;
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
        __NOP();

        while (TSC_GetFlagStatus(TSC_Flag_ScanKey_Busy) == SET)
            ;
        TSC_StartCmd(DISABLE); //停止扫描

        if (TSC_GetFlagStatus(TSC_Flag_TimeSlot) == SET)
        {
            TSC_ClearFlagStatus(TSC_Flag_TimeSlot);
            
            printf("TSC TimeSlot WakeUp");
       
        }

        if (TSC_GetFlagStatus(TSC_Flag_ThresholdMatches) == SET)
        {
            TSC_ClearFlagStatus(TSC_Flag_ThresholdMatches);
            

            printf("TSC Matches[%x][0_3]: ", TSC->TKSR2);
            TSC->TKSR2 = 0xFFFFFF;
            
        }
				TSC_StartCmd(DISABLE); //停止扫描;
				TSC_ClearFlagStatus(TSC_Flag_ThresholdMatches|TSC_Flag_TimeSlot|TSC_Flag_TimeOut_16BIT|TSC_Flag_TimeOut_CF16Bit);   //清零TKRCOV、TKCFOV、TK16OV、TKTHF,写'1'清零
       TSC_ClearKeyFlag(TOUCH_SETCHANNEL); //write '1' to clear TKSR2  清除按键状态 
       TSC->TKCR1 = TSC_TkCR1RegBak;   //恢复触摸时钟等配置
       TSC_StartCmd(ENABLE); //开始扫描; 
				//TSC_StartCmd(ENABLE); //开始扫描
    }
}





