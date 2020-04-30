#include "lora.h"
#include "sx1276-LoRaMisc.h"
#include "board.h"
#include "string.h"
#include <stdio.h>
#include <sysconfig.h>
// Default settings
/*!
 * Module choice. There are three existing module with the SX1276.
 * Please set the connected module to the value 1 and set the others to 0
 */
uint8_t loramode; //for test
__IO u8 test;
#define MODULE_SX1276RF1IAS                         0
#define MODULE_SX1276RF1JAS                         0
#define MODULE_SX1276RF1KAS                         1


extern tLoRaSettings LoRaSettings;

uint8_t SX1276Regs[0x70]; 
tSX1276LR* SX1276LR;


#if RX_SNRESTIMATE
static int8_t RxPacketSnrEstimate;  //信噪�?
inline static void static_lora_getRxSnrEstimate(void);
#endif
#if RSSI
inline static void static_lora_getRssi(void);
static int16_t RxPacketRssiValue;    //Rssi
#endif

int16_t GetLoRaRssi(void)
{
    return RxPacketRssiValue;
}

void SX1276ReadBuffer(uint8_t addr, uint8_t *buffer, uint8_t size)
{
  uint8_t ix = 0;
  LORA_CS_Low;
  spi_communication(addr & 0x7f);
  for(ix = 0; ix < size; ++ix)
    *buffer++ = spi_communication(0);
  LORA_CS_High;
}

void SX1276WriteBuffer(uint8_t addr, uint8_t *buffer, uint8_t size)
{
  uint8_t ix = 0;
  LORA_CS_Low;
  spi_communication(addr | 0x80);
  for(ix = 0; ix < size; ++ix)
    spi_communication(buffer[ix]);
  LORA_CS_High;
}

void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( RFLR_FIFOTXBASEADDR, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer(RFLR_FIFORXBASEADDR, buffer, size );
}



#define RSSI_OFFSET                                 -139.0
#define RSSI_OFFSET_LF                              -164.0
#define RSSI_OFFSET_HF                              -157.0

#define RADIO_ANT_SWITCH_SET_TX                    GPIO_PIN_RESET
#define RADIO_ANT_SWITCH_SET_RX                    GPIO_PIN_SET

void showLoraModeParameter(void)
{
    tLoRaSettings vLoRaSettings;
 

    vLoRaSettings.RFFrequency         = SX1276LoRaGetRFFrequency();
    vLoRaSettings.Power               = SX1276LoRaGetRFPower();
    vLoRaSettings.SignalBw            = SX1276LoRaGetSignalBandwidth();                           
    vLoRaSettings.SpreadingFactor     = SX1276LoRaGetSpreadingFactor(); 
    vLoRaSettings.ErrorCoding         = SX1276LoRaGetErrorCoding();     
    vLoRaSettings.CrcOn               = SX1276LoRaGetPacketCrcOn();              
    vLoRaSettings.ImplicitHeaderOn    = SX1276LoRaGetImplicitHeaderOn();   
    vLoRaSettings.RxSingleOn          = LoRaSettings.RxSingleOn;         
    vLoRaSettings.FreqHopOn           = LoRaSettings.FreqHopOn;          
    vLoRaSettings.HopPeriod           = SX1276LoRaGetHopPeriod();       
    vLoRaSettings.LowDatarateOptimize = SX1276LoRaGetLowDatarateOptimize();
    vLoRaSettings.PreambleLength      = SX1276LoRaGetPreambleLength(); 
    vLoRaSettings.TxPacketTimeout     = LoRaSettings.TxPacketTimeout;
    vLoRaSettings.RxPacketTimeout     = LoRaSettings.RxPacketTimeout;
    vLoRaSettings.PayloadLength       = SX1276LoRaGetPayloadLength();
    printf("\t===========================\r\n");
    printf("\t|| RFFrequency         \t=%9ld\t ||\r\n", vLoRaSettings.RFFrequency          );
    printf("\t|| Power               \t=%9d\t ||\r\n", vLoRaSettings.Power                );
    printf("\t|| SignalBw            \t=%9d\t ||\r\n", vLoRaSettings.SignalBw             );
    printf("\t|| SpreadingFactor     \t=%9d\t ||\r\n", vLoRaSettings.SpreadingFactor      );
    printf("\t|| ErrorCoding         \t=%9d\t ||\r\n", vLoRaSettings.ErrorCoding          );
    printf("\t|| CrcOn               \t=%9d\t ||\r\n", vLoRaSettings.CrcOn                );
    printf("\t|| ImplicitHeaderOn    \t=%9d\t ||\r\n", vLoRaSettings.ImplicitHeaderOn     );
    printf("\t|| RxSingleOn          \t=%9d\t ||\r\n", vLoRaSettings.RxSingleOn           );
    printf("\t|| FreqHopOn           \t=%9d\t ||\r\n", vLoRaSettings.FreqHopOn            );
    printf("\t|| HopPeriod           \t=%9d\t ||\r\n", vLoRaSettings.HopPeriod            );
    printf("\t|| LowDatarateOptimize \t=%9d\t ||\r\n", vLoRaSettings.LowDatarateOptimize  );
    printf("\t|| PreambleLength      \t=%9d\t ||\r\n", vLoRaSettings.PreambleLength       );
    printf("\t|| TxPacketTimeout     \t=%9ld\t ||\r\n", vLoRaSettings.TxPacketTimeout      );
    printf("\t|| RxPacketTimeout     \t=%9ld\t ||\r\n", vLoRaSettings.RxPacketTimeout      );
    printf("\t|| PayloadLength       \t=%9d\t ||\r\n", vLoRaSettings.PayloadLength        );
    printf("\t|| Pa20dBm();          \t=%9d\t ||\r\n", SX1276LoRaGetPa20dBm()             );
    printf("\t|| PAOutput();         \t=%9d\t ||\r\n", SX1276LoRaGetPAOutput()            );
    printf("\t|| PaRamp();           \t=%9d\t ||\r\n", SX1276LoRaGetPaRamp()              );
    printf("\t|| SymbTimeout();      \t=%9d\t ||\r\n", SX1276LoRaGetSymbTimeout()         );
    printf("\t===========================\r\n");
}


void SX1276LoRaSetOpMode( uint8_t opMode )
{
    static uint8_t opModePrev = RFLR_OPMODE_STANDBY;
    opModePrev = SX1276LR->RegOpMode & ~RFLR_OPMODE_MASK;
    if( opMode != opModePrev )
    {
        SX1276LR->RegOpMode = ( SX1276LR->RegOpMode & RFLR_OPMODE_MASK ) | opMode;
        SX1276Write( REG_LR_OPMODE, SX1276LR->RegOpMode );    //do not delete ! hh
        SX1276Write( REG_LR_OPMODE, SX1276LR->RegOpMode );   //do not delete !   
        SX1276Read(REG_LR_OPMODE, &loramode);
    }

    
    
}


void SX1276LoRaSetDefaults( void )
{
    // REMARK: See SX1276 datasheet for modified default values.
    SX1276Read( REG_LR_VERSION, &SX1276LR->RegVersion );
    printf("LoRa version = %02x\r\n", SX1276LR->RegVersion);
}


static void SX1276LoRaReset( void )
{
    GPIO_Init(RF_RST1_PORT, RF_RST1_PIN, GPIO_Mode_Out_PP_Low_Fast); 
    GPIO_ResetBits(RF_RST1_PORT, RF_RST1_PIN);
    Delayms(2);
    GPIO_SetBits(RF_RST1_PORT, RF_RST1_PIN);
    Delayms(6);
}

static void SX1276LoRaInitIo(void)
{
    /*
    LoRa引脚�?
        LORA_DIO0   输入模式
        LORA_CS_Pin 推挽输出
        LORA_RESET  输入模式
    */
    //设置引脚初始状�?
    GPIO_Init(DIO0_PORT, DIO0_PIN, GPIO_Mode_In_FL_No_IT); //LORA_DIO0   输入模式
    
}



    
    
void lora_Init(void)
{
    SX1276LR = (tSX1276LR*)SX1276Regs;
    SX1276LoRaInitIo(); //初始化IO端口
    SX1276LoRaReset(); //reset lora chip After radio reset the default modem is FSK
    
    SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );

    SX1276LR->RegOpMode = ( SX1276LR->RegOpMode & RFLR_OPMODE_LONGRANGEMODE_MASK ) | RFLR_OPMODE_LONGRANGEMODE_ON;  //设置为LoRa模式
    SX1276Write( REG_LR_OPMODE, SX1276LR->RegOpMode ); 
    
    SX1276ReadBuffer( REG_LR_OPMODE, SX1276Regs + 1, 0x70 - 1 );  //读取LoRa所有配    
    SX1276LR->RegLna = RFLR_LNA_GAIN_G1;       //将LAN增益设置为G1（最大增益）         
    SX1276WriteBuffer( REG_LR_OPMODE, SX1276Regs + 1, 0x70 - 1 );

    // set the RF settings 
    SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
    SX1276LoRaSetRFFrequency( LoRaSettings.RFFrequency );  //设置频率
    SX1276LoRaSetSpreadingFactor( LoRaSettings.SpreadingFactor ); // SF6 only operates in implicit header mode.  设置扩频因子
    SX1276LoRaSetErrorCoding( LoRaSettings.ErrorCoding ); //设置纠错�?
    SX1276LoRaSetPacketCrcOn( LoRaSettings.CrcOn );  //设置CRC
    
    SX1276LoRaSetSignalBandwidth( LoRaSettings.SignalBw ); //设置带宽

    SX1276LoRaSetImplicitHeaderOn( LoRaSettings.ImplicitHeaderOn); //设置隐式报头
    SX1276LoRaSetSymbTimeout( 0x3FF );  //设置RX超时有效�?
    SX1276LoRaSetLowDatarateOptimize( LoRaSettings.LowDatarateOptimize ); //设置低数据率优化
    SX1276LoRaSetPreambleLength(LoRaSettings.PreambleLength);

    SX1276LoRaSetPAOutput( RFLR_PACONFIG_PASELECT_PABOOST ); //设置PA_BOOST 高功率发射引�?
    SX1276LoRaSetPa20dBm( TRUE); //高功率设�?
    LoRaSettings.Power = 20; //PWER 设置�?0dbm
    SX1276LoRaSetRFPower( LoRaSettings.Power );
    
    SX1276LR->RegFifoTxBaseAddr = RFLR_FIFOTXBASEADDR;  //设置FIFO数据缓冲区中发送调制器的写入地址 0x80
    SX1276Write( REG_LR_FIFOTXBASEADDR, SX1276LR->RegFifoTxBaseAddr );
    SX1276LR->RegFifoTxBaseAddr = RFLR_FIFORXBASEADDR;  //设置FIFO数据缓冲区中接收调制器的读取基地址�?0x00
    SX1276Write( REG_LR_FIFORXBASEADDR, SX1276LR->RegFifoTxBaseAddr );
    SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
    
}




bool SX1276LoRaSend(uint8_t *RFBuffer, uint16_t TxPacketSize)
{
    u32 timeout = LoRaSettings.TxPacketTimeout;
    bool result = FALSE;
    SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
    SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |    //中断屏蔽，屏蔽除TXDONE之外的所有寄存器中断
                                RFLR_IRQFLAGS_RXDONE |
                                RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                RFLR_IRQFLAGS_VALIDHEADER |
                                //RFLR_IRQFLAGS_TXDONE |
                                RFLR_IRQFLAGS_CADDONE |
                                RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                RFLR_IRQFLAGS_CADDETECTED;
    SX1276LR->RegHopPeriod = 0;
    
    SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );
    SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );

    // Initializes the payload size
    SX1276LR->RegPayloadLength = TxPacketSize;
    SX1276Write( REG_LR_PAYLOADLENGTH, SX1276LR->RegPayloadLength );  //设置负载字节长度，隐式报头模式下需要设置寄存器以达到预期的数据包长度，不允许设置为0 
    
    SX1276LR->RegFifoAddrPtr = RFLR_FIFOTXBASEADDR;  //设置FIFO数据缓冲区中SPI接口地址
    SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
    

    SX1276WriteFifo(RFBuffer, SX1276LR->RegPayloadLength );
                                    // TxDone               RxTimeout                   FhssChangeChannel          ValidHeader         
    SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_01; //| RFLR_DIOMAPPING1_DIO1_00; //| RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_01; //设置DIO0引脚发送中�?
    SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR->RegDioMapping1, 1 ); //设置DIO0_01发送完成中�?
    SX1276LoRaSetOpMode( RFLR_OPMODE_TRANSMITTER );
#if DBG_LORA_REG
    SX1276Read(REG_LR_OPMODE, &loramode);
    printf("after set send mode. mode = %02x\r\n",loramode);
#endif   
    for(;;)
    {
          if(DIO_0)//  todo 用查询寄存器代替引脚状�?
          {
#if DBG_LORA_LOG             
            printf("%d Tx:", TxPacketSize);
            borad_PrintHex(RFBuffer, TxPacketSize);
#endif              
            result = TRUE;
            break;
          }
          Delayms(5);
          timeout -= 5;
          if(timeout <=0)
          {
            printf("send data timeout \r\n");
            break;
          }
    }
#if DBG_LORA_REG
    SX1276Read(REG_LR_OPMODE, &loramode);
    printf("after send data. mode = %02x\r\n",loramode);
#endif
    //发送完成后立即关闭发射机以优化功�?
    SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
    SX1276Write(REG_LR_IRQFLAGSMASK,0xff);
    SX1276Write(REG_LR_IRQFLAGS,0xff);	
#if DBG_LORA_REG
    SX1276Read(REG_LR_OPMODE, &loramode);
    printf("after send  data,set mode standby. mode = %02x\r\n",loramode);
#endif
    return result;
}
//extern uint32_t tempsystick;
void SX1276LoRaSetReceive(void)
{
    SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
    SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |
                                //RFLR_IRQFLAGS_RXDONE |
                                //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                RFLR_IRQFLAGS_VALIDHEADER |
                                RFLR_IRQFLAGS_TXDONE |
                                RFLR_IRQFLAGS_CADDONE |
                                RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                RFLR_IRQFLAGS_CADDETECTED;
    SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );
    
    SX1276LR->RegHopPeriod = 255;
        
    SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod);
    //设置DIO0接收中断，设置DIO1接收超时中断       
                                // RxDone                    RxTimeout                  
    SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_00;//| RFLR_DIOMAPPING1_DIO1_00; 

    SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR->RegDioMapping1, 1);
    
    SX1276LR->RegFifoAddrPtr = RFLR_FIFOTXBASEADDR;  
    SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr ); //设置FIFO数据缓冲�?
    
//    if( LoRaSettings.RxSingleOn == true ) //单次接收
//    {
//    
//        SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER_SINGLE );

//    }
//    else //持续接收
//    {
        SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxBaseAddr;
        SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
        SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER );

//    }
}

uint16_t SX1276LoRaReceive(uint8_t *RFBuffer, uint16_t BufferSize, uint32_t timeout_ms)
{
    uint8_t RxPacketSize = 0;
    SX1276LoRaSetReceive();
#if DBG_LORA_REG    
    SX1276Read(REG_LR_OPMODE, &loramode);
    SX1276Read(REG_LR_OPMODE, &loramode);
   // printf("after set receive mode = %02x\r\n", loramode);
#endif    
    for(;;)
    {
         //printf("----------DIO_0= %d  \r\n",DIO_0);
        if(DIO_0) // RxDone  用查询寄存器代替查询引脚状�?
        {
            // Clear Irq
            SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXDONE);//清除接收中断标志
            SX1276Read( REG_LR_IRQFLAGS, &SX1276LR->RegIrqFlags ); //读取中断标志寄存�?
            if( ( SX1276LR->RegIrqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR ) == RFLR_IRQFLAGS_PAYLOADCRCERROR) //如果出现CRC错误中断
            {
                    // Clear Irq
                    printf("------------- lora crc----");
                    SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR  ); //清除CRC错误中断
            }
#if RX_SNRESTIMATE			
            static_lora_getRxSnrEstimate(); //获取信噪�?
#endif
#if RSSI
            static_lora_getRssi(); //获取Rssi
#endif	
            if( LoRaSettings.RxSingleOn == TRUE ) // Rx single mode
            {
                SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxBaseAddr;
                SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
                if( LoRaSettings.ImplicitHeaderOn == TRUE )
                {
                  RxPacketSize = SX1276LR->RegPayloadLength;
                  SX1276ReadFifo( RFBuffer, SX1276LR->RegPayloadLength );
                }
                else
                {
                        SX1276Read( REG_LR_NBRXBYTES, &SX1276LR->RegNbRxBytes );
                        RxPacketSize = SX1276LR->RegNbRxBytes;
                        SX1276ReadFifo( RFBuffer, SX1276LR->RegNbRxBytes );
                }
            }
            else // Rx continuous mode
            {
                SX1276Read( REG_LR_FIFORXCURRENTADDR, &SX1276LR->RegFifoRxCurrentAddr );
                
                if( LoRaSettings.ImplicitHeaderOn == TRUE )
                {
                  RxPacketSize = SX1276LR->RegPayloadLength;
                  SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxCurrentAddr;
                  SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
                  SX1276ReadFifo( RFBuffer, SX1276LR->RegPayloadLength );
                }
                else
                {
                  SX1276Read( REG_LR_NBRXBYTES, &SX1276LR->RegNbRxBytes );
                  RxPacketSize = SX1276LR->RegNbRxBytes;
                  SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxCurrentAddr;
                  SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
                  SX1276ReadFifo( RFBuffer, SX1276LR->RegNbRxBytes );
                }
            }
            break;
		}
        Delayms(10); //延时10ms
        timeout_ms -= 10;
        if(timeout_ms<=0)
        { 
          //printf("recvive timeout  DIO_0= %d  \r\n",DIO_0);
          break;
        }  
      }
    
#if DBG_LORA_REG    
  SX1276Read(REG_LR_OPMODE, &loramode);
  printf("after receive data mode = %02x\r\n", loramode);
#endif           
  SX1276LoRaSetOpMode(RFLR_OPMODE_STANDBY);
  return RxPacketSize;
}


#if RX_SNRESTIMATE  
inline static void static_lora_getRxSnrEstimate(void)
{

	uint8_t rxSnrEstimate;
	SX1276Read( REG_LR_PKTSNRVALUE, &rxSnrEstimate );
	if( rxSnrEstimate & 0x80 ) // The SNR sign bit is 1
	{
		// Invert and divide by 4
		RxPacketSnrEstimate = ( ( ~rxSnrEstimate + 1 ) & 0xFF ) >> 2;
		RxPacketSnrEstimate = -RxPacketSnrEstimate;
	}
	else
	{
		// Divide by 4
		RxPacketSnrEstimate = ( rxSnrEstimate & 0xFF ) >> 2;
	}	
    printf("RxPacketSnrEstimate = %d\r\n", RxPacketSnrEstimate);
}
#endif	
#if RSSI
inline static void static_lora_getRssi(void)  //只能在SX1276LoRaReceive中调�?
{
	SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
	
	RxPacketRssiValue = SX1276LR->RegPktRssiValue - 137;
//    printf("RxPacketRssiValue = %d\r\n", RxPacketRssiValue);
}
#endif



void LoRaTest(TestMode mode)
{
    uint8_t buffer[16];
    uint8_t rxLenth = 0;
    uint8_t data = 0;
    lora_Init();
    showLoraModeParameter();
    if(TX_MODE == mode)
    {
       for(;;)
        {
            memset(buffer, data++, sizeof(buffer));
            SX1276LoRaSend(buffer, sizeof(buffer));
            Delayms(2000);
        }
    }else if(RX_MODE == mode)
    {
        for(;;)
        {

          rxLenth = SX1276LoRaReceive(buffer, sizeof(buffer), 10000);
            if(rxLenth > 0)
            {
                printf("Rx: ");
                borad_PrintHex( buffer, rxLenth);
                printf("Rssi:%d\r\n", GetLoRaRssi());
            }
            
        }
    }
    

}
