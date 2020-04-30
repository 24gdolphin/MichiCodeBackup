#ifndef __PINSDEF_H
#define __PINSDEF_H
#include "stm8l15x_conf.h"
#include "main.h"


#ifdef User_LowPowerBoard

#define LED4_PORT       GPIOA
#define LED4_PIN        GPIO_Pin_4

#define LED3_PORT       GPIOA
#define LED3_PIN        GPIO_Pin_5

#define LED2_PORT       GPIOA
#define LED2_PIN        GPIO_Pin_6

#define LED1_PORT       GPIOA
#define LED1_PIN        GPIO_Pin_7


#else

#define LED1_PORT       GPIOA
#define LED1_PIN        GPIO_Pin_4

#define LED2_PORT       GPIOA
#define LED2_PIN        GPIO_Pin_5

#define LED3_PORT       GPIOA
#define LED3_PIN        GPIO_Pin_6

#define LED4_PORT       GPIOA
#define LED4_PIN        GPIO_Pin_7

#endif



#define UART3_TX_PORT    GPIOG
#define UART3_TX_PIN     GPIO_Pin_0

#define UART3_RX_PORT    GPIOG
#define UART3_RX_PIN     GPIO_Pin_1

#define RS485_RE_PORT   GPIOG
#define RS485_RE_PIN    GPIO_Pin_3

#define F_CS_PORT       GPIOE
#define F_CS_PIN        GPIO_Pin_0

#define F_WP_PORT       GPIOE
#define F_WP_PIN        GPIO_Pin_1

#define F_HLD_PORT      GPIOE
#define F_HLD_PIN       GPIO_Pin_2

#define UART2_RX_PORT   GPIOE
#define UART2_RX_PIN    GPIO_Pin_3
#define UART2_TX_PORT   GPIOE
#define UART2_TX_PIN    GPIO_Pin_4

#define UART1_RX_PORT   GPIOC
#define UART1_RX_PIN    GPIO_Pin_2
#define UART1_TX_PORT   GPIOC
#define UART1_TX_PIN    GPIO_Pin_3


#define UP_PORT         GPIOD
#define UP_PIN          GPIO_Pin_1

#define AD1_PORT        GPIOD
#define AD1_PIN         GPIO_Pin_2

#define AD2_PORT        GPIOD
#define AD2_PIN         GPIO_Pin_3

#define AD3_PORT        GPIOB
#define AD3_PIN         GPIO_Pin_0

#define AD4_PORT        GPIOB
#define AD4_PIN         GPIO_Pin_1

#define RF_CS1_PORT     GPIOB
#define RF_CS1_PIN      GPIO_Pin_4
#define RF_RST1_PORT    GPIOF
#define RF_RST1_PIN     GPIO_Pin_1
#define DIO0_PORT       GPIOF
#define DIO0_PIN        GPIO_Pin_0

#define SPI_PORT        GPIOB
#define SPI_SCK_PIN     GPIO_Pin_5
#define SPI_I_PIN       GPIO_Pin_6
#define SPI_O_PIN       GPIO_Pin_7


#define KEY1_PORT       GPIOG
#define KEY1_PIN        GPIO_Pin_5

#define KEY2_PORT       GPIOG
#define KEY2_PIN        GPIO_Pin_4

#define B1_RESET_PORT   GPIOE
#define B1_RESET_PIN    GPIO_Pin_7

#define EN33_PORT       GPIOG
#define EN33_PIN        GPIO_Pin_7

#define I2CSCL_PORT     GPIOC
#define I2CSCL_PIN      GPIO_Pin_1
#define I2CSDA_PORT     GPIOC
#define I2CSDA_PIN      GPIO_Pin_0


#define DOOR_IN_PORT     GPIOC
#define DOOR_IN_PIN      GPIO_Pin_0

#define VDD_EN_PORT      GPIOD
#define VDD_EN_PIN       GPIO_Pin_6

#define VBAT_EN_PORT     GPIOD
#define VBAT_EN_PIN      GPIO_Pin_7

#define RS485_EN_PORT   GPIOG
#define RS485_EN_PIN    GPIO_Pin_2


//#define DOOR_IN_PORT     GPIOC
//#define DOOR_IN_PIN      GPIO_Pin_1
#endif


