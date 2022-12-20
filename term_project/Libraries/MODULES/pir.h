#ifndef __PIR
#define __PIR

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void PIR_RCC_Init(void);
void PIR_GPIO_Init(void);
void PIR_EXTI_Init(void);
void PIR_IRQHandler(void);
void PIR_NVIC_Init(void);
void PIR_Init(void);
uint8_t PIR_Get_Exist(void);

#endif