#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"

void usart2_init(void);
void usart2_write_char(char c);
void usart2_write_string(char *str);
void usart2_write_hex(uint8_t value);
void usart2_write_int(int32_t value);

void usart2_enable_rx_interrupt(void);
uint8_t usart2_data_available(void);
char usart2_read_char(void);
void usart2_read_string(char *buffer, uint32_t max_len);

#endif
