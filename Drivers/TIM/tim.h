/*
 * tim.h
 *
 *  Created on: 26-May-2026
 *      Author: iamle
 */

#ifndef TIM_TIM_H_
#define TIM_TIM_H_

#include "stm32f4xx.h"

extern volatile uint8_t timer_flag;
void timer2_init(uint32_t period_ms);
void timer2_start(void);

#endif /* TIM_TIM_H_ */
