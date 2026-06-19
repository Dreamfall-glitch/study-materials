#ifndef  __MYDEFINE_H__
#define  __MYDEFINE_H__

#define PI 3.1416

#define TIM6_SETNUM 1000
#define FLASH_SETNUM 1000
#define SEND_DUTY_SETNUM 1000

#define MAX 40

typedef enum{
	AUTO   = 0,
	MANUAL = 1
}send_duty_state;

typedef enum{
	LIGHTING = 0,
	FLASHING = 1
}led_state;

typedef enum {
	ERR  = 0,
  OK 	= 1
} TX_state;

typedef enum {
	W_FSINE  = 0,
  W_FFLASH = 1,
	R_DUTY  = 2,
	CONV = 3,
	W_FDUTY = 4,
	BREATHING = 5,
	MUSIC = 6,
	NO_COMMAND
} RX_state;

typedef enum {
	STOP = 0,
	PLAY = 1
} music_state;

typedef enum {
	REST = 0,
	DO = 7000,
	SOL = 59500,
	LA = 60000,
}note;

#endif