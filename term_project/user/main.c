// Standard
#include <stdio.h>
#include <stdlib.h>

// System
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

// Modules 
#include "servo.h"
#include "uart.h"
#include "ultra.h"
#include "GP2Y.h"
#include "pir.h"
#include "dcmotor.h"
#include "tim.h"
#include "pwm.h"

enum { OFF = 0, ON = 1 } ; 
enum { LEFT = 0, MID = 1, RIGHT = 2 } ;

/* function prototype */
void Delay(uint16_t);
void RCC_Configure(void);
void GPIO_Configure(void);
void ADC_Configure(void);
void DMA_Configure(void);
void TIM_Configure(void);
void EXTI_Configure(void);
void NVIC_Configure(void);
void Init_Configure(void);

/**
 * @brief Enable or Disable Clocks Using
 *      RCC_APB1PeriphClockCmd , RCC_APB2PeriphClockCmd, RCC_AHBPeriphClockCmd
 *      함수의 definition의 주석을 참고해서 원하는 모듈에 clock을 인가한다.
 */
void RCC_Configure(void) {
    //RCC_APB1PeriphClockCmd(); 
    //RCC_APB2PeriphClockCmd(); 
    //RCC_AHBPeriphClockCmd();
}


/**
 * @brief Enable GPIO Pins using GPIO_InitTypeDef
 */
void GPIO_Configure(void) {
    //GPIO_InitTypeDef GPIO_InitStruct;
}

/**
 * @brief ADC Configure using ADC_InitTypeDef
 */
void ADC_Configure(void) {
    //ADC_InitTypeDef ADC_InitStruct;
}

/**
 * @brief DMA Configure using DMA_InitTypeDef
 */
void DMA_Configure(void) {
    //DMA_InitTypeDef DMA_InitStruct;
}

/**
 * @brief excute All configuration function
 */
void Init_Configure(void) {
    //Init System
    SystemInit();
    //RCC_Configure();
    //GPIO_Configure();
    //ADC_Configure();
    //TIM_Configure();
    //EXTI_Configure();
    //NVIC_Configure();
    //DMA_Configure();

    //Init Modules
    Servo_Init();
    //PIR_Init();
    Ultra_Init();
    //Dist_Init();
    //DC_Init();
    //Gesture_Init();
}

/**
 * @brief Delay around 0.1 * t sec
 */
void Delay(uint16_t t) {
    for (uint32_t j = 0; j < t; j++)
        for (uint32_t i = 0; i < 1000000; i++) ;
}

int main(void) {

    Init_Configure();

    uint16_t state = STATE_OFF;

    uint16_t position = MID;
    uint16_t angle;
    uint16_t pre_angle;

    uint32_t distance;
    uint16_t interval = 50;
    uint32_t speed = 0;
    
    while(1){
        state = UART_GetState();
        // use PIR Sensor
        if(state && STATE_OFF || PIR_Get_Exist() == false ) {
            Delay(2);
            continue;
        }
        // use Ultrasound Sensor
        angle = Usound_Get_Angle();

        if(pre_angle != angle){
            // use Servo Motor
            Servo_Turn(angle);
            Delay(20);
        }
        // use Distance Sensor
        distance = Dist_Get_Distance();
        // use DC Motor
        DC_Update(distance);

        pre_angle = angle;
        Delay(2);
    }
    return 0;
}