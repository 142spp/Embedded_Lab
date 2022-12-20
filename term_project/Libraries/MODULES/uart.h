#ifndef __UART_H
#define __UART_H

void UART_RCC_Init(void);
void UART_GPIO_Init(void);
void UART_USART1_Init(void);
void UART_NVIC_Init(void);
void UART_IRQHandler(void);
void UART_Init(void);

#define STATE_OFF    ((uint16_t)0x1000)
#define STATE_ON     ((uint16_t)0x2000)
#define STATE_MAN    ((uint16_t)0x4000)
#define STATE_AUTO   ((uint16_t)0x8000)
#define STATE_LEFT   ((uint16_t)0x0100)
#define STATE_RIGHT  ((uint16_t)0x0200)

#endif