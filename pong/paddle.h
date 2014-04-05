/*
 *	
 *	Paddle.c header file
 *	
 */

#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include	<unistd.h>
#include	<stdlib.h>
// #include	"pong.h"
#include	"alarmlib.h"

#define		PAD_SYMBOL	'#'
#define 	START_TOP	PADDING+3
#define		PAD_LEN		0.33*LINES  /*pad is one third of screen*/


struct pppaddle {
		int pad_top, pad_bot, pad_col;
		char pad_char;
};


void paddle_init();
void paddle_up();
void paddle_down();
int paddle_contact(int y,int x);