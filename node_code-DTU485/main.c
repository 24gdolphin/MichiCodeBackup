
#include "stm8l15x.h"
#include "board.h"
#include "lora.h"
#include "w25q.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sensor.h"
#include "modbus485.h"
#include "helper.h"
#include "stm8l15x_flash.h"


#define SYS_FLASH_DATA_ADDR 0

#define SYS_Mode_DEINT 0
#define SYS_Loragroup_DEINT 0
#define SYS_Loratime_DEINT 1000
#define SYS_Baudgroup_DEINT 0X45
#define SYS_Rs485time_DEINT 600



//"@michi,mode=1,loragroup=1,loratime=1000,baudgroup=1,rs485time=600,imichi@"
//flash len + data + crc


typedef struct{ 

	uint16_t Mode;
	uint16_t Loragroup;
	uint16_t Loratime;
	uint16_t Baudgroup;
	uint16_t Rs485time;
	
}tsSysConfig; 

tsSysConfig DTUSysConfig;


extern tLoRaSettings LoRaSettings;
tLoRaSettings LoRaSettings;
const tLoRaSettings LoRaSettings_Buf[] =
{
	
{470000000,20, 8,10,1,TRUE,FALSE,FALSE,FALSE,4,FALSE,6,1000,1000,128},
{475000000,20, 8,10,1,TRUE,FALSE,FALSE,FALSE,4,FALSE,6,1000,1000,128},
{480000000,20, 8,10,1,TRUE,FALSE,FALSE,FALSE,4,FALSE,6,1000,1000,128},
{485000000,20, 8,10,1,TRUE,FALSE,FALSE,FALSE,4,FALSE,6,1000,1000,128},
{490000000,20, 8,10,1,TRUE,FALSE,FALSE,FALSE,4,FALSE,6,1000,1000,128},
{495000000,20, 8,10,1,TRUE,FALSE,FALSE,FALSE,4,FALSE,6,1000,1000,128},
{500000000,20, 8,10,1,TRUE,FALSE,FALSE,FALSE,4,FALSE,6,1000,1000,128},
{505000000,20, 8,10,1,TRUE,FALSE,FALSE,FALSE,4,FALSE,6,1000,1000,128},
{510000000,20, 8,10,1,TRUE,FALSE,FALSE,FALSE,4,FALSE,6,1000,1000,128},
{510000000,20, 7,9,1,TRUE,FALSE,FALSE,FALSE,4,FALSE,6,1000,1000,128},

};


//485 
#define Acq_485Buf_max 128
uint8_t Acq_485Recv_Buf[Acq_485Buf_max];
uint8_t Acq_485Recv_Buf_Len = 0;

uint8_t RS485_Recv_Timerflag = 0;
uint16_t RS485_Recv_Countvalue = 0;
uint16_t RS485_Recv_Countvalue_max = 150;
uint8_t RS485_Recv_Countoverflag = 0;


uint8_t LoraRecvBuffer[128] = {0};
uint8_t LoraSendBuffer[128] = {0};
uint16_t loraRecvLength = 0;
uint16_t loraSendLength = 0;

uint16_t Sys_GeneralTimer_count = 0;


static void init(void);

static void idle(void);

                              
typedef enum
{
	enumListenMaster485 = 1,
	enumSendToSlave = 2,
	enumRecvFromSlave = 3,
	enumSendToServer = 4,
}enumRunning;

enumRunning enumRunning_status = enumListenMaster485;




#ifdef SYS_FUNC_TIME_DBG

uint32_t function_time = 0;
 
static void RunCountRest(void)
{
 	function_time = 0;
 }
static void RunCountPrintf(char *p)
{
	printf("[Time]: %s = %ld ms \r\n",p ,function_time);
}

#endif



static void waittingConfig(void);
static void resetSysConfig(void );
static void saveConfig(tsSysConfig *pSysConfig);
static void readConfig(tsSysConfig *pSysConfig);
static int parsParams(tsSysConfig *pSysConfig, char *pIn, int len);
static int checkParams(tsSysConfig *pSysConfig);
static void AddCrc(uint8_t *buf, uint8_t len);
static void sendOKAck(void);
static void sendFaildAck(void);
static void sendParamsAck(tsSysConfig *pSysConfig);
void sendLocalParams(void);



int main( void )
{

	init();
	idle();

}

static void init(void)
{

	board_Init();
	board_RS485Init_Config(0x41);



	waittingConfig();
	readConfig(&DTUSysConfig);

	board_RS485Init_Config(DTUSysConfig.Baudgroup); 
	LoRaSettings = LoRaSettings_Buf[DTUSysConfig.Loragroup];

	
	lora_Init();

	

	//ShowParaSet();


}


//@michi,mode=1,loragroup=8,loratime=800,baudgroup=65,rs485time=1000,michi@

static void waittingConfig(void)
{
	uint8_t timeout = 20;
	
	tsSysConfig SysConfig_new;
	
	memset((uint8_t *)&SysConfig_new,0,sizeof(tsSysConfig));

	memset(Acq_485Recv_Buf,0,sizeof(Acq_485Recv_Buf));
	Acq_485Recv_Buf_Len = 0;
	printf("[Sys]: waittingConfig ......\r\n");

	LED1_OFF
	LED2_OFF
	LED3_OFF

	LED2_ON

	while(timeout > 0)
	{
		Delayms(300);
		if( RS485_Recv_Countoverflag)
		{
			RS485_Recv_Countoverflag = 0;
			Acq_485Recv_Buf[Acq_485Recv_Buf_Len] = '\0';
			printf("[Sys]: Acq_485Recv_Buf = %s \r\n",Acq_485Recv_Buf);
			disableInterrupts();
			if(parsParams(&SysConfig_new, (char *)Acq_485Recv_Buf, Acq_485Recv_Buf_Len))
			{
				LED1_ON
				saveConfig(&SysConfig_new);
				LED1_OFF
			}
			else
			{
				sendFaildAck();
			}
			enableInterrupts();
		}
		timeout--;
	}	

	LED2_OFF
	printf("[Sys]: waittingConfig Outime \r\n");
}


static int checkParams(tsSysConfig *pSysConfig)
{
	if( (pSysConfig->Mode < 0) || (pSysConfig->Mode > 1 ) )
	return 0;
	if( (pSysConfig->Loragroup < 0) ||  (pSysConfig->Loragroup > 9 ) )
	return 0;
	if( (pSysConfig->Loratime < 0) ||  (pSysConfig->Loratime > 10000 ) )
	return 0;
	if( (pSysConfig->Baudgroup < 0) ||  (pSysConfig->Baudgroup > 255 ) )
	return 0;
	if( (pSysConfig->Rs485time < 0) ||  (pSysConfig->Rs485time > 10000 ) )
	return 0;
	
	return 1;
}


static void resetSysConfig(void )
{
	DTUSysConfig.Mode = SYS_Mode_DEINT;
	DTUSysConfig.Loragroup = SYS_Loragroup_DEINT;
	DTUSysConfig.Loratime = SYS_Loratime_DEINT;
	DTUSysConfig.Baudgroup = SYS_Baudgroup_DEINT;
	DTUSysConfig.Rs485time = SYS_Rs485time_DEINT;

	printf("[Sys]: Save config faild, reset Sysconfig \r\n");
	
}


// saved_len = len + data + crc
static void saveConfig(tsSysConfig *pSysConfig)
{

	uint8_t save_buf[20];
	uint8_t save_len = 0;
	
	uint8_t saved_len = 0;
	
	uint8_t read_buf[20];
	uint8_t read_len = 0;

	uint8_t read_old_buf[20];
	uint8_t read_old_len = 0;

	uint8_t i = 0;

	if(!checkParams(pSysConfig))
	{
		printf("[Sys]: Error = checkParams \r\n");
		sendFaildAck();
		return;
	}
	

	memset(save_buf,0,sizeof(save_buf));
	memset(read_buf,0,sizeof(read_buf));
	memset(read_old_buf,0,sizeof(read_old_buf));

	W25Q_Read(&saved_len, SYS_FLASH_DATA_ADDR, 1);
	if(saved_len < 0x20)//有无历史配置
	{
		W25Q_Read(read_old_buf, SYS_FLASH_DATA_ADDR, saved_len);//读一次历史配置
		read_old_len = saved_len;
		Delayms(300);
		
		save_buf[0] = sizeof(tsSysConfig) + 1 + 1;//打包新的配置数据
		memcpy(save_buf+1, pSysConfig, sizeof(tsSysConfig));
		helper_AddCrc(save_buf,sizeof(tsSysConfig) + 1);
		
		W25Q_Erase_Sector(0);
		Delayms(200);
		memset(read_buf,0,sizeof(read_buf));
		W25Q_Write(save_buf, SYS_FLASH_DATA_ADDR, save_buf[0]);//尝试第一次写新配置数据
		Delayms(200);
		W25Q_Read(read_buf, SYS_FLASH_DATA_ADDR, save_buf[0]);

		
		for( i = 0; i < save_buf[0]; i++)//读出循环校验
		{
			if(  save_buf[i] != read_buf[i] )
			{
			
				W25Q_Erase_Sector(0);
				Delayms(200);
				resetSysConfig();
				
				return;
			}
		}
		
		printf("[sys]: Sys Config Update OK, Reboot \r\n");
		sendParamsAck(pSysConfig);
		sendOKAck();
		Delayms(200);
		WWDG_SWReset();
		
	}
	else
	{
		save_buf[0] = sizeof(tsSysConfig) + 1 + 1;//打包新的配置数据
		memcpy(save_buf+1, pSysConfig, sizeof(tsSysConfig));
		helper_AddCrc(save_buf, sizeof(tsSysConfig) + 1);
		
		W25Q_Erase_Sector(0);
		Delayms(200);
		W25Q_Write(save_buf, SYS_FLASH_DATA_ADDR, save_buf[0]);//尝试第一次写新配置数据
		Delayms(200);
		W25Q_Read(read_buf, SYS_FLASH_DATA_ADDR, save_buf[0]);

		
		for( uint8_t i = 0; i < save_buf[0]; i++)//读出循环校验
		{
			if(  save_buf[i] != read_buf[i] )
			{
			
				W25Q_Erase_Sector(0);
				Delayms(200);
				resetSysConfig();
				return;
			}
		}	

		printf("[sys]: Sys Config Get New, Reboot \r\n");
		sendParamsAck(pSysConfig);
		sendOKAck();
		Delayms(200);
		WWDG_SWReset();
	}
	
	
}

static void readConfig(tsSysConfig *pSysConfig)
{

	uint8_t save_buf[20];
	uint8_t save_len = 0;
	
	uint8_t saved_len = 0;
	
	uint8_t read_buf[20];
	uint8_t read_len = 0;

	uint8_t read_old_buf[20];
	uint8_t read_old_len = 0;

	
	uint8_t crc;
	
	memset(save_buf,0,sizeof(save_buf));
	memset(read_buf,0,sizeof(read_buf));
	memset(read_old_buf,0,sizeof(read_old_buf));

	W25Q_Read(&saved_len, SYS_FLASH_DATA_ADDR, 1);
	if(saved_len < 0x20)//有无历史配置
	{
		W25Q_Read(read_old_buf, SYS_FLASH_DATA_ADDR, saved_len);//读一次历史配置

		crc = CheckCrc(read_old_buf, saved_len);
		
		if(crc)
		{
			memcpy((uint8_t *)pSysConfig, read_old_buf + 1, sizeof(tsSysConfig));
			printf("[Sys]: Start ...... \r\n");
			printf("[Sys]: Mode = %d, loragroup = %d, loratime = %d, baudgroup = %d, rs485time = %d \r\n", pSysConfig->Mode,pSysConfig->Loragroup,pSysConfig->Loratime,pSysConfig->Baudgroup,pSysConfig->Rs485time);
		}
		else
		{
			printf("[sys]: Sys Config Read faild, Use deint config \r\n");
			resetSysConfig();
		}	
		
	}
	else
	{
		printf("[sys]: Sys Config is empty \r\n");
		printf("[sys]: Use deint config \r\n");
	}


}


void sendLocalParams(void)
{
	tsSysConfig newSysConfig;

	readConfig(&newSysConfig);
	sendParamsAck(&newSysConfig);

	
}

static int parsParams(tsSysConfig *pSysConfig, char *pIn, int len)
{
	char * p_str = NULL;

	int res = 0;

	if( strstr(pIn, "@michi")  && strstr(pIn, "michi@") )
	{
		while ( (p_str = helper_strsep(&pIn, ",")) )
		{
			if( p_str)
			{
				while(1)
				{
					if( strstr(pIn, "mode") ){
						sscanf(pIn, "mode=%d",  (uint16_t *) &pSysConfig->Mode);
						break;
					}

					if( strstr(pIn, "loragroup") ){
						sscanf(pIn, "loragroup=%d",  (uint16_t *) &pSysConfig->Loragroup);
						break;
					}

					if( strstr(pIn, "loratime") ){
						sscanf(pIn, "loratime=%d",  (uint16_t *) &pSysConfig->Loratime);
						break;
					}

					if( strstr(pIn, "baudgroup") ){
						sscanf(pIn, "baudgroup=%d",  (uint16_t *) &pSysConfig->Baudgroup);
						break;
					}
					
					if( strstr(pIn, "rs485time") ){
						sscanf(pIn, "rs485time=%d",  (uint16_t *) &pSysConfig->Rs485time);
						break;
					}	
						res = 1;
						break;
				}

				
			}
			
		}

	}
	else
	{
		sendLocalParams();
	}
	
	//res = 1;

	printf("[Sys]: Mode = %d, loragroup = %d, loratime = %d, baudgroup = %d, rs485time = %d \r\n", pSysConfig->Mode,pSysConfig->Loragroup,pSysConfig->Loratime,pSysConfig->Baudgroup,pSysConfig->Rs485time);
	return res;

}



static void sendParamsAck(tsSysConfig *pSysConfig)
{
	char OK_char[30];

	sprintf(OK_char,"Mode = %d \r\n",pSysConfig->Mode);
	board_Rs485Send(OK_char, strlen(OK_char));

	sprintf(OK_char,"Loragroup = %d \r\n",pSysConfig->Loragroup);
	board_Rs485Send(OK_char, strlen(OK_char));

	sprintf(OK_char,"Loratime = %dms \r\n",pSysConfig->Loratime);
	board_Rs485Send(OK_char, strlen(OK_char));

	sprintf(OK_char,"Baudgroup = %d \r\n",pSysConfig->Baudgroup);
	board_Rs485Send(OK_char, strlen(OK_char));

	sprintf(OK_char,"Rs485time = %dms \r\n",pSysConfig->Rs485time);
	board_Rs485Send(OK_char, strlen(OK_char));	
}


static void sendOKAck(void)
{
	char OK_char[]={"OK \r\n"};

	board_Rs485Send(OK_char, strlen(OK_char));
}

static void sendFaildAck(void)
{
	char OK_char[]={"Faild \r\n"};

	board_Rs485Send(OK_char, strlen(OK_char));
}


//LED1 RS485 RECV
//LED2 RS485 SEND
//LED3 LORA RECV
//LED4 LORA SEND



static void idle(void)
{

	if( DTUSysConfig.Mode)
	{

		uint8_t dtu485_send_buf[100];
		uint8_t dtu485_recv_buf[100];
		uint8_t dtu485_recv_len = 0;
		uint16_t crc16_1,crc16_2;

		uint8_t test_buf[8] = {0x01 ,0x03 ,0x00 ,0x00, 0x00, 0x09 ,0x85 ,0xcc};
		
		LED1_OFF
		LED2_OFF
		LED3_OFF
		LED4_OFF

		printf("[info]: RS485 Slave \r\n");
		while(1)
		{
			LED3_ON
			
#ifdef SYS_FUNC_TIME_DBG
	RunCountRest();
#endif
			printf("[sys]:\r\n------------------------ \r\n");
			loraRecvLength = SX1276LoRaReceive(LoraRecvBuffer, sizeof(LoraRecvBuffer), DTUSysConfig.Loratime);
			
#ifdef SYS_FUNC_TIME_DBG
	RunCountPrintf("LoRaReceive time");
#endif		
			LED3_OFF		
				if(loraRecvLength > 3)
				{
					LED2_ON
			
					if(1)	//crc16_1 == crc16_2
					{
						
						memcpy(dtu485_send_buf, LoraRecvBuffer, loraRecvLength);
						printf("[Info]: RS485 Slave get data once \r\n");
						
#ifdef SYS_FUNC_TIME_DBG
	RunCountRest();
#endif
						board_Rs485Send(dtu485_send_buf, loraRecvLength);
						dtu485_recv_len = board_Rs485Recv(dtu485_recv_buf, 128, DTUSysConfig.Rs485time);
						
#ifdef SYS_FUNC_TIME_DBG
	RunCountPrintf("RS485 Send and Recv time");
#endif						
						if( dtu485_recv_len > 3)
						{
							helper_printTitleHex("DTU485_recv_buf", dtu485_recv_buf, dtu485_recv_len);
							
							if(1)	//crc16_1 == crc16_2
							{
								LED1_ON

#ifdef SYS_FUNC_TIME_DBG
	RunCountRest();
#endif							
								SX1276LoRaSend(dtu485_recv_buf, dtu485_recv_len);
								
#ifdef SYS_FUNC_TIME_DBG
	RunCountPrintf("LoRaSend time");

#endif							
								memset(dtu485_recv_buf, 0, dtu485_recv_len);
								printf("[Info]: RS485 Slave Lora send once \r\n");
								LED1_OFF
							}	
							else
							{
								printf("[Info]: RS485 Slave CRC Error 2 \r\n");
							}

							dtu485_recv_len = 0;

						}
						
					}
					else
					{
						printf("[Info]: RS485 Slave CRC Error 1 \r\n");
					}
					
					memset(LoraRecvBuffer, 0, loraRecvLength);
					loraRecvLength = 0;
					
					LED2_OFF
				}

				loraRecvLength = 0;
		}

	}
	else
	{


		uint8_t recv_data_buf[128];
		uint8_t recv_data_len = 0;
		LED1_OFF
		LED2_OFF
		LED3_OFF
		LED4_OFF

		printf("[info]: RS485 Master \r\n");
		while(1)
		{
		
			switch (enumRunning_status)
				{
					case enumListenMaster485:
							if( RS485_Recv_Countoverflag)
							{
								LED3_ON
								RS485_Recv_Countoverflag = 0;
								printf("[sys]: ------------------------ \r\n");
								helper_printTitleHex("RS485 buffer", Acq_485Recv_Buf, Acq_485Recv_Buf_Len);

								memcpy(recv_data_buf, Acq_485Recv_Buf, Acq_485Recv_Buf_Len);
								recv_data_len = Acq_485Recv_Buf_Len;
								
								Acq_485Recv_Buf_Len = 0;
								memset(Acq_485Recv_Buf, 0, Acq_485Recv_Buf_Len);

								LED3_OFF
								enumRunning_status = enumSendToSlave;
							}
					
						break;
					case enumSendToSlave:
						LED2_ON
							printf("[info]: SendToSlave Once\r\n");
							//helper_printTitleHex("recv_data_buf", recv_data_buf, recv_data_len);

#ifdef SYS_FUNC_TIME_DBG
	RunCountRest();
#endif							
							SX1276LoRaSend(recv_data_buf, recv_data_len);

#ifdef SYS_FUNC_TIME_DBG
	RunCountPrintf("LoRaSend time");
#endif							
							enumRunning_status = enumRecvFromSlave; 
						//LED2_OFF
					
						break;
					case enumRecvFromSlave:
						//LED3_ON
							//printf("[info]: enumRecvFromSlave \r\n");

#ifdef SYS_FUNC_TIME_DBG
	RunCountRest();
#endif							
							loraRecvLength = SX1276LoRaReceive(LoraRecvBuffer, sizeof(LoraRecvBuffer), DTUSysConfig.Loratime);
							
#ifdef SYS_FUNC_TIME_DBG
	RunCountPrintf("LoRaReceive time");
#endif							
							enumRunning_status = enumSendToServer;
						//LED3_OFF
							LED2_OFF
						break;

					
					case enumSendToServer:	
							//printf("[info]: enumSendToServer \r\n");
							if(loraRecvLength > 3 )
							{
								LED1_ON

#ifdef SYS_FUNC_TIME_DBG
	RunCountRest();
#endif								
								board_Rs485Send(LoraRecvBuffer, loraRecvLength);
								
#ifdef SYS_FUNC_TIME_DBG
	RunCountPrintf("RS485 Send time");
#endif	

								helper_printTitleHex("LoraRecvBuffer", LoraRecvBuffer, loraRecvLength);
								loraRecvLength = 0;
								LED1_OFF
							}
							else
							{
								printf("[info]: Lora recv faild \r\n");
							
							}
							
								RS485_Recv_Countoverflag = 0;
								Acq_485Recv_Buf_Len = 0;	//进等待服务器485数据前，先清空一次，即使在lora发送期间 串口缓存收到了数据
								memset(Acq_485Recv_Buf, 0, Acq_485Recv_Buf_Len);
								
						enumRunning_status = enumListenMaster485;
							
						break;
					default:
					break;

				}
		}


	}
	

}



static void AddCrc(uint8_t *buf, uint8_t len) 
{
	uint8_t crcValue =0;
	uint8_t i;
	for (i=0; i<len; i++) {
		crcValue = crcValue + buf[i];
	}
	*(buf + len) = crcValue;
	return;
}


void ShowParaSet(void)
{	
	printf("\r\n*******************\r\n");
	printf("\r\n*--DTU485--*\r\n");	
	printf("\r\n*--NodeID--*\r\n");


}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

