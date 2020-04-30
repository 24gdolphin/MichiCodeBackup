#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H
#include "main.h"

/******DBUG******/
#define RX_SNRESTIMATE 0                //1->获取信噪�?0->不获�?
#define RSSI 1                          //1->获取RSSI 	0->不获�?
#define DBG_LORA_REG 0                  // 1->打印lora状态调�? 0->屏蔽
#define DBG_LORA_LOG 0                 //1->打印lora收发数据�? 0->不打�?

#define DEBUG 0  //调试
#define GetRunTime 1

#define Lora_GetWay6


#ifdef Lora_GetWay1
/**LORA PARAMETER**/
#define _Frequency       473730000// 473730000//473730000//470000000
#define _BandWidth         8                     // SignalBw [0:7.8kHz, 1:10.4 kHz, 2:15.6 kHz, 3:20.8 kHz, 4:31.2 kHz 5:41.6 kHz, 6:62.5 kHz, 7:125 kHz, 8:250 kHz, 9:500 kHz, other: Reserved]
#define _SpreadingFactor   10                    // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
#define _CodingRate        1                     // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define _TransmitPower     20                    //power
#define _ImplictHeaderOn   FALSE  
#define _CrcEnable         TRUE
#define _PreambleLength    6                    
#endif

#ifdef Lora_GetWay2
/**LORA PARAMETER**/
#define _Frequency       510000000// 473730000//473730000//470000000
#define _BandWidth         8                     // SignalBw [0:7.8kHz, 1:10.4 kHz, 2:15.6 kHz, 3:20.8 kHz, 4:31.2 kHz 5:41.6 kHz, 6:62.5 kHz, 7:125 kHz, 8:250 kHz, 9:500 kHz, other: Reserved]
#define _SpreadingFactor   10                    // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
#define _CodingRate        1                     // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define _TransmitPower     20                    //power
#define _ImplictHeaderOn   FALSE  
#define _CrcEnable         TRUE
#define _PreambleLength    6                    
#endif

#ifdef Lora_GetWay3
/**LORA PARAMETER**/
#define _Frequency       485000000// 473730000//473730000//470000000
#define _BandWidth         7                     // SignalBw [0:7.8kHz, 1:10.4 kHz, 2:15.6 kHz, 3:20.8 kHz, 4:31.2 kHz 5:41.6 kHz, 6:62.5 kHz, 7:125 kHz, 8:250 kHz, 9:500 kHz, other: Reserved]
#define _SpreadingFactor   9                   // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
#define _CodingRate        1                     // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define _TransmitPower     20                    //power
#define _ImplictHeaderOn   FALSE  
#define _CrcEnable         TRUE
#define _PreambleLength    6                    
#endif

#ifdef Lora_GetWay4
/**LORA PARAMETER**/
#define _Frequency      471710000// 473730000//473730000//470000000
#define _BandWidth         8                     // SignalBw [0:7.8kHz, 1:10.4 kHz, 2:15.6 kHz, 3:20.8 kHz, 4:31.2 kHz 5:41.6 kHz, 6:62.5 kHz, 7:125 kHz, 8:250 kHz, 9:500 kHz, other: Reserved]
#define _SpreadingFactor   10                    // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
#define _CodingRate        1                     // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define _TransmitPower     20                    //power
#define _ImplictHeaderOn   FALSE  
#define _CrcEnable         TRUE
#define _PreambleLength    6                    
#endif



#ifdef Lora_GetWay5
/**LORA PARAMETER**/
#define _Frequency       485000000// 473730000//473730000//470000000
#define _BandWidth         8                     // SignalBw [0:7.8kHz, 1:10.4 kHz, 2:15.6 kHz, 3:20.8 kHz, 4:31.2 kHz 5:41.6 kHz, 6:62.5 kHz, 7:125 kHz, 8:250 kHz, 9:500 kHz, other: Reserved]
#define _SpreadingFactor   10                   // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
#define _CodingRate        1                     // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define _TransmitPower     20                    //power
#define _ImplictHeaderOn   FALSE  
#define _CrcEnable         TRUE
#define _PreambleLength    6                    
#endif

#ifdef Lora_GetWay6
/**LORA PARAMETER**/
#define _Frequency       470000000// 473730000//473730000//470000000
#define _BandWidth         8                     // SignalBw [0:7.8kHz, 1:10.4 kHz, 2:15.6 kHz, 3:20.8 kHz, 4:31.2 kHz 5:41.6 kHz, 6:62.5 kHz, 7:125 kHz, 8:250 kHz, 9:500 kHz, other: Reserved]
#define _SpreadingFactor   10                   // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
#define _CodingRate        1                     // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define _TransmitPower     20                    //power
#define _ImplictHeaderOn   FALSE  
#define _CrcEnable         TRUE
#define _PreambleLength    6                    
#endif




/**********传感�?**********/
#define SENSOR  1
#define USE_KEY 1
#define DOOR_LOCK 0




#endif



