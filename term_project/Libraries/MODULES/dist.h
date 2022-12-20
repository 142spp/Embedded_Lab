#ifndef GP2Y
#define GP2Y

#include "stdint.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include <math.h>

void Dist_RCC_Configure(void);
void Dist_GPIO_Configure(void);
void Dist_ADC_Configure(void);
void Dist_Init_Configure(void);
void Dist_IRQHandler(void);
uint32_t Dist_Get_Distance(void);

#endif