#include <stdio.h>
#include <stdlib.h>

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
// #include "stm32f10x_tim.h"

#include "lcd.h"
#include "touch.h"

enum { OFF = 0,
       ON = !OFF };
volatile uint32_t ADC_Value[1]; 

/* function prototype */
void Delay(void);
void RCC_Configure(void);
void GPIO_Configure(void);
void ADC_Configure(void);
void DMA_Configure(void);
void TIM_Configure(void);
void EXTI_Configure(void);
void NVIC_Configure(void);
void Init_Configure(void);

/**
 * @brief Enable or Disable Clocks
 */
void RCC_Configure(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // Photoresistor Pin Enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // ADC Enable
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    // DMA Enable
}
/**
 * @brief Enable GPIO Pins using GPIO_InitStructure
 */
void GPIO_Configure(void) {
    /* Photoresistor Enable */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	// GPIOB의 0번 Pin를 사용함. 
	// 이는 또한 ADC의 Channel8 임. (schematic 참고)
}
/**
 * @brief ADC Configure using ADC_InitStructure
 */
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
	// 위 부분은 8주차때 사용했던 코드를 그대로 쓴거라 따로 기술하지 않아도 됨.

    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);  // ADC1 Channel 8 is GPIOB_Pin0
    // ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);  // interrupt enable
    ADC_DMACmd(ADC1, ENABLE);  // DMA Enable
    ADC_Cmd(ADC1, ENABLE);     // ADC1 enable
	// 11주차 ppt 내용 참고. ADC_ITConfig 대신 ADC_DMACmd 사용함.

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1)) ;
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1)) ;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// 마찬가지로 기술하지 않아도 됨 
}
/**
 * @brief DMA Configure using DMA_InitStructure
 */
void DMA_Configure(void) {
	// 아래 내용들은 stm32f10x_dma.h에 간단한 설명들이 적혀있음.
	// 또한 stm32_ReferenceManual 286p의 내용과 일치함.
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	// 정보를 불러올 Address를 ADC1의 Data Register로 함.
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value[0];
	// 정보를 저장할 Address를 전역변수로 선언된 ADC_Value 에 저장함.
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	// 정보의 흐름. Peripheral -> Memory 로 데이터가 이동.
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	// DMA_Mode_Normal 과 DMA_Mode_Circular 중 Circular를 선택. 이는 주기적으로 정보를 불러옴.
    DMA_InitStructure.DMA_BufferSize = 1;
	// 1개 사이즈 만큼의 Buffer를 둠.
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	// PeripheralBaseAddr에서 DMA_PeripheralDataSize_Word = 32bits 만큼 읽음 . 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;
	// MemoryBaseAddr에서 DMA_MemoryDataSize_Word = 32bits 만큼 읽음 . 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// Enable시 해당 Address를 읽고, 읽을 Address를 Increase함.
	// 우리팀은 항상 같은 Address = ADC1->DR 을 읽어야 하므로 Disable
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	// 항상 같은 Address = ADC_Value 에 저장해야 하므로 Disable
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// 우선순위를 결정해야 하는데, 우리는 하나의 DMA만 사용하므로 아무거나 사용해도 상관 x
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// Memory To Memory. 메모리 -> 메모리로 데이터가 이동할때 Enable 시키는데 우리팀은 해당하지 않으므로 Disable
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	// 실험제공 자료 PPT 참고. DMA1_Channel1 은 ADC1 을 사용할때 사용됨.
    DMA_Cmd(DMA1_Channel1, ENABLE);
	// 실행
}
/**
 * @brief excute All configuration function
 */
void Init_Configure(void) {
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    ADC_Configure();
    // TIM_Configure();
    // EXTI_Configure();
    // NVIC_Configure();
    DMA_Configure();
    LCD_Init();
    // Touch_Configuration();
}

int color[12] =  // Color Array
    {WHITE, CYAN, BLUE, RED, MAGENTA, LGRAY, GREEN, YELLOW, BROWN, BRRED, GRAY};
/**
 * @brief Delay around 0.1 sec
 */
void Delay(void) {
    for (int i = 0; i < 1000000; i++)
        ;
}

int main(void) {
    Init_Configure();

    uint32_t threshold = 100; // threshold of Brightness
    uint16_t state = 0;  // means current color state. ( backcolor is white(0) or gray(1) )
    while (1) {
        if (state && ADC_Value[0] < threshold) { 
			// if current color is gray & Brightness is less than threshold
            LCD_Clear(color[0]);  // change background color - white
            state = 0;  // change state to white
        } else if (!state && ADC_Value[0] > threshold) {
			// if current color is white & Brightness is greater than threshold
            LCD_Clear(color[11]); // change background color - gray
            state = 1;  // change state to gray
        }
        if (!state) // if current color state is white
            LCD_ShowNum(100, 50, ADC_Value[0], 4, color[2], color[0]);  // print brightness
        else	// if current color state is gray
            LCD_ShowNum(100, 50, ADC_Value[0], 4, color[0], color[11]);  // print brightness

        Delay();
    }

    return 0;
}