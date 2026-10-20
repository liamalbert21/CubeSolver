#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

// Eventually, populate these through a file
#define SINGLE_COMMAND_LENGTH 3
#define PARALLEL_COMMAND_LENGTH 5

typedef struct RingBuffer RingBuffer;

// Code format:
// <Count> <Face> <Dir> [<Face> <Dir>]

void Instruction_interpret(RingBuffer *rb);

#endif