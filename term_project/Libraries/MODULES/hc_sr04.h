#ifndef HC_SR04_H_
#define HC_SR04_H_

#include "../CMSIS/DeviceSupport/stm32f10x.h"
#include "../STM32F10x_StdPeriph_Driver_v3.5/inc/stm32f10x_rcc.h"
#include "../STM32F10x_StdPeriph_Driver_v3.5/inc/stm32f10x_tim.h"

#define US_TIMER					TIM3

#define US_TRIG_PORT				GPIOB
#define US_TRIG_PIN					GPIO_Pin_0		//TIM Ch3 (trig output)

#define US_ECHO_PORT				GPIOA
#define US_ECHO_PIN					GPIO_Pin_6		//TIM Ch1 (echo input)
#define US_TIMER_TRIG_SOURCE		TIM_TS_TI1FP1

uint32_t HCSR04GetDistance(void);
void initMeasureTimer(void);

#endif