#ifndef PINOUT_H
#define PINOUT_H

#include "stm32f4xx_ll_gpio.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint32_t pin_mask;
} Pinout;

#endif