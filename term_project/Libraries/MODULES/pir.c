#include "pir.h"

/*void PIR_RCC_Configure(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //PIR Sensor Pin Enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC, ENABLE);     //ADC Enable
}*/

void PIR_RCC_Init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //PIR Sensor Pin Enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);     //ADC Enable
}

void PIR_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    //GPIOA Configuration
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //PIR ADC1?GPIOA 0? Pin ??
}

void PIR_ADC_Init(void){
    ADC_InitTypeDef ADC_InitStructure;

    //ADC1 Configuration
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
?
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);  // ADC1 Channel 0 is GPIOA_Pin0
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);  // interrupt enable
    // ADC_DMACmd(ADC1, ENABLE);  // DMA Enable
    ADC_Cmd(ADC1, ENABLE);     // ADC1 enable

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1)) ;
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1)) ;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void PIR_NVIC_Init(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    // PIR Sensor
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void ADC1_2_IRQHandler(void) {
    //
}

void Pir_Delay(void) {
    for (int i = 0; i < 1000000; i++)
        ;   
}

void PIR_Init(void){
    PIR_RCC_Init(void);
    PIR_ADC_Init(void);
    PIR_GPIO_Init(void);
    PIR_NVIC_Init(void);
    Pir_Delay(void);
}

//HIGH->on,LOW->off

/*val = digitalRead(SensorOut);
if (val == HIGH) {
    digitalWrite(Led, HIGH);
}
else {
    digitalWrite(Led, LOW);
}
delay(100);*/
