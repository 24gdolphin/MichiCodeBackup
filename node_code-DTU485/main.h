#ifndef __MAIN_H
#define __MAIN_H
#include "sysconfig.h"
#if DEBUG
#define debug(format, args...)  do{printf(format, ##args);}while(0)
#else
#define debug(format, args...)  
#endif

#define PKG_HEADER 0x434d


#define Node_Id_Def  0x18120120

#define User_LowPowerBoard 


#define SLAVE


#define  SYS_FUNC_TIME_DBG 1




void ShowParaSet(void);
void testfunction(void);






#endif




