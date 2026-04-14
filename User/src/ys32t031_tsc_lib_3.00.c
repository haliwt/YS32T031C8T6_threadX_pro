#include "ys32t031_tsc_lib.h"
#include "ys32t031_tsc_config.h"
#include "uart.h"  


uint16_t RecailbStatus = 0;
uint32_t TouchResult = 0;
uint32_t TouchResultMap = 0;
uint32_t TouchResultTemp = 0;
uint32_t ResetTime = 0;

/***************************************************/
//	函数名：CapBalance_Restart
//
//	函数功能：电容重新平衡
/***************************************************/
void CapBalance_Restart(void)
{
    uint8_t TKFS_TEMP = 0;
    TSC_StartCmd(DISABLE); //停止扫描
    TKFS_TEMP = (uint8_t)(TSC->TKCR1>>TSC_TKCR1_TKFS_Pos)& 0x03;
    TSC->TKCR1 &= ~TSC_TKCR1_TKFS;
    switch(TKFS_TEMP )   //重新选择其他振荡器频率
    {
    case TSC_Lib_RefOSCFreq_1MHz:
        TSC->TKCR1 |= TSC_Lib_RefOSCFreq_5MHz;
    break;
    case TSC_Lib_RefOSCFreq_5MHz:
        TSC->TKCR1 |= TSC_Lib_RefOSCFreq_13MHz;
    break;
    case TSC_Lib_RefOSCFreq_13MHz:
        TSC->TKCR1 |= TSC_Lib_RefOSCFreq_1MHz;
    break;        
    default: 
    break;
    }
   
    TSC_Reg_ReInit();       //配置触摸寄存器初始参数

    TSC_StartCmd(ENABLE); //开始扫描
}

/***************************************************/
//	函数名：Slip_Averaging
//	函数功能：电容平衡微调控制环境
/***************************************************/
#if DYNAMIC_ENVIRONMENT 
void CapBalance_Spinner(uint16_t Cap_temp,uint8_t Index_Temp)
{ 
      if(Cap_temp > (TouchBaseLine + REFER_CAP_CALIB_RANGE))   //快学习平衡电容
      {  
         CapThSpinnerCnt[Index_Temp].SampleCntH =0; 
         CapThSpinnerCnt[Index_Temp].SampleCntL++;   
         if(CapThSpinnerCnt[Index_Temp].SampleCntL >= 1)       //因为这个计数之前已经有计数8，故填1即可 不能超过16次
         {   
            CapThSpinnerCnt[Index_Temp].SampleCntL = 0;     
            if(CapRCC[Index_Temp] > 0){
                CapRCC[Index_Temp]--;
                // CapRCC[Index_Temp]-=(Cap_temp-(TouchBaseLine+ REFER_CAP_CALIB_RANGE)>>1);  //根据变化快速更新
                TSC_SetKeyRefCAP(Index_Temp,CapRCC[Index_Temp]);
            }
         }
      } 
      else if(Cap_temp<(TouchBaseLine - REFER_CAP_CALIB_RANGE)) //慢学习平衡电容
      {
         CapThSpinnerCnt[Index_Temp].SampleCntL = 0;
         CapThSpinnerCnt[Index_Temp].SampleCntH++;
         if(CapThSpinnerCnt[Index_Temp].SampleCntH >= 1)    //不能超过16次
         {
            CapThSpinnerCnt[Index_Temp].SampleCntH = 0;
            if(CapRCC[Index_Temp] < 1022){
                CapRCC[Index_Temp]++;
                TSC_SetKeyRefCAP(Index_Temp,CapRCC[Index_Temp]);
            } 
         }
      }
}
#endif
/***************************************************/
//	函数名：TSC_Refer_Cap_Cabilb
//
//	函数功能：电容平衡
/***************************************************/
uint8_t  TSC_Refer_Cap_Cabilb(void)
{
    uint8_t i = 0;
    static uint8_t CapRef_Banlance_Flag = 0;
    
    for(i = 0; i < TouchNum; i++)
    {
        if(!(CapReferCalibStatus & (1UL << i)))
        {
            CapReferCalibDebCnt[i]--;
            if(!CapReferCalibDebCnt[i])     //累加N次数据更新一笔
            {
                CapReferCalibDebCnt[i] = CapReferCalibDebNum;
//                if(TSC_GetKeyCount(i) > TouchBaseLine)     //当前环境频率计数值>基准值  此时频率偏大    
//                {
//                    if(TSC_GetKeyCount(i) <= (TouchBaseLine + ReferCapCalibRange))//当前环境频率计数值<=基准值+基准值偏差范围时，则当前通道的参考容值记录下来
//                    {
//                        CapReferCalibStatus |= (1UL << i);  //
//                        CapReferMax[i] = 1024;
//                        CapReferMin[i] = 0;
//                    }
//                    else
//                    {
//                        CapReferMax[i] =  TSC_GetKeyRefCAP(i);
//                        CapRCC[i] -= ((CapReferMax[i]-CapReferMin[i])>>1);
//                        TSC_SetKeyRefCAP(i,CapRCC[i]);      //*(uint32_t *)(&TSC->TKK0CPR+i) -= ((*(uint32_t *)(&TSC->TKK0CPR+i) - CapReferMin[i]) >> 1); 
//                    }
//                }
//                else
//                {
//                    if(TSC_GetKeyCount(i) >= (TouchBaseLine - ReferCapCalibRange))
//                    {
//                        CapReferCalibStatus |= (1UL << i); 
//                        CapReferMax[i] = 1024;
//                        CapReferMin[i] = 0;
//                    }
//                    else
//                    {
//                        CapReferMin[i] = TSC_GetKeyRefCAP(i);
//                        CapRCC[i] += ((CapReferMax[i]-CapReferMin[i])>>1);
//                        TSC_SetKeyRefCAP(i,CapRCC[i]);
//                        // CapReferMin[i] =  *(uint32_t *)(&TSC->TKK0CPR+i);
//                        // *(uint32_t *)(&TSC->TKK0CPR+i) += ((CapReferMax[i] - *(uint32_t *)(&TSC->TKK0CPR+i)) >> 1); 
//                    }
//                }

//                if((1023 <= CapRCC[i]) || (1 >= CapRCC[i]))					//当前通道平衡电容值已到最大最小值强制结束其电容平衡
//                {
//                    CapReferCalibStatus |= (1UL << i);
//                }
                
                if(TSC_GetKeyCount(i) > TouchBaseLine)     //当前环境频率计数值>基准值  此时频率偏大    
                {
                    if(TSC_GetKeyCount(i) <= (TouchBaseLine + ReferCapCalibRange))//当前环境频率计数值<=基准值+基准值偏差范围时，则当前通道的参考容值记录下来
                    {
                        CapReferCalibStatus |= (1UL << i);  //
                        CapReferMax[i] = 1024;
                        CapReferMin[i] = 0;
                    }
                    else
                    {
                        CapReferMax[i] = CapReferMin[i]+(CapReferMax[i]-CapReferMin[i])/2 -1;
                        CapRCC[i] = CapReferMin[i]+(CapReferMax[i]-CapReferMin[i])/2;
                        TSC_SetKeyRefCAP(i,CapRCC[i]);
                    }
                }else{
                    if(TSC_GetKeyRefCAP(i) >= (TouchBaseLine + ReferCapCalibRange))//当前环境频率计数值<=基准值+基准值偏差范围时，则当前通道的参考容值记录下来
                    {
                        CapReferCalibStatus |= (1UL << i);  //
                        CapReferMax[i] = 1024;
                        CapReferMin[i] = 0;
                    }
                    else
                    {
                        CapReferMin[i] = CapReferMin[i]+(CapReferMax[i]-CapReferMin[i])/2 +1;
                        CapRCC[i] = CapReferMin[i]+(CapReferMax[i]-CapReferMin[i])/2;
                        TSC_SetKeyRefCAP(i,CapRCC[i]);
                    }
                }
                //防止平衡不了的情况,强制平衡退出
                if(CapReferMax[i]<CapReferMin[i] ){
                    CapReferCalibStatus |= (1UL << i);
                    // CapBalance_Restart();
                    // return 1;
                }
                
            } 
        }
        if(CapReferCalibPreStatus == CapReferCalibStatus){  //全部通道平衡之后返回1
            if(0==CapRef_Banlance_Flag){
                CapRef_Banlance_Flag = 1;
                //1M  可以不用处理
                //5M处理
                #if TSC_TKCR1_TKFS_5MHz==TSC_RefOSCFrequency
                // for(i=0;i<TouchNum;i++){
                //     if((CapRCC[i])>40){    
                //         TSC_SetKeyRefCAP(i,CapRCC[i]-40);        //最后放大三倍再重新赋值
                //     }else{
                //         TSC_SetKeyRefCAP(i,CapRCC[i]);
                //     }
                //     // TSC_SetKeyRefCAP(i,CapRCC[i]+20);
                // } 
                #elif TSC_TKCR1_TKFS_13MHz==TSC_RefOSCFrequency
                // for(i=0;i<TouchNum;i++){
                //     if((CapRCC[i]*3)>40){    
                //         TSC_SetKeyRefCAP(i,CapRCC[i]*3-40);        //最后放大三倍再重新赋值
                //     }else{
                //         TSC_SetKeyRefCAP(i,CapRCC[i]*3);
                //     }
                    
                // }
                #else
                #endif
            }   
            //13M  
                  
            return 1;
        }else{
            CapRef_Banlance_Flag = 0;
        }
       
    }
    return 0;
}




//y[n] = A*x[n] + (1-A)y[n-1]
/* uint16_t lp_butterWorth(uint16_t CurrentCap, uint16_t *pFilter,uint8_t FilterLevel)
{
   uint16_t x1 = 0,y1 = 0, Ay1 = 0;
   uint16_t temp0 = 0;

   x1 = CurrentCap >> FilterLevel;
   temp0 = *pFilter >> FilterLevel;
   Ay1 = *pFilter - temp0;
   y1 = x1 + Ay1;
   *pFilter = y1;
   return y1 ;
} */

//FilterLevel:1-100
//y[n] = A*x[n] + (1-A)y[n-1]  //一阶滤波
void lp_FirstFilter(uint16_t CurrentCap,uint8_t Index,uint8_t FilterLevel)
{
    uint32_t x1 = 0,y1 = 0, Ay1 = 0;
    if(FilterLevel >=100) FilterLevel = 100; //限制最大100%
    if(CurrentCap>(Cap[Index] +10)){         //按键释放，暂时不滤波
        FilterLevel = 90;
    }    
    x1 = CurrentCap *FilterLevel/100;
    Ay1 = Cap[Index]*FilterLevel/100;
    y1 = Cap[Index] - Ay1;
    Cap[Index] = (uint16_t)(y1+x1);
}


/**********************************************************
 * @brief	对电容值进行动态一阶滤波
 * @param	index: 数据索引
 * @return	无
**********************************************************/
void touchFilterHandle(uint8_t index,uint16_t capTemp)
{
	uint8_t capAlterDirect = 0;
	uint16_t capRealTemp = 0;
	/**
	 * 计算电容值前后差值，标记变化反向
	**/
	capRealTemp = capTemp;
	if (0 == Cap[index]) 
	{
		//第一次获取电容值
		Cap[index] = capTemp;
        
	}

	if (capTemp < Cap[index]) 
	{
		//电容值减小，计算差值，标记1
		capAlterDirect = 1;
		capTemp = (Cap[index] - capTemp);
	}
	else if (capTemp > Cap[index])   
	{
		//电容值增大，计算差值，标记2
		capAlterDirect = 2;
		capTemp = (capTemp - Cap[index]);
	}

	//限制最大变化量，防止计算溢出
	if (1000 <= capTemp)
	{
		capTemp = 1000;
	}

	/**
	 * 根据电容值变化方向以及速度调整K值
	**/
	if ((capFilterStatus[index] >> 4) == capAlterDirect) //
	{
		if (0 != capAlterDirect)
		{
			//同向变化检测消抖
			++capFilterDataCnt[index]; 
			if (filterDif <= capTemp)
			{
				//数据前后变化超过设置阈值加快调整K值
				capFilterDataCnt[index] += 2; 
			}
			if (filterCnt <= capFilterDataCnt[index])
			{
				//确定同向变化后设置新K值，
				capFilterDataCnt[index] = 0;
				
				//K分8级(0、2、4、8、16、32、64、128)
				if (7 <= ((++capFilterStatus[index]) & 0x0f)) 
				{
					capFilterStatus[index] &= 0xf0;
					capFilterStatus[index] |= 7;
				}
			}
		}
		else
		{
			//方向改变，恢复最小K值
			capFilterDataCnt[index] = 0;
			capFilterStatus[index] &= 0xf0;
			capFilterStatus[index] |= filterK; 
		}
	}
	else //方向改变
	{
		//以最小K值开始
		capFilterDataCnt[index] = 0;
		capFilterStatus[index] &= 0xf0;
		capFilterStatus[index] |= filterK; 
	}

	capFilterStatus[index] &= 0x0f;
	capFilterStatus[index] |= (capAlterDirect << 4);

	/**
	 * 一阶滤波算法
	**/
	capTemp <<= (capFilterStatus[index] & 0x0f); 
	capTemp += 128;  //四舍五入
	capTemp >>= 8;   //y=k/256*y1  y1为变化值

	switch (capAlterDirect)
	{
	case 0:
		capTemp = Cap[index];
		break;
	case 1:
		capTemp = (Cap[index] - capTemp);
		break;
	case 2:
		// capTemp = (Cap[index] + capTemp);       //释放的时候快速回到当前释放位置
        capTemp = capRealTemp;
		break;
	default:
		break;
	}

	Cap[index] = capTemp;

}

/**********************************************************
 * @brief	对电容值进行限幅滤波  这个限制的幅值根据阈值来调整
 * @param	index: 数据索引  
 * @return	无
 * 说明：对偶然出现的噪声有作用
**********************************************************/
uint16_t TouchAmpLimitDeal(uint8_t index,uint16_t capTemp)
{
    uint16_t capTempDif = 0;    //当前值与上一次的差值
    //uint16_t caplastTemp = 0;   //暂存上一次滤波值
    if(capTemp>Cap[index]){
        capTempDif = capTemp - Cap[index];
        if(capTempDif>(CapDifLib[index]+CapDifLib[index])){  //当前值大于上次cap值
            capTemp =Cap[index] + capTempDif>>3;   
        }
    }else{
        capTempDif = Cap[index] - capTemp;
        if(capTempDif>(CapDifLib[index]+CapDifLib[index])){  //当前值大于上次cap值
            capTemp =Cap[index] - capTempDif>>3;   
        }
    }
    
    return capTemp;
}  


void TSC_Rawdata_Handle(void)
{
    uint8_t i = 0 ;
//		uint8_t FreqIndex = 0;
//    uint16_t TempCapMin, TempCapMax;
//    uint32_t TempSum;
    uint16_t TempCap;
	for(i = 0; i < TouchNum; i++)
    {
        CapRaw[TkRefOscNearFreqIndex][i] = *(uint32_t *)(&TSC->TKK0CNTR+i);
        TempCap = CapRaw[TkRefOscNearFreqIndex][i];

        // TempCap = TouchAmpLimitDeal(i,TempCap);       //限幅滤波  针对尖峰噪声
    #if FILTER_DYNAMIC
        touchFilterHandle(i,TempCap);     //动态一阶滤波处理
    #else 
        lp_FirstFilter(TempCap,i,filterN);       //一阶滤波
    #endif
        
        // TempSum = 0;
        // for(FreqIndex = 0; FreqIndex < 4; FreqIndex++)      //滑动平均累加
        //     TempSum += CapRaw[FreqIndex][i];

        // CapRawCache[i][RawDataCnt[i]++] = (uint16_t)(TempSum >>= 2);
        // if(RawDataCnt[i] >= RAW_DATA_SIZE)   RawDataCnt[i] = 0;
        // TempSum = 0;
        // TempCapMin = 0xffff;            
        // TempCapMax = 0;
        // for(FreqIndex = 0; FreqIndex < RAW_DATA_SIZE; FreqIndex++)  //滑动平均
        // {
        //     if(CapRawCache[i][FreqIndex] < TempCapMin) 
        //         TempCapMin = CapRawCache[i][FreqIndex];
        //     if(CapRawCache[i][FreqIndex] > TempCapMax)
        //         TempCapMax = CapRawCache[i][FreqIndex];
        //     TempSum +=  CapRawCache[i][FreqIndex];  
        // }
        // TempSum = ((TempSum - TempCapMin - TempCapMax) >> 3);   
        // // Cap[i] = lp_butterWorth((uint16_t)TempSum,&LpButFilterBuff[i],FilterCoeff);  //对当前计数值滤波处理

        // if(CapPeakMin[i] > TempCapMin)
        //     CapPeakMin[i] =  TempCapMin;
        // else
        //     CapPeakMin[i] += ((TempCapMin - CapPeakMin[i]) >> 5);       //对CapPeakMin计数值滤波处理

        // if(CapTh[i] >  CapPeakMin[i])   
        //     TempCapMin = CapTh[i] - CapPeakMin[i];
        // else
        //     TempCapMin = CapPeakMin[i] - CapTh[i];

        // //if(TempCapMin > CapNoiseMax)   TempCapMin = CapNoiseMax;  //噪声影响
        // TempSum = TempCapMin * TempCapMin;
        // TempCapMin =  (uint16_t)((TempSum >> 11) + (TempSum >> 12) + (TempCapMin >> 4));   //峰值影响，调整阈值
        // if(CapTh[i]  >= (CapDifLib[i] + TempCapMin))
        //     CapDynMinTh[i] = CapTh[i] - CapDifLib[i] - TempCapMin ;     //实时值与环境值的差值 +阈值+阈值调整值    
            //CapDynMinTh[i] = CapTh[i] - CapDifLib[i] - TempCapMin + CAP_DIFF_APPROX_MAX;

        // TempCapMin = (uint16_t)(((TempCapMin * TempCapMin) >> 10) + (TempCapMin >> 4));
        CapDynMinTh[i] = CapTh[i] - CapDifLib[i];                              
    }
}




void TSC_Baseline_Calculate(void)
{
    uint8_t i = 0 ,CapDifLibTemp = 0,bCapTrig;
    uint32_t CapThTemp1 = 0, CapThTemp2 = 0;
    static uint8_t u8KeyHoldCnt =0;

    bCapTrig = 0;
    for(i = 0; i < TouchNum; i++)
    {	
        CapDifLibTemp = CapDifLib[i] >> 2;
        if(CapDifLibTemp < CapDiffApproxMax)  CapDifLibTemp = CapDiffApproxMax;            
        if(Cap[i] < (CapTh[i] - CapDifLibTemp ))
        {
            CapThSampleCntH = 0;
            CapThSampleCntL = 0;
            bCapTrig = 1;
            break;
        }
    }
    // if(bCapTrig)
    // {
    //     for (i = 0; i < TouchNum; i++)
    //         CapThSum[i] = 0;    
    // }
    if(bCapTrig){
        for(i = 0; i < TouchNum; i++){
            CapThSum[i] = 0;
		}
		if(!TouchResult){
			if(++u8KeyHoldCnt >= TouchTrigCnt){
				u8KeyHoldCnt = 0;
        		for(i = 0; i < TouchNum; i++){
					CapTh[i] = (Cap[i]>>1)+(CapTh[i]>>1);
				}
			}
		}
    }else{
		u8KeyHoldCnt = 0;
	}

    if(!TouchResult)    //baseline update
    {
        CapThSampleCntH++;
        if(!bPowerOnCapThUpdate)        //baseline initial
        {
            if(CapThSampleCntH < 16)
            {
                for(i = 0; i < TouchNum; i++)
                    CapTh[i] = Cap[i];      	
            }
            else
                bPowerOnCapThUpdate = 1;
        }
        else
        {
            if(CapThSampleCntH >= BaseLineUpdateTime)      //sample gap 
            {
                CapThSampleCntH = 0;
                for(i = 0; i < TouchNum; i++){
                    CapThSum[i] += Cap[i];
                    
                }
                          	
                
                if(++CapThSampleCntL >= 8)
                {
                    CapThSampleCntL = 0;
                    for(i = 0; i < TouchNum; i++)
                    {
                        CapThTemp1 = CapThSum[i] >> 3;
                        //============================//
                        #if DYNAMIC_ENVIRONMENT 
                        CapBalance_Spinner(CapThTemp1,i);  //间隔时间自动校准参考值
                        #endif  
                        //============================//
                        CapThSum[i] = 0;
                        if(CapTh[i] > CapThTemp1 )  //baseline down
                        {
                            if((CapTh[i] - CapThTemp1)>10){         //暂时不处理
                                CapThTemp2 = (CapTh[i] >> 1)  + (CapTh[i] >> 2) + (CapTh[i] >> 3)+(CapTh[i] >> 4)  + (CapTh[i] >> 5) + (CapTh[i] >> 6);		
                                CapThTemp1 >>= 6;  //1/64
                                // CapThTemp2 = ((CapTh[i] *63)+32)/64;		
                                // CapThTemp1 = (CapThTemp1+32)/64; //1/8  四舍五入
                                
                            }else{
                                CapThTemp2 = (CapTh[i] >> 1)  + (CapTh[i] >> 2) + (CapTh[i] >> 3);		
                                CapThTemp1 >>= 3; //1/8
                                // CapThTemp2 = ((CapTh[i] *7)+4)/8;		
                                // CapThTemp1 = (CapThTemp1+4)/8; //1/8  四舍五入
                            }
                                                   
                        }
                        else			           //baseline up
                        {
                            CapThTemp2 = CapTh[i] >> 1;
                            CapThTemp1 >>= 1;
                            // CapThTemp2 = (CapTh[i] +1)/2;		
                            // CapThTemp1 = (CapThTemp1+1)/2; //1/2  四舍五入
                        }
                        CapTh[i] = CapThTemp1 + CapThTemp2;

                        
                    }
                }
            }
            
        }
    }
}



//TSC exception handling
void TSC_Abnormal_Handle(void)
{
    uint8_t i = 0;

    if(TouchResult)      
    {
        RecailbStatus = 0;
        for(i = 0; i < TouchNum; i++)
            CapReSetCnt[i] = 0;
    }

    // for (i = 0; i < TouchNum; i++)
    // {
    //     if(Cap[i] > (TouchBaseLine + ReferCapCalibRange + 5))   //Hand touch continuously when powering on
    //     {
    //         if(!(RecailbStatus & (1 << i)))
    //         {
    //             RecailbStatus |= (1 << i);
    //             CapReSetCnt[i] = 0;
    //         }
    //         else
    //         {
    //             if(!CapReSetCnt[i])
    //                 CapReSetCnt[i] = TIM6->CNT;                
    //             if(get_elap(CapReSetCnt[i]) >= TouchRevInverPersCnt)   //long key reset
    //             {
    //                 RecailbStatus = 0;
    //                 TSC_Reg_ReInit();
    //                 break;
    //             }
    //         }            
    //     }
    // }

    if(PersKeyLockTime)
    {
        if(TouchResult)
        {
            if(++ResetTime > PersKeyLockTime)  //hold key reset
            {
                ResetTime = 0;
                TouchResultMap = 0;
                TouchResult = 0;
                TSC_Reg_ReInit(); 
            }     
        }
        else
				{
            ResetTime=0; 
        }					
    } 
}




void TSC_TouchKey_Handle(void)
{
    uint8_t i;
    uint32_t k,TouchResultTemp;
    
    for(i = 0; i < TouchNum; i++)
    {	
        if(Cap[i] < CapDynMinTh[i])
        {	
            TkDebounce[i].Down++;
            TkDebounce[i].Up = 0;
            if(TkDebounce[i].Down >= SetFilterOn)
            {	
                TkDebounce[i].Down = 0;
                TouchResultMap |= (1ul << i);		
            }
        }
        else if(Cap[i] > (CapDynMinTh[i] + (CapDifLib[i] >> CapDifCoeff)))  
        {	
            TkDebounce[i].Up++;
            TkDebounce[i].Down = 0;
            if(TkDebounce[i].Up >= SetFilterOff)
            {	
                TkDebounce[i].Up = 0;
                TouchResultMap &= (~(1ul << i));	
            }
        }
    }

    k = 1;
    TouchResultTemp = 0;
    for(i = 0; i < 12; i++)
    {
        if(TouchChannel & (1ul << i))
        {
            if(k == (TouchResultMap & k))
                TouchResultTemp |= (1ul << i);
            k <<= 1;
        }        
    }
		
    TouchResult = TouchResultTemp;
		
		//printf("cap0:%d,capth:%d\n",Cap[3],CapTh[3]);
}



void TSC_Handle(void)
{
    //读取TK计数值
    // TouchResultBuff[0] = TSC->TKK0CNTR;
    if(TSC_Refer_Cap_Cabilb())      //平衡之后才处理数据
    {
        TSC_Rawdata_Handle();       //cap实时值滤波处理
        TSC_Baseline_Calculate();   //baseline滤波处理
        TSC_Abnormal_Handle();          
        TSC_TouchKey_Handle();      //获取键值

                 
    }
    else
    { 
//        if(!CapReferCalibPersTime)
//            CapReferCalibPersTime = TIM6->CNT;
//        if(get_elap(CapReferCalibPersTime) >= CapReferCalibPersTimeTH)  //hold key reset 
//            TSC_Reg_ReInit();
    }
}







