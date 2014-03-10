/*
* 
* sttyl  (stty-lite)

* 
*  Purpose: 
*	   This is a utility used to view and modify the terminal control settings 
*	   for your console. In other words, stty allows you to determine how your 
*	   current login session (for example) processes input and output, and to 
*	   control these parameters.
* 
*  Arguments:
*	
*	
*		
*		
*		
*	 
*	 
*   Author: Osazuwa Omigie	
*   Date: March 9,2014.		
*   CSCI-E28
*   Harvard University		
*		
 speed, intr, erase, and kill chars, 

 icrnl, onlcr, echo, echoe, olcuc, icanon, and isig

 local modes: ECHO, ECHOE, ISIG, ICANON
 input modes: ICRNL 
 output modes: OLCUC, ONLCR
*/


#include	<stdio.h>
#include	<stdlib.h>
#include	<termios.h>

struct flaginfo { tcflag_t fl_value; char	*fl_name; };

struct flaginfo input_flags[] = {
		
		IGNBRK	,	"IGNBRK",
		BRKINT	,	"BRKINT",
		IGNPAR	,	"IGNPAR",
		/*PARMRK	,	"Mark parity errors",
		INPCK	,	"Enable input parity check",
		ISTRIP	,	"Strip character", */
		INLCR	,	"INLCR",
		/*IGNCR	,	"Ignore CR", */
		ICRNL	,	"ICRNL",
		IXON	,	"IXON",
		/* _IXANY  ,	"enable any char to restart output",	
		IXOFF   ,	"Enable start/stop input control",*/
		0	,	NULL };

struct flaginfo local_flags[] = {
		ISIG	,	"ISIG",
		ICANON	,	"ICANON",
		/* _XCASE	,	"Canonical upper/lower appearance", */
		ECHO	,	"ECHO",
		ECHOE	,	"ECHOE",
		ECHOK	,	"ECHOK",
		0	,	NULL };

struct flaginfo output_flags[] = {
		OLCUC	,	"OLCUC", 
		ONLCR	,	"ONLCR",
		0	,	NULL};


void showbaud(int thespeed);
void show_flagset( int thevalue, struct flaginfo thebitnames[] );
void show_some_flags( struct termios *ttyp );



int main(int argc, char * argv[])
{	
	struct	termios ttyinfo;	/* this struct holds tty info */

	if ( tcgetattr( 0 , &ttyinfo ) == -1 ){   /* get terminal driver info */
		perror( "cannot get params about stdin");
		exit(1);
	}

	if (argc==1) /*only one commanline argument*/
	{ 	
		/*show info*/
		showbaud(cfgetospeed( &ttyinfo)); /*get and display baud rate */
		show_some_flags(&ttyinfo);

	}

	

	

	printf("\n");
}


/*
 *	show the values of two of the flag sets_: c_iflag and c_lflag
 *	adding c_oflag and c_cflag is pretty routine - just add new
 *	tables above and a bit more code below.
 */
void show_some_flags( struct termios *ttyp )
{
	show_flagset( ttyp->c_iflag, input_flags );
	show_flagset( ttyp->c_lflag, local_flags );
}


/*
 * Check each bit pattern and display descriptive title
 */
void show_flagset( int thevalue, struct flaginfo thebitnames[] )
{
	int	i;
	for ( i=0; thebitnames[i].fl_value ; i++ ) {
		// char * fl_name = thebitnames[i].fl_name;
		if (!(thevalue & thebitnames[i].fl_value)) 
			printf("-"); /*place a '-'' in front to indicate off status*/
		printf( "%s; ", thebitnames[i].fl_name);
		if((i%2)!=0) printf("\n");
	}
}


/*
 *	prints the speed in english
 */
void showbaud( int thespeed ){
	printf("speed ");
	switch ( thespeed ){
		case B300:	printf("300");	break;
		case B600:	printf("600"); 	break;
		case B1200:	printf("1200"); 	break;
		case B1800:	printf("1800"); 	break;
		case B2400:	printf("2400"); 	break;
		case B4800:	printf("4800"); 	break;
		case B9600:	printf("9600"); 	break;
		case B38400:	printf("38400"); 	break;
		case B57600:	printf("57600"); 	break;
		case B115200:	printf("115200"); 	break;
		case B230400:	printf("230400"); 	break;
		default:	printf("n/a");	break;
	}
	printf(" baud; ");
}
