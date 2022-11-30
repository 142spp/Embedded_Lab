#include "ultra.h"

int cap_rising_edge = 0;	// counter value at rising edge
int cap_falling_edge = 0;	// counter value at falling edge
int pulse_width = 0;		// = (counter value at falling edge) - (counter value at rising edge)

/* Distance Result */
int Distance = 0;

void Ultra_RCC_Configure(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

void Ultra_GPIO_Configure(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Trigger (PA7)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// TIM3 Ch1 (PA6) echo
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Ultra_TIM_Configure(void) {
	/* Time base configuration */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock / 1000000) - 1;	// 1Hz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel3 */
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// input capture timing
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	// TIM Input1 is connected to IC1
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
}

void Ultra_NVIC_Configure(void) {
	// Enable the TIM3 global Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Ultra_Init(void) {
	Ultra_RCC_Configure();
	Ultra_GPIO_Configure();
	Ultra_TIM_Configure();
	Ultra_NVIC_Configure();
}

void Ultra_TIM3_IRQHandler(void) {	// if rising(or falling) edge occurs
	/*ITStatus*/
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)	// if there is input value that has been captured
	{												// (= if TIM3->SR register CC1IF bit is High)

		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);	// clear TIM3->SR register CC1IF bit to Low

		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == Bit_SET)	// if Timer3 Ch1 pin(PA6) value is High (rising edge)
		{
			cap_rising_edge = TIM_GetCapture1(TIM3);	// read captured data (counter value at rising edge)
			TIM3->CCER |= TIM_CCER_CC1P;	// change ICPolarity(capture timing) to falling edge
		}
		else			// if Timer3 Ch1 pin(PA6) value is Low (falling edge)
		{
			cap_falling_edge = TIM_GetCapture1(TIM3);	// read captured data (counter value at falling edge)
			pulse_width = (uint32_t)(cap_falling_edge - cap_rising_edge);
			TIM3->CCER &= ~TIM_CCER_CC1P;	// change ICPolarity(capture timing) to rising edge
			


			/*	Result Distance (cm)	*/
			Distance = pulse_width * 17 / 1000;
		}
	}
}

/* Library Function */
void TIM3_IRQHandler(void) {	// if rising(or falling) edge occurs
	Ultra_TIM3_IRQHandler(void);
}

void Ultra_Trigger(void) {
	int i;

	GPIO_SetBits(GPIOA, GPIO_Pin_7);	// set trigger
	for (i = 0; i < 50000; i++) {
		/* delay(); */				// send ultrasonic wave
	}	
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);	// reset trigger
}


void Ultra_Measure_Distance(void) {	// Call in main function
	Ultra_Init();
	while (1) {
		HCSR04_Trigger();
	}
}


/*
int main() {
	Ultra_Measure_Distance();
}
*/
