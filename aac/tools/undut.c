
/* undumputmp.c 
 *
 *	purpose:	convert text table into structs
 *	args:		none, read stdin
 *	action:		expect input looks like

	input is lines with:
	0 printf("%s\t", rp->ut_user );
	1 printf("%s\t", rp->ut_id   );
	2 printf("%s\t", rp->ut_line );
	3 printf("%d\t", rp->ut_pid );
	4 5 printf("%d\t%s\t", rp->ut_type , typename(rp->ut_type) );
	6 7 printf("%d\t%d\t", rp->ut_exit.e_termination, rp->ut_exit.e_exit);
	8 printf("%d\t", (long)rp->ut_time );
	9 printf("%s", rp->ut_host );


 *	history:	Feb 15 1996: added buffering using utmplib
 *	compiling:	to compile this, use
 *			gcc dumputmp.c utmplib.c -o dumputmp
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<utmp.h>

getf(FILE *, char , char *);

main(int ac,char **av)
{
	struct utmp	utrec;		/* one record buffer	*/
	char	        line[BUFSIZ];	/* a buffer of a line   */
	char		*cp;
	int		rv;
	char		data[10][100];
	int		i;
	int		rn = 0;


	while ( getf(stdin, '\t', data[0])  == '\t' )
	{
		rn++;
		for(i=1;i<9;i++)
		{
			if ( getf(stdin,'\t', data[i]) != '\t' )
				error("reading field",i,rn);
		}
		if ( getf(stdin, '\n', data[i]) != '\n' )
			error("reading last field",i,rn);
			
		memset(&utrec, 0, sizeof(struct utmp));

		strcpy( utrec.ut_user, data[0]);
		strcpy( utrec.ut_id  , data[1]);
		strcpy( utrec.ut_line, data[2]);
		utrec.ut_pid   = atoi(data[3]);
		utrec.ut_type  = atoi(data[4]);
		utrec.ut_exit.e_termination = atoi(data[6]);
		utrec.ut_exit.e_exit        = atoi(data[7]);
		utrec.ut_time  = atol(data[8]);
		strcpy( utrec.ut_host, data[9]);

		write(1, &utrec, sizeof(struct utmp));
	}
}

getf(FILE *fp, char term, char *buf)
{
	int	i = 0;
	int	c = 0;
	while( i < 100 && ( c = getc(fp) ) != term && c != EOF )
	{
		buf[i++] = c;
	}
	buf[i] = '\0';
	return c;
}

error(char *s,int i, int r)
{
	fprintf(stderr,"oops: %s : record %d field %d \n", s, r, i);
	exit(1);
}
