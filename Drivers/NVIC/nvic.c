#include "nvic.h"

void nvic_init(void)
{
	NVIC_EnableIRQ(TIM2_IRQn);
}
