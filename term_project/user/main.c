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
#include "dcmotor.h"
#include "dist.h"
#include "pir.h"
#include "servo.h"
#include "uart.h"
#include "ultra.h"
#include "handler.h"

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
    // RCC_APB1PeriphClockCmd();
    // RCC_APB2PeriphClockCmd();
    // RCC_AHBPeriphClockCmd();
}

/**
 * @brief Enable GPIO Pins using GPIO_InitTypeDef
 */
void GPIO_Configure(void) {
    // GPIO_InitTypeDef GPIO_InitStruct;
}

/**
 * @brief ADC Configure using ADC_InitTypeDef
 */
void ADC_Configure(void) {
    // ADC_InitTypeDef ADC_InitStruct;
}
/**
 * @brief TIM Configure using TIM_TimeBaseInitTypeDef, TIM_ICTypedef
 */
void TIM_Configure(void) {
    // TIM_TimeBaseInitTypeDef TIM_InitStruct;
}
/**
 * @brief EXTI Configure using EXTI_InitTypeDef
 */
void EXTI_Configure(void) {
    // EXTI_InitTypeDef EXTI_InitStruct;
}
/**
 * @brief NVIC Configure using NVIC_InitTypeDef
 */
void NVIC_Configure(void) {    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // NVIC_InitTypeDef NVIC_InitStruct;
}
/**
 * @brief DMA Configure using DMA_InitTypeDef
 */
void DMA_Configure(void) {
    // DMA_InitTypeDef DMA_InitStruct;
}

/**
 * @brief excute All configuration function
 */
void Init_Configure(void) {
    // Init System
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    ADC_Configure();
    TIM_Configure();
    EXTI_Configure();
    NVIC_Configure();
    DMA_Configure();

    // Init Modules
    DC_Motor_Init();
    Dist_Init();
    PIR_Init();
    Servo_Init();
    UART_Init();
    Ultra_Init();
}

/**
 * @brief Delay around 0.1 * t sec
 */
void Delay(uint16_t t) {
    for (uint32_t j = 0; j < t; j++)
        for (uint32_t i = 0; i < 1000000; i++)
            ;
}

int main(void) {
    Init_Configure();

    UARTState state;

    uint8_t exist = 0;
    uint16_t position = MID;
    uint16_t pre_pos = position;

    uint32_t distance = 100;

    while (1) {
        // UART use PA2, PA3, 5v, gnd
        state = UART_GetState();
        if (state.power == OFF) {
            Delay(2);
            continue;
        }

        if (state.mode == MAN) {  // Manual
            if (pre_pos != state.position) {
                position = state.position;  // position update
            }
            // DC motor use PB6, 3.3v, gnd, battery
            DC_Motor_UpdateMAN(state.strength);  // strength update
        }

        else if (state.mode == AUTO) {
            // PIR use PC6, PC8, 5V*2, GND*2
            exist = PIR_Get_Exist();
            if (exist == 0) {
                Delay(2);
                continue;
            }

            uint8_t l_exist = exist / 10;
            uint8_t r_exist = exist % 10;

            // Ultrasound use A6, A7
            distance = Ultra_Measure_Distance();

            if (l_exist == r_exist) {
                position = MID;
                // Dist use PB0
                distance = Dist_Get_Distance();
            } else if (l_exist) {
                position = LEFT;
                distance = distance / 100;
            } else {
                position = RIGHT;
                distance = distance % 100;
            }

            if (pre_pos != position) {
                // Servo use PB1
                Servo_Turn(position);
                Delay(10);
            }

            DC_Motor_UpdateAUTO(distance);

            state.position = position;
        }
        pre_pos = position;
        Delay(1);
    }
    return 0;
}
