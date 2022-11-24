#include "servo.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

/**
 * @brief PWM Init Function
 *      ? ms마다 pulse 함. 
 *      TIM3 의 3번 채널 = GPIO B0 Pin 를 사용함
 */
void Servo_Pwm_Init(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // SystemCoreClock = 72MHz = 72,000,000Hz
    // 72MHz * 1/Period * 1/Prescaler = 주파수 (Hz)
    // servo 는 50Hz 를 맞추어야 함.
    
    uint16_t period = (uint16_t) 1000 - 1;
    // Hz = 72MHz * 1/20000 * 1/Prescaler = 3600 * 1/Prescaler
    // Hz = 72MHz * 1/1000 * 1/Prescaler = 720,000 * 1/Prescaler

    uint16_t prescaler = (uint16_t) 1440 -1;
    // Hz = 72MHz * 1/20000 * 1/72 = 50;
    // Hz = 72MHz * 1/1000 * 1/1440 = 50;

    TIM_TimeBaseStructure.TIM_Period = period;         
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    
    TIM_OCInitStructure.TIM_Pulse = (uint16_t) 75;
    
    // servo moter는 0.5ms에서 0도, 2.5ms 에서 180도로 회전함

    // if Period = 1000
    // 0.5/20 * 1000 = 25
    // 2.5/20 * 1000 = 125

    // if Period = 20000
    // 0.5/20 * 20000 = 500
    // 2.5/20 * 20000 = 2500

    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

void Servo_GPIO_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Servo_RCC_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void Servo_Init(void){
    Servo_RCC_Init();
    Servo_GPIO_Init();
    Servo_Pwm_Init();
}
/**
 * @brief servo motor turn phi
 * 
 * @param phi 0 < phi < 180 
 */
void Servo_Turn(uint16_t phi){
    if(phi > 180 || phi < 0) return;

    uint16_t pulse = (uint16_t) (((double) phi/180)*100 + 25);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = (uint16_t) pulse; // us
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);

    return;
}