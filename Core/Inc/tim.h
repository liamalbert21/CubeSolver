/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define ONE_MILLION 1000000
#define CLOCK_FREQ  16 * ONE_MILLION
#define MS_FACTOR   1000

#define TIM3_BASE_FREQ      ONE_MILLION
#define TIM3_MAX_LOAD_VALUE 65535
#define TIM3_PRESCALER      (CLOCK_FREQ / TIM3_BASE_FREQ)

#define TIM4_BASE_FREQ      TIM3_BASE_FREQ 
#define TIM4_MAX_LOAD_VALUE 65535
#define TIM4_PRESCALER      TIM3_PRESCALER
/* USER CODE END Private defines */

void MX_TIM3_Init(void);
void MX_TIM4_Init(void);

/* USER CODE BEGIN Prototypes */
void TIM_generatePWM(TIM_TypeDef *timer, int load_value);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

