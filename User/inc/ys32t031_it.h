/* USER CODE BEGIN header */
/**
  ******************************************************************************
  * @file    ys32t031_it.h 
  * @author  YSPRING Application Team
  * @version V1.0.0
  * @date    {date}
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* USER CODE END header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __YS32T031_IT_H
#define __YS32T031_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ys32t031.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN includes */

/* USER CODE END includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN types */

/* USER CODE END types */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN constants */

/* USER CODE END constants */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN macro */

/* USER CODE END macro */

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
//void PendSV_Handler(void);
//void SysTick_Handler(void);

/* USER CODE BEGIN prototypes */

/* USER CODE END prototypes */
#ifdef __cplusplus
}
#endif

#endif /* __YS32T031_IT_H */
