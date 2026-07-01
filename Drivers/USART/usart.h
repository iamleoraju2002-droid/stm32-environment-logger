#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"

#define USART_TIMEOUT 100000U

void usart2_init(void);
uint8_t usart2_write_char(char c);
uint8_t usart2_write_string(char *str);
uint8_t usart2_write_hex(uint8_t value);
uint8_t usart2_write_int(int32_t value);
uint8_t usart2_write_fixed_point(uint32_t value);


uint8_t usart2_data_available(void);
uint8_t usart2_read_char(char *c);
uint8_t usart2_read_string(char *buffer, uint32_t max_len);




#endif
