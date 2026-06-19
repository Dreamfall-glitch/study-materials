#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "mydefine.h"
#include "stm32f10x_usart.h"

/* 全局变量 */
extern  u8 RxBuffer[MAX];
extern  u8 usart1_flag, RxCount;
extern float fsine, fflash, duty, fduty;
extern send_duty_state send_s;

void usart1_init(void)
{
	/* 配置GPIO */
	GPIOA->CRH &= ~((0x0B<<4) | (0x04<<8));	//GPIOA_Pin9和Pin10的CRH4位清零
	GPIOA->CRH |= (0x0B<<4);								//0x0B = 0b1011,USART1的TX(PA9)配置为复用推挽输出，速度为50MHz
	GPIOA->CRH |= (0x04<<8);								//0x04 = 0b0100,USART1的RX(PA10)配置为复用浮空输入，
	
	/* 配置BRR */
	USART1->BRR = 72000000 / 115200;				//APB2总线时钟为72MHz，波特率设置为115200
	
	/* 配置数据信息 */
	USART1->CR1 = 0x00;
	USART1->CR2 = 0x00;
	USART1->CR3 = 0x00; 					//CR1,2,3全部位清零，默认配置即1位起始位，8位数据位，1位停止位，无校验
	USART1->CR1 |= (0x03<<2);			//接收和发送使能
	
	/* 配置接收中断 */
	USART1->CR1 |= (0x01 << 5); //开启RXNE（接收非空）中断
	NVIC->ISER[1] |= (0x01<<5);	// 37 = 32 + 5

	/* 使能USART */
  USART1->CR1 |= (0x01<<13);
}

/* 数据发送函数 */

/* 发送一字节 */
TX_state send_1byte(u8 data)
{
	vu32 cnt=0;											//超时计时器
	USART_SendData(USART1,data);		//发送
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET)
	{
		cnt++;
		if(cnt>100000) return ERR;		//发送超时，返回ERR
	}                          			//等待发送完成
	return OK;
}

/* 发送字符串 */
void send_string(u8 *message)
{
	while(*message!='\0')
	send_1byte(*message++);
}

/* 数据接收函数 */

/* 接收一字节 */
u8 receive_lbyte(void)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET){}		//等待接收完成
	return (USART_ReceiveData(USART1));														//接收数据并返回
}

/* 接收字符串 */
void receive_string(void)
{
	RxCount = 0;
	while (1)
	{
		u8 ch = receive_lbyte();
		if (ch == '\n' || RxCount >= MAX - 1)
		{
			RxBuffer[RxCount] = '\0';
			break;
		}
		RxBuffer[RxCount++] = ch;
	}
}

/* 数据分析处理函数 */

/* 清空缓冲区函数 */
void empty_RxBuffer(u8 len)
	{
		for(u8 i = 0; i < len; i++) RxBuffer[i]=0;
	}

/* 浮点字符转浮点数函数 */
float floatchar_to_float(const char *p1)		
{		
	float value = 0.0f;
	sscanf(p1, "%*[^=]=%f", &value);	//找“=”
	return value;
}		

/* 分析数据函数 */
RX_state data_analysis(void)
{
	if (strstr((char *)RxBuffer, "fsine="))
	{
		float fl = floatchar_to_float((char *)RxBuffer);
		if(fl <= 0 || fl > 99)	//范围安全判断
		{
			return NO_COMMAND;
		}
		else
		{
			fsine = fl;
			return W_FSINE;
		}
	}
	
	else if (strstr((char *)RxBuffer, "fflash="))
	{
		float fl = floatchar_to_float((char *)RxBuffer);
		if(fl <= 0 || fl > 99) 
		{
			return NO_COMMAND;
		}
		else
		{
			fflash = fl;
			return W_FFLASH;
		}
	}
	
	else if (strstr((char *)RxBuffer, "duty=?"))
	{
		return R_DUTY;
	}
		
	else if (strstr((char *)RxBuffer, "convert"))
	{
		send_s ^= 0x01;
		return CONV;
	}
	
	else if (strstr((char *)RxBuffer, "fduty="))
	{
		float fl = floatchar_to_float((char *)RxBuffer);
		if(fl <= 0 || fl > 99) 
		{
			return NO_COMMAND;
		}
		else
		{
			send_s = AUTO;
			fduty = fl;
			return W_FDUTY;
		}
	}
	
	else
	{
		return NO_COMMAND;
	}
}

/* 处理数据函数 */
void data_process(void)
{
	RX_state RX_s = NO_COMMAND + 1;

	/* 接收完成后处理 */
	if (usart1_flag)
	{
		RX_s = data_analysis();
		usart1_flag = 0;     // 准备下一次中断接收
		RxCount = 0;         // 索引清零，下次继续从接收数组起始赋值
		empty_RxBuffer(MAX); // 清空缓冲区，方便下次数据存放
	}

	if (RX_s != NO_COMMAND + 1)
	{
		char send_buf[MAX];

		switch (RX_s)
		{
			case W_FSINE:
				sprintf(send_buf, "Succeeded! fsine = %.4f Hz\r\n", fsine);
				break;

			case W_FFLASH:
				sprintf(send_buf, "Succeeded! fflash = %.4f Hz\r\n", fflash);
				break;

			case R_DUTY:
				sprintf(send_buf, "Succeeded! duty = %.4f%%\r\n", (1 - duty) * 100.0);
				break;
			
			case CONV:
				sprintf(send_buf, "Succeeded! State has been converted.\r\n");
				break;
			
			case W_FDUTY:
				sprintf(send_buf, "Succeeded! fduty = %.4f Hz\r\n", fduty);
				break;
			
			default:
				sprintf(send_buf, "No command, please enter again!\r\n");
				break;
		}

		send_string((u8 *)send_buf);
	}
}
