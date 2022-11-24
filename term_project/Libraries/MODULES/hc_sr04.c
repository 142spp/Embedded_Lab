#include "hc_sr04.h"

void initMeasureTimer(void) {
	RCC_ClocksTypeDef RCC_ClocksStatus;
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	uint16_t prescaler = RCC_ClocksStatus.SYSCLK_Frequency / 1000000 - 1; //1 tick = 1us (1 tick = 0.165mm resolution)

	TIM_DeInit(US_TIMER);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(US_TIMER, &TIM_TimeBaseStructure);

	TIM_OCInitTypeDef TIM_OCStructure;
	TIM_OCStructInit(&TIM_OCStructure);
	TIM_OCStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStructure.TIM_Pulse = 15; //us
	TIM_OCStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(US_TIMER, &TIM_OCStructure);

	TIM_ICInitTypeDef TIM_ICStructure;
	TIM_ICStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICStructure.TIM_ICFilter = 0;
	TIM_PWMIConfig(US_TIMER, &TIM_ICStructure);

	TIM_SelectInputTrigger(US_TIMER, US_TIMER_TRIG_SOURCE);
	TIM_SelectMasterSlaveMode(US_TIMER, TIM_MasterSlaveMode_Enable);

	TIM_CtrlPWMOutputs(US_TIMER, ENABLE);

	TIM_ClearFlag(US_TIMER, TIM_FLAG_Update);
        TIM_ITConfig(US_TIMER,TIM_IT_Trigger,ENABLE);
}

uint32_t HCSR04GetDistance(void) {
	(US_TIMER)->CNT = 0;
	TIM_Cmd(US_TIMER, ENABLE);
	while(!TIM_GetFlagStatus(US_TIMER, TIM_FLAG_Update));
	TIM_Cmd(US_TIMER, DISABLE);
	TIM_ClearFlag(US_TIMER, TIM_FLAG_Update);
	return (TIM_GetCapture2(US_TIMER)-TIM_GetCapture1(US_TIMER))*165/1000;
}