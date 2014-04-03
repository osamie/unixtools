
/**
 **	some parameters
 **/

#define	BLANK		' '
#define	DFL_SYMBOL	'O'
#define PADDING		3
#define	TOP_ROW		PADDING
#define	BOT_ROW 	LINES-PADDING
#define	LEFT_EDGE	PADDING
#define	RIGHT_EDGE	COLS-PADDING
#define	X_INIT		10		/* ball's starting col	*/
#define	Y_INIT		10		/* ball's starting row	*/
#define	TICKS_PER_SEC	60		/* affects speed	*/

#define	X_DELAY		6
#define	Y_DELAY		6

#define MAX_NUMX		RIGHT_EDGE
#define MAX_NUMY		BOT_ROW 
#define MIN_NUMX		LEFT_EDGE
#define MIN_NUMY		TOP_ROW

/**
 **	the only object in town
 **/

struct ppball {

		/*The ball's position */ 
		int	x_pos, y_pos;
			
		/*This is used as ball's movement direction: and is usully +1 or -1
		to indicate increase or decrement in x or y coordinate */
		int x_dir, y_dir;

		int y_delay, y_count,
			x_delay, x_count;
		char	symbol ;

	} ;
