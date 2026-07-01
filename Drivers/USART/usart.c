#include "usart.h"
#include "ringbuffer.h"
#include "gpio.h"
#include "LOGGER.h"

static ring_buffer_t usart2_rx_buffer;


void usart2_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	usart2_gpio_init();
	ring_buffer_init(&usart2_rx_buffer);

	USART2->BRR = 0x16D;
	USART2->CR1 |= USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART2_IRQn);
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_RE;
	USART2->CR1 |= USART_CR1_UE;
}



uint8_t usart2_write_char (char c)
{
	uint32_t timeout = USART_TIMEOUT;
	while (!(USART2->SR & USART_SR_TXE))
	{
		if(--timeout == 0)
		{
			return 0;
		}
	}
	USART2->DR = c;
	return 1;
}

uint8_t usart2_write_string(char *str)
{
	while(*str)
	{
		if(!(usart2_write_char(*str++)))
			return 0;
	}
	return 1;
}

uint8_t usart2_write_hex(uint8_t value)
{
	char hex_chars[] = "0123456789ABCDEF";

	if(!(usart2_write_char('0')))
		return 0;
	if(!(usart2_write_char('x')))
		return 0;

	if(!(usart2_write_char(hex_chars[(value >> 4) & 0x0F])))
		return 0;
	if(!(usart2_write_char(hex_chars[value & 0x0F])))
		return 0;
	return 1;
}

uint8_t usart2_write_int(int32_t value)
{
	char buffer[12];
	int i = 0;
	if (value == 0)
	{
		if(!(usart2_write_char('0')))
			return 0;
		return 1;
	}
	if (value < 0)
	{
		if(!(usart2_write_char('-')))
			return 0;
		value = -value;
	}
	while(value > 0)
	{
		buffer[i++] = (value%10) + '0';
		value /= 10;
	}
	while(i>0)
	{
		if(!(usart2_write_char(buffer[--i])))
			return 0;
	}
	return 1;
}

uint8_t usart2_write_fixed_point(uint32_t value)
{
	if(!(usart2_write_int(value / 100)))
		return 0;
	if(!(usart2_write_char('.')))
		return 0;
	if((value % 100) < 10)
	{
		if(!(usart2_write_char('0')))
			return 0;
	}
	if(!(usart2_write_int(value % 100)))
		return 0;
	return 1;
}

void USART2_IRQHandler(void)
{
	if(USART2->SR & USART_SR_RXNE)
	{
		uint8_t data = USART2->DR;
		ring_buffer_put(&usart2_rx_buffer, data);
	}

}

uint8_t usart2_data_available(void)
{
	return ring_buffer_available(&usart2_rx_buffer);
}

uint8_t usart2_read_char(char *c)
{
	uint8_t data;
	if(!(ring_buffer_get(&usart2_rx_buffer, &data)))
		return 0;
	*c = (char)data;
	return 1;
}

uint8_t usart2_read_string(char *buffer, uint32_t max_len)
{
	uint32_t i = 0;
	char c;
	while(i < (max_len - 1))
	{
		if(!(usart2_read_char(&c)))
			return 0;
		if(c == '\r' || c == '\n')
		{
			break;
		}
		buffer[i++] = c;
	}
	buffer[i] = '\0';
	return 1;
}



