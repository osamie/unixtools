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
#include	<stdlib.h>
#include 	<fnmatch.h>

void do_ls(char dirname[]);
void dostat(char *filename);
int match_type(struct dirent * entry,char type);
int match_name(struct dirent * entry, const char * pattern);
void parse_dir(char * start_dir,char * name, char type);
void parse_with_type(char * start_dir,char type);
void parse_with_name(char * start_dir,char * name);
int get_stat(const char *filename, struct stat * info);


int main(int argc, char * argv[]){
		char * starting_dir=NULL, *name=NULL;
		char * type;
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
			if(!strcmp(argv[2],"-name")){
				name = argv[3];
				parse_dir(starting_dir,name,"");
				//walk tree hierarchy with name
			}else if(!strcmp(argv[2],"-type")){
				type = argv[3];
				// printf("type:%c\n",type[0]);
				parse_dir(starting_dir,"",type[0]);
			}
			//return 
		}else if(argc==6){
			/* */
			if(strcmp(argv[2],"-name")){
				name = argv[3]; 
				if(strcmp(argv[4],"-type")){
					type = argv[5];
					//walk tree hierarchy with name and type
				}
				//error handling: print usage
			}
				//walk tree hierarchy with name
			}else if(strcmp(argv[2],"-type")){
				type = argv[3];
				if(strcmp(argv[4],"-name")){
					type = argv[5];
					//walk tree hierarchy with name and type
				}
				//error handling: print usage
			}
		
		return 0;
}


int get_stat(const char *filename, struct stat * info)
{
	if (stat(filename, info) == -1){
		/* cannot stat	 */
		perror( filename );			/* say why	 */
		return -1;
	} 
	return 0;	
}

int match_type(struct dirent * entry, char type){
	struct stat * file_info = malloc(sizeof(struct stat));
	int mode;

	// printf("HERE: %s\n",entry->d_name);

	if (get_stat(entry->d_name,file_info) < 0){
		return 0;
	}
	mode = file_info->st_mode;

	// supported types: f|d|b|c|p|l|s
	switch(type){
		case 'f':
			return S_ISREG(mode);
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
	free(file_info);
}


// int fnmatch(const char *pattern, const char *string, int flags);
/*
	Returns zero if matching filename or pattern
*/
int match_name(struct dirent * entry, const char * pattern){
	// int is_a_match=1;
	char * filename = entry->d_name;
	if (fnmatch(pattern, filename, 0) !=0){
		perror(pattern);
		return 1;
	}
	return 0;
}


void parse_dir(char * start_dir,char * name,char type){
	if((strlen(name)==0) && (type > 0)) {
		parse_with_type(start_dir,type);
	}else if ((strlen(name)>0) && (type==0)){
		
		parse_with_name(start_dir,name);
	}
	printf("parsing with name criteria");
		

// match_name(struct dirent * entry, const char * pattern)

	//entry = read the directory
	// if(dir_entry is directory)
	// 	parsedir(dir_entry)
	// else 
	// 	if(match_type(dirname,type))
	// 		printf(dir_entry.filname)
}

void parse_with_type(char * start_dir,char type){

	DIR	*dir_ptr;		/* the directory */
	struct dirent *direntp;		/* each entry	 */

	if ( (dir_ptr=opendir(start_dir))==NULL )
		fprintf(stderr,"pfind: cannot open %s\n", start_dir);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL ){
			/*check if it is a file*/
			if(match_type(direntp,type)){
				printf("%s\n",direntp->d_name);	
			}
		}
		closedir(dir_ptr);
	}

}

void parse_with_name(char * start_dir,char * name){
	DIR	*dir_ptr;		/* the directory */
	struct dirent *direntp;		/* each entry	 */

	if ( (dir_ptr=opendir(start_dir))==NULL )
		fprintf(stderr,"pfind: cannot open %s\n", start_dir);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL ){
			/*check if it is a file*/
			if(match_name(direntp,name)){
				printf("%s\n",direntp->d_name);	
			}
		}
		closedir(dir_ptr);
	}

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