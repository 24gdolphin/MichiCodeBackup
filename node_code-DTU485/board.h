#ifndef __BOARD_H
#define __BOARD_H
#include "PinsDef.h"
#include "sysconfig.h"
#include "main.h"

#define DATA_MEMORY_START_ADDR (FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS)
#define DATA_MEMORY_STOP_ADDR  (FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 128)



#ifdef User_LowPowerBoard 

#define VDD_ENABLE      GPIO_SetBits(VDD_EN_PORT, VDD_EN_PIN);
#define VDD_DISABLE     GPIO_ResetBits(VDD_EN_PORT, VDD_EN_PIN);
#define VBAT_ENABLE     GPIO_SetBits(VBAT_EN_PORT, VBAT_EN_PIN);
#define VBAT_DISABLE    GPIO_ResetBits(VBAT_EN_PORT, VBAT_EN_PIN);
#define EN33_ENABLE     GPIO_SetBits(EN33_PORT, EN33_PIN);
#define EN33_DISABLE    GPIO_ResetBits(EN33_PORT, EN33_PIN);
#define RS485_ENABLE    GPIO_SetBits(RS485_EN_PORT, RS485_EN_PIN);
#define RS485_DISABLE   GPIO_ResetBits(RS485_EN_PORT, RS485_EN_PIN);

#endif


void board_RS485Init_Config(uint8_t configchioce);

void board_ConfigNodeID(uint16_t timeoutms);

void board_EEPROM_init(void);
uint8_t board_EEPROM_read(uint32_t FlashAddr, uint8_t *dest, uint8_t nbyte);
uint8_t board_EEPROM_write(uint32_t FlashAddr, uint8_t *source, uint8_t nbyte);


void board_EXIT(void);

uint16_t board_Invtert_Uint16(uint16_t a);

uint32_t board_Invtert_Uint32(uint32_t a);

void board_Init(void);

void board_LedTest(void);
void board_Test(void);
 void board_rtcInit(void);
 void showRTC();

uint32_t board_getTimestamp();
void board_setTimeStamp(uint32_t timestamp);

void Delay(__IO uint32_t nCount);
void Delayms(uint32_t time);
//#define RS485_TX GPIO_SetBits(RS485_RE_PORT, RS485_RE_PIN)

#ifdef User_LowPowerBoard
#define RS485_RX GPIO_ResetBits(RS485_RE_PORT, RS485_RE_PIN)
#define RS485_TX GPIO_SetBits(RS485_RE_PORT, RS485_RE_PIN)
#else

#define RS485_TX GPIO_ResetBits(RS485_RE_PORT, RS485_RE_PIN)
#define RS485_RX GPIO_SetBits(RS485_RE_PORT, RS485_RE_PIN)

#endif

void board_Rs485Send(uint8_t* pIn, uint16_t length);
void board_Rs485Init();
uint16_t board_Rs485Recv(uint8_t* pOut, uint16_t size, uint16_t timeoutms);


void borad_PrintHex(uint8_t* pIn, uint16_t Length);



void board_IICReset(void);





#if GetRunTime
void board_TIME2Init(void);
uint16_t board_GetRuningTime(uint8_t status);
//uint16_t getruntime_count;

#endif



uint8_t spi_communication(uint8_t txData);  

#define	DIO_0 		  GPIO_ReadInputDataBit(DIO0_PORT, DIO0_PIN)



#define LORA_CS_High    GPIO_SetBits(RF_CS1_PORT, RF_CS1_PIN) 
#define LORA_CS_Low     GPIO_ResetBits(RF_CS1_PORT, RF_CS1_PIN)
#define F_CS_High   GPIO_SetBits(F_CS_PORT, F_CS_PIN) 
#define F_CS_Low    GPIO_ResetBits(F_CS_PORT, F_CS_PIN)



#define LED1_ON    GPIO_ResetBits(LED1_PORT, LED1_PIN);
#define LED2_ON    GPIO_ResetBits(LED2_PORT, LED2_PIN);
#define LED3_ON    GPIO_ResetBits(LED3_PORT, LED3_PIN);
#define LED4_ON    GPIO_ResetBits(LED4_PORT, LED4_PIN);


#define LED1_OFF    GPIO_SetBits(LED1_PORT, LED1_PIN);
#define LED2_OFF    GPIO_SetBits(LED2_PORT, LED2_PIN);
#define LED3_OFF    GPIO_SetBits(LED3_PORT, LED3_PIN);
#define LED4_OFF    GPIO_SetBits(LED4_PORT, LED4_PIN);


#define LED1_REVERSE  GPIO_ToggleBits(LED1_PORT, LED1_PIN);
#define LED2_REVERSE  GPIO_ToggleBits(LED2_PORT, LED2_PIN);
#define LED3_REVERSE  GPIO_ToggleBits(LED3_PORT, LED3_PIN);
#define LED4_REVERSE  GPIO_ToggleBits(LED4_PORT, LED4_PIN);


#endif


