#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

uint16_t uart_state;

void UART_RCC_Init(void){
    /* UART TX/RX port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /* USART1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    /* Alternate Function IO clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void UART_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    /* UART1 pin setting */
    // TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void UART_USART1_Init(void) {
    USART_InitTypeDef USART1_InitStructure;

    USART_Cmd(USART1, ENABLE);
    USART1_InitStructure.USART_BaudRate = 9600;
    USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART1_InitStructure.USART_StopBits = USART_StopBits_1;
    USART1_InitStructure.USART_Parity = USART_Parity_No;
    USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART1_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void UART_NVIC_Init(void){
    NVIC_InitTypeDef NVIC_InitStructure;
    // UART1
    // 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         // TODO
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void UART_UpdateState(uint16_t data){
    uint16_t to_update;
         if(data == 's') to_update = STATE_OFF;
    else if(data == 'o') to_update = STATE_ON;
    else if(data == 'm') to_update = STATE_MAN;
    else if(data == 'a') to_update = STATE_AUTO;
    else if(data == 'l') to_update = STATE_LEFT;
    else if(data == 'r') to_update = STATE_RIGHT;

    uart_state |= to_update;
}

void USART1_IRQHandler(void) {
    uint16_t data
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        // the most recent received data by the USART1 peripheral
        data = USART_ReceiveData(USART1);
        UART_UpdateState(data);
        // clear 'Read data register not empty' flag
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

void UART_Init(void){
    UART_RCC_Init();
    UART_GPIO_Init();
    URAT_USART1_Init();
    UART_NVIC_Init();
}

uint16_t UART_GetState(void){
    return uart_state;
}
