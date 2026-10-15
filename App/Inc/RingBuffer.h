#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_RING_BUFFER_SIZE 16

typedef struct
{
    uint8_t content[MAX_RING_BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} RingBuffer;

extern RingBuffer rx_buffer;

#define RingBuffer_init() \
    ((RingBuffer){ {}, 0, 0 })

bool RingBuffer_fill(RingBuffer *rb, int *start, const char *content);

bool RingBuffer_push(RingBuffer *rb, uint8_t element);

bool RingBuffer_pop(RingBuffer *rb);

uint8_t RingBuffer_peek(RingBuffer *rb);

bool RingBuffer_isEmpty(RingBuffer *rb);

bool RingBuffer_isFull(RingBuffer *rb);

#endif