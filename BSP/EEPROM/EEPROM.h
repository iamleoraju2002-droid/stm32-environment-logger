#ifndef EEPROM_H_
#define EEPROM_H_

#include "stm32f4xx.h"
#include "string.h"

#define EEPROM_ADDR 0x50
#define EEPROM_PAGE_SIZE 64
#define EEPROM_SIZE 32768

void eeprom_wait_for_write(void);
uint8_t eeprom_clear_all(void);
uint8_t eeprom_write_data(uint16_t mem_addr, uint8_t data);
uint8_t eeprom_read_data(uint16_t mem_addr, uint8_t *data);
uint8_t eeprom_write_buffer(uint16_t mem_addr, uint8_t *data, uint16_t len);
uint8_t eeprom_read_buffer(uint16_t mem_addr, uint8_t *buffer, uint16_t len);

#endif
