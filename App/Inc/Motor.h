#ifndef MOTOR_H
#define MOTOR_H

#include "MotorDefs.h"
#include "Pinout.h"

// Primary motor structure
typedef struct Motor
{
    MotorConfig config;
    Pinout enable_pinout;
} Motor;

// External FSM definition (used for callback function)
typedef enum FSM FSM;

#define DEGREES_FULL_ROTATION    360
#define DEGREES_HALF_ROTATION    (DEGREES_FULL_ROTATION / 2)
#define DEGREES_QUARTER_ROTATION (DEGREES_HALF_ROTATION / 2)

// Each PWM channel (timer) controls a set number of motors (3)
#define MOTOR_COUNT_PER_PWM    MOTOR_NUM_SIDES / 2 

// Maximum number of motors that can run in parallel at once
#define MAX_MOTORS_IN_PARALLEL 2

// Enable the motor
#define Motor_disable(motor) \
    LL_GPIO_SetOutputPin( \
        (motor)->enable_pinout.port, \
        (motor)->enable_pinout.pin_mask \
    );

// Disable the motor
#define Motor_enable(motor) \
    LL_GPIO_ResetOutputPin( \
        (motor)->enable_pinout.port, \
        (motor)->enable_pinout.pin_mask \
    );

// Initializes a single motor
void Motor_init(Motor *motor, const MotorConfig *config);

// Initializes motor pins
void Motor_initPins(Motor motors[]);

// Sets the direction of a motor
void Motor_setDirection(Motor *motor, MotorDir dir);

// Rotates a single motor
void Motor_rotate(Motor *motor, MotorPWMConfig pwm_config);

// Sets the rotation callback function
void Motor_setCallback(FSM state);

// Converts a symbol--typically received over UART--to a side
MotorSide Motor_symbolToSide(char symbol);

// Converts a symbol--typically received over UART--to a dir
MotorDir Motor_symbolToDir(char symbol);

#endif