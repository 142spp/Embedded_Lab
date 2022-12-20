#ifndef __UART_H
#define __UART_H

typedef enum {OFF, ON} Power;
typedef enum {MAN, AUTO} Mode;
typedef enum {LEFT = 30, MID = 90, RIGHT = 150} Position;
typedef enum {KEEP, STRONG, WEAK} Strength;

typedef struct {
    Power power;
    Mode mode;
    Position position;
    Strength strength;
} UARTState;

void UART_RCC_Init(void);
void UART_GPIO_Init(void);
void UART_USART2_Init(void);
void UART_NVIC_Init(void);
void UART_IRQHandler(void);
void UART_Init(void);
UARTState UART_GetState(void);

#endif