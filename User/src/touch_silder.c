
#include "touch_silder.h"

#if(SILDER_EN)

#define SLIDER_FILTER_4_5		4
#define SLIDER_FILTER_PTR		(1<<SLIDER_FILTER_4_5)

#define SLIDER_DELAY			8

static uint8_t SliderStatus = 0;
static uint16_t SliderVal[SILDER_CHANNEL_NUM] = {0};
static uint8_t u8GetChIdx[SILDER_CHANNEL_NUM] = {0};
static const uint8_t u8SetSilderCh[] = { S0_CH, S1_CH};
uint16_t  SliderResultValue = 0;

void set_silder_channel(void){
	uint8_t i;
    for ( i= 0; i < SILDER_CHANNEL_NUM; i++){
       u8GetChIdx[i] = u8SetSilderCh[i];
    }
}

static void silder_diff_value(void){
	uint8_t	i = 0;
	if(TouchResult & TOUCH_SILDER_CH){
		set_silder_channel();
		SliderStatus = 1;
        for(i=0; i< SILDER_CHANNEL_NUM; i++ ){	
            if (Cap[u8GetChIdx[i]] <= CapTh[u8GetChIdx[i]]){	
                SliderVal[i] = CapTh[u8GetChIdx[i]] - Cap[u8GetChIdx[i]];
			}else{
                SliderVal[i] = 0x00;
            }
		}   
	}else{
        SliderVal[i] = 0x00;
		SliderStatus = 0;
	}
}

/*
获取滑条值
滑条触摸通道2个
输入: *pDif滑条两个通道触摸变化量
			SliderAreaResolution 滑条分辨率，最大不能超过64,必须是2的指数，
			SliderStatus 圆环有没有触摸，0没有触摸，非0有触摸

输出:有触摸返回滑条数值，无触摸返回0XFF
*/
//加了一阶滤波后数据较稳定，
#if SILDER_FILTE_EN
uint8_t get_silder_value(uint16_t *pDif, uint8_t SliderAreaResolution){
	uint8_t i;
	uint8_t Slider;
	uint16_t tempInt;
	uint32_t tempLong;
	uint8_t SliderValue;
	static uint8_t SliderCnt = 0x00;
	static uint8_t SliderFilterPtr = 0x00;
	static uint8_t SliderFilter[SLIDER_FILTER_PTR] = {0,};
	static uint8_t SliderBuf[SLIDER_DELAY];
	static uint8_t SliderBufCnt = 0x00;

	if (SliderStatus){
		SliderCnt &= 0x0f;
		
		tempLong = pDif[1];
		tempLong *= SliderAreaResolution;
		Slider = tempLong / (pDif[0] + pDif[1]);

		// printf("%d", pDif[0]);
		// printf(",");
		// printf("%d", pDif[1]);
			
		if (SliderCnt < 0x04){
			SliderCnt++;
			for (i = 0x00; i < SLIDER_FILTER_PTR; i++){
				SliderFilter[i] = Slider;
			}

			for (i = 0x00; i < SLIDER_DELAY; i++){
				SliderBuf[i] = Slider;
			}
			SliderBufCnt = 0x00;
			SliderValue = 0xff;
		}else{
			if (++SliderFilterPtr > (SLIDER_FILTER_PTR - 1)){
				SliderFilterPtr = 0x00;
			}
			SliderFilter[SliderFilterPtr] = Slider;
			
			for (i = 0x00, tempInt = 0x0000; i < SLIDER_FILTER_PTR; i++){
				tempInt += SliderFilter[i];
			}
			tempInt >>= SLIDER_FILTER_4_5;

			if (tempInt > SliderAreaResolution){
				tempInt = SliderAreaResolution;
			}
			SliderBuf[SliderBufCnt] = tempInt;
			
			if (++SliderBufCnt > (SLIDER_DELAY -1)){
				SliderBufCnt = 0x00;
			}
			if(SliderBuf[SliderBufCnt]>=24)
				SliderValue = SliderBuf[SliderBufCnt] - 24;
			else
				SliderValue = 0;	
		}
	}else{
		if((SliderCnt & 0x0f) > 3){
			SliderCnt += 0x10;
			if ((SliderCnt & 0xf0) > 0x30){
				SliderCnt &= 0xf0;
				SliderValue = 0xff;
			}else{
				// SliderValue = SliderBuf[SliderBufCnt];
				if(SliderBuf[SliderBufCnt]>=24)
					SliderValue = SliderBuf[SliderBufCnt] - 24;
				else
					SliderValue = 0;
				}
		}else{
			SliderCnt = 0x00;
			SliderValue = 0xff;
		}
	}
	return SliderValue;
}

#else 
//简化滑条程序
uint8_t get_silder_value(uint16_t *pDif, uint8_t SliderAreaResolution){

	uint8_t  i = 0;
	uint8_t  SliderValue = 0;
	uint16_t  Slider = 0;
	unsigned long  tempLong = 0;
	
	if (SliderStatus){	
		while(i < SliderAreaResolution){
			tempLong += pDif[1];
			++i;
		}
		Slider = (pDif[0] + pDif[1]);

		while(Slider <= tempLong){
			tempLong -= Slider;
			++SliderValue;
		}

		if(!SliderValue){
			SliderValue = 0xff;
		}else{
			if(SliderValue>=25){
				SliderValue -=25;
			}else{
				SliderValue = 0;
			}
		}
	}else{
		SliderValue = 0xff;
	}

	return SliderValue;
}
#endif

uint8_t silder_process(void){
    silder_diff_value();
    if(SliderStatus){
        SliderResultValue = get_silder_value(&SliderVal[0], 180); // SLIDER_AREA_RESOLUTION
		// if(SliderResultValue==0xff){
		// 	SliderResultValue = 0;		
		// }else if(SliderResultValue>=128){
		// 	SliderResultValue = 128;
		// }
    }else{
		SliderResultValue = 0;
	}
	return SliderResultValue;
}


#endif

