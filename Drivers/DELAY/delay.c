#include "delay.h"

void delay_ms(uint32_t ms)
{
	while(ms--)
	{
		while(!(SysTick->CTRL &SysTick_CTRL_COUNTFLAG_Msk));
	}
}

