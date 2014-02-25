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
int match_type(struct dirent * entry,char type);
void parse_dir(char * start_dir,char * name, char type);
int get_stat(char *filename, struct stat * info);

int main(int argc, char * argv[]){
		char * starting_dir=NULL, *name=NULL;
		char type;
		struct stat * info;

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
				parse_dir(starting_dir,NULL,type);
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


int get_stat(char *filename, struct stat * info)
{
	if (lstat(filename, info) == -1){
		/* cannot stat	 */
		perror( filename );			/* say why	 */
		return -1;
	} 
	return 0;	
}


/*

parse_dir(dirname,name,type){

	//entry = read the directory
	if(dir_entry is directory)
		parsedir(dir_entry)
	else 
		if(match_type(dirname,type))
			printf(dir_entry.filname)
}


*/
int match_type(struct dirent * entry, char type){
	struct stat * file_info;
	int mode;

	if (get_stat(entry->d_name,file_info) < 0){
		perror(entry->d_name);
		return 0;
	}
	mode = file_info->st_mode;


	// supported types: f|d|b|c|p|l|s
	switch(type){
		case 'f':
			return ISREG(mode);
		case 'd':
			return S_ISDIR(mode);
		case 'b':
			return S_ISBLK(mode);
		case 'c':
			return S_ISCHR(mode);
		case 'p':
			return S_ISFIFO(mode);
		case 'l':
			return S_ISLNK(mode);
		case 's':
			return S_ISSOCK(mode);
		default: 
			return 0;
	}
}



void parse_dir(char * start_dir,char * name,char type){
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */

	if ( ( dir_ptr = opendir(start_dir))==NULL )
		fprintf(stderr,"pfind: cannot open %s\n", start_dir);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL ){
			/*check if it is a file*/
			if(match_type(direntp,'d')){
				printf("%s\n",direntp->d_name);	
			}
		}
		closedir(dir_ptr);
	}


	//entry = read the directory
	// if(dir_entry is directory)
	// 	parsedir(dir_entry)
	// else 
	// 	if(match_type(dirname,type))
	// 		printf(dir_entry.filname)
}

/*
 *	list files in directory called dirname
 */
void do_ls(char dirname[]){
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