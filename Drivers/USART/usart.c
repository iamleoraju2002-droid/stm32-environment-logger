#include "usart.h"
#include "ringbuffer.h"

static ring_buffer_t usart2_rx_buffer;
static void usart2_gpio_init(void);

void usart2_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	usart2_gpio_init();
	ring_buffer_init(&usart2_rx_buffer);

	USART2->BRR = 0x16D;
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_UE;
}

static void usart2_gpio_init(void)
{
	GPIOA->MODER &= ~(3U << (2 * 2));
	GPIOA->MODER |= (2U << (2 * 2));

	GPIOA->MODER &= ~(3U << (2 * 3));
	GPIOA->MODER |= (2U << (2 * 3));

	GPIOA->AFR[0] &= ~(0xF << (2 * 4));
	GPIOA->AFR[0]|= (7U << (2 * 4));

	GPIOA->AFR[0] &= ~(0xF << (3 * 4));
	GPIOA->AFR[0]|= (7U << (3 * 4));
}

void usart2_write_char (char c)
{
	while (!(USART2->SR & USART_SR_TXE));
	USART2->DR = c;
}

void usart2_write_string(char *str)
{
	while(*str)
	{
		usart2_write_char(*str++);
	}
}

void usart2_write_hex(uint8_t value)
{
	char hex_chars[] = "0123456789ABCDEF";

	usart2_write_char('0');
	usart2_write_char('x');

	usart2_write_char(hex_chars[(value >> 4) & 0x0F]);
	usart2_write_char(hex_chars[value & 0x0F]);
}

void usart2_write_int(int32_t value)
{
	char buffer[12];
	int i = 0;
	if (value == 0)
	{
		usart2_write_char('0');
		return;
	}
	if (value < 0)
	{
		usart2_write_char('-');
		value = -value;
	}
	while(value > 0)
	{
		buffer[i++] = (value%10) + '0';
		value /= 10;
	}
	while(i>0)
	{
		usart2_write_char(buffer[--i]);
	}
}

void usart2_enable_rx_interrupt(void)
{
	USART2->CR1 |= USART_CR1_RE;
	USART2->CR1 |= USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART2_IRQn);
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

char usart2_read_char(void)
{
	uint8_t data;
	while(!ring_buffer_get(&usart2_rx_buffer, &data));
	return (char)data;
}

void usart2_read_string(char *buffer, uint32_t max_len)
{
	uint32_t i = 0;
	char c;
	while(i < (max_len - 1))
	{
		c = usart2_read_char();
		if(c == '\r' || c == '\n')
		{
			break;
		}
		buffer[i++] = c;
	}
	buffer[i] = '\0';
}









