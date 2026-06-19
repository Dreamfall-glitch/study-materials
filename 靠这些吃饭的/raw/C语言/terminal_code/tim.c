#include <math.h>
#include "stm32f10x.h"
#include "mydefine.h"

/* 全局变量 */
extern u16 tim3_arr;

void tim3_init(u16 psc, u16 arr)
{
	/* 配置GPIO */
	GPIOB->CRL &= ~(0x0F<<(4*5));	//GPIOB_Pin5的CRL4位清零
	GPIOB->CRL |=(0x0B<<(4*5));		//0x0B = 0b1011, 即设置GPIOB_Pin5为复用功能推挽输出，速度为50MHz
	
	/* 配置AFIO */
	AFIO->MAPR &=~(0x03<<10);			//AFIO的MAPR第11,10位清零
	AFIO->MAPR |=(0x02<<10);      //采用部分重映射，将TIM3_CH2重映射到PB5
	
	/* 配置时基单元 */
	TIM3->CR1 &= ~(0x01<<4);			//采用向上计数
	TIM3->CR1 |= (0x01<<7);				//启动TIM3_ARR缓冲器

	TIM3->PSC = psc;							//写入预分频器的值
	TIM3->ARR = arr;							//写入自动重装载寄存器的值
	
	/* 配置输出比较功能 */
	TIM3->CCMR1 &= ~(0xFF<<8);		//TIM3_CH2清零
  TIM3->CCMR1 |= (0x60<<8);			//TIM3_CH2采用PWM模式1 
	TIM3->CCER |= (0x01<<4);			//设置输出使能
	TIM3->CCER &= ~(0x01<<5);			//设置输出极性
	TIM3->CCMR1 &= ~(0x01<<15); 	//不使用清0使能
	
	/* 使能定时器 */
	TIM3->CR1 |= 0x01; 
}

void tim4_init(u16 psc, u16 arr)
{
	/* 配置GPIO */
	GPIOB->CRL &= ~(0x0F<<(4*6));	//GPIOB_Pin6的CRL4位清零
	GPIOB->CRL |= (0x04<<(4*6));  //0x04 = 0b0100, 即设置GPIOB_Pin6为浮空输入
	
	/* 配置AFIO */
	AFIO->MAPR &=~(0x01<<12);			//AFIO的MAPR第12位清零，无需重映射
	
	/* 配置时基单元 */
	TIM4->CR1 &= ~(0x01<<4);			//采用向上计数
	TIM4->CR1 |= (0x01<<7);				//启动TIM4_ARR缓冲器
	
	TIM4->PSC = psc;							//写入预分频器的值
	TIM4->ARR = arr;							//写入自动重装载寄存器的值
	
	/* 配置输入捕获功能 */
	
	/* 配置CC1 */
	TIM4->CCMR1 &= ~(0xFF);				//TIM4_CH1清零
	TIM4->CCMR1 |= 0x01;        	//TIM4_CH1映射到IC1
  TIM4->CCMR1 &= ~(0x0F<<4);  	//不滤波
  TIM4->CCMR1 &= ~(0x03<<2);		//不分频 
  TIM4->CCER &= ~(0x01<<1);   	//上升沿捕获
	TIM4->CCER |= 0x01;         	//捕获使能
	
	/* 配置CC2 */
	TIM4->CCMR1 &= ~(0xFF<<8);		//TIM4_CH2清零
	TIM4->CCMR1 |= (0x02<<8);			//TIM4_CH2映射到IC1
  TIM4->CCMR1 &= ~(0x0F<<12);		//不滤波
  TIM4->CCMR1 &= ~(0x03<<10);		//不分频 
  TIM4->CCER |= (0x01<<5);			//下降沿捕获
	TIM4->CCER |= (0x01<<4);			//捕获使能
	
	/* 配置从模式 */
	TIM4->SMCR |= (0x05<<4);     	//TS=101,选择TI1FP1
	TIM4->SMCR |= 0x04;          	//SMS=100,复位模式
	
	/* 使能定时器 */
	TIM4->CR1 |= 0x01; 
}

void tim6_init(void)
{			
	/* 使能NVIC */
	NVIC->ISER[1] |= (0x01<<22);	// 54 = 32 + 22
	
	/* 配置时基单元 */
	TIM6->CR1 |= (0x01<<7);				//启动TIM6_ARR缓冲器
	
	/* t = (arr + 1) * (psc + 1) /72MHz */
	TIM6->PSC = 999;		
	TIM6->ARR = 71;
	/* 实现 1ms 计数 */
		
	/* 配置中断 */
	TIM6->SR = 0x0;   						//清除更新中断标志位
	TIM6->DIER = 0x01;						//使能更新中断

	/* 使能定时器 */
	TIM6->CR1 |= 0x01; 
}

void breathing_light(float f_sine, u16 *count_nums)
{
	float T_sine,T_nums,ratio;
	
	/* 频率与周期换算 */
	T_sine = 1.00/ f_sine;
	T_nums=((float) T_sine) * 1000;
	
	/* 占空比采样计算 */
	if ((*count_nums) >= T_nums)	(*count_nums) = 0;
	ratio = 0.5 + 0.5*sin(2*PI * f_sine * (*count_nums) * 0.001);
	TIM3->CCR2 = (u16)(ratio * tim3_arr);
}

float duty_capture(void)
{
	float period = 0.0;
	period	= (float)(TIM4->CCR1 + 1);
	float width = 0.0;
	width = (float)(TIM4->CCR2 + 1);
	return (width / period);
}
