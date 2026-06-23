#include "ringbuffer.h"

void ring_buffer_init(ring_buffer_t *rb)
{
	rb->head = 0;
	rb->tail = 0;
}
uint8_t ring_buffer_put(ring_buffer_t *rb, uint8_t data)
{
	uint16_t next_head = (rb->head + 1) % RING_BUFFER_SIZE;
	if(next_head == rb->tail)
	{
		return 0;
	}
	rb->buffer[rb->head] = data;
	rb->head = next_head;
	return 1;
}
uint8_t ring_buffer_get(ring_buffer_t *rb, uint8_t *data)
{
	if (rb->head == rb->tail)
	{
		return 0;
	}
	*data = rb->buffer[rb->tail];
	rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
	return 1;
}
uint8_t ring_buffer_available(ring_buffer_t *rb)
{
	return (rb->head != rb->tail);
}
