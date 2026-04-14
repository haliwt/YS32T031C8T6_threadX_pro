#include "ys32t031.h"
#include "system_ys32t031.h"
#include "ys32t031_tsc_config.h"


#ifndef __YS32T031_TSC_LIB_H
#define __YS32T031_TSC_LIB_H

#define SYS_FREQ                        24000000
#define PWM_FREQ                        4000        
#define SET_PWM_FRE_DIV                 (SYS_FREQ/PWM_FREQ/2)
#define CAP_NOSIZE_MAX                  16384   //User not allowed to change
#define RAW_DATA_SIZE                   10      //User not allowed to change
#define CAP_DIFF_APPROX_MAX             8      //User not allowed to change
#define LP_FILTER_COEFF                 3       //User not allowed to change

#define PWR_Redef_Regulator_ON                ((uint32_t)0x00000000)
#define PWR_Redef_Regulator_LowPower          ((uint32_t)0x00000001)

#define PWR_STOPEntry_WFI                     ((uint8_t)0x01)
#define PWR_STOPEntry_WFE                     ((uint8_t)0x02)
#define PWR_STOPEntry_SLEEPONEXIT             ((uint8_t)0x03)


typedef struct
{
  uint8_t NVIC_IRQChannel;             /*!< Specifies the IRQ channel to be enabled or disabled.
                                            This parameter can be a value of @ref IRQn_Type 
                                            (For the complete YS32 Devices IRQ Channels list, 
                                            please refer to ys32f032.h file) */

  uint8_t NVIC_IRQChannelPriority;     /*!< Specifies the priority level for the IRQ channel specified
                                            in NVIC_IRQChannel. This parameter can be a value
                                            between 0 and 3.  */

  FunctionalState NVIC_IRQChannelCmd;  /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
                                            will be enabled or disabled. 
                                            This parameter can be set either to ENABLE or DISABLE */   
} NVIC_StructDef;



typedef struct
{
  uint32_t EXTI_Line;               /*!< Specifies the EXTI lines to be enabled or disabled.
                                         This parameter can be any combination of @ref EXTI_Lines */

  uint8_t EXTI_Mode;       /*!< Specifies the mode for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */

  uint8_t EXTI_Trigger; /*!< Specifies the trigger signal active edge for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */

  FunctionalState EXTI_LineCmd;     /*!< Specifies the new state of the selected EXTI lines.
                                         This parameter can be set either to ENABLE or DISABLE */
}EXTI_StructDef;



typedef struct
{
   uint8_t Down : 4;
   uint8_t Up : 4;
} TkDebounceStruct;

#define  TSC_Lib_ClkSrc_HCLK      0x00
#define  TSC_Lib_ClkSrc_LSI       (TSC_TKCR1_TKCLKSEL)



/** @defgroup TSC_AutoScanModeTime_define 
 * @{
 */ 
#define  TSC_Lib_AutoScanModeTime_2n13      TSC_TKCR1_ASMTO_000  //2^13/fLSI 
#define  TSC_Lib_AutoScanModeTime_2n14      TSC_TKCR1_ASMTO_001  //2^14/fLSI 
#define  TSC_Lib_AutoScanModeTime_2n15      TSC_TKCR1_ASMTO_010  //2^15/fLSI 
#define  TSC_Lib_AutoScanModeTime_2n16      TSC_TKCR1_ASMTO_011  //2^16/fLSI 
#define  TSC_Lib_AutoScanModeTime_2n17      TSC_TKCR1_ASMTO_100  //2^17/fLSI 
#define  TSC_Lib_AutoScanModeTime_2n18      TSC_TKCR1_ASMTO_101  //2^18/fLSI                      
/**
  * @}
  */ 
  
/** @defgroup TSC_AutoScanModeTimeDiv_define 
 * @{
 */ 
#define  TSC_Lib_AutoScanModeTime_Div2      TSC_TKCR1_ASMP_2  // TSC_AutoScanModeTime/2 
#define  TSC_Lib_AutoScanModeTime_Div4      TSC_TKCR1_ASMP_4  // TSC_AutoScanModeTime/4 
#define  TSC_Lib_AutoScanModeTime_Div8      TSC_TKCR1_ASMP_8  // TSC_AutoScanModeTime/8 
#define  TSC_Lib_AutoScanModeTime_Div16     TSC_TKCR1_ASMP_16 // TSC_AutoScanModeTime/16                                
/**
  * @}
  */ 

/** @defgroup TSC_T16Src_define 
 * @{
 */ 
#define  TSC_Lib_T16Src_Div16      TSC_TKCR1_TK16S_16  // fTKCLK/16  
#define  TSC_Lib_T16Src_Div32      TSC_TKCR1_TK16S_32  // fTKCLK/32  
#define  TSC_Lib_T16Src_Div64      TSC_TKCR1_TK16S_64  // fTKCLK/64  
#define  TSC_Lib_T16Src_Div128     TSC_TKCR1_TK16S_128 // fTKCLK/128                              
/**
  * @}
  */ 
  
/** @defgroup TSC_RefOSCFreq_define 
 * @{
 */ 
#define  TSC_Lib_RefOSCFreq_1MHz       TSC_TKCR1_TKFS_1MHz   // 3MHz   
#define  TSC_Lib_RefOSCFreq_5MHz       TSC_TKCR1_TKFS_5MHz   // 7MHz   
#define  TSC_Lib_RefOSCFreq_13MHz      TSC_TKCR1_TKFS_13MHz  // 11MHz                               
/**Lib_
  * @}
  */ 
  
/** @defgroup TSC_ScanMode_define 
 * @{
 */ 
#define  TSC_Lib_ScanMode_Auto          TSC_TKCR1_TKMOD_00  // Auto                          
#define  TSC_Lib_ScanMode_Manual        TSC_TKCR1_TKMOD_01  // Manual                        
#define  TSC_Lib_ScanMode_PeriodicAuto1  TSC_TKCR1_TKMOD_10  // Periodic automatic scan mode 1
#define  TSC_Lib_ScanMode_PeriodicAuto2  TSC_TKCR1_TKMOD_11  // Periodic automatic scan mode 2                               
/**
  * @}
  */ 
  
/** @defgroup TSC_TimeSlotSrc_define 
 * @{
 */ 
#define  TSC_Lib_TimeSlotSrc_Div1     TSC_TKCR2_TSS_00  // Reference oscillator clock  
#define  TSC_Lib_TimeSlotSrc_Div32    TSC_TKCR2_TSS_01  // fTKCLK/32                   
#define  TSC_Lib_TimeSlotSrc_Div64    TSC_TKCR2_TSS_10  // fTKCLK/64                   
#define  TSC_Lib_TimeSlotSrc_Div128   TSC_TKCR2_TSS_11  // fTKCLK/128                  
                             
/**
  * @}
  */ 
  
/** @defgroup TSC_OSCHopSel_define 
 * @{
 */ 
#define  TSC_Lib_OSCHopSel_Sofe      0               // Sofe Hop                         
#define  TSC_Lib_OSCHopSel_Hardware  TSC_TKCR2_SOFC  // Hardware Hop                      
                          
/**
  * @}
  */ 
  
/** @defgroup TSC_OSCHopFreq_define 
 * @{
 */ 

  
/** @defgroup TSC_Key_define 
 * @{
 */ 
// #define  TSC_CCH0     (uint32_t)(1<<0 )  // K0
// #define  TSC_CCH1     (uint32_t)(1<<1 )  // K1 
// #define  TSC_CCH2     (uint32_t)(1<<2 )  // K2  
// #define  TSC_CCH3     (uint32_t)(1<<3 )  // K3  
// #define  TSC_CCH4     (uint32_t)(1<<4 )  // K4  
// #define  TSC_CCH5     (uint32_t)(1<<5 )  // K5  
// #define  TSC_CCH6     (uint32_t)(1<<6 )  // K6  
// #define  TSC_CCH7     (uint32_t)(1<<7 )  // K7  
// #define  TSC_CCH8     (uint32_t)(1<<8 )  // K8  
// #define  TSC_CCH9     (uint32_t)(1<<9 )  // K9  
// #define  TSC_CCH10    (uint32_t)(1<<10)  // K10  
// #define  TSC_CCH11    (uint32_t)(1<<11)  // K11  
// #define  TSC_CCH12    (uint32_t)(1<<12)  // K12  
// #define  TSC_CCH13    (uint32_t)(1<<13)  // K13 
// #define  TSC_CCH14    (uint32_t)(1<<14)  // K14 
// #define  TSC_CCH15    (uint32_t)(1<<15)  // K15 
// #define  TSC_CCH16    (uint32_t)(1<<16)  // K16 
// #define  TSC_CCH17    (uint32_t)(1<<17)  // K17 
// #define  TSC_CCH18    (uint32_t)(1<<18)  // K18 
// #define  TSC_CCH19    (uint32_t)(1<<19)  // K19 
// #define  TSC_CCH20    (uint32_t)(1<<20)  // K20
// #define  TSC_CCH21    (uint32_t)(1<<21)  // K21 
// #define  TSC_CCH22    (uint32_t)(1<<22)  // K22 
// #define  TSC_CCH23    (uint32_t)(1<<23)  // K23 
                            
/**
  * @}
  */ 

/** @defgroup TSC_ThresholdConfig_define 
 * @{
 */ 
#define  TSC_Lib_UpperThreshold           ((uint16_t)0x0001)         
#define  TSC_Lib_LowerThreshold           ((uint16_t)0x0000)            
/**
  * @}
  */ 
  
/** @defgroup TSC_IT_Config_define 
 * @{
 */ 
#define  TSC_Lib_IT_TimeSlot                   TSC_TKIER_TKRCOVE     //TimeSlot               
#define  TSC_Lib_IT_TimeSlot_WakeUp            TSC_TKIER_TKRCOVWUEN  //TimeSlot WakeUp        
#define  TSC_Lib_IT_ThresholdMatches           TSC_TKIER_TKTHE       //Threshold Matches       
#define  TSC_Lib_IT_ThresholdMatches_WakeUp    TSC_TKIER_TKTHWUEN    //Threshold Matches WakeUp         
/**
  * @}
  */ 

/** @defgroup TSC_IT_Flags_define 
 * @{
 */
#define  TSC_Lib_Flag_TimeSlot                   TSC_TKSR1_TKRCOV          //Time Slot
#define  TSC_Lib_Flag_ThresholdMatches           TSC_TKSR1_TKTHF	       //Threshold Matches         

/**
  * @}
  */ 

/** @defgroup TSC_Flags_define 
 * @{
 */
#define  TSC_Lib_Flag_Clk_RDY                   TSC_TKSR1_HCKRDY          //Clock source switch ready state
#define  TSC_Lib_Flag_ScanKey_Busy              TSC_TKSR1_TKBUSY          //ScanKey Busy        
#define  TSC_Lib_Flag_TimeOut_CF16Bit           TSC_TKSR1_TKCFOV          //Time Out CF 16BIT
#define  TSC_Lib_Flag_TimeOut_16BIT             TSC_TKSR1_TK16OV          //Time Out 16BIT
/**
  * @}
  */ 


typedef struct 
{
   uint8_t SampleCntH :4;
   uint8_t SampleCntL :4;
}CapParaStruct;          
extern CapParaStruct CapThSpinnerCnt[TOUCH_NUM];


extern uint8_t CapDiffApproxMax;
extern uint8_t CapDif[];
extern uint8_t SetFilterOn ;
extern uint8_t SetFilterOff ;
extern uint8_t CapDifCoeff ;
extern uint8_t CapReferCalibDebNum ;
extern  uint8_t  TouchNum ;
extern const uint8_t SOF_HOPE_SEL[];
extern uint16_t TouchBaseLine ;


extern uint16_t ReferCapCalibRange;
extern uint16_t BaseLineUpdateTime;
extern uint32_t  PersKeyLockTime;
extern uint32_t TouchChannel ;
extern uint8_t filterDif;
extern uint8_t filterCnt;
extern uint8_t filterK;
extern uint8_t capFilterStatus[TOUCH_NUM];
extern uint8_t capFilterDataCnt[TOUCH_NUM];
extern uint8_t filterN;

extern TkDebounceStruct TkDebounce[];
extern uint8_t TkRefOscNearFreqIndex;    
extern uint8_t bPowerOnCapThUpdate;
extern uint8_t CapThSampleCntH;
extern uint8_t CapThSampleCntL;
extern uint8_t CapReferCalibDebCnt[];
extern uint8_t CapDifLib[];
extern uint8_t RawDataCnt[];
extern uint16_t CapReferCalibPersTime;
extern uint16_t CapRaw[4][TOUCH_NUM];
extern uint16_t CapDynMinTh[];
//extern uint16_t CapDynMaxTh[];
extern uint16_t SleepTimeCnt;
extern uint16_t CapRCC[TOUCH_NUM];
extern uint16_t CapReferMax[];
extern uint16_t CapReferMin[];
extern uint16_t CapTh[]; 
extern uint16_t CapReSetCnt[];
extern uint16_t Cap[];
extern uint16_t CapPeakMin[];
extern uint16_t CapPeakMax[];
extern uint16_t LpButFilterBuff[];
extern uint16_t CapRawCache[TOUCH_NUM][RAW_DATA_SIZE];
extern uint32_t CapReferCalibStatus;
extern uint32_t CapReferCalibPreStatus;
extern uint32_t CapThSum[];
extern uint32_t TouchResult;
extern uint32_t TouchResultBuff[];
extern uint8_t TouchTrigCnt;

extern void TSC_Handle(void);
extern void TSC_TouchKey_Handle(void);
extern void TSC_Abnormal_Handle(void);
extern uint8_t  TSC_Refer_Cap_Cabilb(void);
extern void TSC_Lib_Init(void);
extern void TSC_Reg_ReInit(void);
extern void TSC_Sleep_Proc(void);
extern void Timer6_Init(void);
extern uint16_t get_elap(uint16_t CurCnt);
extern void NVIC_Redefine_Config(void);
extern void BSP_Uart_Proc(void);
extern void TSC_Lib_GPIO_Config(void);
extern void TSC_Lib_Config(void);
extern void TSC_Lib_SleepConfig(void);

#endif


