#include "dcmotor.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

void DC_Motor_PWM_Configure(void) {
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1;   // clock�� ���ļ��� 100Hz.
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //TIM_Output_Compare_ and _PWM_modes
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = (uint16_t)CCR1_Val4; // 1000 -> 100%
  
  TIM_OC1Init(TIM4, &TIM_OCInitStructure); // PWM1 Mode configuration: Channel 1
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
  
  TIM_ARRPreloadConfig(TIM4, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
}
void DC_Motor_GPIO_Configure(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  /*GPIOA Configuration: TIM4 channel 1 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}
void DC_Motor_RCC_configure(void) {
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // Tim4 Clock enable.
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void DC_Motor_Speed(float distance) {
   if(distance < 50.0f)
     TIM4->CCR1 = CCR3_Val4;
   else if(distance < 100.0f)
     TIM4->CCR1 = CCR2_Val4;
   else if(distance < 150.0f)
     TIM4->CCR1 = CCR1_Val4;
   
}