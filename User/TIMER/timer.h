#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f10x.h"
#include "bsp_adc.h"

#define true 1
#define false 0

void TIM3_Int_Init(u16 arr,u16 psc);
void freq_led(uint8_t freq_set);

#endif
