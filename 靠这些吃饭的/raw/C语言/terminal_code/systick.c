#include "stm32f10x.h"

void systick_init(void)
{
		SysTick->CTRL |= 0x06;	//选择内核时钟（72MHz），启动中断
		SysTick->LOAD = 71999;	//定时周期 (71999+1)/72MHz=1ms
		SysTick->CTRL |= 0x01;	//使能SysTick
}