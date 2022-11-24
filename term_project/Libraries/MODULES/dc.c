#include "stm32f10x.h"

void DC_RCC_Init(void){

}

void DC_GPIO_Init(void){

}

void DC_PWM_Init(void){

}

void DC_Init(void){
    DC_RCC_Init();
    DC_GPIO_Init();
    DC_PWM_Init();
}

void DC_Update(uint32_t dist){

}