/*
 *	
 *	 Clocktimer.c header file
 *	
 *
 *   Author: Osazuwa Omigie	
 *   Date: April 8,2014.		
 *   CSCI-E28
 *   Harvard University	
 */
 
#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	"alarmlib.h"



/* Holds the properties of the clock */
struct clocktimer {
	int mins,  /* minutes*/
		secs,  /* seconds */
		ticks, /* ticks from timed signal handler*/
		is_paused; /* boolean for indicating clock pause or resume state */
};

/**
	Initializes the clock properties
	@params 	none
	@return 	void
**/
void clock_init();


/**
	This function is usually called by the signal handler to
	indicate a clock tick to the clock.
	The function interpretes the ticks, adjusting the 
	clock's seconds or minutes accordingly. 
	@params 	none
	@return 	void
**/
void clock_tick();


/**
	This function forms a string representation of the clock output
	in the format "TOTAL TIME: mins:secs"
	@params 	char_array: where the clock output is stored
	@return 	void
**/
void get_time(char * char_array);


/**
	Resets the minutes, seconds and ticks to zero.
	@params		none
	@return 	void
**/
void clock_reset();


/**
	Changes the state of the clock timer to paused.
	In this state, tick events been sent to the clock timer
	are not processed.
	@params		none
	@return 	void
**/
void clock_pause();


/**
	Changes the state of the clock timer to resumed.
	In this state, tick events been sent to the clock timer
	can now be processed.
	@params		none
	@return 	void
**/
void clock_resume();