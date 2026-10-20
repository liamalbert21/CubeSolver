#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "Motor.h"

typedef struct RingBuffer RingBuffer;

#define SINGLE_COMMAND_LENGTH 3
#define PARALLEL_COMMAND_LENGTH 5

void Instruction_interpret(RingBuffer *rb, Motor motors[]);

#endif