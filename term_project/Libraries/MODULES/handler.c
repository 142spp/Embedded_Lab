#include "handler.h"
#include "stm32f10x.h"
#include "dist.h"
#include "ultra.h"
#include "pir.h"
#include "uart.h"

void ADC1_2_IRQHandler(void) {
    Dist_IRQHandler();
}

void EXTI9_5_IRQHandler(void) {
    PIR_IRQHandler();
}

void USART2_IRQHandler(void) {
    UART_IRQHandler();
}

void TIM3_IRQHandler(void) {
    Ultra_TIM3_IRQHandler();
}
