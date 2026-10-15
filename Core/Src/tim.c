#include "tim.h"

static void TIM3_initPins();
static void TIM4_initPins();

void MX_TIM3_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = { 0 };
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = { 0 };

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  NVIC_SetPriority(TIM3_IRQn,
    NVIC_EncodePriority(
      NVIC_GetPriorityGrouping(),
      0,
      0
    )
  );
  NVIC_EnableIRQ(TIM3_IRQn);

  TIM_InitStruct.Prescaler = TIM3_PRESCALER;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = TIM3_MAX_LOAD_VALUE;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;

  LL_TIM_Init(TIM3, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM3);
  LL_TIM_SetClockSource(TIM3, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH1);

  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = TIM3_MAX_LOAD_VALUE / 2;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_LOW;

  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM3, LL_TIM_CHANNEL_CH1);
  LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM3);

  LL_TIM_EnableIT_UPDATE(TIM3);

  TIM3_initPins();
}

void TIM3_initPins()
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);  

  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;

  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* TIM4 init function */
void MX_TIM4_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = { 0 };
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = { 0 };
  
  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

  /* TIM4 interrupt Init */
  NVIC_SetPriority(
    TIM4_IRQn,
    NVIC_EncodePriority(
      NVIC_GetPriorityGrouping(),
      0,
      0
    )
  );
  NVIC_EnableIRQ(TIM4_IRQn);

  TIM_InitStruct.Prescaler = TIM4_PRESCALER;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = TIM4_MAX_LOAD_VALUE;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;

  LL_TIM_Init(TIM4, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM4);
  LL_TIM_SetClockSource(TIM4, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_OC_EnablePreload(TIM4, LL_TIM_CHANNEL_CH1);

  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = TIM4_MAX_LOAD_VALUE / 2;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_LOW;

  LL_TIM_OC_Init(TIM4, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM4, LL_TIM_CHANNEL_CH1);
  LL_TIM_SetTriggerOutput(TIM4, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM4);
  
  LL_TIM_EnableIT_UPDATE(TIM4);

  TIM4_initPins();
}

void TIM4_initPins()
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;

  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 1 */
void TIM_generatePWM(TIM_TypeDef *timer, int load_value)
{
  LL_TIM_SetAutoReload(timer, load_value);
  LL_TIM_OC_SetCompareCH1(timer, load_value / 2);
  LL_TIM_EnableCounter(timer);
}
/* USER CODE END 1 */

