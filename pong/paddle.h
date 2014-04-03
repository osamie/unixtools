/*
 *	
 *	Paddle.c header file
 *	
 */



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
void paddle_contact(int y,int x);