/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    iwdg.C
  * @author  Yspring Firmware Team  
  * @brief   iwdg Source Code.
  ******************************************************************************      
*/

#include "iwdg.h"
#include "ys32t031.h"


void IWDG_Configuration(void);



// IWDG ≥ı ºªØ≈‰÷√
void IWDG_Configuration(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_IWDG, ENABLE);
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 
  while (IWDG_GetFlagStatus(IWDG_FLAG_PVU) != 0x00); 
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  IWDG_SetReload(1024);
  IWDG_SetWindowValue(4095);           
  IWDG_ReloadCounter();
  IWDG_Enable();
}









