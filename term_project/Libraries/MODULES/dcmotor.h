#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__
#define CCR1_Val4 1000
#define CCR2_Val4 750
#define CCR3_Val4 500
#define CCR4_Val4 250

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"

void DC_Motor_PWM_Configure(void);
void DC_Motor_GPIO_Configure(void);
void DC_Motor_RCC_configure(void);
void DC_Motor_Init(void);
void DC_Motor_Speed(uint32_t distance);

#endif