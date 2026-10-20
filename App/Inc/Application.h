#ifndef APPLICATION_H
#define APPLICATION_H

#include "FSM.h"
#include "HAL.h"

typedef struct
{
    FSM state;
    HAL hal;
} Application;

Application Application_init();

#endif