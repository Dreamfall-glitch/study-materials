#include "stm32f10x.h"

void key0_init(void)
{
	/* 配置GPIO*/
	GPIOE->CRL &= ~(0x0F<<4*4);   //GPIOE_Pin4的CRL4位清零
	GPIOE->CRL |= (0x08<<4*4);    //0x08 = 0b1000, 即设置GPIOE_Pin4为上/下拉输入模式
	GPIOE->ODR |= (0x01<<4);      //配置PE4为上拉
	
	/* 配置AFIO */
	AFIO->EXTICR[1] &= ~(0x0F);	//EXTICR[1]的bit[3:0]清零
	AFIO->EXTICR[1] |= 0x04;		//0x04 = 0b0100，配置EXTI4为GPIOE的Pin_4
		
	/* 配置EXTI */
	EXTI->RTSR |= (0x01<<4);	//EXTI4配置为上升沿触发
	EXTI->EMR &= ~(0x01<<4);	//屏蔽EXTI4作为事件输出
	EXTI->IMR |= (0x01<<4);		//使能EXTI4作为中断输出
	
	/* 使能NVIC */
	NVIC->ISER[0] |= (0x01<<10);
}

void key1_init(void)
{
	/* 配置GPIO*/
	GPIOE->CRL &= ~(0x0F<<4*3);   //GPIOE_Pin3的CRL4位清零
	GPIOE->CRL |= (0x08<<4*3);    //0x08 = 0b1000, 即设置GPIOE_Pin3为上/下拉输入模式
	GPIOE->ODR |= (0x01<<3);      //配置PE3为上拉

	/* 配置AFIO */
	AFIO->EXTICR[0] &= ~(0x0F<<12);	//EXTICR[0]的bit[15:12]清零
	AFIO->EXTICR[0] |= (0x04<<12);	//0x04 = 0b0100，配置EXTI3为GPIOE的Pin_3
	
	/* 配置EXTI */
	EXTI->RTSR |= (0x01<<3);		//EXTI3配置为上升沿触发
	EXTI->EMR &= ~(0x01<<3);		//屏蔽EXTI3作为事件输出
	EXTI->IMR |= (0x01<<3); 		//使能EXTI3作为中断输出
	
	/* 使能NVIC */
	NVIC->ISER[0] |= (0x01<<9);
}