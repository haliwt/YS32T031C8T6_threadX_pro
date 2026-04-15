/**
  ******************************************************************************
  * @file    system_ys32t031.h
  * @author  YSPRING Application Team
  * @brief   CMSIS Cortex-M0 Device System Source File for YS32T031 devices.  
  ******************************************************************************
 */

#ifndef __SYSTEM_YS32T031_H__
#define __SYSTEM_YS32T031_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif
/******************************************************************************/
/* Global pre-processor symbols/macros ('define')                             */
/******************************************************************************/


/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/
extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_YS32T031_H__ */

