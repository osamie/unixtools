/* dumputmp.c 
 *
 *	purpose:	display contents of wtmp or utmp in readable form
 *	args:		none for default (/etc/utmp) or a filename
 *	action:		open the file and read it struct by struct,
 *			displaying all members of struct in nice columns
 *	history:	Feb 15 1996: added buffering using utmplib
 *	compiling:	to compile this, use
 *			gcc dumputmp.c utmplib.c -o dumputmp
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<utmp.h>

// #define	BLANKS

main(int ac,char **av)
{
	if ( ac == 1 )
		dumpfile( UTMP_FILE );
	else
		dumpfile( av[1] );
}

dumpfile( char *fn )
/*
 * open file and dump records
 */
{
	struct utmp	utrec;		/* one record buffer	*/
	int		fd;		/* file descriptor	*/
	struct utmp	*utp,		/* ptr to struct	*/
			*utmp_next();	/* declare its type	*/

	if ( utmp_open( fn ) == -1 )	/* open file		*/
	{
		perror( fn );
		return ;
	}

	/* loop, reading records and showing them */

	while( utp = utmp_next() )
		show_utrec( utp );
	utmp_close();
}
	
show_utrec( struct utmp *rp )
{
	char	*typename();

	showfld("%-10.10s", rp->ut_user );
	showfld("%-5.5",  rp->ut_id   );
	showfld("%-8.8s", rp->ut_line );
	printf("%d\t", rp->ut_pid );
	printf("%d\t%-15.15s", rp->ut_type , typename(rp->ut_type) );
	printf("%d\t%d\t", rp->ut_exit.e_termination, rp->ut_exit.e_exit);
	printf("%ld\t", rp->ut_time );
	showfld("%s", rp->ut_host );
	putchar('\n');
}

#ifdef BLANKS
showfld(char *dummy, char *s)
{
	int	i;
	for( i = 0 ;i<8; i++ )
	{
		putchar(s[i]? s[i] : '_');
	}
	putchar('\t');
}
#else
showfld(char *fmt, char *s)
{
	printf(fmt, s);
}
#endif

char *uttypes[] = {  	"EMPTY", "RUN_LVL", "BOOT_TIME", "OLD_TIME", 
			"NEW_TIME", "INIT_PROCESS", "LOGIN_PROCESS", 
			"USER_PROCESS", "DEAD_PROCESS", "ACCOUNTING"
	};

char *
typename( typenum )
{
	return uttypes[typenum];
}
