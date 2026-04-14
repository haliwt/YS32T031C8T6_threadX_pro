/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    adc.C
  * @author  Yspring Firmware Team  
  * @brief   adc Source Code.
  ******************************************************************************      
*/

#include "adc.h"   
#include "ys32t031.h"



void ADC_Configuration(void);
void ADC_Channel_Init(uint8_t CH);
void ADC_Vrefbuf_Select(uint32_t Mode);
void ADC_Channel_Select(uint8_t CH);
uint16_t ADC_GetValue(uint8_t CHx,uint32_t Vrefx);



// ADC 初始化配置
void ADC_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStruct;    	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC, ENABLE);
	
	  ADC_Cmd(ADC, DISABLE);
	  ADC_DeInit(ADC);
	  ADC_StructInit(&ADC_InitStruct);
	
	  ADC_BiasCurrentConfig(ADC, ADC_Current_24uA);
	
	  ADC_InitStruct.ClockMode = ADC_ClockMode_HSI_Div8;                       /* ADC时钟为HSI 8分频 */    
	  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                      /* 数据右对齐 */    
	  ADC_InitStruct.ADC_ContinuousConvMode = ADC_ContinuousConvMode_OFF;      /* 连续转换关闭 */    
	  ADC_InitStruct.ADC_ScanMode = ADC_ScanMode_OFF;                          /* 扫描模式关闭 */     
	  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_SWSTART;      /* 软件启动转换 */    
	  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_EN;   /* 外部触发开启 */    
	  ADC_Init(ADC, &ADC_InitStruct);
}



/**
  * @brief  ADC 初始化
  * @param  None
  * @retval None
  */
void ADC_Channel_Init(uint8_t CH)
{
    ADC_InitTypeDef ADC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC, ENABLE);
	
    ADC_DeInit(ADC);
    ADC_StructInit(&ADC_InitStruct);
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ContinuousConvMode = ADC_ContinuousConvMode_OFF;
    ADC_InitStruct.ADC_ScanMode = ADC_ScanMode_OFF;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_SWSTART;
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_EN;
    ADC_Init(ADC, &ADC_InitStruct);
    
    ADC_ClockModeConfig(ADC, ADC_ClockMode_HSI_Div8);  
    ADC_RegularSequencerLengthConfig(ADC, 1);
      
    switch(CH)  //选择测试通道
    {
        case 1:    ADC_RegularChannelConfig(ADC, ADC_Channel_1, 1, ADC_SampleTime_31_5Cycles);   break;          //CH1  PF4  
			  case 2:    ADC_RegularChannelConfig(ADC, ADC_Channel_2, 1, ADC_SampleTime_31_5Cycles);   break;          //CH2  PA0
			  case 3:    ADC_RegularChannelConfig(ADC, ADC_Channel_3, 1, ADC_SampleTime_31_5Cycles);   break;          //CH3  PA1
        case 5:    ADC_RegularChannelConfig(ADC, ADC_Channel_5, 1, ADC_SampleTime_31_5Cycles);   break;          //CH5  PA3      
        case 6:    ADC_RegularChannelConfig(ADC, ADC_Channel_6, 1, ADC_SampleTime_31_5Cycles);   break;          //CH6  PA4      
        case 7:    ADC_RegularChannelConfig(ADC, ADC_Channel_7, 1, ADC_SampleTime_31_5Cycles);   break;          //CH7  PA5      
        case 8:    ADC_RegularChannelConfig(ADC, ADC_Channel_8, 1, ADC_SampleTime_31_5Cycles);   break;          //CH8  PA6      
        case 9:    ADC_RegularChannelConfig(ADC, ADC_Channel_9, 1, ADC_SampleTime_31_5Cycles);   break;          //CH9  PA7    
        default :  break;
    }
        
    ADC_ClearFlag(ADC, ADC_FLAG_EOC);
    ADC_Cmd(ADC, ENABLE);
}



/************************************************
函数名称 ： ADC_Vrefbuf_Select
功    能 ： ADC参考电压配置
参    数 ： Mode:ADC参考电压选择
                 在以下值中选择：VREFBUF_ADC_VREFBUF
                                 VREFBUF_ADC_VREF
                                 VREFBUF_ADC_VCC
返 回 值 ： 无
*************************************************/
void ADC_Vrefbuf_Select(uint32_t Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    if(Mode == VREFBUF_ADC_VREFBUF){
        VREFBUF_ADC_Config(VREFBUF_ADC_VREFBUF);               /* ADC参考电压为VREFBUF1.2V */		
        VREFBUF_Cmd(ENABLE);				
    }else if(Mode == VREFBUF_ADC_VREF){
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;   
        GPIO_Init(GPIOA, &GPIO_InitStructure);                 /* 配置PA4为VREF */
        VREFBUF_ADC_Config(Mode);                              /* ADC参考电压为VREF pin */
    }else{
        VREFBUF_ADC_Config(Mode);                              /* ADC参考电压为VCC */
    }
}



/************************************************
函数名称 ： ADC_Channel_Select
功    能 ： ADC初始化函数
参    数 ： CH:ADC通道
返 回 值 ： 无
*************************************************/
void ADC_Channel_Select(uint8_t CH)
{
    if(CH == 0) return;
	
    ADC_RegularSequencerLengthConfig(ADC, 1);
	
    if(CH != 20){
        ADC_RegularChannelConfig(ADC, CH, 1, ADC_SampleTime_15_5Cycles);                        // 各个引脚的采样
    }else{
        ADC_RegularChannelConfig(ADC, ADC_Channel_Vrefint, 1, ADC_SampleTime_15_5Cycles);       // Vrefbuf为参考电压采样
    }
		
    ADC_ClearFlag(ADC, ADC_FLAG_EOC);
    ADC_Cmd(ADC, ENABLE);	
}



//获取通道转换数据
uint16_t ADC_GetValue(uint8_t CHx,uint32_t Vrefx)
{    
    ADC_Vrefbuf_Select(Vrefx);  
    ADC_Channel_Select(CHx);    
    ADC_SoftwareStartConvCmd(ADC);                                          /* 启动软件触发 */ 
    while(!ADC_GetFlagStatus(ADC, ADC_FLAG_EOC));                           /* 等待转换完成 */ 
    return  ADC_GetConversionValue(ADC);                                    /* 获取转换数据 */	
}












