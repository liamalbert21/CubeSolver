#include "Motor.h"
#include "tim.h"

#include "FSM.h"

#include <limits.h>

#define MOTOR_STEP_FACTOR        4
#define MOTOR_BASE_STEPS_PER_REV (MOTOR_STEP_FACTOR * 200)

int pwm1_target_step_count = INT_MAX;
static Motor *motor_pwm1 = NULL;

int pwm2_target_step_count = INT_MAX;
static Motor *motor_pwm2 = NULL;

static MotorPinout Motor_getPinout(MotorSide side);
static int Motor_ready(Motor *motor, MotorPWMConfig pwm_config);
static void Motor_deactivate(MotorPWMChannel ch);

// Callback for when a motor completes its rotation
// Change this as you expand the program
void (*Motor_callback)(MotorPWMChannel) = Motor_deactivate;

void Motor_init(Motor *motor, const MotorConfig *config)
{
    motor->config = *config;
    motor->pinout = Motor_getPinout(config->side);
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
        GPIO_InitStruct.Pin = motors[i].pinout.sleep.pin_mask;
        LL_GPIO_Init(motors[i].pinout.sleep.port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = motors[i].pinout.dir.pin_mask;
        LL_GPIO_Init(motors[i].pinout.dir.port, &GPIO_InitStruct);
    }
}

void Motor_setDirection(Motor *motor, MotorDir dir)
{
    motor->config.dir = dir;
    switch (dir)
    {
        case MOTOR_DIR_CCW:
            LL_GPIO_ResetOutputPin(
                motor->pinout.dir.port,
                motor->pinout.dir.pin_mask
            );
            break;
        
        // Accounts for clockwise turns & half rotations
        default:
            LL_GPIO_SetOutputPin(
                motor->pinout.dir.port,
                motor->pinout.dir.pin_mask
            );
    }
}

MotorPinout Motor_getPinout(MotorSide side)
{   
    switch (side)
    {
        case MOTOR_SIDE_RED:
            return (MotorPinout){
                { RED_SLEEP_PINOUT },
                { RED_DIR_PINOUT }
            };
        case MOTOR_SIDE_ORANGE:
            return (MotorPinout){ 
                { ORANGE_SLEEP_PINOUT },
                { ORANGE_DIR_PINOUT }
            };
        case MOTOR_SIDE_YELLOW:
            return (MotorPinout){
                { YELLOW_SLEEP_PINOUT },
                { YELLOW_DIR_PINOUT }
            };
        case MOTOR_SIDE_GREEN:
            return (MotorPinout){
                { GREEN_SLEEP_PINOUT },
                { GREEN_DIR_PINOUT }
            };
        case MOTOR_SIDE_BLUE:
            return (MotorPinout){
                { BLUE_SLEEP_PINOUT },
                { BLUE_DIR_PINOUT },
            };
        case MOTOR_SIDE_WHITE:
            return (MotorPinout){
                { WHITE_SLEEP_PINOUT },
                { WHITE_DIR_PINOUT }
            };
        default:
            return (MotorPinout){};
    }
}

void Motor_enqueue(Motor *motor)
{
    if (motor->config.side < MOTOR_COUNT_PER_PWM)
    {
        motor_pwm1 = motor;
    }
    else
    {
        motor_pwm2 = motor;
    }
}

void Motor_rotate(Motor *motor, MotorPWMConfig pwm_config)
{
    int load_value = Motor_ready(motor, pwm_config);
    if (motor->config.side < MOTOR_COUNT_PER_PWM)
    {
        TIM_generatePWM(TIM3, load_value);
    }
    else
    {
        TIM_generatePWM(TIM4, load_value);
    }
}

void Motor_rotatePair(MotorPWMConfig pwm1_config, MotorPWMConfig pwm2_config)
{
    int lv_pwm1 = Motor_ready(motor_pwm1, pwm1_config);
    int lv_pwm2 = Motor_ready(motor_pwm2, pwm2_config);

    TIM_generatePWM(TIM3, lv_pwm1);
    TIM_generatePWM(TIM4, lv_pwm2);
}

int Motor_ready(Motor *motor, MotorPWMConfig pwm_config)
{
    int tsc = MOTOR_BASE_STEPS_PER_REV * pwm_config.degrees / DEGREES_FULL_ROTATION;
    uint32_t load_value =
        ((CLOCK_FREQ / MS_FACTOR * pwm_config.time_ms) / tsc) / (TIM3_PRESCALER + 1) - 1;

    if (motor->config.side < MOTOR_COUNT_PER_PWM)
    {
        pwm1_target_step_count = tsc;
    }
    else
    {
        pwm2_target_step_count = tsc;
    }

    Motor_awake(motor);
    Motor_setDirection(motor, motor->config.dir);

    return load_value;
}

void Motor_setCallback(FSM state)
{
    return;
}

void Motor_deactivate(MotorPWMChannel ch)
{
    Motor_sleep(
        ch == MOTOR_CHANNEL_PWM1 ?
            motor_pwm1 : motor_pwm2
    );
}
