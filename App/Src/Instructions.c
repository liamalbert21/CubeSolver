#include "Instructions.h"

#include "RingBuffer.h"
#include "Symbols.h"

#include "tim.h"

#define NUM_INSTRS_PER_MOTOR   2
#define MS_QUARTER_ROTATION    1000

void Instruction_interpret(RingBuffer *rb, Motor motors[])
{
    const char type = RingBuffer_peek(rb);
    RingBuffer_pop(rb);
    
    const int count = (type == SYMBOL_INSTR_SINGULAR) ?
        1 : MAX_MOTORS_IN_PARALLEL;

    for (int i = 0; i < count; ++i)
    {
        const MotorSide side = Motor_symbolToSide(RingBuffer_peek(rb));
        RingBuffer_pop(rb);

        Motor *motor = &motors[side];

        const char sym_dir = RingBuffer_peek(rb);
        RingBuffer_pop(rb);

        Motor_setDirection(motor, Motor_symbolToDir(sym_dir));

        // Symbol carries more infomration than the direction, so it's used instead
        MotorPWMConfig pwm_config = (sym_dir == SYMBOL_DIR_HALF_ROTATION) ?
            (MotorPWMConfig){ DEGREES_HALF_ROTATION,    MS_QUARTER_ROTATION / 2 } :
            (MotorPWMConfig){ DEGREES_QUARTER_ROTATION, MS_QUARTER_ROTATION / 4 };
        
        Motor_rotate(motor, pwm_config);
    }

    extern bool action_pending;
    action_pending = false;
}