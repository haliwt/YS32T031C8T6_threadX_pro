
#include"touch_circle.h"




#define TOUCH_Circle_NUM 4
#define TOUCH_Circle_StartNUM  4
#define TOUCH_Circle_CHANNEL  (TSC_CCH4| TSC_CCH3| TSC_CCH2| TSC_CCH5)

uint16_t  ResultValue = 0;
static uint8_t CircleStatus = 0;
static uint16_t CircleVal[TOUCH_Circle_NUM] = {0};
static uint8_t  u8GetSilderCh[TOUCH_Circle_NUM] = {0};

static const uint8_t u8SetSilderCh[] = {
    C0_CH,C1_CH,C2_CH,C3_CH,
};

void set_channal_map(void){
	uint8_t i;
    for ( i= 0; i < TOUCH_Circle_NUM; i++){
       u8GetSilderCh[i] = u8SetSilderCh[i];
    }
}

static void circle_diff_value(void){
	uint8_t i = 0;
	if(TouchResult & TOUCH_Circle_CHANNEL){
        set_channal_map();
        for(i=0; i< TOUCH_Circle_NUM; i++ ){
            // if(CapTh[u8GetSilderCh[i]]>Cap[u8GetSilderCh[i]]){
            //     CapMap[u8GetSilderCh[i]] = CapTh[u8GetSilderCh[i]] - Cap[u8GetSilderCh[i]];
            // }else{
            //     CapMap[u8GetSilderCh[i]] = 0;
            // }
            	
            
            if (Cap[u8GetSilderCh[i]] < CapTh[u8GetSilderCh[i]]){	
                CircleVal[i] = CapTh[u8GetSilderCh[i]] - Cap[u8GetSilderCh[i]];
                //CircleVal[i] = CapMap[u8GetSilderCh[i]] ;
			}else{
                CircleVal[i] = 0x00;
            }
            // CircleVal[i] = CapMap[u8GetSilderCh[i]] ;
            
		}   
		CircleStatus = 1;
        
	}else{
        CircleVal[i] = 0x00;
		CircleStatus = 0;
	}
}

static uint16_t get_circle_value(uint16_t *pDif, uint8_t circleRes) {
    uint8_t i;
    uint8_t maxIndex = 0;
	uint16_t addMol, addDen;
    uint8_t IdxLeft,IdxRight;
	uint32_t tempLong = 0;
	uint16_t circleValue = 0;
    uint16_t maxValue = pDif[0];
    
	//获取最大的Index以及对应的差值
    for (i = 1; i < TOUCH_Circle_NUM; i++) {
        if (pDif[i] > maxValue) {
            maxValue = pDif[i];
            maxIndex = i;
        }
    }
    
    if(pDif[maxIndex] < 50) return 0xffff;

    //靠近圆环尽头端
    // if(maxIndex == 7){  //N=x/(x+y)*60  占的比例位置
    //     addMol = pDif[7];   
    //     addDen = (pDif[6] + pDif[7]);
    // }else if(maxIndex == 0){  //靠近圆环开头端,  
    //     // if(pDif[7] >= 150){
    //     //     return 250;// 5
    //     // }else{
    //     //     return 10;// 5
    //     // }
    // }else{      //处于圆环中间端
    //     IdxLeft = maxIndex-1;
    //     IdxRight = maxIndex+1;
    //     if (pDif[IdxLeft] > pDif[IdxRight]){  
    //         addMol = pDif[maxIndex];
    //         addDen = (pDif[IdxLeft] + pDif[maxIndex]);
    //         maxIndex = IdxLeft;  //范围则为0-circleRes
    //     } else {
    //         addMol = pDif[IdxRight];    //circleRes-3circleRes
    //         addDen = (pDif[maxIndex] + pDif[IdxRight]);
    //     }
    // }

    if(maxIndex == 0){
        if (pDif[1] > pDif[3]) {
            addMol = pDif[1];
            addDen = (pDif[0] + pDif[1]);
            maxIndex = 0;
        }else{
            addMol = pDif[0];
            addDen = (pDif[3] + pDif[0]);
            maxIndex = 3;
        }
    }else if(maxIndex == 3){
        if (pDif[2] > pDif[0]){
            addMol = pDif[3];
            addDen = (pDif[2] + pDif[3]);
            maxIndex = 2;
        }else{
            addMol = pDif[0];
            addDen = (pDif[3] + pDif[0]);
            maxIndex = 3;
        }
    }else{
        IdxLeft = maxIndex-1;
        IdxRight = maxIndex+1;
        if (pDif[IdxLeft] > pDif[IdxRight]){
            addMol = pDif[maxIndex];
            addDen = (pDif[IdxLeft] + pDif[maxIndex]);
            maxIndex = IdxLeft;
        } else {
            addMol = pDif[IdxRight];
            addDen = (pDif[maxIndex] + pDif[IdxRight]);
        }
    }

    // 计算圆周上的位置
    for (i = 0; i <= circleRes; ++i){ tempLong += addMol;}
    while (addDen <= tempLong) {
        tempLong -= addDen;
        ++circleValue;
    }
    
    //circleValue = circleValue + circleRes * maxIndex;  //y=N+60*maxIndexs
    // printf("%d", addDen);
    // printf(",");
    // printf("%d", addMol);
    // printf(",");
    // printf("%d", maxIndex);
    // printf(",");
    // printf("%d", circleValue);
	
    // if(maxIndex ==0){
    //     if(circleValue >= 13){  // 去掉首位的值
    //         circleValue -= 13;
    //     }else{
    //         circleValue = 0;
    //     }
    // }else if(maxIndex == 1){
    //     if(circleValue >= 23){  // 去掉首位的值
    //         circleValue -= 23;
    //     }else{
    //         circleValue = 0;
    //     }    
    // }else{
    //     if(circleValue >= 20){  // 去掉首位的值
    //         circleValue -= 20;
    //     }else{
    //         circleValue = 0;
    //     }
    // }
		
		if(circleValue >= 12){  // 去掉首位的值
             circleValue -= 12;
         }else{
             circleValue = 0;
         }
    circleValue = circleValue + 64 * maxIndex;
    // 根据索引转换为圆周上的最终位置
    return circleValue;
}

uint16_t circle_process(void){
    uint16_t ResultValue_t = 0;
    circle_diff_value();  //获取圆环按键的diff值
    if(CircleStatus){
        ResultValue = get_circle_value(&CircleVal[0], 90);
    }else{
        ResultValue = 0xffff;
    }
    //LED显示
    if(TouchResult && (ResultValue != 0xffff)) {
        if(ResultValue >= 255){
            ResultValue = 255;
        }
        ResultValue_t = ResultValue;
    }else{
      
        ResultValue_t = 0xffff;
    }
    return ResultValue_t;
}

