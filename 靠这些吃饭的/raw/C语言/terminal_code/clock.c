#include "stm32f10x.h"

void APB1_clock_init(void)
{
	RCC->APB1ENR |= (0x01<<0);  //开启TIM2的时钟
	RCC->APB1ENR |= (0x01<<1);  //开启TIM3的时钟
	RCC->APB1ENR |= (0x01<<2);  //开启TIM4的时钟
	RCC->APB1ENR |= (0x01<<4);  //开启TIM6的时钟
	RCC->APB1ENR |= (0x01<<18);	//开启USART3时钟
}

void APB2_clock_init(void)
{
	RCC->APB2ENR |= (0x01<<0);	//开启AFIO时钟
	RCC->APB2ENR |= (0x01<<2);	//开启GPIOA时钟
	RCC->APB2ENR |= (0x01<<3);	//开启GPIOB时钟
	RCC->APB2ENR |= (0x01<<6);	//开启GPIOE时钟
	RCC->APB2ENR |= (0x01<<14);	//开启USART1时钟
}