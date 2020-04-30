
#include "modbus485.h"
#include "board.h"
#include "stm8l15x.h"
#include "stdio.h"
#include "helper.h"


float IEEE724_INTtoFloat(uint32_t datain)
{
	uint8_t i=0;
	typedef union
	{
	float a;
	uint8_t b[4];
	}unionIEEE724;	
	
 	unionIEEE724 data;	

	data.a = 0.0;
	memset( data.b , 0 , 4);
	
	for( i=0; i<4; i++)
	{
		data.b[i] = (uint8_t)(datain>>(i*8));
	}
	printf("IEEE2floatToInt = %lf \r\n",data.a);	
	return data.a;	
}
// 鍑芥暟 灏嗘寜鐓у崗璁鍙栧埌鐨勪笁鐩哥數琛?4瀛楄妭鍘熷鏁版嵁杞崲鎴愬崗璁牸寮忚緭鍑?

/*
- L1鐩哥數鍘?(V), uint16, 鏁存暟锛屾湯涓や綅涓哄皬鏁帮紝word
- L2鐩哥數鍘?(V), uint16, 鏁存暟锛屾湯涓や綅涓哄皬鏁帮紝word
- L3鐩哥數鍘?(V), uint16, 鏁存暟锛屾湯涓や綅涓哄皬鏁帮紝word
- L1鐢垫祦 (A), uint16, 鏁存暟锛屾湯涓や綅涓哄皬鏁帮紝word
- L2鐢垫祦 (A), uint16, 鏁存暟锛屾湯涓や綅涓哄皬鏁帮紝word
- L3鐢垫祦 (A), uint16, 鏁存暟锛屾湯涓や綅涓哄皬鏁帮紝word
- 绱鐢甸噺 (kWh), uint32锛屾暣鏁帮紝鏈袱浣嶄负灏忔暟锛宒word
- 鍔熺巼鍥犳暟 (-1000 ~ 1000), int16,鏁存暟锛屾湯涓変綅涓哄皬鏁帮紝word
- 鏈夊姛鍔熺巼 (kW)锛寀int16, 鏁存暟锛屾湯涓や綅涓哄皬鏁? word
*/

#ifdef HengtongThree
	extern uint16_t HengtongThreeFailTimes_count;
	extern uint8_t HengtongThreeFailDataBuffer[36];
#endif

void  Modebus485_ThreeWattMeter_ConverIEEE724(uint8_t *pin, uint8_t *pout, uint8_t len)
{
	stThreephaseelectricmeterIEEE *pstThreephaseelectricmeterIEEE = NULL;
	stThreephaseelectricmeterFLOAT *PstThreephaseelectricmeterFLOAT = NULL;
	stThreephaseelectricmeterFINALLY *pstThreephaseelectricmeterFINALLY =NULL;
	
	uint8_t IEEE_BUFFER[36];
	uint8_t FLOAT_BUFFER[36];
	uint8_t FINALLY_BUFFER[20];
	
	pstThreephaseelectricmeterIEEE = (stThreephaseelectricmeterIEEE *)IEEE_BUFFER;
	PstThreephaseelectricmeterFLOAT = (stThreephaseelectricmeterFLOAT *)FLOAT_BUFFER;
	pstThreephaseelectricmeterFINALLY = (stThreephaseelectricmeterFINALLY *)FINALLY_BUFFER;
	
	memset(IEEE_BUFFER,0, 36);
	memset(FLOAT_BUFFER,0, 36);
	memset(FINALLY_BUFFER,0, 20);

	pstThreephaseelectricmeterIEEE = (stThreephaseelectricmeterIEEE *)pin;

	pstThreephaseelectricmeterIEEE->Avolat_IEEE = board_Invtert_Uint32(pstThreephaseelectricmeterIEEE->Avolat_IEEE);  //灏忔暟绔浆澶ф暟绔?
	pstThreephaseelectricmeterIEEE->Bvolat_IEEE = board_Invtert_Uint32(pstThreephaseelectricmeterIEEE->Bvolat_IEEE);
	pstThreephaseelectricmeterIEEE->Cvolat_IEEE = board_Invtert_Uint32(pstThreephaseelectricmeterIEEE->Cvolat_IEEE);
	pstThreephaseelectricmeterIEEE->Acurrent_IEEE= board_Invtert_Uint32(pstThreephaseelectricmeterIEEE->Acurrent_IEEE);
	pstThreephaseelectricmeterIEEE->Bcurrent_IEEE = board_Invtert_Uint32(pstThreephaseelectricmeterIEEE->Bcurrent_IEEE);
	pstThreephaseelectricmeterIEEE->Ccurrent_IEEE = board_Invtert_Uint32(pstThreephaseelectricmeterIEEE->Ccurrent_IEEE);
	pstThreephaseelectricmeterIEEE->Countpower_IEEE = board_Invtert_Uint32(pstThreephaseelectricmeterIEEE->Countpower_IEEE);
	pstThreephaseelectricmeterIEEE->Powerfactor_IEEE = board_Invtert_Uint32(pstThreephaseelectricmeterIEEE->Powerfactor_IEEE);
	pstThreephaseelectricmeterIEEE->Userfulpower_IEEE = board_Invtert_Uint32(pstThreephaseelectricmeterIEEE->Userfulpower_IEEE);

	//printf("stThreephaseelectricmeter--IEEE  \r\n");
	//helper_printHex((uint8_t *)(&pstThreephaseelectricmeterIEEE->Avolat_IEEE) ,36);
	//printf("\r\n\r\n");

	PstThreephaseelectricmeterFLOAT->Avolat_float = IEEE724_INTtoFloat(pstThreephaseelectricmeterIEEE->Avolat_IEEE);  //鏁村瀷杞诞鐐?
	PstThreephaseelectricmeterFLOAT->Bvolat_float = IEEE724_INTtoFloat(pstThreephaseelectricmeterIEEE->Bvolat_IEEE);
	PstThreephaseelectricmeterFLOAT->Cvolat_float = IEEE724_INTtoFloat(pstThreephaseelectricmeterIEEE->Cvolat_IEEE);
	PstThreephaseelectricmeterFLOAT->Acurrent_float = IEEE724_INTtoFloat(pstThreephaseelectricmeterIEEE->Acurrent_IEEE);
	PstThreephaseelectricmeterFLOAT->Bcurrent_float = IEEE724_INTtoFloat(pstThreephaseelectricmeterIEEE->Bcurrent_IEEE);
	PstThreephaseelectricmeterFLOAT->Ccurrent_float = IEEE724_INTtoFloat(pstThreephaseelectricmeterIEEE->Ccurrent_IEEE);
	PstThreephaseelectricmeterFLOAT->Countpower_float = IEEE724_INTtoFloat(pstThreephaseelectricmeterIEEE->Countpower_IEEE);
	PstThreephaseelectricmeterFLOAT->Powerfactor_float = IEEE724_INTtoFloat(pstThreephaseelectricmeterIEEE->Powerfactor_IEEE);
	PstThreephaseelectricmeterFLOAT->Userfulpower_float = IEEE724_INTtoFloat(pstThreephaseelectricmeterIEEE->Userfulpower_IEEE);

	//printf("stThreephaseelectricmeter--float \r\n");
	//helper_printHex((uint8_t *)(&PstThreephaseelectricmeterFLOAT->Avolat_float) ,36);
	//printf("\r\n\r\n");


	pstThreephaseelectricmeterFINALLY->Avolat_fina = (uint16_t )(PstThreephaseelectricmeterFLOAT->Avolat_float * 100);  //鏈�鍚庣殑鏁版嵁锛屾寜鐓ф枃妗ｅ崗璁斁澶ч�傚綋鍊嶆暟
	pstThreephaseelectricmeterFINALLY->Bvolat_fina = (uint16_t )(PstThreephaseelectricmeterFLOAT->Bvolat_float * 100);
	pstThreephaseelectricmeterFINALLY->Cvolat_fina = (uint16_t )(PstThreephaseelectricmeterFLOAT->Cvolat_float * 100);
	pstThreephaseelectricmeterFINALLY->Acurrent_fina = (uint16_t )(PstThreephaseelectricmeterFLOAT->Acurrent_float * 100);
	pstThreephaseelectricmeterFINALLY->Bcurrent_fina = (uint16_t )(PstThreephaseelectricmeterFLOAT->Bcurrent_float * 100);
	pstThreephaseelectricmeterFINALLY->Ccurrent_fina = (uint16_t )(PstThreephaseelectricmeterFLOAT->Ccurrent_float * 100);
	pstThreephaseelectricmeterFINALLY->Countpower_fina = (uint32_t )(PstThreephaseelectricmeterFLOAT->Countpower_float * 100);
	pstThreephaseelectricmeterFINALLY->Powerfactor_fina = (int16_t )(PstThreephaseelectricmeterFLOAT->Powerfactor_float * 1000);
	pstThreephaseelectricmeterFINALLY->Userfulpower_fina = (uint16_t )(PstThreephaseelectricmeterFLOAT->Userfulpower_float * 100);


	//helper_printTitleHex("Modebus485_ThreeWattMeter_ConverIEEE724" ,(uint8_t *)(&pstThreephaseelectricmeterFINALLY->Avolat_fina) ,20);
	printf("\r\n**************");
	printf("\r\n Av = %d, Bv = %d, Cv = %d \n",pstThreephaseelectricmeterFINALLY->Avolat_fina,pstThreephaseelectricmeterFINALLY->Bvolat_fina,pstThreephaseelectricmeterFINALLY->Cvolat_fina);
	printf("\r\n Ac = %d, Bc = %d, Cc = %d \n",pstThreephaseelectricmeterFINALLY->Acurrent_fina,pstThreephaseelectricmeterFINALLY->Bcurrent_fina,pstThreephaseelectricmeterFINALLY->Ccurrent_fina);
	printf("\r\n Countpower = %d, Powerfactor = %d, Userfulpower = %d \n",pstThreephaseelectricmeterFINALLY->Countpower_fina,pstThreephaseelectricmeterFINALLY->Powerfactor_fina,pstThreephaseelectricmeterFINALLY->Userfulpower_fina);
	printf("\r\n**************\r\n");	
	//printf("\r\n [Buffer]: stThreephaseelectricmeter--fina \r\n");
	helper_printHex((uint8_t *)(&pstThreephaseelectricmeterFINALLY->Avolat_fina) ,20);
	printf("\r\n\r\n");

	memcpy(pout,(uint8_t *)pstThreephaseelectricmeterFINALLY,len);	
	
}





uint32_t Modebus485_Setuint8Uint32( uint8_t *pbuffer)
{
   uint8_t data_buffer[4];
   uint32_t data;
   data = (((uint32_t)(*(pbuffer)))<<24)+(((uint32_t)(*(pbuffer+1)))<<16)+(((uint32_t)(*(pbuffer+2)))<<8)+(((uint32_t)(*(pbuffer+3))));
   return data; 
}


void Modebus485_Get_KEY(uint8_t Addr ,uint8_t ChannelChoice)
{
   uint8_t status;
	status = Modebus485_Get_JDQ0202(Addr, ChannelChoice);  //
	if(status == 0x01)
	Modebus485_Set_JDQ0202(0xfe, 0x03, 0x01, 3);
	else
	Modebus485_Set_JDQ0202(0xfe, 0x03, 0x02, 3);
}


// Three loop Ancrl Threephaseelectricmeter
//reg_buf 
//0x11 data_UI_buf	   		
//0x21 data_Userfulpower_buf
//0x0d data_Powerfactor_buf 
//0x70 data_CountPower_buf 

 




void Modebus485_Get_AncrlElectricMeter(uint8_t addr ,uint8_t *pin , uint8_t len, uint8_t times)
{

	stThreephaseelectricmeterFINALLY_Acrel *pstThreephaseelectricmeterFINALLY_Acrel = (stThreephaseelectricmeterFINALLY_Acrel *) pin;
	
	//memcpy((uint8_t *)pstThreephaseelectricmeterFINALLY, pin, 
	uint8_t loopchioce;
	uint8_t data_UI_buf[36] = {0};			//鐢垫祦鐢靛帇
	uint8_t data_CountPower_buf[12] = {0};		//绱鐢甸噺
	uint8_t data_Powerfactor_buf[6] = {0};	//鍔熺巼鍥犳暟
	uint8_t data_Userfulpower_buf[6] = {0};	//鍔熺巼

	uint8_t data_all_buf[70];
	
	 uint8_t reg_addr_buf[4] = {0x11,0x21,0x0d,0x70};
	 uint8_t reg_count_buf[4] = {12,3,3,6};
	uint8_t send_buf[20];
	uint8_t rec_buf[50];
	uint8_t i = 0;
	uint8_t k = 0;
	uint8_t j = 0;
	uint8_t data_lenth = 0;
	uint16_t crc_uint16 = 0;
	uint8_t error = 0;
	uint16_t crc_uint16_test;
	uint16_t Powerfactor_fina_int16 = 0;
	uint16_t Userfulpower_fina_int16 = 0;

#ifdef AcrelThreeWattMeter

	loopchioce = addr - AcrelThreeWattMeter_BaseAddr;

	addr = AcrelThreeWattMeter_BaseAddr;

#endif


	for( i = 0; i < times; i++)
	{

			memset(data_UI_buf , 0, 36);
			memset(data_CountPower_buf , 0, 12);
			memset(data_Powerfactor_buf , 0, 6);
			memset(data_Userfulpower_buf , 0, 6);
			memset(rec_buf , 0, 50);	
			memset(data_all_buf , 0, 70);
			memset(send_buf , 0, 20);
			//memset(data_all_buf , 0, 70);
			
		printf("Modebus485_Get_AncrlElectricMeter START --------\r\n");
		for( k = 0; k < 4 ; k++)
		{

			
			send_buf[0] = addr;
			send_buf[1] = 0x03;
			send_buf[2] = 0x00;
			send_buf[3] = reg_addr_buf[k];
			send_buf[4] = 0x00;
			send_buf[5] = reg_count_buf[k];

			crc_uint16 = MGM_u16MBCRC16(send_buf,6);
			send_buf[6] = (uint8_t)crc_uint16;
			send_buf[7] = (uint8_t)(crc_uint16>>8); 
			helper_printHex(send_buf,8); 
			printf("\r\n");	
			
			board_Rs485Send(send_buf,8);
			data_lenth = board_Rs485Recv(rec_buf, 30, 100);
			printf("data_lenth = %d:",data_lenth);
			helper_printHex(rec_buf,data_lenth);
			printf("\r\n");
				if( data_lenth > 7)
				{
						crc_uint16 = ((uint16_t)(rec_buf[data_lenth - 2])) + (((uint16_t)(rec_buf[data_lenth - 1]))<<8);
						crc_uint16_test = MGM_u16MBCRC16(rec_buf,data_lenth-2);
						if( crc_uint16 == crc_uint16_test )
						{
							//printf("crc_uint16 OK\r\n");
							error = 1;
							
							if(k == 0)
							{
									data_UI_buf[0] =  rec_buf[0 + 3];	//v1A
									data_UI_buf[1] =  rec_buf[1 + 3];
									data_UI_buf[2] =  rec_buf[0 + 3];	//v1B
									data_UI_buf[3] =  rec_buf[1 + 3];
									data_UI_buf[4] =  rec_buf[0 + 3];	//v1C
									data_UI_buf[5] =  rec_buf[1 + 3];
	
									data_UI_buf[6] =  rec_buf[2 + 3];	//v2A
									data_UI_buf[7] =  rec_buf[3 + 3];
									data_UI_buf[8] =  rec_buf[2 + 3];	//v2B
									data_UI_buf[9] =  rec_buf[3 + 3];
									data_UI_buf[10] =  rec_buf[2 + 3];	//v2A
									data_UI_buf[11] =  rec_buf[3 + 3];
									
									data_UI_buf[12] =  rec_buf[4 + 3];	//v3A
									data_UI_buf[13] =  rec_buf[5 + 3];
									data_UI_buf[14] =  rec_buf[4 + 3];	//v3B
									data_UI_buf[15] =  rec_buf[5 + 3];
									data_UI_buf[16] =  rec_buf[4 + 3];	//v3C
									data_UI_buf[17] =  rec_buf[5 + 3];
								
								memcpy( (uint8_t *)&data_UI_buf[18],(uint8_t *)&rec_buf[9] ,18);// 0-35
								memcpy(data_all_buf,data_UI_buf,36); 


								
							}
								
							if(k == 1)
							{
								//for( j = 0 ; j < 6 ; j++)
									//{
								//		rec_buf[ 3+ j] = j+54;
								//	}
								//memcpy(data_Userfulpower_buf,rec_buf+3,6);
								
								memcpy((uint8_t *)&data_all_buf[54],rec_buf+3,6);  //55-60
								
							}
							if(k == 2)
							{
								//	for( j = 0 ; j < 6 ; j++)
								//	{
								//		rec_buf[ 3+ j] = j+48;
								//	}
								//memcpy(data_Powerfactor_buf,rec_buf+3,6);
								memcpy((uint8_t *)&data_all_buf[48],rec_buf+3,6); //48-54
							}

							if(k == 3)
							{
								//	for( j = 0 ; j < 12 ; j++)
								//	{
								//		rec_buf[ 3+ j] = j+36;
								//	}
								//memcpy(data_CountPower_buf,rec_buf+3,12);

								memcpy((uint8_t *)&data_all_buf[36],rec_buf+3,12); // 36-47

							}
			
						}
						else
						{
							printf("crc_uint16_test ERROR \r\n");
							error = 0;
						}
						
				}
				else
				{
					error = 0;
				}

		}

		if( error == 1)
			break;

	}

	helper_printTitleHex("data_UI_buf", data_UI_buf, 36);
	helper_printTitleHex("data_CountPower_buf", data_CountPower_buf, 12);
	helper_printTitleHex("data_Powerfactor_buf", data_Powerfactor_buf, 6);
	helper_printTitleHex("data_Userfulpower_buf", data_Userfulpower_buf, 6);

	helper_printTitleHex("data_all_buf", data_all_buf, 60);

	memcpy((uint8_t *)&pstThreephaseelectricmeterFINALLY_Acrel->Avolat_fina,(uint8_t *)&data_all_buf[loopchioce * 6 + 0],2);
	memcpy((uint8_t *)&pstThreephaseelectricmeterFINALLY_Acrel->Bvolat_fina,(uint8_t *)&data_all_buf[loopchioce * 6 + 2],2);
	memcpy((uint8_t *)&pstThreephaseelectricmeterFINALLY_Acrel->Cvolat_fina,(uint8_t *)&data_all_buf[loopchioce * 6 + 4],2);


	pstThreephaseelectricmeterFINALLY_Acrel->Avolat_fina = pstThreephaseelectricmeterFINALLY_Acrel->Avolat_fina * 10;
	pstThreephaseelectricmeterFINALLY_Acrel->Bvolat_fina = pstThreephaseelectricmeterFINALLY_Acrel->Bvolat_fina * 10;
	pstThreephaseelectricmeterFINALLY_Acrel->Cvolat_fina = pstThreephaseelectricmeterFINALLY_Acrel->Cvolat_fina * 10;


	//memcpy((uint8_t *)&pstThreephaseelectricmeterFINALLY_Acrel->Powerfactor_fina,data_all_buf+(loopchioce * 2 + 48),2);	
	//memcpy((uint8_t *)&pstThreephaseelectricmeterFINALLY_Acrel->Userfulpower_fina,data_all_buf+(loopchioce * 2 + 54),2);

	memcpy((uint8_t *)&Powerfactor_fina_int16,(uint8_t *)&data_all_buf[loopchioce * 2 + 54],2);	

	memcpy((uint8_t *)&Userfulpower_fina_int16,(uint8_t *)&data_all_buf[loopchioce * 2 + 48],2);


	//int16_t Powerfactor_fina_int16 = 0;
	//int16_t Userfulpower_fina_int16 = 0;


	pstThreephaseelectricmeterFINALLY_Acrel->Userfulpower_fina = (int16_t)Powerfactor_fina_int16;
	pstThreephaseelectricmeterFINALLY_Acrel->Powerfactor_fina = (int16_t)Userfulpower_fina_int16;

	
	memcpy((uint8_t *)&pstThreephaseelectricmeterFINALLY_Acrel->Acurrent_fina,(uint8_t *)&data_all_buf[loopchioce * 6 + 0 + 18],2);
	memcpy((uint8_t *)&pstThreephaseelectricmeterFINALLY_Acrel->Bcurrent_fina,(uint8_t *)&data_all_buf[loopchioce * 6 + 2 + 18],2);
	memcpy((uint8_t *)&pstThreephaseelectricmeterFINALLY_Acrel->Ccurrent_fina,(uint8_t *)&data_all_buf[loopchioce * 6 + 4 + 18],2);	

	memcpy((uint8_t *)&pstThreephaseelectricmeterFINALLY_Acrel->Countpower_fina,(uint8_t *)&data_all_buf[loopchioce * 4 +  36],4);		
	

	helper_printTitleHex("AncrlElectricMeter",(uint8_t *)pstThreephaseelectricmeterFINALLY_Acrel,20);


	printf("Modebus485_Get_AncrlElectricMeter END--------\r\n");

	
}



//恒通单相电表 导轨式 DDS8588
//485地址 参考表 2018-1110-010  后三位的 hex即为表地址
//通讯格式 1(地址)+1(功能码-查询0x03)+2(寄存器地址)+2(字节数)+2(CRC)(校验 低字节在前)
//寄存器地址
//0          总有功电量          float
//0x64       电压				float
//0x6A       电流             float
//0x76       有功功率    		float
//0x8e       总功率因数 			float
//0x90       电网频率 			float

void Modebus485_Get_Singlephaseelectricmeter(uint8_t Addr,uint8_t *poutbuffer,uint8_t out_len,uint8_t times)  ///
{
  uint8_t i,j,k;
  uint8_t end_buffer[30] = {0};
  uint8_t data_buffer[20] = {0}; 
  uint8_t send_buffer[10] = {0};
  uint8_t reg_buffer[6] = {0x00,0x64,0x6a,0x76,0x8e,0x90};
  uint8_t data_lenth = 0;
  uint16_t crc_uint16 = 0;
  uint8_t error = 0;

   send_buffer[0] = Addr;  //设备地址
   send_buffer[1] = 0x03;  // 功能码 查询
   
	for(k = 0;k<times;k++)
	{
	   for(i = 0;i<6;i++)
	   	{
	       send_buffer[2] = 0x00;  //寄存器地址  //(uint8_t)(reg_buffer[i]>>8)
		   send_buffer[3] = (reg_buffer[i]);
		   
		   send_buffer[4] = 0x00;  //查询2字节
		   send_buffer[5] = 0x02;
		   
		   crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
		   send_buffer[6] = (uint8_t)crc_uint16;
		   send_buffer[7] = (uint8_t)(crc_uint16>>8);  
		   
		   board_Rs485Send(send_buffer,8);
		   helper_printHex(send_buffer,8);   
		   data_lenth = board_Rs485Recv(data_buffer, 20, 100);
		   if(data_lenth>3)
		   	{
		   	   error = 0;
		   	   for(j = 0;j<4;j++)
		   	   	{
		   	   	  end_buffer[j+i*4] = data_buffer[j+3];
		   	   	}
		   	}
		   else error = 1;
	   	}  

	//for(i =0;i<out_len;i++)
	 // {
	 //   *(poutbuffer+i) = end_buffer[i];
	//  }	

	memcpy(poutbuffer, end_buffer,out_len);
	//helper_printTitleHex("5555555555555555555", poutbuffer, 24);
	if(error == 0)	//如果第一个轮询里 每包数据的data_lenth 都大于3 默认收到了，退出大循环，有一次不大于3，就遵循times
	  break;   	
}

	helper_printTitleHex("Modebus_485 Singlephaseelectricmeter",end_buffer, 24);   
  
}


void Modebus_HengTongThree_ReadALL(uint8_t Addr,uint8_t *poutbuffer,uint8_t out_len,uint8_t times)
{


  uint8_t i,j,k;
  uint8_t m = 0;

  uint8_t data_buffer[50] = {0}; 
  uint8_t send_buffer[]= {0x01,0x03,0x17,0x70,0x00,0x12,0xd4,0x76};

  uint8_t data_lenth = 0;
  uint16_t crc_uint16 = 0;
  uint16_t crc_uint16_test = 0;
  uint8_t error = 0;

  printf("\r\n Modebus_HengTongThree_ReadALL \r\n");

	for(k = 0; k<times; k++)
	{

		send_buffer[0] = Addr; 
		send_buffer[1] = 0x03; 


		send_buffer[2] = 0x17;	//寄存器地址  //(uint8_t)(reg_buffer[i]>>8)
		send_buffer[3] = 0x70;

		send_buffer[4] = 0x00;	//查询2字节
		send_buffer[5] = 0x012;

		crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
		send_buffer[6] = (uint8_t)crc_uint16;
		send_buffer[7] = (uint8_t)(crc_uint16>>8);	
		
			helper_printTitleHex("send_buffer", send_buffer, 8);
			board_Rs485Send(send_buffer,8);   
			data_lenth = board_Rs485Recv(data_buffer, 50, 300);
			helper_printTitleHex("data_buffer", data_buffer, data_lenth);

			//printf("[INFO]: Modebus_HengTongThree_ReadALL ---55555555555555555555555555-- NOT OK \r\n");

			

			if(data_lenth > 8)
			{
				crc_uint16 = ((uint16_t)(data_buffer[data_lenth - 2])) + (((uint16_t)(data_buffer[data_lenth - 1]))<<8);
				crc_uint16_test = MGM_u16MBCRC16(data_buffer,data_lenth-2);
				if( crc_uint16 == crc_uint16_test )
				{
					printf("[INFO]: Modebus_HengTongThree_ReadALL -----  OK \r\n");
					memcpy( poutbuffer, data_buffer+3, 36);
				    memset(data_buffer , 0, 50);
					error = 1;
				   
				}
				else
				{
						//HengtongThreeCRCFailTimes_count ++;
						error = 0;
						
						memset(data_buffer , 0, 50);
						
						break;
				}

			}
			else 
			{
					// HengtongThreeCRCFailTimes_count ++;
					memset(data_buffer , 0, 40);
					//break; //error = 0;
			}

			m++;
			Delayms(30);
			printf("[Value]: Modebus_HengTongThree_ReadALL try to send times = %d \r\n\r\n",m);
			if(error == 1)	//涓�娆℃垚鍔熷悗 閫�鍑?
			  break;   	
	


		printf("\r\n\r\n");
	}


	
}



//鎭掗�氫笁鐩哥數琛?瀵艰建寮?DTSU8588
//485鍦板潃 鍙傝�冭〃 2018-1110-010  鍚庝笁浣嶇殑 hex鍗充负琛ㄥ湴鍧�
//閫氳鏍煎紡 1(鍦板潃)+1(鍔熻兘鐮?鏌ヨ0x03)+2(瀵勫瓨鍣ㄥ湴鍧�)+2(瀛楄妭鏁?+2(CRC)(鏍￠獙 浣庡瓧鑺傚湪鍓?
//瀵勫瓨鍣ㄥ湴鍧�
//0x64       A鐩哥數鍘?          float
//0x66       B鐩哥數鍘?			float
//0x68       C鐩哥數鍘?       	float
//0x6a       A鐩哥數娴?   		float
//0x6c       B鐩哥數娴?		float
//0x6e       C鐩哥數娴?			float
//0x00       绱鐢甸噺 			float
//0x8e      鍔熺巼鍥犳暟 			float
//0x86       鏈夊姛鍔熺巼			float

//19.3.13 涔愯櫣鍧婃柊澧炶妭鐐?杩欐壒485寤舵椂300ms


void Modebus485_Get_Threephaseelectricmeter(uint8_t Addr,uint8_t *poutbuffer,uint8_t out_len,uint8_t times)  ///
{
  uint8_t i,j,k;
  uint8_t m = 0;
  uint8_t end_buffer[40] = {0};
  uint8_t data_buffer[40] = {0}; 
  uint8_t send_buffer[10] = {0};
  uint8_t reg_buffer[] = {0x64,0x66,0x68,0x6a,0x6c,0x6e,0x00,0x8e,0x76};
  uint8_t data_lenth = 0;
  uint16_t crc_uint16 = 0;
  uint16_t crc_uint16_test = 0;
  uint8_t error = 0;


   memset(end_buffer, 0 , 40);
   memset(data_buffer, 0 , 40);
   memset(send_buffer, 0 , 10);

   send_buffer[0] = Addr; 
   send_buffer[1] = 0x03; 

	for(k = 0; k<times; k++)
	{
		   for(i = 0; i<sizeof(reg_buffer); i++)
		   	{
		       send_buffer[2] = 0x00;  //寄存器地址  //(uint8_t)(reg_buffer[i]>>8)
			   send_buffer[3] = (reg_buffer[i]);
			   
			   send_buffer[4] = 0x00;  //查询2字节
			   send_buffer[5] = 0x02;
			   
			   crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
			   send_buffer[6] = (uint8_t)crc_uint16;
			   send_buffer[7] = (uint8_t)(crc_uint16>>8);  

			   memset(data_buffer , 0, 40);
			   
			   board_Rs485Send(send_buffer,8);
			   //helper_printHex(send_buffer,8);   
			   
			   data_lenth = board_Rs485Recv(data_buffer, 20, 300);
			  // printf("[Value]: i = %d , data_length = %d \n",i,data_lenth);
			   //helper_printHex(data_buffer,data_lenth); 
			   if(data_lenth > 8)
			   	{
					crc_uint16 = ((uint16_t)(data_buffer[data_lenth - 2])) + (((uint16_t)(data_buffer[data_lenth - 1]))<<8);
					crc_uint16_test = MGM_u16MBCRC16(data_buffer,data_lenth-2);
					//printf("[Value]: crc_uint16 = %x ,crc_uint16_test = %x\r\n",crc_uint16,crc_uint16_test);
					if( crc_uint16 == crc_uint16_test )
					{
						printf("[INFO]: CRC OK \r\n");

				   	   for(j = 0;j<4 ;j++)
				   	   	{
				   	   	  end_buffer[j+i*4] = data_buffer[j+3];
				   	   	}
					   memset(data_buffer , 0, 40);
					   
					}
					else
					{
						//HengtongThreeCRCFailTimes_count ++;
						error = 0;
						memset(data_buffer , 0, 40);
						memset(end_buffer , 0, 40);
						printf("[INFO]: CRC error memset(end_buffer , 0, 40); \r\n");
						break;
					}
					if( i == 8)
					{
						error = 1;
						printf("[INFO]: Get All Data \r\n");
						
					}
			   	}
			   else 
			   	{
					 //HengtongThreeCRCFailTimes_count ++;
					memset(end_buffer , 0, 40);
					memset(data_buffer , 0, 40);
					printf("[INFO]: data_lenth > 8 ,memset(end_buffer , 0, 40); \r\n");
					break; //error = 0;
			    }
		   	}  
	   m++;
		   Delayms(30);
	printf("[Value]: Modebus485_Get_Threephaseelectricmeter try to send times = %d \r\n\r\n",m);
	if(error == 1)	//涓�娆℃垚鍔熷悗 閫�鍑?
	  break;   	
}

	if( m < (times - 2))
	{
		//AcqOK_flag = 1;
		printf("[INFO]: Modebus485_Get_Threephaseelectricmeter -----  OK \r\n");
	}
	else
	{
		//AcqOK_flag = 0;
		printf("[INFO]: Modebus485_Get_Threephaseelectricmeter ----- NOT OK \r\n");
	}
	memcpy(poutbuffer , end_buffer , out_len);
	helper_printHex(poutbuffer, out_len);
	printf("\r\n\r\n");
  
}


void Modebus485_Get_Threephaseelectricmeter_Test(uint8_t add1,uint8_t add2,uint8_t add3,uint8_t add4)
{
	uint8_t data_buffer[40];
	uint8_t i;
	uint8_t addr_buffer[4];

	printf("\r\n Modebus485_Get_ThreephaseelectricmeterTest  1 \r\n");
	Modebus485_Get_Threephaseelectricmeter(add1,data_buffer,36,1);
	printf("\r\n Modebus485_Get_ThreephaseelectricmeterTest  2 \r\n");
	Modebus485_Get_Threephaseelectricmeter(add2,data_buffer,36,1);
	printf("\r\n Modebus485_Get_ThreephaseelectricmeterTest  3 \r\n");	
	Modebus485_Get_Threephaseelectricmeter(add3,data_buffer,36,1);
	printf("\r\n Modebus485_Get_ThreephaseelectricmeterTest  4 \r\n");	
	Modebus485_Get_Threephaseelectricmeter(add4,data_buffer,36,1);	
}



//力创 LCR-U型热水表  需要手动上位机软件配置 波特率等 初始地址0x01  红 + 绿A 蓝B 黄-
//查询格式 01(地址)+03(功能码-查询)+00 10(寄存器地址)+00 01(查询寄存器个数）+CRCX(2字节)
//寄存器定义 地址范围                      说明                           
// 热功率       000A-000B        BCD 高字节在前		xxxxxx.xx
// 瞬时流量      000C-000D        BCD 高字节在前xxxx.xxxx
// 累计流量     000E-000F 	  BCD DN125及以上的无小数 高字节在前xxxxxx.xx
// 进水温度      0010-0011        BCD 最高字节无效，高字节在前xxxxxx.xx              只有低3字节有效
// 回水温度      0012-0013        BCD 最高字节无效，高字节在前xxxxxx.xx
// FE FE 68 20 (01 00 64 31) 00 59 42 21 03 61 0A 00 48 16
// FE FE 68 20 01 00 64 31 00 59 42 A1 1A 61 0A 00    current heat 80 EC DC 15 00 00 00 00   current flow count 98 B1 F5 01 00 00 00 00    00 00 00 00  0C 03  0A 94 16 
void Modebus485_Get_Hotwatermeter(uint8_t Addr,uint16_t RegType,uint8_t *poutbuffer,uint8_t times)
{
   uint8_t i,j;
   uint8_t data_buffer[20] = {0};
   uint8_t send_buffer[20] = {0};
   //uint8_t test_buffer[8] = {0x01 ,0x03 ,0x00 ,0x04 ,0x00 ,0x04 ,0x05 ,0xc8};
   uint8_t data_lenth = 0;
   uint16_t crc_uint16 = 0;
   uint8_t error = 0;


   //Addr = 0x01;
   //RegType = 0x0004;
   
   send_buffer[0] = Addr;  //设备地址
   send_buffer[1] = 0x03;  // 功能码 查询
   send_buffer[2] = (uint8_t)(RegType>>8);  //寄存器地址
   send_buffer[3] = (uint8_t)(RegType);
   
   send_buffer[4] = 0x00;  //查询4字节
   send_buffer[5] = 0x02;

	   crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
	   send_buffer[6] = (uint8_t)crc_uint16;
	   send_buffer[7] = (uint8_t)(crc_uint16>>8); 
	   
   for(i = 0;i<times ;i++)
   	{	   
	   
	   //helper_printHex(send_buffer, 8);
	   
	   board_Rs485Send(send_buffer,8);
	   data_lenth = board_Rs485Recv(data_buffer, 20, 50);
	   if(data_lenth > 3)
	   	{
	   		error = 0;
		   	   for(j = 0;j<4;j++)
		   	   	{
		   	   	  *(poutbuffer+j) = data_buffer[j+3];
		   	   	}					
	   	}
	   else 
	   	{
	   		error = 1; 
	   	}
	   if(error == 0)
	   	break;
   	}

   helper_printHex(data_buffer, data_lenth); 


}




// FE FE 68 20 (01 00 64 31) 00 59 42 21 03 61 0A 00 48 16
// FE FE 68 20 01 00 64 31 00 59 42 A1 1A 61 0A 00    current heat 80 EC DC 15 00 00 00 00   current flow count 98 B1 F5 01 00 00 00 00    00 00 00 00  0C 03  0A 94 16 


uint32_t Modebus485_188_Get_Hotwatermeter(uint32_t Addr,uint8_t *poutbuffer,uint8_t times)
{		
	uint8_t send_buffer[20];
	uint8_t i,j,data_lenth;
	uint8_t data_buffer[50];
	uint8_t error = 0;
	uint8_t flow_buffer[8];
	uint32_t flow_a = 0;  //鏁存暟
	uint32_t flow_b = 0;  //灏忔暟


	send_buffer[0] = 0xfe;
	send_buffer[1] = 0xfe;
	send_buffer[2] = 0x68;
	send_buffer[3] = 0x20;
	send_buffer[4] = (uint8_t)Addr;
	send_buffer[5] = ((uint8_t)(Addr>>8));
	send_buffer[6] = ((uint8_t)(Addr>>16));
	send_buffer[7] = ((uint8_t)(Addr>>24));
	send_buffer[8] = 0x00;
	send_buffer[9] = 0x59;
	send_buffer[10] = 0x42;
	send_buffer[11] = 0x21;
	send_buffer[12] = 0x03;
	send_buffer[13] = 0x61;
	send_buffer[14] = 0x0a;
	send_buffer[15] = 0x00;
	send_buffer[16] = Modebus485_GetCrc(send_buffer+2, 14);
	send_buffer[17] = 0x16;

	for(i = 0;i<times ;i++)
	 {		
		board_Rs485Send(send_buffer,18);
		//printf(" board_Rs485Send(send_buffer,18); \r\n");
		//helper_printHex(send_buffer,18);		
		//printf(" board_Rs485Send(send_buffer,18); \r\n");		
		data_lenth = board_Rs485Recv(data_buffer, 50, 100);
		//helper_printHex(data_buffer,data_lenth);
	
		if(data_lenth > 3)
		 {
			 error = 0;
			for(j = 0;j<8;j++)
			 {
			   //*(poutbuffer+j) = data_buffer[j+24];
			   flow_buffer[j] = data_buffer[31-j];
			 }
		printf(" flow_buffer[j] \r\n");			
			helper_printHex(flow_buffer,8);

			
			flow_a = (uint32_t)(flow_buffer[3]) + (((uint32_t)(flow_buffer[2]))<<8) + (((uint32_t)(flow_buffer[1]))<<16) + (((uint32_t)(flow_buffer[0]))<<24);
			flow_b = (uint32_t)(flow_buffer[7]) + (((uint32_t)(flow_buffer[6]))<<8) + (((uint32_t)(flow_buffer[5]))<<16) + (((uint32_t)(flow_buffer[4]))<<24);
			flow_a = (flow_a * 1000) + (flow_b / 100000);
			printf(" @@Modebus485_188_Get_Hotwatermeter flow_a = %ld \r\n ",flow_a);
			//memcpy((uint8_t *)&flow_a , poutbuffer, 4);

			
		 }
		else 
		 {
			 error = 1; 
		 }
		if(error == 0)
		 break;
	 }
	
	//helper_printHex(data_buffer, data_lenth); 
			return 	flow_a;
	
}







//得到水表流量
//地址为水表条码 后2位 0x99
// 读取总用水量 发送格式 99 03 00 00 00 02 d8 13 返回 99 03 04 （00 00 00 64 ）E2 11   0x00000064 *0.01 = 实际水量  大水表 50
// 12 03 00 00 00 02 C6 A8   0x12,0x03,0x00,0x00,0x00,0x02,0xc6,0xa8   //2018F101-37
void Modebus485_Get_Flow(uint8_t Addr, uint8_t times,uint8_t *poutbuffer)
{
   uint8_t data_lenth;
   uint8_t i,j;
   uint8_t error = 0;
   uint16_t  crc_uint16;
   //uint8_t flow_buffer[] = {0x99,0x03,0x00,0x00,0x00,0x02,0xd8,0x13};  //读固定地址水表的流量   
  // uint8_t flow_buffer[] = { 0x09,0x03,0x00,0x00,0x00,0x02,0xc5,0x43};  //读固定地址水表的流量  
   uint8_t send_buffer[15] = {0};
   uint8_t data_buffer[15] = {0};

	for(i = 0; i<times; i++)
	{
	   send_buffer[0] = Addr;  //设备地址
	   send_buffer[1] = 0x03;  // 功能码 
	   send_buffer[2] = 0x00;  //寄存器地址
	   send_buffer[3] = 0x00;
	   send_buffer[4] = 0x00;
	   send_buffer[5] = 0x02;
	   crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
	   send_buffer[6] = (uint8_t)crc_uint16;
	   send_buffer[7] = (uint8_t)(crc_uint16>>8); 
	   board_Rs485Send(send_buffer,8);
	   //helper_printHex(send_buffer, 8);
	   data_lenth = board_Rs485Recv(data_buffer, 10, 100);
	   if(data_lenth > 3)
	   	{	   	
		    error = 0;
		   	   for(j = 0;j<4;j++)
		   	   	{
		   	   	  *(poutbuffer+j) = data_buffer[j+3];
		   	   	}					
	   	}
	   else 
	   	{
		   	error = 1;
	   	}
	   if(error == 0)
	   	break;
   
	}
   helper_printHex(data_buffer, data_lenth);
}



// 得到C6 各个参数 地址 01
//发送的原始数据 01 03 00 00 00 09 85 cc
//                         甲醛      TVOC   PM25   温度     湿度     AQI    PM1    PM10   CO2
//返回的原始数据 01 03 12| 00 00 |00 00| 00 1B |00 DD| 00 33 |00 26| 00 15| 00 21 |02 26 |9F B6   高字节在前 低字节在后
//甲醛：3位小数 mg/m3
//TVOC: 3位小数 mg/m3
//PM25: 无小数 ug/m3
//温度: 1位小数
//湿度: 无小数 百分比
//AQI:  无小数
//PM1:  无小数 ug/m3
//PM10: 无小数 ug/m3
//CO2:  无小数
void Modebus485_Get_C6( uint8_t *poutbuffer, uint8_t len, uint8_t times)
{
  uint8_t data_len;
  uint8_t i,k;
  uint8_t data_buffer[30] = {0};
  uint8_t send_buffer[] = {0x01 ,0x03 ,0x00 ,0x00, 0x00, 0x09 ,0x85 ,0xcc};
  uint8_t error = 0;
  for(k = 0;k<times;k++)
  	{
	   board_Rs485Send(send_buffer,sizeof(send_buffer));   
	   data_len = board_Rs485Recv(data_buffer, 30, 100); 
	   helper_printHex(data_buffer, data_len); 
	   if(data_len>3)
	   	{  
	   	  error = 0;
	   	  for(i=0;i<len;i++)
	   	  	{
		   	   *(poutbuffer+i) =  data_buffer[i+3];
	   	  	}
	   	}
	   else error = 1;
       if(error == 0)
	   	break;	   
  	}
}



// 针对DMA0202型继电器状态读取           继电器吸合	光耦
// ChannelChoice 	0x01-通道1  	   0x02-通道2 		0x03-通道1 2全选
// ChannelChoice 	0x04-继电器	1  0x05-继电器2 		0x06-继电器1 2全选
// 继电器地址 测试0xfe
//返回   状态
//    开    闭
//  0x01 0x02 

//// 锛侊紒锛侊紒锛侊紒锛侊紒锛佷笉鑳藉崟绾潬杩斿洖鍊煎垽璇伙紝鏈夊彲鑳芥帴2璺紝閮芥墦寮�锛岄潬涓�涓暟鍊煎垽鏂氨涓嶅浜?

 uint8_t Modebus485_Get_JDQ0202(uint8_t Addr,uint8_t ChannelChoice)
{
	uint8_t data_lenth = 0;
	uint8_t data_buffer[10];
	uint8_t send_buffer[10];
	uint16_t crc_uint16 = 0;
	switch (ChannelChoice)
		{
		case 0x01:
					send_buffer[0] = Addr;	//设备地址
					send_buffer[1] = 0x02;	// 功能码 
					send_buffer[2] = 0x00;	//寄存器地址
					send_buffer[3] = 0x00;
					send_buffer[4] = 0x00;
					send_buffer[5] = 0x02;
					
					crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
					send_buffer[6] = (uint8_t)crc_uint16;
					send_buffer[7] = (uint8_t)(crc_uint16>>8); 
					board_Rs485Send(send_buffer,8);
					data_lenth = board_Rs485Recv(data_buffer, 10, 50);  
					//helper_printHex(data_buffer, data_lenth);
					if(data_buffer[3] == 0x01)
						{
						    //printf("guang ou1----  0x01\n");
							return 0x01;
						}
					else 
						{
							if(data_buffer[3] == 0x03)
								{
								    //printf("guang ou1----  0x01\n");
									return 0x01;
								}
							else 
								{
									return 0x00;
								}
						}



					
			break;
		case 0x02:	
			        send_buffer[0] = Addr;	//设备地址
					send_buffer[1] = 0x02;	// 功能码 
					send_buffer[2] = 0x00;	//寄存器地址
					send_buffer[3] = 0x00;
					send_buffer[4] = 0x00;
					send_buffer[5] = 0x02;
					
					crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
					send_buffer[6] = (uint8_t)crc_uint16;
					send_buffer[7] = (uint8_t)(crc_uint16>>8); 
					board_Rs485Send(send_buffer,8);
					data_lenth = board_Rs485Recv(data_buffer, 10, 50);  
					helper_printHex(data_buffer, data_lenth);
					if(data_buffer[3] == 0x02)
							{
								printf("guang ou2----  0x01\n");
								return 0x01;
							}
						else 
							{
							if(data_buffer[3] == 0x03)
								{
								    //printf("guang ou2----  0x01\n");
									return 0x01;
								}
							else 
								{
									return 0x00;
								}

							}

		
			break;		
		case 0x03:	
					send_buffer[0] = Addr;	//设备地址
					send_buffer[1] = 0x02;	// 功能码 
					send_buffer[2] = 0x00;	//寄存器地址
					send_buffer[3] = 0x00;
					send_buffer[4] = 0x00;
					send_buffer[5] = 0x02;
					
					crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
					send_buffer[6] = (uint8_t)crc_uint16;
					send_buffer[7] = (uint8_t)(crc_uint16>>8); 
					board_Rs485Send(send_buffer,8);
					data_lenth = board_Rs485Recv(data_buffer, 10, 50);  
					helper_printHex(data_buffer, data_lenth);
					if(data_buffer[3] == 0x03)
							{
								printf("guang ou12----  0x01\n");
								return 0x01;
							}
						else 
							{
								printf("guang ou12----  0x00\n");
								return 0x00;
							}


			break;
		case 0x04:
					send_buffer[0] = Addr;	//设备地址
					send_buffer[1] = 0x01;	// 功能码 
					send_buffer[2] = 0x00;	//寄存器地址
					send_buffer[3] = 0x00;
					send_buffer[4] = 0x00;
					send_buffer[5] = 0x02;
					
					crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
					send_buffer[6] = (uint8_t)crc_uint16;
					send_buffer[7] = (uint8_t)(crc_uint16>>8); 
					 board_Rs485Send(send_buffer,8);
					data_lenth = board_Rs485Recv(data_buffer, 10, 50);  
					helper_printHex(data_buffer, data_lenth);
					if(data_buffer[3] == 0x01)
							{
								printf("ji dian qi1----  0x01\n");
								return 0x01;
							}
						else 
							{
								printf("ji dian qi1----  0x00\n");
								return 0x00;
							}


			break;
		case 0x05:
					send_buffer[0] = Addr;	//设备地址
					send_buffer[1] = 0x01;	// 功能码 
					send_buffer[2] = 0x00;	//寄存器地址
					send_buffer[3] = 0x00;
					send_buffer[4] = 0x00;
					send_buffer[5] = 0x02;
					
					crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
					send_buffer[6] = (uint8_t)crc_uint16;
					send_buffer[7] = (uint8_t)(crc_uint16>>8); 
					 board_Rs485Send(send_buffer,8);
					data_lenth = board_Rs485Recv(data_buffer, 10, 50);  
					helper_printHex(data_buffer, data_lenth);
					if(data_buffer[3] == 0x02)
							{
								printf("ji dian qi2----  0x01\n");
								return 0x01;
							}
						else 
							{
								printf("ji dian qi2---  0x00\n");
								return 0x00;
							}


			break;
		case 0x06:
					send_buffer[0] = Addr;	//设备地址
					send_buffer[1] = 0x01;	// 功能码 
					send_buffer[2] = 0x00;	//寄存器地址
					send_buffer[3] = 0x00;
					send_buffer[4] = 0x00;
					send_buffer[5] = 0x02;
					
					crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
					send_buffer[6] = (uint8_t)crc_uint16;
					send_buffer[7] = (uint8_t)(crc_uint16>>8); 
					 board_Rs485Send(send_buffer,8);
					//helper_printHex(send_buffer, 8);					 
					data_lenth = board_Rs485Recv(data_buffer, 10, 50);  
					printf("\r\n @@getjdq-- \r\n");
					helper_printHex(data_buffer, data_lenth);
					printf("\r\n @@getjdq-- \r\n");
					if(data_buffer[3] == 0x03)
							{
								printf("\r\n $-Relay Open----  0x01 \r\n");
								return 0x01;
							}
						else 
							{
								printf("\r\n $-Relay Close----  0x00 \r\n");
								return 0x00;
							}


					
			break;			
			default:
			break;
		}



}



// 针对DMA2022型继电器控制
// ChannelChoice 0x01-通道1  0x02-通道2 0x03-通道1 2全选
// SetState 0x01-打开 0x02-闭合
// 继电器地址 测试0xfe
void Modebus485_Set_JDQ0202(uint8_t Addr,uint8_t ChannelChoice,uint8_t SetState,uint8_t times)
{
   uint8_t data_lenth = 0;
   uint8_t data_buffer[15];
   uint8_t send_buffer[15];
   uint16_t  crc_uint16 = 0;
   uint8_t error = 0;
   uint8_t k = 0;

for(k = 0;k<times ; k++)
	{
   
		   switch (ChannelChoice)
			   {
			   case 0x01:
			   	           send_buffer[0] = Addr;  //设备地址
						   send_buffer[1] = 0x05;  // 功能码 
						   send_buffer[2] = 0x00;  //寄存器地址
						   send_buffer[3] = 0x00;
						   if(SetState == 0x01)
						     send_buffer[4] = 0xFF;
						   else if(SetState == 0x02)
						     send_buffer[4] = 0x00;		   
						   send_buffer[5] = 0x00;
						   crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
						   send_buffer[6] = (uint8_t)crc_uint16;
						   send_buffer[7] = (uint8_t)(crc_uint16>>8); 
						   
						    board_Rs485Send(send_buffer,8);
						   data_lenth = board_Rs485Recv(data_buffer, 10, 50);  
						   //helper_printHex(data_buffer, data_lenth);
						   if(data_lenth>3)
						   	{
						   	  error = 0;
						   	}
						   else
						   	{
						   	  error = 1;
						   	}
				   break;
			   case 0x02:
						   send_buffer[0] = Addr;  //设备地址
						   send_buffer[1] = 0x05;  // 功能码 
						   send_buffer[2] = 0x00;  //寄存器地址
						   send_buffer[3] = 0x01;
						   if(SetState == 0x01)
							 send_buffer[4] = 0xFF;
						   else if(SetState == 0x02)
							 send_buffer[4] = 0x00; 	   
						   send_buffer[5] = 0x00;
						   crc_uint16 = MGM_u16MBCRC16(send_buffer,6);
						   send_buffer[6] = (uint8_t)crc_uint16;
						   send_buffer[7] = (uint8_t)(crc_uint16>>8); 
						   
							board_Rs485Send(send_buffer,8);
						   data_lenth = board_Rs485Recv(data_buffer, 10, 50);	
						   helper_printHex(data_buffer, data_lenth);
						   if(data_lenth>3)
						   	{
						   	  error = 0;
						   	}
						   else
						   	{
						   	  error = 1;
						   	}						   
			   
				   break;	   
			   case 0x03:
						   send_buffer[0] = Addr;  //设备地址
						   send_buffer[1] = 0x0f;  // 功能码 
						   send_buffer[2] = 0x00;  //寄存器地址
						   send_buffer[3] = 0x00;
						   send_buffer[4] = 0x00;
						   send_buffer[5] = 0x02;
						   send_buffer[6] = 0x01;	
						   if(SetState == 0x01)
							 send_buffer[7] = 0xFF;
						   else if(SetState == 0x02)
							 send_buffer[7] = 0x00; 
						   crc_uint16 = MGM_u16MBCRC16(send_buffer,8);
						   send_buffer[8] = (uint8_t)crc_uint16;
						   send_buffer[9] = (uint8_t)(crc_uint16>>8); 
						   
							board_Rs485Send(send_buffer,10);
						   data_lenth = board_Rs485Recv(data_buffer, 10, 50);	
						   helper_printHex(data_buffer, data_lenth);
						   if(data_lenth>3)
						   	{
						   	  error = 0;
						   	}
						   else
						   	{
						   	  error = 1;
						   	}						   
				   break;
				   
				   default:
				   break;
		}

		   if(error == 0)
		   	break;
		  
	}

}



void BCDtoHex(uint8_t *pBuff,uint8_t len) //BCD码转为十六进制
{
    uint8_t i,temp;
	for(i = 0;i < len;i ++)
	{
		temp =pBuff[i] / 16;
		pBuff[i] = temp * 10 + pBuff[i] % 16;
	}
}







static const uint8_t aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const uint8_t aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};



 uint16_t MGM_u16MBCRC16( uint8_t * pucFrame, uint16_t usLen )
{
    uint8_t           ucCRCHi = 0xFF;
    uint8_t           ucCRCLo = 0xFF;
    int             iIndex;

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( uint8_t )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( uint16_t )( ucCRCHi << 8 | ucCRCLo );
}
 uint8_t ModbusCrc16Check(uint8_t *frame, uint16_t len) 
{
    uint8_t crcHi = 0xFF;
    uint8_t crcLo = 0xFF;
    int iIndex;
    uint8_t * pCount = frame;
    while( len-- )
    {
        iIndex = crcLo ^ *( pCount++ );
        crcLo = ( uint8_t )( crcHi ^ aucCRCHi[iIndex] );
        crcHi = aucCRCLo[iIndex];
    }
	return ((crcLo==frame[3+frame[2]]) && (crcHi==frame[4+frame[2]]));

}


/*璁＄畻鏁版嵁鐨勬牎楠屽拰*/
uint8_t Modebus485_GetCrc(uint8_t *buf, uint8_t len) 
{
	uint8_t crcValue =0;
	uint8_t i;
	for (i=0; i<len; i++) {
		crcValue = crcValue + buf[i];
	}
	return crcValue;
}








/* -------------------------------  end code  --------------------------------*/



















