
#ifndef _touch_circle_h_
#define _touch_circle_h_

#include "ys32t031.h"
#include "ys32t031_tsc.h"
#include "ys32t031_tsc_lib.h"
#include "ys32t031_tsc_config.h"

//===============================================//
//圆环参数
#define FILTER_EN   0   // 是否增加滤波

#define C0_CH					    2	
#define C1_CH						1	
#define C2_CH						0	
#define C3_CH						3

//===============================================//

extern uint16_t circle_process(void);


#endif

