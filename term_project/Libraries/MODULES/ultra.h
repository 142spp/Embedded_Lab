#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

// Trigger pin : PA7
// Echo pin(TIM3 Ch1) : PA6

void Ultra_RCC_Configure(void);
void Ultra_GPIO_Configure(void);
void Ultra_TIM_Configure(void);
void Ultra_NVIC_Configure(void);
void Ultra_Init(void);
void Ultra_TIM3_IRQHandler(void);
void Ultra_Trigger(void);

void Ultra_Measure_Distance(void);
int Ultra_Get_Distance(void);