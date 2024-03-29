#ifndef __ws2812_H
#define __ws2812_H


#include <stdint.h>

// enable TIM1
// ARR -1 = cpu in KHz / 800 KHz
// egzample:
// 90 -1 = 72000 KHz / 800 KHz

#define LED_COUNT 2
#define END_DELAY 50

void WS2812_Init(void);
void WS2812_SetLed(uint16_t led, uint8_t red, uint8_t green, uint8_t blue);
void WS2812_Send(void);


#endif /* __ws2812_H */
