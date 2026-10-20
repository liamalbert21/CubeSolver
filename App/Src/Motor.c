#include "Motor.h"
#include "tim.h"

#include "FSM.h"
#include "Symbols.h"

#include <limits.h>

// The step factor--configure this here and on the stepper drivers
#define MOTOR_STEP_FACTOR        4
#define MOTOR_BASE_STEPS_PER_REV (MOTOR_STEP_FACTOR * 200)

// "Returns" the PWM channel that controls a motor
#define Motor_getPWMChannel(side) \
    (side < MOTOR_COUNT_PER_PWM ? MOTOR_CHANNEL_PWM1 : MOTOR_CHANNEL_PWM2)

// This is not clean nor scalable. I have to hardcode the individual step counts as
// variables, yet I give the motor queue a nominal size.
int pwm1_target_step_count = INT_MAX;
int pwm2_target_step_count = INT_MAX;
static Motor *motor_queue[MAX_MOTORS_IN_PARALLEL];

// This is a bad location. As I expand the program, I will move it elsewhere
bool action_pending = false;

// Returns the pinout configuration that corresponds to a motor
static Pinout Motor_getSleepPinout(MotorSide side);

// Readies a motor for rotation
static int Motor_ready(Motor *motor, MotorPWMConfig pwm_config);

// Deactivates a motor
static void Motor_deactivate(MotorPWMChannel ch);

// Callback for when a motor completes its rotation
// Change this as you expand the program
void (*Motor_callback)(MotorPWMChannel) = Motor_deactivate;

void Motor_init(Motor *motor, const MotorConfig *config)
{
    motor->config = *config;
    motor->sleep_pinout = Motor_getSleepPinout(config->side);
}

void Motor_initPins(Motor motors[])
{
    if (motors == NULL)
    {
        return;
    }
    
    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;

    for (int i = 0; i < MOTOR_NUM_SIDES; ++i)
    {
        GPIO_InitStruct.Pin = motors[i].sleep_pinout.pin_mask;
        LL_GPIO_Init(motors[i].sleep_pinout.port, &GPIO_InitStruct);
    }

    GPIO_InitStruct.Pin = MOTOR_PWM1_DIR_PIN;
    LL_GPIO_Init(MOTOR_PWM1_DIR_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MOTOR_PWM2_DIR_PIN;
    LL_GPIO_Init(MOTOR_PWM2_DIR_PORT, &GPIO_InitStruct);
}

void Motor_setDirection(Motor *motor, MotorDir dir)
{
    motor->config.dir = dir;
    Pinout dir_pinout = Motor_getPWMChannel(motor->config.side) == MOTOR_CHANNEL_PWM1 ? 
        (Pinout){ MOTOR_PWM1_DIR_PORT, MOTOR_PWM1_DIR_PIN } :
        (Pinout){ MOTOR_PWM2_DIR_PORT, MOTOR_PWM2_DIR_PIN };

    switch (dir)
    {
        case MOTOR_DIR_CCW:
            LL_GPIO_ResetOutputPin(
                dir_pinout.port,
                dir_pinout.pin_mask
            );
            break;
        
        // Accounts for clockwise turns & half rotations
        default:
            LL_GPIO_SetOutputPin(
                dir_pinout.port,
                dir_pinout.pin_mask
            );
    }
}

void Motor_rotate(Motor *motor, MotorPWMConfig pwm_config)
{
    int load_value = Motor_ready(motor, pwm_config);
    TIM_TypeDef *timer =
        (Motor_getPWMChannel(motor->config.side) == MOTOR_CHANNEL_PWM1) ?
            TIM3 : TIM4;
    TIM_generatePWM(timer, load_value);
}

int Motor_ready(Motor *motor, MotorPWMConfig pwm_config)
{
    int tsc = MOTOR_BASE_STEPS_PER_REV * pwm_config.degrees / DEGREES_FULL_ROTATION;
    uint32_t load_value =
        ((CLOCK_FREQ / MS_FACTOR * pwm_config.time_ms) / tsc) / (TIM3_PRESCALER + 1) - 1;

    MotorPWMChannel channel = Motor_getPWMChannel(motor->config.side);

    if (channel == MOTOR_CHANNEL_PWM1)
    {
        pwm1_target_step_count = tsc;
    }
    else
    {
        pwm2_target_step_count = tsc;
    }

    motor_queue[channel] = motor;
    Motor_setDirection(motor, motor->config.dir);
    Motor_awake(motor);

    return load_value;
}

void Motor_setCallback(FSM state)
{
    return;
}

void Motor_deactivate(MotorPWMChannel ch)
{
    // Not addressasble, so it must be wrapped here
    Motor_sleep(motor_queue[ch]);
    
    // Send "complete" indication to interface
}

MotorSide Motor_symbolToSide(char symbol)
{
    switch (symbol)
    {
        case SYMBOL_MOTOR_SIDE_RED:
            return MOTOR_SIDE_RED;
        case SYMBOL_MOTOR_SIDE_ORANGE:
            return MOTOR_SIDE_ORANGE;
        case SYMBOL_MOTOR_SIDE_YELLOW:
            return MOTOR_SIDE_YELLOW;
        case SYMBOL_MOTOR_SIDE_GREEN:
            return MOTOR_SIDE_GREEN;
        case SYMBOL_MOTOR_SIDE_BLUE:
            return MOTOR_SIDE_BLUE;
        case SYMBOL_MOTOR_SIDE_WHITE:
            return MOTOR_SIDE_WHITE;
        default:
            return MOTOR_NUM_SIDES;
    }
}

MotorDir Motor_symbolToDir(char symbol)
{
    switch (symbol)
    {
        case SYMBOL_DIR_COUNTERCLOCKWISE:
            return MOTOR_DIR_CCW;
        default:
            return MOTOR_DIR_CW;
    }
}

Pinout Motor_getSleepPinout(MotorSide side)
{   
    switch (side)
    {
        case MOTOR_SIDE_RED:
            return (Pinout){
                RED_SLEEP_PINOUT
            };
        case MOTOR_SIDE_ORANGE:
            return (Pinout){
                ORANGE_SLEEP_PINOUT
            };
        case MOTOR_SIDE_YELLOW:
            return (Pinout){
                YELLOW_SLEEP_PINOUT
            };
        case MOTOR_SIDE_GREEN:
            return (Pinout){
                GREEN_SLEEP_PINOUT
            };
        case MOTOR_SIDE_BLUE:
            return (Pinout){
                BLUE_SLEEP_PINOUT
            };
        case MOTOR_SIDE_WHITE:
            return (Pinout){
                WHITE_SLEEP_PINOUT
            };
        default:
            return (Pinout){};
    }
}