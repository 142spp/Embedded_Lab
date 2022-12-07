#include <math.h>
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/* function prototype */
void GP2Y_Delay(void);
void GP2Y_RCC_Configure(void);
void GP2Y_GPIO_Configure(void);
void GP2Y_ADC_Configure(void);
void GP2Y_Init_Configure(void);
float GP2Y_Get_Distance(void);

float ADC_Val;

/**
 * @brief Enable or Disable Clocks
 */
void GP2Y_RCC_Configure(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // Photoresistor Pin Enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // ADC Enable
}
/**
 * @brief Enable GPIO Pins using GPIO_InitStructure
 */
void GP2Y_GPIO_Configure(void) {
    /* Photoresistor Enable */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	// GPIOA�� 0�� Pin�� �����. 
	// �̴� ���� ADC�� Channel0 ��. (schematic ����)
}
/**
 * @brief ADC Configure using ADC_InitStructure
 */
void GP2Y_ADC_Configure(void) {
    ADC_InitTypeDef ADC_InitStructure;

    // ADC1 Configuration
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
	// �� �κ��� 8������ ����ߴ� �ڵ带 �״�� ���Ŷ� ���� ������� �ʾƵ� ��.
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);  // ADC1 Channel 8 is GPIOB_Pin0
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);  // interrupt enable
    // ADC_DMACmd(ADC1, ENABLE);  // DMA Enable
    ADC_Cmd(ADC1, ENABLE);     // ADC1 enable

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1)) ;
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1)) ;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void GP2Y_NVIC_Configure(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void ADC1_2_IRQHandler(void) {
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) {
        ADC_Val = ADC_GetConversionValue(ADC1 );
        //--- Clear ADC1 AWD pending interrupt bit
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
}

/**
 * @brief excute All configuration function
 */
void GP2Y_Init_Configure(void) {
    SystemInit();
    GP2Y_RCC_Configure();
    GP2Y_GPIO_Configure();
    GP2Y_ADC_Configure();
    GP2Y_NVIC_Configure();
}

void GP2Y_Delay(void) {
    for (int i = 0; i < 1000000; i++)
        ;
}

float GP2Y_Get_Distance() {
    float distance = ADC_Val;
   
        return distance;
}
