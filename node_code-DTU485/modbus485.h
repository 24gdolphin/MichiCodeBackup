
#ifndef __MODBUS485_H
#define __MODBUS485_H
#include "stm8l15x.h"
#include "main.h"



void Modebus_HengTongThree_ReadALL(uint8_t Addr,uint8_t *poutbuffer,uint8_t out_len,uint8_t times);



typedef struct
{
	uint32_t Avolat_IEEE;
	uint32_t Bvolat_IEEE;
	uint32_t Cvolat_IEEE;
	uint32_t Acurrent_IEEE;
	uint32_t Bcurrent_IEEE;				
	uint32_t Ccurrent_IEEE;	
	uint32_t Countpower_IEEE;
	uint32_t Powerfactor_IEEE;
	uint32_t Userfulpower_IEEE;
}stThreephaseelectricmeterIEEE;


typedef struct
{
	float  Avolat_float;
	float  Bvolat_float;
	float  Cvolat_float;
	float  Acurrent_float;
	float  Bcurrent_float;				
	float  Ccurrent_float;	
	float  Countpower_float;
	float  Powerfactor_float;
	float  Userfulpower_float;
}stThreephaseelectricmeterFLOAT;



typedef struct
{
	uint16_t Avolat_fina;
	uint16_t Bvolat_fina;
	uint16_t Cvolat_fina;
	uint16_t Acurrent_fina;
	uint16_t Bcurrent_fina;				
	uint16_t Ccurrent_fina;	
	uint32_t Countpower_fina;
	int16_t Powerfactor_fina;
	uint16_t Userfulpower_fina;
}stThreephaseelectricmeterFINALLY;


typedef struct
{
	uint16_t Avolat_fina;
	uint16_t Bvolat_fina;
	uint16_t Cvolat_fina;
	uint16_t Acurrent_fina;
	uint16_t Bcurrent_fina;				
	uint16_t Ccurrent_fina;	
	uint32_t Countpower_fina;
	int16_t Powerfactor_fina;
	int16_t Userfulpower_fina;
}stThreephaseelectricmeterFINALLY_Acrel;


void Modebus485_Get_AncrlElectricMeter(uint8_t addr ,uint8_t *pin , uint8_t len, uint8_t times);


float IEEE724_INTtoFloat(uint32_t datain);


void  Modebus485_ThreeWattMeter_ConverIEEE724(uint8_t *pin, uint8_t *pout, uint8_t len);


uint32_t Modebus485_Setuint8Uint32( uint8_t *pbuffer);

void Modebus485_Get_KEY(uint8_t Addr ,uint8_t ChannelChoice);


uint8_t Modebus485_Get_JDQ0202(uint8_t Addr,uint8_t ChannelChoice);  //


void Modebus485_Set_JDQ0202(uint8_t Addr,uint8_t ChannelChoice,uint8_t SetState,uint8_t times);  //

void Modebus485_Get_Hotwatermeter(uint8_t Addr,uint16_t RegType,uint8_t *poutbuffer,uint8_t times);

uint32_t Modebus485_188_Get_Hotwatermeter(uint32_t Addr,uint8_t *poutbuffer,uint8_t times);


void Modebus485_Get_Singlephaseelectricmeter(uint8_t Addr,uint8_t *poutbuffer,uint8_t out_len,uint8_t times);

void Modebus485_Get_Threephaseelectricmeter(uint8_t Addr,uint8_t *poutbuffer,uint8_t out_len,uint8_t times);  ///

void Modebus485_Get_Threephaseelectricmeter_Test(uint8_t add1,uint8_t add2,uint8_t add3,uint8_t add4);


void Modebus485_Get_C6( uint8_t *poutbuffer, uint8_t len, uint8_t times);


void Modebus485_Get_Flow(uint8_t Addr, uint8_t times,uint8_t *poutbuffer);




 uint8_t ModbusCrc16Check(uint8_t *frame, uint16_t len);

 uint16_t MGM_u16MBCRC16( uint8_t * pucFrame, uint16_t usLen );

uint8_t Modebus485_GetCrc(uint8_t *buf, uint8_t len);


void BCDtoHex(uint8_t *pBuff,uint8_t len); //BCD码转为十六进制




#endif


