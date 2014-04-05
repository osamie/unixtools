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

#include	"pong.h"
#include	"paddle.h"

static struct ppball the_ball;
// static int game_lose;
static int balls_left;

void set_up();
void wrap_up();
int  bounce_or_lose(struct ppball *);
void init_walls();
void init_ball_pos();
void start_round();
void set_up();
void ball_move(int); 
void print_headers();
void update_left_header(int ballnum);
void update_right_header();
static void change_ball_speed();

/** the main loop **/

int main()
{
	int c;
	balls_left = INIT_BALLS-1; /*one ball currently in play*/
	int temp = balls_left;
	set_up();
	start_round(); //serves the balls and updates headers
	while ( ( c = getch()) != 'Q' && balls_left >= 0){
		if (c=='k'){
			paddle_up();
		}
		else if (c=='m'){
			paddle_down();
		}
		if((temp - balls_left) >= 1){ //lost a ball?
			start_round(); //restart round
		}
		temp = balls_left;
	}
	wrap_up();
	return 0;
}

void start_round(){
	// if (balls_left < 0) game_lose=TRUE;
	init_ball_pos();
	update_left_header(balls_left);
	mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol); //serve ball
	refresh();
	signal(SIGALRM, ball_move);		/* re-enable handler	*/
	
}

/*	init ppball struct, signal handler, curses	*/

void set_up()
{

	initscr();		/* turn on curses	*/
	noecho();		/* turn off echo	*/
	cbreak();		/* turn off buffering	*/
	init_walls();
	paddle_init();
	print_headers();

	signal(SIGINT, SIG_IGN);	/* ignore SIGINT	*/
	refresh();
	signal( SIGALRM, ball_move );
	set_ticker( 1000 / TICKS_PER_SEC );	/* send millisecs per tick */
}

void print_headers(){
	update_left_header(balls_left);
	update_right_header();
}

/**
	Updates the balls left 
**/
void update_left_header(int ballnum){
	int i,j,header_len = 14;
	char char_array[header_len];

	if(ballnum<0) return; //no need to update after game over.

	// char * str = "BALLS LEFT";
	char ball_num = (char) balls_left;

	snprintf(char_array,header_len,"BALLS LEFT: %d",ball_num);

	//write on the screen
	for(i=LEFT_EDGE,j=0;j<header_len-1;i++,j++){
		mvaddch(TOP_ROW-2,i,char_array[j]);
	}
}

/**
	Updates game clock
**/
void update_right_header(){
	int header_len = 18;
	char char_array[header_len];
	memset(char_array, '\0', header_len);
	int i,j,startingX=RIGHT_EDGE - header_len;
	char * time = "00:00"; 

	sprintf(char_array,"TOTAL TIME: %s",time);

	//write on the screen
	for(i=startingX,j=0;j<header_len-1;i++,j++){
		mvaddch(TOP_ROW-2,i,char_array[j]);
	}
}
void init_ball_pos(){
	srand(getpid()); //initialize random number generator
	change_ball_speed();
	the_ball.x_pos = X_INIT;
	the_ball.y_pos = Y_INIT;
	the_ball.y_dir = 1  ;
	the_ball.x_dir = 1  ;
	the_ball.symbol = DFL_SYMBOL ;
}

static void change_ball_speed(){
	int delay=MAX_DELAY;
	/* find random value for x_delay and y_delay greater than 0 */
	while ((delay=rand() % MAX_DELAY) <= 0) {}
	the_ball.y_count = the_ball.y_delay = delay ;
	the_ball.x_count = the_ball.x_delay = delay ;
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

/* 
	SIGARLM handler: decr directional counters, move when they hit 0			
*/

void ball_move(int s)
{
	int	y_cur, x_cur, moved =0;
	signal( SIGALRM , SIG_IGN );		/* dont get caught now 	*/
	y_cur = the_ball.y_pos ;		/* old spot		*/
	x_cur = the_ball.x_pos ;

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

		move(LINES-1, COLS-1);		/* park cursor */

		/* check for collision or game lose */
		if (bounce_or_lose(&the_ball)==-1) update_left_header(--balls_left);

		
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
			change_ball_speed();
		}else{
			return_val = -1;
		}		
	}
	return return_val;	
}

