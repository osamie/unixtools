/*  more04.c  - version 0.4 of more
 *	read and print 24 lines then pause for a few special commands
 *      feature of 0.4 : uses terminal control to turn off echo and
 *                       turn off buffering
 *      problem: press ^C in the middle and see what happens
 */
#include	<stdio.h>
#include	<fcntl.h>
#include	<termios.h>

#define	PAGELEN	24
#define	LINELEN	512
#define	BEEP	{putchar('\a'); fflush(stdout);}

void do_more(FILE *, int, int);
int see_more(int,int);

int main(int ac , char *av[])
{
	FILE	*fp;
	int	terminalfd;

	int	rows = PAGELEN;

	/* open terminal for user input */
	terminalfd = open("/dev/tty", O_RDONLY);
	if ( terminalfd == -1 ){
		perror("/dev/tty");
		exit(1);
	}
	program_mode(1,terminalfd);

	/* process args */
	if (ac == 1)
		do_more( stdin , rows, terminalfd );
	else
		while (--ac)
			if ( (fp = fopen(*++av , "r")) != NULL )
			{
				do_more(fp, rows, terminalfd); 
				fclose(fp);
			}
			else
				perror(*av);
	/* done */
	program_mode(0,terminalfd);
	close(terminalfd);
	return 0;
}

void do_more(FILE *fp , int rows, int termfd)
/*
 *  read PAGELEN lines, then call see_more() for further instructions
 */
{
	char	line[LINELEN];
	int	num_of_lines = 0;
	int	reply;

	while ( fgets( line, LINELEN, fp ) ){		/* more input	*/
		if ( num_of_lines == rows ) {		/* full screen?	*/
			reply = see_more(termfd,rows); 	/* do more?	*/
			if ( reply == 0 )		/*    n: done   */
				break;
			num_of_lines -= reply;		/* reset count	*/
		}
		if ( fputs( line, stdout )  == EOF )	/* show line	*/
			exit(1);			/* or die	*/
		num_of_lines++;				/* count it	*/
	}
}

int see_more(int term, int rows)
/*
 *	print message, wait for response, return # of lines to advance
 *	q means no, space means yes, CR means one line
 */
{
	char	c;
	int	rv = 0;

	printf("\033[7m more? \033[m");		/* reverse on a vt100	*/
	fflush(stdout);
	while ( read(term, &c, 1) == 1 )
	{
		switch ( c ) {
			case 'q':	rv = 0;	   break;
			case ' ':	rv = rows; break;
			case '\n':	rv = 1;    break;
			default:	BEEP;	   continue;
		}
		break;
	}
	printf("\r                    \r");	/* clear the message	*/
	return rv;
}

/*
 * set program mode on or off based on 'how'
 * note: stores previous mode for restore later
 */
program_mode(int how, int fd)
{
	static struct termios prevmode;
	struct termios settings;
	static int prev_stored = 0;
	int rv;

	if ( how == 1 ){
		tcgetattr(fd, &settings);	/* get current mode */
		prevmode = settings;		/* make a copy	    */
		prev_stored = 1;		/* and note it	    */
		settings.c_lflag &= ~ECHO;	/* no echo	    */
		settings.c_lflag &= ~ICANON;	/* no buffer/edit   */
		settings.c_cc[VMIN] = 1;        /* get chars 1 by 1 */
		rv = tcsetattr(fd, TCSANOW, &settings);
	} 
	else if ( how == 0 && prev_stored ==1 ){
		tcsetattr(fd, TCSANOW, &prevmode);
	}
}