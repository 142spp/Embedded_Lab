#include "stm32f10x.h"
#include "dist.h"

void ADC1_2_IRQHandler(void) {
    Dist_IRQHandler();
}

void TIM3_IRQHandler(void) {
    Ultra_TIM3_IRQHandler();
    
}