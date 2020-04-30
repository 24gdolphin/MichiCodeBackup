#include "w25q.h"
#include "board.h"
#include "PinsDef.h"
#include <stdio.h>
#include <string.h>
//  _Declare


//#define	 F_CS_High  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)
//#define	 F_CS_Low   HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)
#define FLAHS_ID 0xEF14  //Q16=0xEF14, Q256=0xEF18

/* ----------------------------------------------------------------- */
#define FLASH_TEST_ADDR 0
#define FLASH_TEST_MAGIC 0xA5

uint8_t W25Q_test(void)
{
  uint8_t data;
  uint8_t errorCnt;
  uint16_t id;

  errorCnt = 0;
  id = W25Q_ReadDeviceID();

  if ( FLAHS_ID != id ) errorCnt++;

  data = 0;
  W25Q_Read(&data, FLASH_TEST_ADDR, 1);

  if ( FLASH_TEST_MAGIC != data)
  {
      W25Q_Erase_Sector(FLASH_TEST_ADDR);

      data = FLASH_TEST_MAGIC;
      W25Q_Write(&data, FLASH_TEST_ADDR, 1);
      printf("writ data = %x\r\n", data);
      data = 0;
      W25Q_Read(&data, FLASH_TEST_ADDR, 1);
      printf("read data = %x\r\n", data);
      
      if ( FLASH_TEST_MAGIC != data) errorCnt++;
  }

  return ( 0 == errorCnt );

}


void printHex(uint8_t *pIn, uint16_t Length)
{
  
    for(int i = 0; i < Length; ++i)
    {
      printf("%02x ", pIn[i]);
    }
  printf("\r\n");
}

void writeReadTest()
{
#define READ_SIZE 10
    uint32_t i = 0;
    uint8_t readbuffer[READ_SIZE];
    uint8_t writebuffer[READ_SIZE];
    memset(writebuffer, 0xbb, sizeof(writebuffer));
   // for(i = ((uint32_t)1024u*(uint32_t)1024u*(uint32_t)2u - (uint32_t)1000u); i < (uint32_t)1024*(uint32_t)1024u*(uint32_t)2u; i+=READ_SIZE)
    W25Q_Erase_Chip();
    for(i = (1024ul*1024ul*1024ul); i < 1024ul*1024ul*1024ul+READ_SIZE*100; i+=READ_SIZE)  
    {
      memset(readbuffer, 0x00, sizeof(readbuffer));
      W25Q_Read(readbuffer, i, sizeof(readbuffer));
      printHex(readbuffer, sizeof(readbuffer));
      W25Q_Write(writebuffer, i, sizeof(writebuffer));
      printf("i = %ld===========================\r\n", i);
      memset(readbuffer, 0x00, sizeof(readbuffer));
      W25Q_Read(readbuffer, i, sizeof(readbuffer));
      printHex(readbuffer, sizeof(readbuffer));
    }

}



/**powerdown state,  1uA**/
void W25Q_disable(void)
{
  W25Q_WriteSR(W25Q_PowerDown);
}

void W25Q_enable(void)
{
  W25Q_WriteSR(W25Q_ReleasePowerDown);
}

/* -------------------------------  W25Q  driver ------------------------------ */

//W25Q 读状态
uint8_t W25Q_ReadSR(void)
{  
	uint8_t byte=0;

	F_CS_Low;      
  spi_communication(W25Q_ReadStatusReg);  
	byte=spi_communication(0Xff); 
	F_CS_High;  
	     
	return byte;   
} 


//W25Q 写状态
void W25Q_WriteSR(uint8_t sr)
{   
	F_CS_Low;                               
	spi_communication(W25Q_WriteStatusReg);     
	spi_communication(sr);                 
	F_CS_High;                                 	      
}   



//W25Q 写使能
void W25Q_Write_Enable(void)   
{
	F_CS_Low;                             
  spi_communication(W25Q_WriteEnable);      
	F_CS_High;                               	      
} 


//W25Q16 写禁止 
void W25Q_Write_Disable(void)   
{  
	F_CS_Low;                            
  spi_communication(W25Q_WriteDisable);        
	F_CS_High;                                	      
}


//等待空闲
void W25Q_Wait_Busy(void)   
{   
	while ((W25Q_ReadSR()&0x01)==0x01);   
}

// 读取ID
uint16_t W25Q_ReadDeviceID(void)
{
  uint16_t Tempa,temp1,temp2;

  F_CS_Low;

  /* Send "RDID " instruction */
  spi_communication(0x90);
  spi_communication(0x00);
  spi_communication(0x00);
  spi_communication(0x00);
  
  /* Read a byte from the FLASH */
  temp1 = spi_communication(0x00);
  temp2 = spi_communication(0x00);

  /* Deselect the FLASH: Chip Select high */
  F_CS_High;
	
	Tempa = (temp1<<8) + temp2;

  return Tempa;
}


//读取数据
void W25Q_Read(uint8_t* Buffer,uint32_t Addr,uint16_t ByteNum)
{ 
 	uint16_t i;
						    
	F_CS_Low;      
	spi_communication(W25Q_ReadData);            
	spi_communication((uint8_t)((Addr)>>16));
	spi_communication((uint8_t)((Addr)>>8));
	spi_communication((uint8_t)Addr);
	for(i=0;i<ByteNum;i++)
	{ 
		Buffer[i]=spi_communication(0XFF);    
	}
	F_CS_High;      	      
} 



//W25Q 写一页
void W25Q_Write_Page(uint8_t* Buffer,uint32_t Addr,uint16_t ByteNum)
{
  uint16_t i; 
  
  W25Q_Write_Enable();   
  F_CS_Low;   
  spi_communication(W25Q_PageProgram);   
  spi_communication((uint8_t)((Addr)>>16));
  spi_communication((uint8_t)((Addr)>>8));
  spi_communication((uint8_t)Addr);
  for(i=0;i<ByteNum;i++)spi_communication(Buffer[i]);  
  F_CS_High;  
  W25Q_Wait_Busy();  
} 



//W25Q 写
void W25Q_Write(uint8_t* Buffer,uint32_t Addr,uint16_t ByteNum)
{ 			 		 
  uint16_t pageRemain;
            
  pageRemain=256-Addr%256;
  if(ByteNum<=pageRemain)pageRemain=ByteNum;
  
  while(1)
  {	   
      W25Q_Write_Page(Buffer, Addr, pageRemain);
      if(ByteNum==pageRemain)
      {
          break;
      }
      else 
      {
          Buffer+=pageRemain;
          Addr+=pageRemain;
          ByteNum-=pageRemain;
          if(ByteNum>256)pageRemain=256;
          else pageRemain=ByteNum;
      }
  }	    
} 
 		


// W25Q 擦除一片
void W25Q_Erase_Chip(void)   
{                                             
	W25Q_Write_Enable();                  
	W25Q_Wait_Busy();   
	F_CS_Low;                            
	spi_communication(W25Q_ChipErase);        
	F_CS_High;                                 	      
	W25Q_Wait_Busy();   				  
} 



//W25Q 擦除一扇区：根据扇区（从0开始的）编号进行删除
void W25Q_Erase_Sector(uint32_t SectorIdx)
{   
	uint32_t Dst_Addr = SectorIdx * 4096;
	//Dst_Addr*=4096;

	W25Q_Write_Enable();    	 
	W25Q_Wait_Busy();   
	F_CS_Low;        
	spi_communication(W25Q_SectorErase);      
	spi_communication((uint8_t)((Dst_Addr)>>16));
	spi_communication((uint8_t)((Dst_Addr)>>8));
	spi_communication((uint8_t)Dst_Addr);
	F_CS_High;       	      
  W25Q_Wait_Busy(); 
} 

//W25Q 擦除一扇区：指定地址开始的扇区
void W25Q_Erase_Sector_ByAddr(uint32_t Dst_Addr)
{   
	//Dst_Addr*=4096;
	W25Q_Write_Enable();    	 
	W25Q_Wait_Busy();   
	F_CS_Low;        
	spi_communication(W25Q_SectorErase);      
	spi_communication((uint8_t)((Dst_Addr)>>16));
	spi_communication((uint8_t)((Dst_Addr)>>8));
	spi_communication((uint8_t)Dst_Addr);
	F_CS_High;       	      
        W25Q_Wait_Busy(); 
} 



//W25Q 擦除一块区
void W25Q_Erase_Block(uint32_t Bst_Addr)
{   
	Bst_Addr*=65536;
	W25Q_Write_Enable();    
	W25Q_Wait_Busy();   
	F_CS_Low;        
	spi_communication(W25Q_BlockErase);      
	spi_communication((uint8_t)((Bst_Addr)>>16));   
	spi_communication((uint8_t)((Bst_Addr)>>8));   
	spi_communication((uint8_t)Bst_Addr);  
	F_CS_High;       	      
  W25Q_Wait_Busy(); 
}   




/*
void FindReadWitePointer(SaveReadWriteConf *pReadWrite)
	分析W25Q Flash，获得读写指针
	Flash逻辑如下：	
1.以Sector为单位每个Sector4096字节，首4个字节是开始首个数据记录的时间，后面两个字节是表示当前Sector数据全部上传的标志。
2.每组数据10个字节：TimeStamp(4)+Temperature(2),Humidity(2),SentOutTag(2)

*/
void FindReadWitePointer(SaveReadWriteConf *pReadWrite)
{
	#define MAX_TIMESTAMP 1893427201	//2030-01-01 00:00:01 used for flash timestamp search
		uint32_t i = 0;
		uint16_t j = 0;
		uint16_t AlreadySentTag = 0;
		uint32_t TimeStamp = 0;
		uint32_t MinReadTime = MAX_TIMESTAMP;
		uint32_t MaxWriteTime = 0;
		pReadWrite->u32ReadAddr = 0x00;
		pReadWrite->u32WriteAddr = 0x00;
		
	MinReadTime = MAX_TIMESTAMP;
	MaxWriteTime = 0;
  	for (i=0; i<W25Q_SECTOR_NUMBER; i++) {
		W25Q_Read((uint8_t *)&TimeStamp,i*W25Q_SECTOR_SIZE,4);
		if (TimeStamp == 0xffffffff) { //当前Sector为空：通过每个Sector首4个字节标识是否有数据
			continue;
		}
		if (TimeStamp > MaxWriteTime) {
			MaxWriteTime = TimeStamp;
			pReadWrite->u32WriteAddr = i;
		}
			
		W25Q_Read((uint8_t *)&AlreadySentTag, i*W25Q_SECTOR_SIZE + 4, 2);
		if (AlreadySentTag == 0xffff) { //当前Sector数据是否已发送的标志（紧跟在TimeStamp之后的2个字节）：0xffff表示未发送
			if (TimeStamp < MinReadTime) {
				MinReadTime = TimeStamp;
				pReadWrite->u32ReadAddr = i;
			}
		}
	}
	if (MaxWriteTime == 0) { //整个flash均为空
		pReadWrite->u32WriteAddr = 0;
	} else {
		//在WriteAddr所在的Sector中查找，找到真正的写指针：即未填数据的位置
		j = pReadWrite->u32WriteAddr; //写指针所在Sector的Index
		//WriteAddr = j * W25Q_SECTOR_SIZE + 6;
		for (i=j * W25Q_SECTOR_SIZE + 6; i<(j+1) * W25Q_SECTOR_SIZE; i=i+10) {
			W25Q_Read((uint8_t *)&TimeStamp,i,4);
			 //		 printf("j = %d \t i = %d \t(j+1) * W25Q_SECTOR_SIZE = %d\r\n", j,i, (j+1) * W25Q_SECTOR_SIZE);
			if (TimeStamp == 0xffffffff) { //找到未写数据的空间，作为写指针的初始地址
				pReadWrite->u32WriteAddr = i;
				break;
			}
		}
	}
	if (MinReadTime == MAX_TIMESTAMP) { //整个flash均为空，或者全部数据均已发送
		pReadWrite->u32ReadAddr = pReadWrite->u32WriteAddr; 
		MinReadTime = 0;
	} else {
		//在ReadAddr所在的Sector中查找，找到真正的未发送数据，作为读指针
		j = pReadWrite->u32ReadAddr;	//读指针所在Sector的Index
		//ReadAddr = j * W25Q_SECTOR_SIZE + 6;
		for (i=j * W25Q_SECTOR_SIZE + 6; i<(j+1) * W25Q_SECTOR_SIZE; i=i+10) {
			//ReadAddr = i;
			W25Q_Read((uint8_t *)&TimeStamp,i,4);
			W25Q_Read((uint8_t *)&AlreadySentTag,i+8,2);
			if (TimeStamp == 0xffffffff) { //无数据，中止搜索
				break;
			}
			if (AlreadySentTag == 0xffff) { //未发送的数据，后面的肯定均未发送，中止搜索
				break;
			}
		}
		pReadWrite->u32ReadAddr = i; //一种极端情况：整个Sector中的数据均被发送，ReadAddr指向下一个Sector
						//但Sector的发送标志仍为未发送（未能及时更新该标志）：此时也无伤害
	}
	printf("ReadAddr: 0x%lx(%ld), WriteAddr: 0x%lx(%ld)\n",pReadWrite->u32ReadAddr, MinReadTime, pReadWrite->u32WriteAddr, MaxWriteTime);
}

/* -------------------------------  end code  --------------------------------*/
