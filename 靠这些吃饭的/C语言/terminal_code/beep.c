#include "stm32f10x.h"
#include "mydefine.h"

void tim2_init(void)
{
	/* 配置GPIO */
	GPIOA->CRL &= ~(0x0F<<(4*1));	//GPIOA_Pin1的CRL4位清零
	GPIOA->CRL |=(0x0B<<(4*1));		//0x0B = 0b1011, 即设置GPIOA_Pin1为复用功能推挽输出，速度为50MHz
	GPIOB->CRH &= ~(0x0F<<(4*0));	//GPIOB_Pin8的CRL4位清零
	GPIOB->CRH |= (0x04<<(4*0));  //0x04 = 0b0100, 即设置GPIOB_Pin8为浮空输入
	
	/* 配置AFIO */
	AFIO->MAPR &= ~(0x03<<8);			//AFIO的MAPR第8,9位清零
	AFIO->MAPR |= (0x00<<8);      //无需重映射
	
	/* 配置时基单元 */
	TIM2->CR1 &= ~(0x01<<4);//采用向上计数
	TIM2->CR1 |= (0x01<<7);	//启动TIM2_ARR缓冲器

		/* 
	 * f = TIM2_CLK(72MHz) / (arr + 1) / (psc + 1) 
	 * duty = ccr / (arr + 1 ) 
	 */
	TIM2->PSC = 0;				//写入预分频器的值
	TIM2->ARR = 59999;			//写入自动重装载寄存器的值
	
	/* 配置输出比较功能 */
	TIM2->CCMR1 &= ~(0xFF<<8);	//TIM2_CH2清零
  TIM2->CCMR1 |= (0x60<<8);		//TIM2_CH2采用PWM模式1 
	TIM2->CCER |= (0x01<<4);		//设置输出使能
	TIM2->CCER &= ~(0x01<<5);		//设置输出极性
	TIM2->CCMR1 &= ~(0x01<<15); //不使用清0使能		
	
	/* 使能定时器 */
	TIM2->CR1 |= 0x01;
}

