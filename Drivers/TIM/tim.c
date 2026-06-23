#include "tim.h"

volatile uint8_t timer_flag = 0;

void timer2_init(uint32_t period_ms)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	TIM2->PSC = 8399;
	TIM2->ARR = (period_ms *10) - 1;
	TIM2->CNT = 0;
	TIM2->DIER |= TIM_DIER_UIE;
}

void timer2_start(void)
{
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF;
		timer_flag = 1;
	}
}
