/*
* 
* pfind: 
* 
*  Purpose: 
*	   
* 
*   Args:
*	starting directory, filename-or-pattern, type-of-file
*	$ pfind starting_dir [-name filename-or-pattern] [-type {f|d|b|c|p|l|s}]
*		$ pfind /home/s/smith -name core
*		$ pfind . -name ’*.c’ -type f
*		$ pfind cscie215/hw
*	 
*	 
*	
*	 
*   Author: Osazuwa Omigie	
*   Date: February 22,2014.		
*   CSCI-E28
*   Harvard University		
*		
*	
*/


#include 	<stdio.h>
#include	<sys/types.h>
#include	<dirent.h>
#include	<sys/stat.h>
#include	<string.h>


void do_ls(char dirname[]);
void dostat(char *filename);

int main(int argc, char * argv[]){
		char * starting_dir=NULL, *name=NULL;
		char type;

		if(argc==1)
			do_ls(".");
		else if(argc==2){
			/*pfind <dir> */
			starting_dir =  argv[1];
			do_ls(starting_dir);
		}else if(argc==4){
			/* pfind <dir> -name <pattern>
			   pfind <dir> -type <filetype>
			*/
			starting_dir =  argv[1];

			if(strcmp(argv[2],"-name")){
				name = argv[3]; 
				//walk tree hierarchy with name
			}else if(strcmp(argv[2],"-type")){
				type = atoi(argv[3]);
				//walk tree hierarchy with type
			}
			//return 
		}else if(argc==6){
			/* */
			if(strcmp(argv[2],"-name")){
				name = argv[3]; 
				if(strcmp(argv[4],"-type")){
					type = atoi(argv[5]);
					//walk tree hierarchy with name and type
				}
				//error handling: print usage
			}
				//walk tree hierarchy with name
			}else if(strcmp(argv[2],"-type")){
				type = atoi(argv[3]);
				if(strcmp(argv[4],"-name")){
					type = atoi(argv[5]);
					//walk tree hierarchy with name and type
				}
				//error handling: print usage
			}
		
		return 0;
}


/*
parse_dir(){
	if(dir_entry is directory)
		parsedir(dir_entry)
	else 
		if(dir_entry.filename == filename)
			printf(dir_entry.filname)
}

*/

void do_ls( char dirname[] )
/*
 *	list files in directory called dirname
 */
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */

	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr,"pfind: cannot open %s\n", dirname);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL )
			printf("%s\n",direntp->d_name);
		closedir(dir_ptr);
	}
}