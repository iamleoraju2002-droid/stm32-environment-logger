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
