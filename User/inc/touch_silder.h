
#ifndef _touch_silder_h_
#define _touch_silder_h_

#include "ys32t031.h"
#include "ys32t031_tsc.h"
#include "ys32t031_tsc_lib.h"
#include "ys32t031_tsc_config.h"


#define  SILDER_EN     1

#define  SILDER_FILTE_EN     0  //加上滤波

#define SILDER_CHANNEL_NUM		2
#define TOUCH_SILDER_CH			(TSC_CCH6 |TSC_CCH7)    // 圆环对应通道

#if(SILDER_EN)

#define S0_CH                   4
#define S1_CH                   5

/*滑条分辨率，最大不能超过64*/
#define SLIDER_AREA_RESOLUTION	158

// extern bit SliderStatus;
// extern uint16_t SliderVal[2];

extern uint8_t silder_process(void);
// extern void silder_diff_value(void);
// extern uint8_t get_silder_value(uint16_t *pDif, uint8_t SliderAreaResolution, uint8_t SliderStatus);

#endif

#endif

