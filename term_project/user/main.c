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
#include "module_servo.h"
#include "module_usound.h"
#include "module_dist.h"
#include "module_pir.h"
#include "module_.h"

enum { OFF = 0, ON = 1 } ; 
enum { LEFT = 0, MID = 1, RIGHT = 2 } ;

/* function prototype */
void Delay(void);
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
}

/**
 * @brief Delay around 0.1 * t sec
 */
void Delay(uint16_t t) {
    for (uint16_t j = 0; j < t; j++)
        for (uint16_t i = 0; i < 1000000; i++) ;
}

int main(void) {
    
    Init_Configure();

    uint16_t state = OFF;

    uint16_t position = MID;
    uint16_t angle;
    uint16_t pre_angle;

    uint32_t distance;

    while(1){
        // use PIR Sensor
        if(state == OFF || Get_Exist() == false ) {
            Delay(2);
            continue;
        }
        // use Ultrasound Sensor
        position = Get_Position();
        angle = Get_Angle(position);

        if(pre_angle != angle){
            // use Servo Motor
            Servo_Turn(angle);
            Delay(20);
        }
        // use Distance Sensor
        distance = Get_Distance();
        // use DC Motor
        DC_Update(distance);

        Delay(2);
        pre_angle = angle;
    }
    return 0;
}