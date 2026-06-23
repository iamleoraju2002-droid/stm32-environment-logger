#include "LOGGER.h"
#include "EEPROM.h"

static uint16_t record_count = 0;

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













