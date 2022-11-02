#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "LCD_library/lcd.h"
#include "LCD_library/touch.h"

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void NVIC_Configure(void);
void Delay(void);

//---------------------------------------------------------------------------------------------------
void RCC_Configure(void) {
    // TODO: Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'

    /* UART TX/RX port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /* Alternate Function IO clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}
void Delay(void) {
    int i;
    for (i = 0; i < 2000000; i++) {
    }
}

int color[12] = {WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};

int main(void) {
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    ADC_Configure();
    NVIC_Configure();
/*--------------------------*/
    LCD_Init();
    Touch_Configuration();
    Touch_Adjust();
    LCD_Clear(WHITE);
    NVIC_Configure();
/*--------------------------*/

    uint16_t c_x,c_y;
    uint16_t p_x,p_y;
    uint16_t brightness;
    char* team_str = "MON_Team02";
    LCD_ShowString(30,30,team_str,color[2],color[0]); // team

    while (1) {
        Touch_GetXY(c_x,c_y,1);
        Convert_Pos(c_x,c_y,&p_x,&p_y);
        LCD_DrawCircle(c_x,c_y,2);

        LCD_ShowNum(100,50,(uint8_t)p_x,3,color[2],color[0]); // cursor x
        LCD_ShowNum(100,70,(uint8_t)p_y,3,color[2],color[0]); // cursor y
        LCD_ShowNum(30,90,brightness,4,color[2],color[0]); // bright sensor
    }

    return 0;
}