#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include "mydefine.h"

void usart1_init(void);

/* 数据发送函数 */


/* 发送一字节 */
TX_state send_1byte(u8 data);
/* 发送字符串 */
void send_string(u8 *message);


/* 数据接收函数 */


/* 接收一字节 */
u8 receive_lbyte(void);
/* 接收字符串 */
void receive_string(void);


/* 数据分析处理函数 */


/* 清空缓冲区函数 */
void empty_RxBuffer(u8 len);
/* 浮点字符转浮点数函数 */
float floatchar_to_float(char *p1);
/* 分析数据函数 */
RX_state data_analysis(void);
/* 处理数据函数 */
void data_process(void);

#endif