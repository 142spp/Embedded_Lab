#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_usart.h"
#include "lcd.h"
#include "touch.h"
#include <stdlib.h>
#include <stdio.h>

/* function prototype */
void Delay(void);

uint16_t brightness = 0;

//---------------------------------------------------------------------------------------------------
void RCC_Configure(void) {
    // TODO: Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'

    /* UART TX/RX port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /* Alternate Function IO clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

// void ADC_Configure(void){
//     ADC_InitTypeDef ADC_struct;
//     ADC_Init(1,ADC_struct);
//     ADC_RegularChannelConfig()
//     ADC_ITConfig()
//     ADC_Cmd()
//     ADC_ResetCalibration()
//     While(ADC_GetResetCalibrationStatus) // ->while ����Ͽ� üũ
//     ADC_StartCalibration()
//     While(ADC_GetCalibrationStatus) // ->while ����Ͽ� üũ
//     ADC_SoftwareStartConvCmd()

// }

void Delay(void) {
    int i;
    for (i = 0; i < 2000000; i++) {
    }
}

int color[12] = {WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};


int main(void) {
    SystemInit();
    RCC_Configure();
    
/*--------------------------*/
    LCD_Init();
    Touch_Configuration();
    //Touch_Adjust();
    //LCD_Clear(color[0]);
/*--------------------------*/
    uint16_t c_x = 10;
    uint16_t c_y = 10;
    u8* team_str = "MON_Team02";
    
    while (1) {
        //Touch_GetXY(&c_x,&c_y,1);
        LCD_DrawCircle(c_x,c_y,3);
        
        
        LCD_ShowString(30,40,team_str,color[2],color[0]); // team
        LCD_ShowNum(100,60,c_x,3,color[2],color[0]); // team
        LCD_ShowNum(100,80,c_y,3,color[2],color[0]); // team
        Delay();
    }

    return 0;
}