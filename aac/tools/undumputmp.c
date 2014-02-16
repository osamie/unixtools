/* undumputmp.c 
 *
 *	purpose:	convert text table into structs
 *	args:		none, read stdin
 *	action:		expect input looks like
         sdas     ttys6    ttys6      8834    7 USER_PROCESS      949441628 ..
         bfriedm  ttys6    ttys6      8834    7 USER_PROCESS      949441637 
         ahprice  ttysa    ttysa     18425    7 USER_PROCESS      949441651 ..

matev	ttyp5	ttyp5	4429	7	USER_PROCESS	0	0	1013414423	roam179-50.student.harvard.edu
	ttyp2	ttyp2	12900	8	DEAD_PROCESS	0	0	1013414627	
 *	history:	Feb 15 1996: added buffering using utmplib
 *	compiling:	to compile this, use
 *			gcc dumputmp.c utmplib.c -o dumputmp
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<utmp.h>
#include	<string.h>

main(int ac,char **av)
{
	struct utmp	utrec;		/* one record buffer	*/
	char	        line[BUFSIZ];	/* a buffer of a line   */
	char		*cp;
	int		rv;
	char		dummy[BUFSIZ];	/* can't %*s working	*/
	char		pid[20], type[20], thetime[20];
	char		termstat[20], exitstat[20];

	while( fgets(line, BUFSIZ, stdin) )
	{
		memset(&utrec, 0, sizeof(struct utmp));

		if ( *line != ' ' )
			sscanf(line,"%s",utrec.ut_user);

		rv = sscanf(line+9,"%8s %8s %s%s %s %s%s %s %s", 
				utrec.ut_id, utrec.ut_line, 
				pid, type, 	/*  pid, type */
				dummy,		/* name of type */
				termstat, exitstat,
				thetime,		/* time */
				utrec.ut_host);

		utrec.ut_pid  = atoi(pid);
		utrec.ut_type = atoi(type);

		utrec.ut_exit.e_termination = atoi(termstat);
		utrec.ut_exit.e_exit        = atoi(exitstat);

		utrec.ut_time = atol(thetime);

		fprintf(stderr,"pid = %d, type = %d, time = %ld\n",
			utrec.ut_pid, utrec.ut_type, (long)utrec.ut_time);
		fprintf(stderr,"%d -> %s", rv, line);

		if ( rv )
			write(1, &utrec, sizeof(struct utmp));
	}
}
