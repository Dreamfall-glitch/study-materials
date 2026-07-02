#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "stm32f10x.h"
#include "mydefine.h"

void bluetooth_init(void);

/* 数据发送函数 */


/* 发送一字节 */
TX_state bt_send_1byte(u8 data);
/* 发送字符串 */
void bt_send_string(u8 *message);


/* 数据接收函数 */


/* 接收一字节 */
u8 bt_receive_lbyte(void);
/* 接收字符串 */
void bt_receive_string(void);


/* 数据分析处理函数 */


/* 清空缓冲区函数 */
void bt_empty_RxBuffer(u8 len);
/* 浮点字符转浮点数函数 */
float bt_floatchar_to_float(char *p1);
/* 分析数据函数 */
RX_state bt_data_analysis(void);
/* 处理数据函数 */
void bt_data_process(void);

#endif