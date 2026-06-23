/*
 * ringbuffer.h
 *
 *  Created on: 02-Jun-2026
 *      Author: iamle
 */

#ifndef RINGBUFFER_RINGBUFFER_H_
#define RINGBUFFER_RINGBUFFER_H_

#include "stm32f4xx.h"

#define RING_BUFFER_SIZE 64

typedef struct
{
	volatile uint8_t buffer[RING_BUFFER_SIZE];
	volatile uint16_t head;
	volatile uint16_t tail;
}ring_buffer_t;

void ring_buffer_init(ring_buffer_t *rb);
uint8_t ring_buffer_put(ring_buffer_t *rb, uint8_t data);
uint8_t ring_buffer_get(ring_buffer_t *rb, uint8_t *data);
uint8_t ring_buffer_available(ring_buffer_t *rb);

#endif /* RINGBUFFER_RINGBUFFER_H_ */
