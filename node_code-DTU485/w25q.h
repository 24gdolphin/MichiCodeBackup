#ifndef __W25Q_H
#define __W25Q_H
#include "stm8l15x.h"
#define W25Q_SECTOR_NUMBER	4096
#define W25Q_SECTOR_SIZE	4096
#define W25Q_FULL_SIZE		1024ul*1024ul*2ul	//2M ‬

/* -------------------------------- W25Q ָ ------------------------------ */





#define W25Q_WriteEnable		0x06 
#define W25Q_WriteDisable	  	0x04 
#define W25Q_ReadStatusReg		0x05 
#define W25Q_WriteStatusReg		0x01 
#define W25Q_ReadData		        0x03 
#define W25Q_FastReadData	  	0x0B 
#define W25Q_FastReadDual	  	0x3B 
#define W25Q_PageProgram	  	0x02 
#define W25Q_BlockErase		  	0xD8    //Block Erase (64KB)
#define W25Q_SectorErase		0x20 
#define W25Q_ChipErase		  	0xC7 
#define W25Q_PowerDown			0xB9 
#define W25Q_ReleasePowerDown	        0xAB 
#define W25Q_ManufactDeviceID	        0x90 


typedef struct{
	uint32_t u32ReadAddr;
	uint32_t u32WriteAddr;
	uint32_t u32TempAddr;
}SaveReadWriteConf;


uint8_t W25Q_test(void);
void W25Q_init(void);
void W25Q_disable(void);
void W25Q_enable(void);

uint16_t  W25Q_ReadDeviceID(void);
uint8_t   W25Q_ReadSR(void);
void W25Q_WriteSR(uint8_t sr);
void W25Q_Write_Enable(void);  
void W25Q_Write_Disable(void);	
void W25Q_Read(uint8_t* Buffer,uint32_t Addr,uint16_t ByteNum);
void W25Q_Write_Page(uint8_t* Buffer,uint32_t Addr,uint16_t ByteNum);
void W25Q_Write(uint8_t* Buffer,uint32_t Addr,uint16_t NumByte);
void W25Q_Erase_Chip(void);   
void W25Q_Erase_Sector(uint32_t SectorIdx);
void W25Q_Erase_Sector_ByAddr(uint32_t Dst_Addr);
void W25Q_Erase_Block(uint32_t Bst_Addr);
void W25Q_Wait_Busy(void);
void FindReadWitePointer(SaveReadWriteConf *pReadWrite); //找读写指针


void writeReadTest();


#endif



