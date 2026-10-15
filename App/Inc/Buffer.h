#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

typedef struct
{
    const uint8_t *content;
    int size;
    volatile int pos;
} Buffer;

extern Buffer tx_buffer;

#define Buffer_init() \
    ((Buffer){ NULL, 0, 0 })

#endif