#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"

void Servo_Pwm_Init(void);
void Servo_RCC_Init(void);
void Servo_GPIO_Init(void);
void Servo_Init(void);
void Servo_Turn(uint16_t phi);

#endif