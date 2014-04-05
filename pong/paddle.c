/*
 *	
 *	Pong
 *
 *	User Input:
 *		 	s slow down x component, S: slow y component
 *		 	f speed up x component,  F: speed y component
 *			Q quit
 *
 *	blocks on read, but timer tick sets SIGALRM which are caught
 *	by ball_move
 */



#include	"paddle.h"
#include	"pong.h"


struct pppaddle the_paddle;


void paddle_init(){
	the_paddle.pad_top = START_TOP;
	the_paddle.pad_bot = the_paddle.pad_top + PAD_LEN;
	the_paddle.pad_col = RIGHT_EDGE - 1;
	the_paddle.pad_char =  PAD_SYMBOL;

	mvvline(the_paddle.pad_top,the_paddle.pad_col,the_paddle.pad_char,PAD_LEN);
}

void paddle_up(){
	int bot_cur, moved;

	moved = 0;

	//check if pad_top is not trying to go outside the court
	if((the_paddle.pad_top-1)>TOP_ROW){
		bot_cur = the_paddle.pad_bot;
		the_paddle.pad_top-=1;
		the_paddle.pad_bot-=1;
		moved = 1;		
	} 

	if (moved){
		mvaddch(the_paddle.pad_top, the_paddle.pad_col, the_paddle.pad_char);
		mvaddch(bot_cur, the_paddle.pad_col, BLANK);
	}
	
}

void paddle_down(){
	int top_cur, moved;

	moved = 0;

	//check if pad_top is not trying to go outside the court
	if((the_paddle.pad_bot+1)<BOT_ROW){
		top_cur = the_paddle.pad_top;
		the_paddle.pad_top+=1;
		the_paddle.pad_bot+=1;
		moved = 1;		
	} 

	if (moved){
		mvaddch(the_paddle.pad_bot, the_paddle.pad_col, the_paddle.pad_char);
		mvaddch(top_cur, the_paddle.pad_col, BLANK);
	}
	
}

/**
	Checks if the given coordinates are in contact with the paddle
	@params int y, int x
	@return Returns 0 if not in contact and 1 otherwise
**/
int paddle_contact(int y,int x){
	int i;
	if (x+2 == RIGHT_EDGE){
		for (i=the_paddle.pad_top; i<=the_paddle.pad_bot; i++){
			if (y == i){
				return 1;
			}
		}
	}
	return 0;

}