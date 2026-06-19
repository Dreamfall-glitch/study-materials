#include "stm32f10x.h"

void led_init(void)
{
	/* 配置PE5 */
	GPIOE -> CRL &= ~(0x0F <<(4*5));	//GPIOE_Pin5的CRL4位清零
	GPIOE -> CRL |= (0x06 << (4*5));	//0x06 = 0b0110, 即设置GPIOE_Pin5为通用开漏输出，速度为2MHz ,初始状态为“常亮”
}