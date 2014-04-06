/*
 *	
 *	 Paddle.c
 *
 *	
 *	 Paddle object for pong game
 *	
 *	
 *
 *   Author: Osazuwa Omigie	
 *   Date: April 8,2014.		
 *   CSCI-E28
 *   Harvard University	
 */



#include	"paddle.h"
#include	"pong.h"


struct pppaddle the_paddle;


/*
	Initialize the paddle's properties
	@params 	none
	@return 	void
*/
void paddle_init(){
	the_paddle.pad_top = START_TOP;
	the_paddle.pad_bot = the_paddle.pad_top + PAD_LEN;
	the_paddle.pad_col = RIGHT_EDGE - 1;
	the_paddle.pad_char =  PAD_SYMBOL;

	mvvline(the_paddle.pad_top,the_paddle.pad_col,the_paddle.pad_char,PAD_LEN);
}

/*
	Moves the paddle up one row. 
	@params		none
	@return 	void
*/
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

/*
	Moves the paddle down one row. 
	@params 	none
	@return 	void
*/
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

/*
	Checks if a given coordinate (usually the ball's) is in contact 
	with the paddle.
	@params		y: a given row position,
				x: a given column posiiton.
	@return		1 if in contact or 0 otherwise
*/
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