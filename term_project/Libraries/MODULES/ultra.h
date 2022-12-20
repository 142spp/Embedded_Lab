#ifndef __ULTRA
#define __ULTRA

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
uint32_t Ultra_Measure_Distance(void);

#endif