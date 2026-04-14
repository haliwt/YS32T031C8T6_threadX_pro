/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    key.H
  * @version V1.0.0
  * @date    2024
  * @author  Yspring Firmware Team  
  * @brief   key Header Code.
  ******************************************************************************      
*/
#ifndef __KEY_H
#define __KEY_H


#ifdef __cplusplus
extern "C" {
#endif
  
#include "ys32t031.h"
#include "system_ys32t031.h"	
#include <stdint.h>  
#include "ys32t031_tsc.h"
#include "ys32t031_tsc_lib.h"
	
	
#define K0_BIT				0x0001	// CCH0
#define K1_BIT			  0x0002	// CCH1
#define K2_BIT			  0x0004	// CCH2
#define K3_BIT				0x0008	// CCH3
#define K4_BIT				0x0010	// CCH4
#define K5_BIT				0x0020	// CCH5
#define K6_BIT				0x0040	// CCH6
#define K7_BIT				0x0080	// CCH7
#define K8_BIT				0x0100	// CCH8
#define K9_BIT				0x0200	// CCH9
#define K10_BIT				0x0400	// CCH10
#define K11_BIT				0x0800	// CCH11

#define KEY0_PIN			(TouchResult&K0_BIT)
#define KEY1_PIN			(TouchResult&K1_BIT)
#define KEY2_PIN			(TouchResult&K2_BIT)
#define KEY3_PIN			(TouchResult&K3_BIT)
#define KEY4_PIN			(TouchResult&K4_BIT)
#define KEY5_PIN			(TouchResult&K5_BIT)
#define KEY6_PIN			(TouchResult&K6_BIT)
#define KEY7_PIN			(TouchResult&K7_BIT)
#define KEY8_PIN			(TouchResult&K8_BIT)
#define KEY9_PIN			(TouchResult&K9_BIT)
#define KEY10_PIN			(TouchResult&K10_BIT)
#define KEY11_PIN			(TouchResult&K11_BIT)



extern void TSC_Configuration(void);



#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
