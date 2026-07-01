#include "EEPROM.h"
#include "i2c.h"
#include "delay.h"


void eeprom_wait_for_write(void)
{
	uint32_t timeout = 10000;
	while(timeout--)
	{
		i2c1_start();
		if(i2c1_send_address(EEPROM_ADDR, I2C_WRITE))
		{
			i2c1_stop();
			break;
		}
		i2c1_stop();
	}
}

uint8_t eeprom_write_data(uint16_t mem_addr, uint8_t data)
{
	if(mem_addr >= EEPROM_SIZE)
		return 0;
	if(!(i2c1_start()))
		return 0;
	if(!(i2c1_send_address(EEPROM_ADDR, I2C_WRITE)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte((mem_addr >> 8) & 0xFF)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte(mem_addr & 0xFF)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte(data)))
	{
		i2c1_stop();
		return 0;
	}
	i2c1_stop();
	eeprom_wait_for_write();
	return 1;

}

uint8_t eeprom_read_data(uint16_t mem_addr, uint8_t *data)
{
	if(mem_addr >= EEPROM_SIZE)
		return 0;
	if(!(i2c1_start()))
	{
		return 0;
	}
	if(!(i2c1_send_address(EEPROM_ADDR, I2C_WRITE)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte((mem_addr >> 8) & 0xFF)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte(mem_addr & 0xFF)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_restart()))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_send_address(EEPROM_ADDR, I2C_READ)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_read_byte_nack(data)))
	{
		i2c1_stop();
		return 0;
	}
	return 1;
}


static uint8_t eeprom_write_page(uint16_t mem_addr, uint8_t *data, uint16_t len)
{
	uint16_t i;
	if(!(i2c1_start()))
		return 0;
	if(!(i2c1_send_address(EEPROM_ADDR, I2C_WRITE)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte((mem_addr >> 8) & 0xFF)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte(mem_addr & 0xFF)))
	{
		i2c1_stop();
		return 0;
	}
	for(i = 0; i < len; i++)
	{
		if(!(i2c1_write_byte(data[i])))
		{
			i2c1_stop();
			return 0;
		}
	}
	i2c1_stop();
	eeprom_wait_for_write();
	return 1;
}


uint8_t eeprom_write_buffer(uint16_t mem_addr, uint8_t *data, uint16_t len)
{
	if((mem_addr + len) > EEPROM_SIZE)
		return 0;
	uint16_t bytesremaining;
	uint16_t pagespace;
	while(len>0)
	{
		pagespace = EEPROM_PAGE_SIZE - (mem_addr % EEPROM_PAGE_SIZE);
		if(len < pagespace)
		{
			bytesremaining = len;
		}
		else
		{
			bytesremaining = pagespace;
		}
		if(!(eeprom_write_page(mem_addr, data, bytesremaining)))
			return 0;
		mem_addr += bytesremaining;
		data += bytesremaining;
		len -= bytesremaining;
	}
	return 1;
}


uint8_t eeprom_read_buffer(uint16_t mem_addr, uint8_t *buffer, uint16_t len)
{
	if((mem_addr + len) > EEPROM_SIZE)
		return 0;
	uint16_t i;
	if(!(i2c1_start()))
		return 0;
	if(!(i2c1_send_address(EEPROM_ADDR, I2C_WRITE)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte((mem_addr >> 8) & 0xFF)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte(mem_addr & 0xFF)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_restart()))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_send_address(EEPROM_ADDR, I2C_READ)))
	{
		i2c1_stop();
		return 0;
	}
	if(len == 0)
	{
		i2c1_stop();
		return 1;
	}
	for (i=0;i<(len-1);i++)
	{
		if(!(i2c1_read_byte_ack(&buffer[i])))
		{
			i2c1_stop();
			return 0;
		}
	}
	if(!(i2c1_read_byte_nack(&buffer[len-1])))
	{
		i2c1_stop();
		return 0;
	}
	return 1;
}


uint8_t eeprom_clear_all(void)
{
	uint8_t erase_page[64];
	memset(erase_page, 0x00, 64);
	for(uint16_t addr = 0; addr < EEPROM_SIZE; addr += 64)
	{
		if(!(eeprom_write_buffer(addr, erase_page, 64)))
			return 0;
	}
	return 1;
}
















