
/**
 **	some parameters
 **/


#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<string.h>
#include	"alarmlib.h"


#define	BLANK		' '
#define	DFL_SYMBOL	'O'
#define PADDING		3
#define	TOP_ROW		PADDING
#define	BOT_ROW 	LINES-PADDING
#define	LEFT_EDGE	PADDING
#define	RIGHT_EDGE	COLS-PADDING
#define	X_INIT		10		/* ball's starting col	*/
#define	Y_INIT		10		/* ball's starting row	*/

#define	MAX_DELAY		4


#define INIT_BALLS	2		/* Maximum number of balls is 9 */


/**
 **	Ball object
 **/

struct ppball {

		/*The ball's position */ 
		int	x_pos, y_pos;
			
		/*This is used as ball's movement direction: and is usully +1 or -1
		to indicate increase or decrement in x or y coordinate */
		int x_dir, y_dir;

		int delay,count;

		char	symbol ;

	} ;

/*	
	Initializes ppball struct, signal handler, curses	
	@param		void
	@return 	void
*/
void set_up();

/* 
	Stop ticker and curses 
	@params none
	@return void
*/
void wrap_up();


/* 
 *	 Checks if a ball hits the walls, paddle or goes leaves the court. 
 *   @params	args address to ppball
 *   @return    0 for no contact, 1 for a bounce, or -1 for a lose
 */
int  bounce_or_lose(struct ppball *);

/*
	Draws the ping pong court/walls on the screen 

	@params none
	@return void
*/
void init_walls();

/*
	Initializes the ball's starting position, speed, and direction
	@params	none
	@return void
*/
void init_ball_pos();


/**
	Starting a new round: Initializes the ball's position, updates the headers and 
	serves the ball, starting a new round. 
	@params		none
	@return   	void
**/
void start_round();


/* 
	This is the SIGARLM signal handler. It updates the clock timer first, 
	then updates the ball's position after its delay count reached.
	@params signal args
	@return void
*/
void sigarlm_handler();

/*
	Calls the appropriate functions to update the balls left and clock header
	@param	void
	@return void
*/ 
void print_headers();

/**
	Outputs the balls left by concatenating the actual number of balls left
	with the "BALLS LEFT:" string. 
	@params	ballnum: the number of balls left
	@return void
**/
void update_left_header(int ballnum);

/**
	Gets the clock's current string ouput and writes it to the screen 
	@params	none
	@return void
**/
void update_right_header();