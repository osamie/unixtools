
/* noftps.c 
 *
 *	purpose:	copy utmpfile remove all ftp lines
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<utmp.h>

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

	if ( strncmp(rp->ut_line, "ftp", 3 ) != 0 )
		write(1, rp, sizeof(struct utmp) );
}

