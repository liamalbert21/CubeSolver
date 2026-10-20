#ifndef MOTOR_H
#define MOTOR_H

#include "MotorDefs.h"
#include "Pinout.h"

typedef struct
{
    MotorConfig config;
    MotorStatus status;
    Pinout sleep_pinout;
} Motor;

enum FSM;

#define DEGREES_FULL_ROTATION    360
#define DEGREES_HALF_ROTATION    (DEGREES_FULL_ROTATION / 2)
#define DEGREES_QUARTER_ROTATION (DEGREES_HALF_ROTATION / 2)

#define MOTOR_COUNT_PER_PWM MOTOR_NUM_SIDES / 2

#define Motor_awake(motor) \
    LL_GPIO_SetOutputPin( \
        (motor)->sleep_pinout.port, \
        (motor)->sleep_pinout.pin_mask \
    );

#define Motor_sleep(motor) \
    LL_GPIO_ResetOutputPin( \
        (motor)->sleep_pinout.port, \
        (motor)->sleep_pinout.pin_mask \
    );

// Initializes a single motor
void Motor_init(Motor *motor, const MotorConfig *config);

// Initializes motor pins
void Motor_initPins(Motor motors[]);

// Sets the direction of a motor
void Motor_setDirection(Motor *motor, MotorDir dir);

// Rotates a single motor
void Motor_rotate(Motor *motor, MotorPWMConfig pwm_config);

// Enqueues a motor to rotate in a pair
void Motor_enqueue(Motor *motor);

// Sets the rotation callback function
void Motor_setCallback(enum FSM state);

// Rotates a pair of motors
void Motor_rotatePair(MotorPWMConfig pwm1_config, MotorPWMConfig pwm2_config);

#endif