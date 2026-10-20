#include "HAL.h"

void HAL_initMotors(Motor motors[]);

HAL HAL_init()
{
    HAL hal;

    HAL_initMotors(hal.motors);

    return hal;
}

void HAL_initMotors(Motor motors[])
{
    for (int i = 0; i < MOTOR_NUM_SIDES; ++i)
    {
        MotorConfig config = { (MotorSide)i, MOTOR_DIR_CW };
        Motor_init(&motors[i], &config);
    }
    Motor_initPins(motors);
}