#include "pir.h"

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"

bool exist;

void PIR_RCC_Init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // PIR Sensor Pin Enable
}

void PIR_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    // GPIOA Configuration
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void PIR_EXTI_Init(void) {
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
    EXTI_InitStructure.EXTI_Line = EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void PIR_NVIC_Init(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    // PIR Sensor
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void PIR_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line6) != RESET || 
        EXTI_GetITStatus(EXTI_Line8) != RESET) {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == Bit_SET ||
            GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == Bit_SET) {
            exist = true;
        } else {
            exist = false;
        }
        EXTI_ClearITPendingBit(EXTI_Line6);
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
}

void PIR_Delay(void) {
    for (int i = 0; i < 1000000; i++)
        ;
}

void PIR_Init(void) {
    PIR_RCC_Init();
    PIR_ADC_Init();
    PIR_GPIO_Init();
    PIR_EXIT_Init();
    PIR_NVIC_Init();
}

bool PIR_Get_Exist(void){
    return exist;
}