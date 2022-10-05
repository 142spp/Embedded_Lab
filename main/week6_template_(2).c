
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "misc.h"

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void EXTI_Configure(void);
void USART1_Init(void);
void NVIC_Configure(void);

void EXTI15_10_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI2_IRQHandler(void);

void Delay(void);

void sendDataUART1(uint16_t data);

//---------------------------------------------------------------------------------------------------

void RCC_Configure(void)
{
	// TODO: Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'
	
	/* UART TX/RX port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* JoyStick Up/Down port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	/* JoyStick Selection port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* LED port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	/* USART1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	// TODO: Initialize the GPIO pins using the structure 'GPIO_InitTypeDef' and the function 'GPIO_Init'
	
    /* JoyStick up, down pin setting */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* JoyStick selection pin setting */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* button pin setting */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* LED pin setting*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
    /* UART pin setting */
    //TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	//RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

void EXTI_Configure(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    // TODO: Select the GPIO pin (Joystick, button) used as EXTI Line using function 'GPIO_EXTILineConfig'
    // TODO: Initialize the EXTI using the structure 'EXTI_InitTypeDef' and the function 'EXTI_Init'

    /* Joystick Down */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
    /* Joystick Up */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
    /* Button */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11);
    /* Joystick Selection */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);

    EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line5 | EXTI_Line8 | EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // NOTE: do not select the UART GPIO pin used as EXTI Line here
}

void USART1_Init(void)
{
	USART_InitTypeDef USART1_InitStructure;

	// Enable the USART1 peripheral
	USART_Cmd(USART1, ENABLE);
	
	// TODO: Initialize the USART using the structure 'USART_InitTypeDef' 
    and the function 'USART_Init'
	USART1_InitStructure.USART_BaudRate = 0x1E4;
    USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS;
    USART1_InitStructure.USART_Mode = USART_Mode_Tx;
    USART1_InitStructure.USART_Parity = USART_Parity_Even;
    USART1_InitStructure.USART_StopBits = USART_StopBits_1;
    USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, USART1_InitStructure);
	
	// TODO: Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
	USART_ITConfig(USART1,USART_IT_RXNE, )
}

void NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure;
    
    // TODO: fill the arg you want
    NVIC_PriorityGroupConfig(???);
	// TODO: Initialize the NVIC using the structure 'NVIC_InitTypeDef' and the function 'NVIC_Init'
	
    // Joystick Down
    NVIC_EnableIRQ(EXTI2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ; // TODO

    // Joystick Up
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ; // TODO
    
    // User S1 Button
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ; // TODO

    // UART1
	// 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ; // TODO
    

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn | EXTI9_5_IRQn | EXTI15_10_IRQn | USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void USART1_IRQHandler() {  // a or b putty input
	uint16_t word;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
    	// the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART1);
        // TODO implement
 
        if (word &= ~(0x61));// ex) global var x = 0;
        if (word &= ~(0x62)) // ex) global var x = 1;
        // clear 'Read data register not empty' flag
    	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

void EXTI15_10_IRQHandler(void) { // button putty output "TEAM XX"

    char msg[] = "Team02.\r\n"; // -*- length = 14;

	if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
        if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == Bit_RESET) {
            for (i = 0; i < 9; i++) { sendDataUART1(msg[i]); }
        }
        EXTI_ClearITPendingBit(EXTI_Line11);
	}
}

void EXTI9_5_IRQHandler(void) { // when the button is pressed, joy up

    if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
        if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == Bit_RESET) {
            // TODO implement
            // ex) global var x = 0;
        }
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
}

void EXTI2_IRQHandler(void) { // when the button is pressed, joy down

    if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
        if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) == Bit_RESET) {
            // TODO implement
            // �������� x = 1
        }
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
}


void Delay(void) {
	int i;

	for (i = 0; i < 2000000; i++) {}
}

void sendDataUART1(uint16_t data) {
	/* Wait till TC is set */
	while ((USART1->SR & USART_SR_TC) == 0);
	USART_SendData(USART1, data);
}

int main(void)
{

    SystemInit();

    RCC_Configure();

    GPIO_Configure();

    EXTI_Configure();

    USART1_Init();

    NVIC_Configure();

    while (1) {
    	// TODO: implement 
    	
    	// Delay
    	Delay();
    }
    return 0;
}
