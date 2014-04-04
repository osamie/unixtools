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


#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	"pong.h"
#include	"paddle.h"
#include	"alarmlib.h"


struct ppball the_ball;
static int game_lose;

void set_up();
void wrap_up();
int  bounce_or_lose(struct ppball *);
void init_walls();
void init_ball_pos();

/** the main loop **/

int main()
{
	int	c;
	void set_up();

	set_up();
	mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol); //serve ball

	while ( ( c = getch()) != 'Q' || game_lose == FALSE){
		if (c=='k'){
			paddle_up();
		}
		else if (c=='m'){
			paddle_down();
		}
	}
	wrap_up();
	return 0;
}

/*	init ppball struct, signal handler, curses	*/

void set_up()
{
	void ball_move(int); 

	initscr();		/* turn on curses	*/
	noecho();		/* turn off echo	*/
	cbreak();		/* turn off buffering	*/

	
	init_walls();
	init_ball_pos();
	paddle_init();
	
	signal(SIGINT, SIG_IGN);	/* ignore SIGINT	*/
	
	
	refresh();
	
	signal( SIGALRM, ball_move );
	set_ticker( 1000 / TICKS_PER_SEC );	/* send millisecs per tick */
}

void init_ball_pos(){
	int delay=MAX_DELAY;
	srand(getpid()); //initialize random number generator

	/* find random value for x_delay and y_delay greater than 0 */
	while ((delay=rand() % MAX_DELAY) <= 0) {}
	
	the_ball.x_pos = X_INIT;
	the_ball.y_pos = Y_INIT;
	the_ball.y_count = the_ball.y_delay = delay ;
	the_ball.x_count = the_ball.x_delay = delay ;
	the_ball.y_dir = 1  ;
	the_ball.x_dir = 1  ;
	the_ball.symbol = DFL_SYMBOL ;
}

/*
	Builds the walls 
*/
void init_walls(){
	//ACS_HLINE ACS_VLINE
	clear();

	/* Draw top horizontal wall */
	mvhline(LEFT_EDGE,TOP_ROW,ACS_HLINE,COLS-(PADDING*2));

	/* Draw left vertical wall */
	mvvline(TOP_ROW+1,LEFT_EDGE,ACS_VLINE,LINES-(PADDING*2));

	/* Draw bottom horizontal wall */
	mvhline(BOT_ROW,LEFT_EDGE,ACS_HLINE,COLS-(PADDING*2));

	move(LINES-1, COLS-1);		/* park cursor	*/

}

/* stop ticker and curses */
void wrap_up()
{

	set_ticker( 0 );
	endwin();		/* put back to normal	*/
}

/* SIGARLM handler: decr directional counters, move when they hit 0	
 note: may have too much going on in this handler			
*/

void ball_move(int s)
{
	int	y_cur, x_cur, moved, game_lose;

	signal( SIGALRM , SIG_IGN );		/* dont get caught now 	*/
	y_cur = the_ball.y_pos ;		/* old spot		*/
	x_cur = the_ball.x_pos ;
	game_lose = moved = 0 ;

	if ( the_ball.y_delay > 0 && --the_ball.y_count == 0 ){
		the_ball.y_pos += the_ball.y_dir ;	/* move	*/
		the_ball.y_count = the_ball.y_delay  ;	/* reset*/
		moved = 1;
	}
	if ( the_ball.x_delay > 0 && --the_ball.x_count == 0 ){
		the_ball.x_pos += the_ball.x_dir ;	/* move	*/
		the_ball.x_count = the_ball.x_delay  ;	/* reset*/
		moved = 1;
	}
	if ( moved ){ 
		/* erase ball from previous location */
		mvaddch(y_cur, x_cur, BLANK); 

		/* draw the ball on the new location */
		mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol); 

		/* check for collision or game lose */
		if (bounce_or_lose(&the_ball)==-1){
			game_lose = TRUE;
		} 
		move(LINES-1, COLS-1);		/* park cursor */
		refresh();
	}
	signal(SIGALRM, ball_move);		/* re-enable handler	*/
	
}

/* bounce_or_lose: if ball hits walls, change its direction
 *   args: address to ppball
 *   @return: 0 for no contact, 1 for a bounce, or -1 for a lose
 */
int bounce_or_lose(struct ppball *bp)
{
	int	return_val = 0 ;

	if ( (bp->y_pos-1) == TOP_ROW ){
		//ball bounces off top wall 
		bp->y_dir = 1; //change direction
		return_val = 1 ; 
	}else if ( (bp->y_pos+1) == BOT_ROW ){
		//ball bounces off bottom wall
		bp->y_dir = -1; //change direction
		return_val = 1;
	}
	if ( (bp->x_pos-1) == LEFT_EDGE ){
		//ball bounces off left wall
		bp->x_dir = 1;	//change direction
		return_val = 1 ;
	}else if ( (bp->x_pos+2) == RIGHT_EDGE ){
		if (paddle_contact(bp->y_pos,bp->x_pos)){
			//ball bounces off paddle
			bp->x_dir = -1; //change direction
			return_val = 1;
		}else{
			return_val = -1;
		}		
	}
	return return_val;	
}

