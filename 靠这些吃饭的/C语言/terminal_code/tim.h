#ifndef  __TIM_H__
#define  __TIM_H__

#include "stm32f10x.h"

void tim3_init(u16 psc, u16 arr);
void tim4_init(u16 psc, u16 arr);
void tim6_init(void);

void breathing_light(float f_sine, u16 *count_nums);
float duty_capture(void);

#endif