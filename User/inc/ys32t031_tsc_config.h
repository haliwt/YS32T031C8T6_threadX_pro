#include "ys32t031.h"
#include "system_ys32t031.h"

#ifndef __YS32T031_TSC_CONFIG_H
#define __YS32T031_TSC_CONFIG_H

//************************************************************
//用户不需更改
//************************************************************
#define CAP_DIFF_COEFF                  3
#define CAP_REFER_CALIB_DEB_NUM         8
//************************************************************


//************************************************************
//低功耗模式下，TSC硬件自动扫描唤醒周期
//0: 2^13(0.25s) , 1: 2^14(0.5s), 2: 2^15(1s),  3: 2^16(2s),  4: 2^17(4s), 5: 2^18(8s) 
//例：TSC_WAKE_CYCLE = 0， 表示溢出周期 : 2^13/32768 = 0.25s
//************************************************************
#define TSC_WAKE_CYCLE                  2

//************************************************************
//低功耗模式下，一个周期内，TSC硬件扫描的次数
//0: 2次,     1: 4次    2: 8次    3: 16次
//例：TSC_CYCLE_SCAN_TIMES = 1，表示一个周期内扫描4次
//************************************************************
#define TSC_CYCLE_SCAN_TIMES            1

//************************************************************
//触摸无操作，持续SLEEP_TIME 时间后，进入低功耗模式。
// 2ms/unit .  
//例： SLEEP_TIME = 1500 1500*2ms = 3s
//************************************************************
#define SLEEP_TIME                      500    //20ms/unit .
//************************************************************
//环境采样间隔
//8：表示扫描8轮，采样一次数据用作更新
//************************************************************
#define BASE_LINE_UPDATE_TIME           1
//************************************************************
//触摸按键长按复位时间  (MAX = 65535 * nms)  0 :表示关闭该功能
//PERS_LOCK_KEY_TIME_TH 时间后，TSC重新校准 
//如下：10000* 20ms = 200s,(20ms等于一个时隙溢出周期)
//************************************************************
#define PERS_LOCK_KEY_TIME_TH           10000     //touch scanning times
#define TOUCH_TRIG_CNT                  30      // 触摸使用过程中值突然增大，但没有超过触摸响应阈值，累计多少次后自动更新环境值，10 - 255，建议：20-255，最大不超过255。

//************************************************************
//一阶动态滤波参数  适用不需要反应太灵敏的场合
#define FILTERDIF 8 //一般设置成已使能通道中最小触发阈值的1/4
#define FILTERCNT 2 //值越大滤波强度越大	范围0 ~ 255  缓慢
#define FILTERK 4	//值越大滤波强度越小	范围0 ~ 7    最大滤波强度128/256
//************************************************************
//滤波参数  一阶滤波(例：70%的滤波，即实时值取70%)  较灵敏
#define FILTERN 60	//值越大滤波强度越小	范围0 ~ 100    最大滤波强度128/256
#define FILTER_DYNAMIC   0      //1:开启动态一阶滤波  0：开启固定一阶滤波
/**********************TOUCH_FUNCTION_SET**********************/
#define SET_FILTER_ON			1		//按下消抖时间，MAX = 15;
#define SET_FILTER_OFF			1		//松开消抖时间，MAX = 15;
#define SET_CAPTHUPDATE_L		8       //环境更新间隔时间，8*8*scan_time  max= 15
#define SET_CAPTHUPDATE_H		8       //环境更新间隔时间，8*8*scan_time  max= 15

//************************************************************

//************************************************************
//各通道参考电容自动校准时的误差范围  =REFER_CAP_CALIB_RANGE
//若计数值在+/-(REFER_CAP_CALIB_RANGE)范围内,则表示校准成功
//注：若TK_BASE_LINE，设置较大且自动校准不成功时，可以调整此值
//************************************************************
//时隙溢出时间设置值
//注：是否要规定一个通道扫描时间，然后根据设定的扫描时间调整时隙
//************************************************************
#define TSC_RefOSCFrequency             TSC_TKCR1_TKFS_5MHz     //参考振荡频率选择，频率越高，响应越灵敏 
#define TK_BASE_LINE                    4608   //1152                 //T=(TK_BASE_LINE/128)*32*t   t=当前参考cap下的振荡周期    高感度=数值小   数值越大，变化量越大，同时采样时间变长  //最大8192
#define REFER_CAP_CALIB_RANGE           5                       //若计数值在+/-(REFER_CAP_CALIB_RANGE)范围内,则表示校准成功

//************************************************************
#define DYNAMIC_ENVIRONMENT     1           //置一打开动态环境更新

//#define ANTI_INTERFERENCE       0           //干扰调频选项标志位
//************************************************************
//使能触摸总通道数 //使能相应触摸通道
//注：最多使能12通道
//************************************************************
#define TOUCH_NUM                       4      
#define TOUCH_CHANNEL                  (TSC_CCH7 | TSC_CCH8 | TSC_CCH9 | TSC_CCH10)	
#define TOUCH_SETCHANNEL               (TSC_KEY7 | TSC_KEY8 | TSC_KEY9 | TSC_KEY10)	//初始化触摸使能时用到,跟随CCH填写			
//************************************************************
//触摸阈值设置
//一般设置 10~255,具体需要结合实际设置 ,数值越小越灵敏
//************************************************************
#define TOUCH_COMDIF                     150
#define TOUCH0_DIF                       TOUCH_COMDIF
#define TOUCH1_DIF                       TOUCH_COMDIF
#define TOUCH2_DIF                       TOUCH_COMDIF
#define TOUCH3_DIF                       TOUCH_COMDIF
#define TOUCH4_DIF                       TOUCH_COMDIF
#define TOUCH5_DIF                       TOUCH_COMDIF
#define TOUCH6_DIF                       TOUCH_COMDIF
#define TOUCH7_DIF                       30
#define TOUCH8_DIF                       35
#define TOUCH9_DIF                       30
#define TOUCH10_DIF                      35
#define TOUCH11_DIF                      TOUCH_COMDIF


#if ((TOUCH_NUM > 16) || (CAP_REFER_CALIB_DEB_NUM <= 0))
    #error The TSC supports a maximum of 16 channels or CAP_REFER_CALIB_DEB_NUM <= 0!!!
#endif

#endif
