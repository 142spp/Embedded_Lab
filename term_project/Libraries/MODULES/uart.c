#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "uart.h"

UARTState ustate;

void UART_RCC_Init(void){
    /* UART TX/RX port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /* USART2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    /* Alternate Function IO clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void UART_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    /* UART1 pin setting */
    // TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void UART_USART2_Init(void) {
    USART_InitTypeDef USART2_InitStructure;

    USART_Cmd(USART2, ENABLE);
    USART2_InitStructure.USART_BaudRate = 9600;
    USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART2_InitStructure.USART_StopBits = USART_StopBits_1;
    USART2_InitStructure.USART_Parity = USART_Parity_No;
    USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART2_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void UART_NVIC_Init(void){
    NVIC_InitTypeDef NVIC_InitStructure;
    // 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // TODO
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void UART_UpdateState(uint16_t data){
         if(data == '0') ustate.power = OFF;
    else if(data == '1') ustate.power = ON;
    else if(data == 'a') ustate.mode = AUTO;
    else if(data == 'm') ustate.mode = MAN;
    else if(data == 'l') ustate.position = LEFT;
    else if(data == 'i') ustate.position = MID;
    else if(data == 'r') ustate.position = RIGHT;
    else if(data == 's') ustate.strength = STRONG;
    else if(data == 'w') ustate.strength = WEAK;
    else return;
}

void UART_IRQHandler(void) {
    uint16_t data;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        // the most recent received data by the USART1 peripheral
        data = USART_ReceiveData(USART2);
        UART_UpdateState(data);
        // clear 'Read data register not empty' flag
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

void UART_Init(void){
    UART_RCC_Init();
    UART_GPIO_Init();
    UART_USART2_Init();
    UART_NVIC_Init();
}

UARTState UART_GetState(void){
    UARTState copyState = ustate;
    ustate.strength = KEEP;
    return copyState;
}

