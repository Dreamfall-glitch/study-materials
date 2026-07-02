#include "stm32f10x.h"

void nvic_init(void)
{
	/* 中断优先级分组 */
	SCB->AIRCR |= (0x05FA<<16);	//密钥标志
	SCB->AIRCR &= ~(0x07<<8);		//8,9,10位清零
	SCB->AIRCR |= (0x06<<8);		//1位抢占优先级，3位相应优先级
	
	/* 设置各中断优先级 */
	NVIC->IPR[9]  &= ~(0xF0);
	NVIC->IPR[9]  |= 0x00;	//EXTI3		0 000
	NVIC->IPR[10] &= ~(0xF0);
	NVIC->IPR[10] |= 0x10;	//EXTI4 	0 001
	NVIC->IPR[37] &= ~(0xF0);
	NVIC->IPR[37] |= 0x80;	//USART1 	1 000
	NVIC->IPR[39] &= ~(0xF0);
	NVIC->IPR[39] |= 0x90;	//USART3 	1 001
	NVIC->IPR[54] &= ~(0xF0);
	NVIC->IPR[54] |= 0xA0;	//TIM6 		1 010
}