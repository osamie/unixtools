/*
 *	
 *	
 *	 Clocktimer.c
 *
 *	 Timer object for pong game
 *	
 *   Author: Osazuwa Omigie	
 *   Date: April 8,2014.		
 *   CSCI-E28
 *   Harvard University	
 */

#include	"clocktimer.h"
#include	"pong.h"


/*Holds the properties of the clock*/
static struct clocktimer the_clock; 


/**
	Initializes the clock properties
	@params 	none
	@return 	void
**/
void clock_init(){
	/*initialize all value to 0*/
	the_clock.mins = 0;
	the_clock.secs = 0;
	the_clock.ticks = 0;
	the_clock.is_paused = 0;  //initial state is resumed
}

/**
	This function is usually called by the signal handler to
	indicate a clock tick to the clock.
	The function interpretes the ticks, adjusting the 
	clock's seconds or minutes accordingly. 
	@params 	none
	@return 	void
**/
void clock_tick(){	
	if(the_clock.is_paused) return;
	if (++the_clock.ticks == 60){
		//after 60 ticks, increment clock by a second
		if (++the_clock.secs == 60){
			//60 seconds = 1 minute
			if (++the_clock.mins==99){
				clock_reset(); //2 digit timer minute maximum
			}	
			the_clock.secs = 0; 
		}
		the_clock.ticks=0; 	
	}
	
}

/**
	This function forms a string representation of the clock output
	in the format "TOTAL TIME: mins:secs"
	@params 	char_array: where the clock output is stored
	@return 	void
**/
void get_time(char * char_array){
	// snprintf(char_array,header_len,"BALLS LEFT: %d",ball_num);
	sprintf(char_array,"TOTAL TIME:%2d:%2d",the_clock.mins,the_clock.secs);
	// snprintf(char_array,6,"%d:%d",the_clock.mins,the_clock.secs);
}

/**
	Changes the state of the clock timer to paused.
	In this state, tick events been sent to the clock timer
	are not processed.
	@params		none
	@return 	void
**/
void clock_pause(){
	the_clock.is_paused = 1;	//change state to paused
}

/**
	Changes the state of the clock timer to resumed.
	In this state, tick events been sent to the clock timer
	can now be processed.
	@params		none
	@return 	void
**/
void clock_resume(){
	the_clock.is_paused = 0;	
}

/**
	Resets the minutes, seconds and ticks to zero.
	@params		none
	@return 	void
**/
void clock_reset(){
	the_clock.ticks = the_clock.mins = the_clock.secs = 0;
}