#ifndef __UART_H
#define __UART_H

void UART_RCC_Init(void);
void UART_GPIO_Init(void);
void UART_USART1_Init(void);
void UART_NVIC_Init(void);
void USART1_IRQHandler(void);
void UART_Init(void);

#endif