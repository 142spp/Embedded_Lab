#ifndef GP2Y
#define GP2Y

#include "stdint.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include <math.h>

void GP2Y_Delay(void);
void GP2Y_RCC_Configure(void);
void GP2Y_GPIO_Configure(void);
void GP2Y_ADC_Configure(void);
void GP2Y_Init_Configure(void);
float GP2Y_Get_Distance(void);

#endif