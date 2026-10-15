#include "RingBuffer.h"

#include <string.h>

#define RING_BUFFER_MASK MAX_RING_BUFFER_SIZE - 1

bool RingBuffer_fill(RingBuffer *rb, int *start, const char *content)
{
    int length = strlen(content);
    bool is_spacious = !RingBuffer_isFull(rb);

    while (*start < length && is_spacious)
    {
        is_spacious = RingBuffer_push(rb, content[*start++]);
    }
    return *start == length;
}

bool RingBuffer_push(RingBuffer *rb, uint8_t element)
{
    if (RingBuffer_isFull(rb))
    {
        return false;
    }

    rb->content[rb->head++ & RING_BUFFER_MASK] = element;
    return true;
}

bool RingBuffer_pop(RingBuffer *rb)
{   
    if (RingBuffer_isEmpty(rb))
    {
        return false;
    }

    rb->tail++;
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


