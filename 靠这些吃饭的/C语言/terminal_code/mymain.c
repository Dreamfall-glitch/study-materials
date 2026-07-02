/* 头文件 */
#include "stdio.h"
#include "stm32f10x.h"
#include "bit_band.h"
#include "clock.h"
#include "nvic.h"
#include "systick.h"
#include "tim.h"
#include "usart.h"
#include "key.h"
#include "led.h"
#include "bluetooth.h"
#include "beep.h"

/* 宏定义 */
#include "mydefine.h"

/* 全局变量 */

/* 定时器相关 */
float fsine  = 1.00;
float fflash = 1.00;
float duty 	 = 0.00;
float fduty  = 5.00;

u16 tim3_psc=0, tim3_arr=35999;	//TIM3输出PWM频率为2KHz
u16 tim4_psc=0, tim4_arr=59999;	//TIM4计数频率为72MHz，记60000个数停止，保证比外部输入PWM的周期大

u16 tim6_nums_samp;	
u16* ptim6_nums_samp = &(tim6_nums_samp);

u16 sys_nums_set;
u16 sys_nums_flash;

u16 send_duty_set;
u16 send_duty_nums;

/* 状态变量 */
send_duty_state send_s 	= MANUAL;
led_state led_s 				= LIGHTING;
music_state music_s 	 	= STOP;

/* 串口相关 */
u8 usart1_flag = 0;
u8 RxBuffer[MAX];
u8 RxCount = 0;

/* 蓝牙相关 */
u8 bluetooth_flag = 0;
u8 bt_RxBuffer[MAX];
u8 bt_RxCount = 0;

/* 蜂鸣器相关 */
u8 songs_count = 0;
note songs[8] = {REST,DO,DO,SOL,SOL,LA,LA,SOL};
u8 lens = sizeof(songs) / sizeof(songs[0]);

/* main函数 */
int main(void)
{
	/*初始化函数*/
	
	APB1_clock_init();
	APB2_clock_init();
	nvic_init();
	systick_init();
	
	/* 
	 * f = TIM3_CLK(72MHz) / (arr + 1) / (psc + 1) 
	 * duty = ccr / (arr + 1 ) 
	 */
	tim3_init(tim3_psc, tim3_arr);
	tim4_init(tim4_psc, tim4_arr);
	
	tim6_init();
	usart1_init();
	key0_init();
	key1_init();
	led_init();
	bluetooth_init();
	tim2_init();

	/*初始化变量*/
	
	tim6_nums_samp = TIM6_SETNUM;
	sys_nums_set 	 = FLASH_SETNUM;
	sys_nums_flash = sys_nums_set;
	send_duty_set  = SEND_DUTY_SETNUM;
	send_duty_nums = send_duty_set;
	songs_count = 0;
	
	/*发送提示词*/
	send_string((u8 *)("Welcome to mycontroller!\r\n"));
	send_string((u8 *)("Enter \"fsine=xxx(Hz)\" to write frequence of breathing light.\r\n"));
	send_string((u8 *)("Enter \"fflash=xxx(Hz)\" to write frequence of flashing light.\r\n"));
	send_string((u8 *)("Enter \"duty=?\" to read current duty radio of breathing light.\r\n"));
	send_string((u8 *)("Enter \"convert\" to change the mode of sending duty.\r\n"));
	send_string((u8 *)("Enter \"fduty=xxx(Hz)\" to write sending frequence of duty radio.\r\n"));
	
	bt_send_string((u8 *)("Welcome to mycontroller!\r\n"));
	bt_send_string((u8 *)("Enter \"fsine=xxx(Hz)\" to write frequence of breathing light.\r\n"));
	bt_send_string((u8 *)("Enter \"fflash=xxx(Hz)\" to write frequence of flashing light.\r\n"));
	bt_send_string((u8 *)("Enter \"duty=?\" to read current duty radio of breathing light.\r\n"));
	bt_send_string((u8 *)("Enter \"convert\" to change the mode of sending duty.\r\n"));
	bt_send_string((u8 *)("Enter \"fduty=xxx(Hz)\" to write sending frequence of duty radio.\r\n"));
	bt_send_string((u8 *)("Enter \"20260101\" to change the mode of breathing light.\r\n"));
	bt_send_string((u8 *)("Enter \"music\" to play / stop music.\r\n"));

	while(1)
	{
		/* 串口通信控制 */
		data_process();
		
		/* 蓝牙通信控制 */
		bt_data_process();
			
		/* 呼吸灯 */
		breathing_light(fsine ,ptim6_nums_samp);
		
		/* 占空比 */
		duty = duty_capture();																				//捕捉占空比
		send_duty_set = (u16)((1 / fduty) * 1000);										//发送频率转计数
		if((send_duty_nums == (send_duty_set/2)) && send_s == AUTO)		//自动发送设置
		{
			char send_buf[MAX];
			sprintf(send_buf, "duty = %.4f%%\r\n", (1 - duty) * 100.0);
			send_string((u8 *)send_buf);
			bt_send_string((u8 *)send_buf);
		}
		
		/* 闪烁灯 */
		sys_nums_set = (u16)((1 / fflash) * 2000);																//闪烁频率转计数
		if((sys_nums_flash >= (sys_nums_set/2)) || led_s == LIGHTING)	PEout(5)=0;	//亮灯
		else																													PEout(5)=1;	//灭灯
		
		/* 蜂鸣器 */
		if((sys_nums_flash >= (sys_nums_set/2)) && music_s == PLAY)		TIM2->CCR2 =  songs[songs_count];
		else																													TIM2->CCR2 = 0;
		if(songs_count == lens)																				songs_count = 0;	//循环播放

	}//while(1)
}//main()