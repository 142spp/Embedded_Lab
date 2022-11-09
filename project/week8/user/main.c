#include "lcd.h"
#include "touch.h"
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void ADC_Configure(void);
void NVIC_Configure(void);
void Delay(void);

uint16_t Brightness = 0;

//---------------------------------------------------------------------------------------------------
void RCC_Configure(void) {
    // TODO: Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

void GPIO_Configure(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void ADC_Configure(void) {
    ADC_InitTypeDef ADC_InitStructure;

    // ADC1 Configuration
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5); // ADC1 Channel 8 is GPIOB_Pin0
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);  // interrupt enable
    //ADC_DMACmd(ADC1, ENABLE);                   // DMA Enable
    ADC_Cmd(ADC1, ENABLE);                   // ADC1 enable

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1)) ;
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1)) ;
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void NVIC_Configure(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void ADC1_2_IRQHandler(void) {
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) {
        Brightness = ADC_GetConversionValue(ADC1);
        //--- Clear ADC1 AWD pending interrupt bit
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
}

void Delay(void) {
    int i;
    for (i = 0; i < 2000000; i++) {
    }
}

int color[12] = {WHITE, CYAN, BLUE, RED, MAGENTA, LGRAY, GREEN, YELLOW, BROWN, BRRED, GRAY};

void Init(void) {
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    ADC_Configure();
    NVIC_Configure();

    LCD_Init();
    Touch_Configuration();
}


int main(void) {
    Init();
    
    //Touch_Adjust();
    
    uint16_t c_x, c_y;
    uint16_t p_x, p_y;
    
    char* team_str = "MON_Team02";

    while (1) {
       
        LCD_ShowString(30, 30, team_str, color[2], color[0]);  // team
        Touch_GetXY(&c_x, &c_y, 1);
        Convert_Pos(c_x, c_y, &p_x, &p_y);
        LCD_DrawCircle(p_x, p_y, 2);

        LCD_ShowNum(100, 50, p_x, 3, color[2], color[0]);  // cursor x
        LCD_ShowNum(100, 70, p_y, 3, color[2], color[0]);  // cursor y
        LCD_ShowNum(30, 90, Brightness, 4, color[2], color[0]);     // bright sensor
       
    }

    return 0;
}