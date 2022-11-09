#include <stdio.h>
#include <stdlib.h>

#include "lcd.h"
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "touch.h"

uint16_t count = 0;
enum {OFF=0,ON=!OFF};

/* function prototype */
void Delay(void);
void RCC_Configure(void);
void GPIO_Configure(void);
void TIM_Configure(void);
void EXTI_Configure(void);
void NVIC_Configure(void);
void TIM2_IRQHandler(void);
void Init_Configure(void);
void Set_LED(uint16_t&,uint16_t&);

/**
 * @brief Enable or Disable Clocks
 */
void RCC_Configure(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // Enable LED
    // Enable the TIM2 clock
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE);
}
/**
 * @brief Enable GPIO Pins using GPIO_InitStructure
 */
void GPIO_Configure(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    /* LED pin setting*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
/**
 * @brief Enable TIM using TIM_TimeBaseStructure
 */
void TIM_Configure(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Period = 10000;
    TIM_TimeBaseStructure.TIM_Prescaler = 7200;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}
/**
 * @brief configure EXTI using EXTI_InitStructure
 */
void EXTI_Configure(void) {
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
/**
 * @brief configure NVIC using NVIC_InitStructure
 */
void NVIC_Configure(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // TIM2 interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief Define interrupt handler function refer to IRQn_Type 
 *      defind in stm32f10x.h 167 ~ 472 Line
 */
void TIM2_IRQHandler(void) {
    // TIM2_IRQn;
    /* ITStatus */
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        count++;    // count is incremented;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
/**
 * @brief All Configure or Init function will be include.
 *      Don't delete code! please use Ctrl+l to unable line
 */
void Init_Configure(void){
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    TIM_Configure();
    // EXTI_Configure();
    NVIC_Configure();
    LCD_Init();
    Touch_Configuration();
}

int color[12] =  // Color Array
    {WHITE, CYAN, BLUE, RED, MAGENTA, LGRAY, GREEN, YELLOW, BROWN, BRRED, GRAY};

void Set_LED(uint16_t& GPIO_Pin, uint16_t& state){
    if(!state) {
        GPIO_SetBits(GPIOD, GPIO_Pin);
        state = ON;
    } else {
        GPIO_ResetBits(GPIOD, GPIO_Pin);
        state = OFF;
    }
}

int main(void) {
    Init_Configure();
    DMA1

    LCD_ShowString(30, 30, "MON_Team02", BLUE, WHITE);  // team
    LCD_ShowString(45, 105, "BUT", RED, WHITE);  // Button
    LCD_DrawRectangle(30, 90, 60, 120);

    uint16_t LED1_state = OFF;
    uint16_t LED4_state = OFF;
    uint16_t doToggling = OFF;
    uint16_t precount = count; 

    char** state = ["OFF","ON"];
    while (1) {
        if(!T_INT){                    // LCD touch interrupt
            doToggling = !doToggling;  // toggle switch
            count = 0; }

        if(count != precount && doToggling == ON) 
            Set_LED(GPIO_Pin_2,LED1_state);

        if(!(count % 5) && doToggling == ON)
            Set_LED(GPIO_Pin_7,LED4_state);

        LCD_ShowString(30, 60, state[doToggling],RED,WHITE);
        precount = count;
    }
    return 0;
}