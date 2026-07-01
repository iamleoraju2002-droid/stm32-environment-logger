#include "gpio.h"

void gpio_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER &= ~(3U << (5*2));
	GPIOA->MODER |= (1U << (5*2));

	GPIOA->OTYPER &= ~(1U << 5);
	GPIOA->OSPEEDR |= (3U << (5 * 2));
	GPIOA->PUPDR &= ~(3U << (5*2));
}

void usart2_gpio_init(void)
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
