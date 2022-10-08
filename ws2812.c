#include "ws2812.h"
#include "main.h"
#include <stdbool.h>

#define LED_DATA_LENGTH (24 * LED_COUNT + END_DELAY)

extern TIM_HandleTypeDef htim1;

static uint16_t TimerAutoReloadRegisterPeriodOneThird;
static uint16_t TimerAutoReloadRegisterPeriodTwoThirds;
static uint16_t LedData[LED_DATA_LENGTH];
static volatile bool DataSentFlag;

void WS2812_Init(void)
{
	TimerAutoReloadRegisterPeriodOneThird = (htim1.Init.Period + 1) / 3;
	TimerAutoReloadRegisterPeriodTwoThirds = TimerAutoReloadRegisterPeriodOneThird * 2;
}

void WS2812_SetLed(uint16_t led, uint8_t red, uint8_t green, uint8_t blue)
{
	uint32_t color = (green << 16) | (red << 8) | blue;

	for (uint8_t i = 0; i < 24; i++)
	{
		LedData[led * 24 + 23 - i] = (color & (1 << i))
			? TimerAutoReloadRegisterPeriodTwoThirds
			: TimerAutoReloadRegisterPeriodOneThird;
	}
}

void WS2812_Send(void)
{
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)LedData, LED_DATA_LENGTH);
	
	while (!DataSentFlag);
	
	DataSentFlag = false;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	
	DataSentFlag = true;
}
