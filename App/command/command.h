#ifndef COMMAND_H_
#define COMMAND_H_

#include "stm32f4xx.h"

typedef enum
{
	CMD_NONE,
	CMD_LIVE,
	CMD_LOG_ON,
	CMD_LOG_OFF,
	CMD_COUNT,
	CMD_READ,
	CMD_CLEAR,
	CMD_HELP,
}command_t;

typedef struct
{
	command_t name;
	uint8_t argument;
}command_packet_t;



uint8_t command_available(void);
uint8_t command_parser(void);
uint8_t command_process(command_packet_t *packet);
uint8_t command_execute(command_packet_t *packet);
void command_terminal(void);
void command_print_help(void);
void command_clear(void);

#endif
