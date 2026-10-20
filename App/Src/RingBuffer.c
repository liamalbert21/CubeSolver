#include "RingBuffer.h"

#include <string.h>

#define RING_BUFFER_MASK (MAX_RING_BUFFER_SIZE - 1)

bool RingBuffer_fill(RingBuffer *rb, const char *content)
{
    int length = strlen(content);

    if (MAX_RING_BUFFER_SIZE - 1 - RingBuffer_size(rb) < length)
    {
        return false;
    }

    for (int i = 0; i < length; ++i)
    {
        RingBuffer_push(rb, content[i]);
    }

    return true;
}

bool RingBuffer_push(RingBuffer *rb, uint8_t element)
{
    if (RingBuffer_isFull(rb))
    {
        return false;
    }

    rb->content[rb->head] = element;
    rb->head = (rb->head + 1) & RING_BUFFER_MASK;
    return true;
}

bool RingBuffer_pop(RingBuffer *rb)
{   
    if (RingBuffer_isEmpty(rb))
    {
        return false;
    }

    rb->tail = (rb->tail + 1) & RING_BUFFER_MASK;
    return true;
}

uint8_t RingBuffer_peek(RingBuffer *rb)
{
    return rb->content[rb->tail];
}

bool RingBuffer_isEmpty(RingBuffer *rb)
{
    return rb->head == rb->tail;
}

bool RingBuffer_isFull(RingBuffer *rb)
{
    return ((rb->head + 1) & RING_BUFFER_MASK) == rb->tail;
}

size_t RingBuffer_size(RingBuffer *rb)
{
    const int distance = abs(rb->head - rb->tail);
    return (rb->head >= rb->tail) ? distance : MAX_RING_BUFFER_SIZE - distance;
}