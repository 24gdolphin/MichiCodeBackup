/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
#include "PinsDef.h"
#include "stdio.h"
#include "board.h"



extern uint16_t Sys_GeneralTimer_count;
extern uint8_t RS485_Recv_Timerflag;
extern uint16_t RS485_Recv_Countvalue;
extern uint16_t RS485_Recv_Countvalue_max;
extern uint8_t RS485_Recv_Countoverflag;













/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler,6)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler,7)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  FlagStatus State;
  if(EXTI_GetITStatus(EXTI_IT_Pin0) != RESET) 
  { 
    EXTI_ClearITPendingBit(EXTI_IT_Pin0);
  }
 // Delayms(100);
  State = GPIO_ReadInputDataBit(DOOR_IN_PORT, DOOR_IN_PIN);
  if(RESET == State)
  {
    printf("open\n");
  }else
  {
     printf("close\n");
  }
  
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    FlagStatus State;
  if(EXTI_GetITStatus(EXTI_IT_Pin1) != RESET) 
  { 
    EXTI_ClearITPendingBit(EXTI_IT_Pin1);
  }
  State = GPIO_ReadInputDataBit(DOOR_IN_PORT, DOOR_IN_PIN);
  if(RESET == State)
  {
    printf("open door...\n");
  }else
  {
     printf("close door...\n");
  }
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
		if(EXTI_GetITStatus(EXTI_IT_Pin2) != RESET) 
		{ 
		  EXTI_ClearITPendingBit(EXTI_IT_Pin2);
		  printf("------------EXIT----------.\n");
		  
		}


	
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */



	
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    FlagStatus State;
  //Delayms(50);
  if(EXTI_GetITStatus(EXTI_IT_Pin7) != RESET) 
  { 
    EXTI_ClearITPendingBit(EXTI_IT_Pin7);
    //WWDG_SWReset();	
  }
  State = GPIO_ReadInputDataBit(B1_RESET_PORT, B1_RESET_PIN);
  if(RESET == State)
  {
    //printf("key release\n");	
    WWDG_SWReset();		
  }else
  {
     //printf("key press\n"); 
  }
}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler,16)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */


#ifdef SYS_FUNC_TIME_DBG

extern uint32_t function_time;
 
#endif



INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */ 

    TIM2_ClearITPendingBit(TIM2_IT_Update);


    #ifdef SYS_FUNC_TIME_DBG

	function_time++;
 
#endif


	Sys_GeneralTimer_count++;
	LED4_OFF
	if(Sys_GeneralTimer_count >= 1000)
	{
		LED4_ON
		Sys_GeneralTimer_count = 0;
		//printf(" 1s \r\n");
	}

	if(RS485_Recv_Timerflag)
	{
		RS485_Recv_Countvalue++;
		if(RS485_Recv_Countvalue > RS485_Recv_Countvalue_max)
		{
			RS485_Recv_Countoverflag = 1;
			RS485_Recv_Timerflag = 0;
			RS485_Recv_Countvalue = 0;
		}
	}
	else
	{
		RS485_Recv_Countvalue = 0;
	}

	
/*
    getruntime_count++;
	LED4_OFF
	if(getruntime_count >= 1000)
	   {
         LED4_ON
         if(ResetFlag == 1)
         	{
         		ResetTimeCount++;
				//printf("$$$$$$$$ ResetTimeCount =%d &&&&& \r\n",ResetTimeCount);
				if(ResetTimeCount == ResetTime)
					{
					  ResetTimeCount = 0;
					  ResetFlag = 0;
					  WWDG_SWReset();
					}
				if(ResetTimeCount > 60000)
					{
						ResetTimeCount = 0;
						 WWDG_SWReset();
					}	
         	}
		 else
		 	{
		 		ResetTimeCount = 0;
		 	}
		 
			if(Timer_flag ==1)
				{
					Timer_count++;
				}
			if(Timer_count >10000)  //10s
				{
					Timer_count = 0;
				}

		 getruntime_count = 0;	 
		}

*/
	
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
#define Acq_485Buf_max 128
extern uint8_t Acq_485Recv_Buf[Acq_485Buf_max];
extern uint8_t Acq_485Recv_Buf_Len;


INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler,20)
{
  
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//判断接收中断发生
    {
        Acq_485Recv_Buf[Acq_485Recv_Buf_Len++] = USART_ReceiveData8(USART2);
		RS485_Recv_Timerflag = 1;
		RS485_Recv_Countvalue = 0;
        if(Acq_485Recv_Buf_Len >= Acq_485Buf_max)
        {
            Acq_485Recv_Buf_Len = 0;
        }
    }
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */

#if GetRunTime 
extern uint16_t getruntime_count;
#endif
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler,21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler,22)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler,24)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */		
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */


	#define Uart1MaxSize 30
	uint8_t Uart1_buffer[Uart1MaxSize];
	uint8_t Uart1RecLen = 0;

INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  if( USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
  	{
  		//USART_ClearITPendingBit(USART1, USART_IT_RXNE); 
		Uart1_buffer[Uart1RecLen++] = USART_ReceiveData8(USART1);
		if( Uart1RecLen > 30)
			{
				Uart1RecLen = 0;
			}
  	}

	
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler,29)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
