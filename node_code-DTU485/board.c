#include "stm8l15x_conf.h"
#include "board.h"
#include "PinsDef.h"
#include "stdio.h" 
#include "string.h"
#include "time.h"
#include "main.h"
//#include "config.h"
#include "sysconfig.h"

#include "stm8l15x.h"



static void board_RccInit(void);
static void board_LedInit(void);
static void board_rtcInit(void);
static void board_UartInit(void);
static void board_SpiInit(void);
static void board_adc_Init(void);
#if USE_KEY
static void board_keyInit(void);
#endif
#if SENSOR
static void board_i2cInit(void);
#endif
#if DOOR_LOCK
static void board_DoorLockInit(void);
#endif
/*
*/


void board_Init(void)
{

	board_RccInit();
#ifdef User_LowPowerBoard 
	VDD_ENABLE 
	VBAT_ENABLE
	EN33_ENABLE
	RS485_ENABLE	  
#endif

	board_LedInit();
	board_rtcInit();
	board_UartInit();
	board_SpiInit();

#if GetRunTime 
	board_TIME2Init();
#endif 

#if USE_KEY 
	board_keyInit();
#endif  


}









//void test
//#define AD1_PORT        GPIOD
//#define AD1_PIN         GPIO_Pin_2
 void board_EXIT(void)
{
	disableInterrupts();
  GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_In_FL_IT);
  EXTI_DeInit();
  EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising_Falling);
  //EXTI_SetPinSensitivity(EXTI_Pin_7, EXTI_Trigger_Falling);
    if(EXTI_GetITStatus(EXTI_IT_Pin2) != RESET) 
  { 
    EXTI_ClearITPendingBit(EXTI_IT_Pin2);
    printf("EXTI\n");
  }
    printf("EXTI-----------INIT\n");	
  enableInterrupts();
}





uint16_t board_Invtert_Uint16(uint16_t a)
{
   uint8_t i,j;
   i = (uint8_t)a;
   j = (uint8_t)(a>>8);
   a = (((uint16_t)i)&0xf0)|  (((uint16_t)j)&0x0f);
   return a;   
}

uint32_t board_Invtert_Uint32(uint32_t a)
{
   uint8_t i,j,k,m;
   uint32_t b;
   
   i = (uint8_t)(a);
   j = (uint8_t)((a>>8)&0x000000ff);  
   k = (uint8_t)((a>>16)&0x000000ff);
   m = (uint8_t)((a>>24)&0x000000ff); 
   b =  (((uint32_t)(i))<<24)|(((uint32_t)(j))<<16)|(((uint32_t)(k))<<8)|((uint32_t)(m));

  // printf("\n a--%lx    b--%lx    i--%x    j--%x  k--%x  m--%x  \n",a,b,i,j,m,k);
  return b;
}



/*

*/
static void board_RccInit(void)
{
  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  /* High speed external clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
  {}

}

/*
  功能：指示灯初始化
*/
static void board_LedInit(void)
{
   /* Initialize LEDs mounted on STM8L152X-EVAL board */
  GPIO_Init(LED1_PORT, LED1_PIN, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(LED2_PORT, LED2_PIN, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(LED3_PORT, LED3_PIN, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(LED4_PORT, LED4_PIN, GPIO_Mode_Out_PP_Low_Fast);
}



/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}

void Delayus(void)
{
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
}


void Delayms(uint32_t time)   
{   
    unsigned int i;   
    while(time--)     
    for(i=900;i>0;i--)   
    Delayus();    
}





/*################################### ABOUT TIME2 END #########################################*/
   

/*
  功能：UART1初始化（用来打印调试信息）
*/
static void board_UartInit(void)
{
  /*************************** 初始化UART1 ***********************************/
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,ENABLE);
    /* Configure USART Tx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(UART1_TX_PORT, UART1_TX_PIN, ENABLE);

  /* Configure USART Rx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(UART1_RX_PORT,UART1_RX_PIN , ENABLE);
  USART_Init(USART1, 
	     115200,
             USART_WordLength_8b,
             USART_StopBits_1,
             USART_Parity_No,
             (USART_Mode_TypeDef)(USART_Mode_Tx)|(USART_Mode_Rx));
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

  
}


/***************************** 初始化UART1 end	*****************************/


/*##################################### TIME2 ############################################*/

#if GetRunTime
void board_TIME2Init(void){

	TIM2_DeInit();
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);         
    TIM2_TimeBaseInit(TIM2_Prescaler_16, TIM2_CounterMode_Up,1000 );
    TIM2_ITConfig(TIM2_IT_Update, ENABLE);
    TIM2_ARRPreloadConfig(ENABLE);
    ITC_SetSoftwarePriority(TIM2_UPD_OVF_TRG_BRK_IRQn , ITC_PriorityLevel_2);
	TIM2_ClearITPendingBit(TIM2_IT_Update);
    TIM2_Cmd(ENABLE);
	
}



/*################################### ABOUT TIME2 END #########################################*/




/*############################################ 485 ########################################################*/
/*
  功能：485初始化（UART2）
*/


//0x41 65 9600 8 no
//0x21 33 2400 8 no
//0x22 34 2400 8 even

void board_RS485Init_Config(uint8_t configchioce)
{
	uint32_t bauds = 9600;
	
	uint8_t group_baud = 1;
	uint8_t group_check_len = 1;



	USART_WordLength_TypeDef WordLength = USART_WordLength_8b;
	USART_Parity_TypeDef Parity = USART_Parity_No;

	group_baud = (configchioce>>4)&0x0f;

	group_check_len = configchioce&0x0f;


	switch (group_baud)
		{
			case 0x01:
				bauds = 1200;
				break;

			case 0x02: 
				bauds = 2400;
				break;
					
			case 0x03:
				bauds = 4800;
				break;
			case 0x04:
				bauds = 9600;
				break;
			case 0x05:
				bauds = 19200;
				break;
			case 0x06:
				bauds = 38400;
				break;
			case 0x07:
				bauds = 115200;
				break;

			default:
			break;
		}

	switch (group_check_len)
		{
			case 0x01:
			WordLength = USART_WordLength_8b;
			Parity = USART_Parity_No;
			break;
			case 0x02:
			WordLength = USART_WordLength_8b;
			Parity = USART_Parity_Even;
			break;
			case 0x03:
			WordLength = USART_WordLength_8b;
			Parity = USART_Parity_Odd;
			break;

			case 0x04:
			WordLength = USART_WordLength_9b;
			Parity = USART_Parity_No;
			break;
			case 0x05:
			WordLength = USART_WordLength_9b;
			Parity = USART_Parity_Even;
			break;
			case 0x06:
			WordLength = USART_WordLength_9b;
			Parity = USART_Parity_Odd;
			break;				

			default:
			break;
		}


	 USART_DeInit(USART2);
	 disableInterrupts();  

	 CLK_PeripheralClockConfig(CLK_Peripheral_USART2,ENABLE);
	 GPIO_ExternalPullUpConfig(UART2_TX_PORT, UART2_TX_PIN, ENABLE);
	 GPIO_ExternalPullUpConfig(UART2_RX_PORT,UART2_RX_PIN , ENABLE);
	 USART_Init(USART2, 
			bauds,
				WordLength,
				USART_StopBits_1,
				Parity,
				(USART_Mode_TypeDef)(USART_Mode_Tx|USART_Mode_Rx));
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	enableInterrupts();  
	GPIO_Init(RS485_RE_PORT, RS485_RE_PIN, GPIO_Mode_Out_PP_High_Fast);
	RS485_RX;
	
}


void board_Rs485Init()
{

  CLK_PeripheralClockConfig(CLK_Peripheral_USART2,ENABLE);
    /* Configure USART Tx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(UART2_TX_PORT, UART2_TX_PIN, ENABLE);

  /* Configure USART Rx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(UART2_RX_PORT,UART2_RX_PIN , ENABLE);
  USART_Init(USART2, 
	     9600,
             USART_WordLength_8b,  //USART_WordLength_9b
             USART_StopBits_1,
             USART_Parity_No,   //USART_Parity_Even
             (USART_Mode_TypeDef)(USART_Mode_Tx|USART_Mode_Rx));
  
    /* Enable general interrupts */

  /* Enable the USART Receive interrupt: this interrupt is generated when the USART
    receive data register is not empty */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  /* Enable the USART Transmit complete interrupt: this interrupt is generated when the USART
    transmit Shift Register is empty */

  /* Enable USART */
  USART_Cmd(USART2, ENABLE);
 enableInterrupts();  
  /*    初始化485芯片控制引脚 */
  GPIO_Init(RS485_RE_PORT, RS485_RE_PIN, GPIO_Mode_Out_PP_High_Fast);
  RS485_RX;//默认设置为发送模式
  
}

/*
  功能：通过485发送一段数据
*/
void board_Rs485Send(uint8_t* pIn, uint16_t length)
{
  uint16_t i = 0;

  
  RS485_TX; //切换485为发送模式

  
  USART_ClearFlag(USART2,USART_FLAG_TC); //初始化 发送的第一个字节丢失 先清除下
  for(i = 0; i < length; ++i)
  {
   // while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData8(USART2, *(pIn+i));
    //USART2->DR = pIn[i];
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
 //   GPIO_ToggleBits(LED1_PORT, LED1_PIN);
 
  }


  RS485_RX; 
}

/*
  功能：接收485数据
  pOut： 接收数据要存储的buffer
  size: buffer大小
  timeoutms:超时时间
  return：返回接收到的数据长度
*/

#define Acq_485Buf_max 128
extern uint8_t Acq_485Recv_Buf[Acq_485Buf_max];
extern uint8_t Acq_485Recv_Buf_Len;
extern uint8_t RS485_Recv_Countoverflag;


uint16_t board_Rs485Recv(uint8_t* pOut, uint16_t size, uint16_t timeoutms)
{

	uint16_t times = timeoutms;
	Acq_485Recv_Buf_Len = 0;   
	RS485_Recv_Countoverflag = 0;
	memset(Acq_485Recv_Buf,0,sizeof(Acq_485Recv_Buf));
	Delayms(50);
	while(1)
	{
		Delayms(10);
		if(RS485_Recv_Countoverflag)
		{
			memcpy(pOut, Acq_485Recv_Buf, Acq_485Recv_Buf_Len);
			RS485_Recv_Countoverflag = 0;
			break;
		}

		times -= 10;
		if(times < 20)
		break;
	}

  return Acq_485Recv_Buf_Len;
}


/*

Acq_485Recv_Buf_Len = 0;   

uint16_t tempCount = 0;
RS485_RX; 
//enableInterrupts();
Delayms(timeoutms);
while(1)
{
  tempCount = Acq_485Recv_Buf_Len;
  Delayms(10);
  if(tempCount == Acq_485Recv_Buf_Len)
  {
	break;
  }
}
if(size < Acq_485Recv_Buf_Len)
{
  return 0;
}
memcpy(pOut, Acq_485Recv_Buf, Acq_485Recv_Buf_Len);
//RS485_TX; 
//disableInterrupts();
return Acq_485Recv_Buf_Len;

*/

/*#################################################### 485 #########################################################*/

/*################################################閲嶅畾鍚?Printf ##############################################*/
#ifdef _RAISONANCE_
#define PUTCHAR_PROTOTYPE int putchar (char c)
#define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
#define PUTCHAR_PROTOTYPE char putchar (char c)
#define GETCHAR_PROTOTYPE char getchar (void)
#else /* _IAR_ */
#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)
#endif /* _RAISONANCE_ */
PUTCHAR_PROTOTYPE
{
  /* Write a character to the USART */
  USART_SendData8(USART1, c);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

  return (c);
  
}

#elif(DOOR_LOCK)

static void board_DoorLockInit(void)
{
  GPIO_Init(DOOR_IN_PORT, DOOR_IN_PIN, GPIO_Mode_In_FL_IT);
  disableInterrupts();
  EXTI_SetPinSensitivity(EXTI_Pin_0, EXTI_Trigger_Rising_Falling);
  //  EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising_Falling);

  enableInterrupts();
}

#endif


#if USE_KEY
static void board_keyInit(void)
{
	disableInterrupts();
  GPIO_Init(B1_RESET_PORT, B1_RESET_PIN, GPIO_Mode_In_FL_IT);
  EXTI_DeInit();
  EXTI_SetPinSensitivity(EXTI_Pin_7, EXTI_Trigger_Rising_Falling);
  //EXTI_SetPinSensitivity(EXTI_Pin_7, EXTI_Trigger_Falling);
    if(EXTI_GetITStatus(EXTI_IT_Pin7) != RESET) 
  { 
    EXTI_ClearITPendingBit(EXTI_IT_Pin7);
    printf("board_keyInit\n");
  }
  enableInterrupts();
}

#endif


/*################################# ABOUT  LORA #########################################*/

static void board_SpiInit(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
    
    GPIO_ExternalPullUpConfig(GPIOB, SPI_SCK_PIN | SPI_I_PIN | SPI_O_PIN, ENABLE);
  
    GPIO_Init(RF_CS1_PORT, RF_CS1_PIN, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(F_CS_PORT, F_CS_PIN, GPIO_Mode_Out_PP_Low_Fast);
    LORA_CS_High;
    F_CS_High;
    SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4, SPI_Mode_Master,
           SPI_CPOL_High, SPI_CPHA_2Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, 0x07);
    SPI_Cmd(SPI1, ENABLE);
  
}


uint8_t spi_communication(uint8_t txData)
{
  uint8_t count = 0;
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET)
  {
    count++;
    if(count > 100)
    {
      printf("spi communication timeout\n");
      return 0;
    }
  }

  SPI_SendData(SPI1, txData);
  count = 0;
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET)
  {
    count++;
    if(count > 100)
    {
      printf("spi communication timeout\n");
      return 0;
    }
  }
  return SPI_ReceiveData(SPI1);

}










static void board_rtcInit(void)
{
  RTC_InitTypeDef   RTC_InitStr;
  RTC_TimeTypeDef   RTC_TimeStr;
  RTC_DateTypeDef   RTC_DateStr;

    /* Enable LSE */
  CLK_LSEConfig(CLK_LSE_ON);
  /* Wait for LSE clock to be ready*/
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
  /* Select LSE (32.768 KHz) as RTC clock source */
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //使能RTC外设时钟
  /*Calendar init*/
  RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
  RTC_InitStr.RTC_AsynchPrediv = 0x1F;
  RTC_InitStr.RTC_SynchPrediv = 0x03FF;
  RTC_Init(&RTC_InitStr);
  /*set default time*/
  RTC_TimeStructInit(&RTC_TimeStr);
  RTC_TimeStr.RTC_Hours = 17;
  RTC_TimeStr.RTC_Minutes = 57;
  RTC_TimeStr.RTC_Seconds = 00;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);
  /*set default date*/
  RTC_DateStructInit(&RTC_DateStr);
  RTC_DateStr.RTC_WeekDay = RTC_Weekday_Tuesday;
  RTC_DateStr.RTC_Date = 3;  
  RTC_DateStr.RTC_Month = RTC_Month_July;
  RTC_DateStr.RTC_Year = 18;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);
  
  
}




void borad_PrintHex(uint8_t* pIn, uint16_t Length)
{
  printf("\n");
  for(uint16_t i = 0; i < Length; ++i)
    printf(" %02x", pIn[i]);
  printf("\n");
}










