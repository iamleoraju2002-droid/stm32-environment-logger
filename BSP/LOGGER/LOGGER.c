#include "LOGGER.h"
#include "EEPROM.h"
#include "usart.h"

static uint16_t record_count = 0;
static uint8_t logging_enabled = 1;

uint8_t logger_init(void)
{
	if(!(eeprom_read_buffer(LOG_COUNT_ADDR, (uint8_t *)&record_count, sizeof(record_count))))
		return 0;
	return 1;
}

uint8_t logger_save_record(int16_t temp, uint32_t pressure)
{
	log_record_t record;
	if(record_count >= MAX_RECORDS)
	{
		return 0;
	}
	record.temperature = temp;
	record.pressure = pressure;
	uint16_t address = LOG_DATA_ADDR + (record_count * sizeof(log_record_t));
	if(!(eeprom_write_buffer(address, (uint8_t *)&record, sizeof(log_record_t))))
		return 0;
	record_count++;
	if(!(eeprom_write_buffer(LOG_COUNT_ADDR, (uint8_t *)&record_count, sizeof(record_count))))
		return 0;
	return 1;
}

uint8_t logger_read_record(uint16_t index, log_record_t *record)
{
	if(index >= record_count)
		return 0;
	uint16_t address = LOG_DATA_ADDR + (index * sizeof(log_record_t));
	if(!(eeprom_read_buffer(address, (uint8_t *)record, sizeof(log_record_t))))
		return 0;
	return 1;

}

uint16_t logger_get_record_count(void)
{
	return record_count;
}



uint8_t logger_send_count(void)
{
	if(!(usart2_write_int(record_count)))
		return 0;
	if(!(usart2_write_string("\r\n")))
		return 0;
	return 1;
}

uint8_t logger_send_log(uint16_t index)
{
	log_record_t record;
	if(!(logger_read_record(index, &record)))
		return 0;
	if(!(usart2_write_string("Temperature = ")))
		return 0;
	if(!(usart2_write_fixed_point(record.temperature)))
		return 0;
	if(!(usart2_write_string("C\r\n")))
		return 0;

	if(!(usart2_write_string("Pressure = ")))
		return 0;
	if(!(usart2_write_fixed_point(record.pressure)))
		return 0;
	if(!(usart2_write_string("hPa\r\n")))
		return 0;
	return 1;
}


void logger_enable(void)
{
	logging_enabled = 1;
}

void logger_disable(void)
{
	logging_enabled = 0;
}

uint8_t logger_is_enabled(void)
{
	return logging_enabled;
}

uint8_t logger_clear(void)
{
	if(!(eeprom_clear_all()))
		return 0;
	record_count = 0;
	if(!(eeprom_write_buffer(LOG_COUNT_ADDR, (uint8_t *)&record_count, sizeof(record_count))))
		return 0;
	return 1;
}




