#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_RING_BUFFER_SIZE 16

typedef struct RingBuffer
{
    uint8_t content[MAX_RING_BUFFER_SIZE];
    volatile int head;
    volatile int tail;
} RingBuffer;

#define RingBuffer_init() \
    ((RingBuffer){ {}, 0, 0 })

bool RingBuffer_fill(RingBuffer *rb, const char *content);

bool RingBuffer_push(RingBuffer *rb, uint8_t element);

bool RingBuffer_pop(RingBuffer *rb);

uint8_t RingBuffer_peek(RingBuffer *rb);

bool RingBuffer_isEmpty(RingBuffer *rb);

bool RingBuffer_isFull(RingBuffer *rb);

size_t RingBuffer_size(RingBuffer *rb);

#endif