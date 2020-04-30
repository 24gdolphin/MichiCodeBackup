#ifndef __HELPER_H
#define __HELPER_H

#include "stm8l15x.h"

char *helper_strsep (char **stringp, const char *delim);


void helper_printTitleHex(uint8_t * title, uint8_t *pin, uint16_t Length);

void helper_printTitleDec(uint8_t *title, uint8_t *pin, uint16_t Length);


uint8_t GetCrc(uint8_t *buf, uint8_t len);

void helper_AddCrc(uint8_t *buf, uint8_t len); 


uint8_t CheckCrc(uint8_t *buf, uint8_t len);


void helper_printHex(uint8_t * pIn, uint16_t Length);


void helper_printDec(uint8_t * pIn, uint16_t Length);


//void helper_printHex(uint8_t * pIn, uint16_t Length);


/*

#define DATA_MEMORY_START_ADDR (FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS)
#define DATA_MEMORY_STOP_ADDR  (FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 128)



void board_EEPROM_init(void);
uint8_t board_EEPROM_read(uint32_t FlashAddr, uint8_t *dest, uint8_t nbyte);
uint8_t board_EEPROM_write(uint32_t FlashAddr, uint8_t *source, uint8_t nbyte);

*/
#endif





