/**
  ******************************************************************************
  * @file    ys32t031_conf.h 
  * @author  YSPRING Application Team
  * @version V1.0.1
  * @date    2023.3.20
  * @brief   Library configuration file.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __YS32T031_CONF_H
#define __YS32T031_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Comment the line below to disable peripheral header file inclusion         */
#include "ys32t031_adc.h"
#include "ys32t031_comp.h"
#include "ys32t031_crc.h"
#include "ys32t031_dbgmcu.h"
#include "ys32t031_dma.h"
#include "ys32t031_exti.h"
#include "ys32t031_flash.h"
#include "ys32t031_gpio.h"
#include "ys32t031_i2c.h"
#include "ys32t031_iwdg.h"
#include "ys32t031_led.h"
#include "ys32t031_lptim.h"
#include "ys32t031_misc.h"   /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */
#include "ys32t031_pwr.h"
#include "ys32t031_rcc.h"
#include "ys32t031_rtc.h"
#include "ys32t031_spi.h"
#include "ys32t031_syscfg.h"
#include "ys32t031_tim.h"
#include "ys32t031_tsc.h"
#include "ys32t031_uart.h"
#include "ys32t031_vrefbuf.h"
#include "ys32t031_wwdg.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
  #define assert_param(x) do{ assert((x)> 0u) ; }while(0);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __YS32T031_CONF_H */
