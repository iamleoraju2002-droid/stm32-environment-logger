#include "command.h"
#include "usart.h"
#include "ringbuffer.h"
#include "string.h"
#include "stdlib.h"
#include "eeprom.h"
#include "LOGGER.h"
#include "BMP280.h"
#include "app_data.h"


static uint8_t command_ready = 0;
static uint16_t cmd_index = 0;
static char cmd[32];
static uint8_t cmd_overflow = 0;



uint8_t command_parser(void)
{
	char data;
	if(command_ready == 1)
		return 1;
	while(usart2_read_char(&data))
	{
		if (cmd_overflow)
		{
			if(data == '\n' || data == '\r')
			{
				cmd_overflow = 0;
				cmd_index = 0;
			}
			continue;
		}

		if(data == '\n' || data == '\r')
		{
			if(cmd_index > 0)
			{
				cmd[cmd_index] = '\0';
				cmd_index = 0;
				cmd_overflow = 0;
				command_ready = 1;
				return 1;
			}
		}
		else
		{
			if(cmd_index < sizeof(cmd) - 1)
				cmd[cmd_index++] = data;
			else
				cmd_overflow = 1;
		}
	}
	return 0;
}

uint8_t command_available(void)
{
	return command_ready;
}

uint8_t command_process(command_packet_t *packet)
{
	if(!(command_ready))
		return 0;
    packet->argument = 0;

    if(strcmp(cmd, "live") == 0)
    {
        packet->name = CMD_LIVE;
    }
    else if(strcmp(cmd, "log on") == 0)
    {
        packet->name = CMD_LOG_ON;
    }
    else if(strcmp(cmd, "log off") == 0)
    {
        packet->name = CMD_LOG_OFF;
    }
    else if(strcmp(cmd, "count") == 0)
    {
        packet->name = CMD_COUNT;
    }
    else if(strncmp(cmd, "read", 4) == 0)
    {
        packet->name = CMD_READ;
        packet->argument = atoi(&cmd[5]);
    }
    else if(strcmp(cmd, "clear") == 0)
    {
        packet->name = CMD_CLEAR;
    }
    else if(strcmp(cmd, "help") == 0)
    {
        packet->name = CMD_HELP;
    }
    else
    {
        usart2_write_string("\r\nWrong command\r\n");
        return 0;
    }
    return 1;
}

uint8_t command_execute(command_packet_t *packet)
{
	switch (packet->name)
	{
	case CMD_LIVE:
		if(!(bmp280_send_temp(&int_temp)))
			return 0;
		if(!(bmp280_send_pressure(&int_pressure)))
			return 0;
		return 1;
	case CMD_LOG_ON:
		logger_enable();
		if(!(usart2_write_string("Logging is enabled\r\n")))
			return 0;
		return 1;
	case CMD_LOG_OFF:
		logger_disable();
		if(!(usart2_write_string("Logging is disabled\r\n")))
			return 0;
		return 1;
	case CMD_COUNT:
		if(!(logger_send_count()))
			return 0;
		return 1;
	case CMD_READ:
		if(!(logger_send_log(packet->argument)))
			return 0;
		return 1;
	case CMD_CLEAR:
		if(!(usart2_write_string("inside clear\r\n")))
			return 0;
		if(!(logger_clear()))
			return 0;
		if(!(usart2_write_string("eeprom is cleared\r\n")))
			return 0;
		return 1;
	case CMD_HELP:
		command_print_help();
		return 1;
	default :
		return 0;
	}
}

void command_terminal(void)
{
    usart2_write_string("\r\n");
    usart2_write_string("==========================================\r\n");
    usart2_write_string("      STM32 Environment Logger\r\n");
    usart2_write_string("==========================================\r\n");
    usart2_write_string("System Ready\r\n");
    usart2_write_string("Type 'help' to list available commands.\r\n");
    usart2_write_string(">\r\n");
}


void command_print_help(void)
{
	usart2_write_string("\r\n=====================================\r\n");
	usart2_write_string("STM32 Environment Logger Commands\r\n");
	usart2_write_string("=====================================\r\n\r\n");

	usart2_write_string("live        - Show current temperature and pressure\r\n");
	usart2_write_string("log on      - Enable periodic logging\r\n");
	usart2_write_string("log off     - Disable periodic logging\r\n");
	usart2_write_string("count       - Show total number of logged records\r\n");
	usart2_write_string("read <n>    - Read record number n\r\n");
	usart2_write_string("clear       - Erase all logged records\r\n");
	usart2_write_string("help        - Show this help menu\r\n");

	usart2_write_string("\r\nExample:\r\n");
	usart2_write_string("read 5\r\n");

	usart2_write_string("\r\n=====================================\r\n");
}

void command_clear(void)
{
	command_ready = 0;
}



