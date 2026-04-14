/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    key.C
  * @author  Yspring Firmware Team  
  * @brief   key Source Code.
  ******************************************************************************      
*/

#include "key.h"
#include "ys32t031.h"
//#include "bsp.h"
#include "system_ys32t031.h"	
#include <stdint.h> 
#include "display.h"



void TSC_Configuration(void);



// TSC ≥ı ºªØ≈‰÷√
void TSC_Configuration(void)
{
    TSC_InitTypeDef TSC_InitStruct;
    TSC_StructInit(&TSC_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_TSC, ENABLE);

    TSC_InitStruct.TSC_ScanMode = TSC_ScanMode_Manual;
    TSC_InitStruct.TSC_ClkSrc = TSC_ClkSrc_HCLK;
    TSC_InitStruct.TSC_T16Src = TSC_T16Src_Div16;
    TSC_InitStruct.TSC_TimeSlotSrc = TSC_TimeSlotSrc_RefOSC;
    TSC_InitStruct.TSC_RefOSCFreq = TSC_RefOSCFreq_5M;
    TSC_InitStruct.TSC_AutoScanModeTime = TSC_AutoScanModeTime_2n15;
    TSC_InitStruct.TSC_AutoScanModeTimeDiv = TSC_AutoScanModeTime_Div4;
    TSC_InitStruct.TSC_OSCHopSel = TSC_OSCHopSel_Hardware;
    TSC_InitStruct.TSC_OSCHopFreq = TSC_OSCHopFreq_1_429M;
    TSC_InitStruct.TSC_TouchKey = TSC_KEY7 | TSC_KEY8 | TSC_KEY9 | TSC_KEY10;
    TSC_Init(&TSC_InitStruct);

    TSC_SetTimeSlotCount(100);
    TSC_RefCAP_Config(50);

    TSC_FilterCmd(ENABLE);

    TSC_OSC_Cmd(TSC_OSC_REF | TSC_OSC_KEY, ENABLE);

    TSC_StartCmd(ENABLE);
}











