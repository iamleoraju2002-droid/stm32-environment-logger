#include "systick.h"
void systick_init(void)
{
	SysTick->LOAD = 84000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}
