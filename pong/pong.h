
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
// #define	TICKS_PER_SEC	60		/* affects speed	*/

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


void set_up();
void wrap_up();
int  bounce_or_lose(struct ppball *);
void init_walls();
void init_ball_pos();
void start_round();
void sigarlm_handler(); 
void print_headers();
void update_left_header(int ballnum);
void update_right_header();