#ifndef HAL_H
#define HAL_H

#include "Motor.h"

typedef struct
{
    Motor motors[MOTOR_NUM_SIDES];
} HAL;

HAL HAL_init();

#endif