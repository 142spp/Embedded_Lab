#ifndef __DC_H
#define __DC_H

#include "stm32f10x.h"

void DC_GPIO_Init(void);
void DC_RCC_Init(void);
void DC_PWM_Init(void);
void DC_Init(void);
void DC_Update(uint32_t);

#endif