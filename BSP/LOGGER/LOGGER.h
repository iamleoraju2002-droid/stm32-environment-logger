/*
 * LOGGER.h
 *
 *  Created on: 17-Jun-2026
 *      Author: iamle
 */

#ifndef LOGGER_LOGGER_H_
#define LOGGER_LOGGER_H_

#include "stm32f4xx.h"
#include "EEPROM.h"

typedef struct
{
	int16_t temperature;
	uint32_t pressure;
}log_record_t;


#define LOG_COUNT_ADDR 0x0000
#define LOG_DATA_ADDR 0x0002
#define MAX_RECORDS ((EEPROM_SIZE - LOG_DATA_ADDR) / sizeof(log_record_t))
#define LOGGING_ON 1
#define LOGGING_OFF 0


uint8_t logger_init(void);
uint8_t logger_save_record(int16_t temp, uint32_t pressure);
uint8_t logger_read_record(uint16_t index, log_record_t *record);
uint16_t logger_get_record_count(void);
uint8_t logger_send_count(void);
uint8_t logger_send_log(uint16_t index);
void logger_enable(void);
void logger_disable(void);
uint8_t logger_is_enabled(void);
uint8_t logger_clear(void);


#endif /* LOGGER_LOGGER_H_ */
