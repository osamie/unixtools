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
#include 	<errno.h>


//search criteria flags
#define		CR_ALL		0
#define 	CR_NAME		1
#define 	CR_TYPE		2

#define		FALSE		0
#define		TRUE		1

#define		BUFF_SIZE	1500


void do_ls(char dirname[]);
void dostat(char *filename);
int match_type(char * entry_pathname,char type);
int match_name(char * entry_pathname, const char * pattern);
void parse_dir(char * start_dir,char * name, const char type);
void parse_with_type(char * start_dir,char type);
void parse_with_name(char * start_dir,char * name);
int get_stat(const char *filename, struct stat * info);
void parse_dir_helper(char * start_dir,char * name,const char type,char * find_results[],int * current_index, int criteria_flag);

static int * current_index;

int main(int argc, char * argv[]){
		char * starting_dir, *name=NULL;
		char * type;
		struct stat * info;
		// printf("HERE:");

		starting_dir =(argc>=2)? argv[1] : NULL;
		if(argc==1)
			do_ls(".");
		else if(argc==2){
			/*pfind <dir> */		
			do_ls(starting_dir);
		}else if(argc==4){
			/* pfind <dir> -name <pattern>
			   pfind <dir> -type <filetype>
			*/			
			if(!strcmp(argv[2],"-name")){
				name = argv[3];
				parse_dir(starting_dir,name,' ');
			}else if(!strcmp(argv[2],"-type")){
				type = argv[3];
				parse_dir(starting_dir,"",type[0]);
			}
		}else if(argc==6){
			/* */
			if(!strcmp(argv[2],"-name")){
				name = argv[3]; 
				if(!strcmp(argv[4],"-type")){
					type = argv[5];
					parse_dir(starting_dir,name,type[0]);		
				}
				//TODO error handling: print usage
			} else if(!strcmp(argv[2],"-type")){
				type = argv[3];
				if(!strcmp(argv[4],"-name")){
					name = argv[5];
					parse_dir(starting_dir,name,type[0]);
				}
				//TODO error handling: print usage
			}
		}		
		return 0;
}


int get_stat(const char *filename, struct stat * info)
{
	if (lstat(filename, info) == -1){
		/* cannot stat	 */
		perror( filename );			/* say why	 */
		return -1;
	} 
	return 0;	
}

int match_type(char * entry_pathname, char type){
	struct stat * file_info = malloc(sizeof(struct stat));
	int mode;

	// printf("HERE: %s\n",entry->d_name);

	if (get_stat(entry_pathname,file_info) < 0){
		return FALSE;
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
			return FALSE;
	}
	free(file_info);
}


/*
	Returns zero if matching filename or pattern
*/
int match_name(char * entry_pathname, const char * pattern){
	// char * filename = entry->d_name;
	if (fnmatch(pattern, entry_pathname, 0) !=0){
		return FALSE;
	}
	return TRUE;
}


void parse_dir(char * start_dir,char * name,const char type){
	DIR	*dir_ptr;		/* the directory */
	struct dirent *direntp;		/* each entry	 */
	int criteria_flag=CR_ALL;  
	char * find_results[BUFF_SIZE];  //holds the final search results
	int index=0;
	int free_start_dir_string = TRUE; //flag 
	current_index = &index;

	if((strlen(name)==0) && (type==' '))
		return;
	else if((strlen(name)==0) && (type > 0)) {
		criteria_flag =CR_TYPE;
	}else if ((strlen(name)>0) && (type==' ')){
		criteria_flag=CR_NAME;
	}

	if((criteria_flag==CR_TYPE) && match_type(start_dir,type)) {		
		find_results[*current_index] = start_dir;
		*current_index +=1;	
		free_start_dir_string = FALSE;
	}else if((criteria_flag==CR_NAME) && match_name(start_dir,name)){
		find_results[*current_index] = start_dir;
		*current_index +=1;
		free_start_dir_string = FALSE;
	}else if ((criteria_flag==CR_ALL) && match_name(start_dir,name) && match_type(start_dir,type)){
		find_results[*current_index] = start_dir;
		*current_index +=1;
		free_start_dir_string = FALSE;
	}

	parse_dir_helper(start_dir,name,type,find_results,current_index, criteria_flag);
	int i;
	//TODO: sort find_results 
	for(i=0;i<index;i++){
		printf("%s\n",find_results[i]);
		if(free_start_dir_string)
			free(find_results[i]);
	}
}


// parse_with_type(start_dir,type); parse_with_name(start_dir,name);
void parse_dir_helper(char * start_dir,char * name,const char type,char * find_results[],int * current_index, int criteria_flag){
	DIR	*dir_ptr;		/* the directory */
	struct dirent *direntp;		/* each entry	 */
	char * sub_directories[BUFF_SIZE];  //holds the final search results
	int index = 0;
	int is_parent,is_grandparent;
	char * back_slash = "/";
	char * subpath;

	if ((dir_ptr=opendir(start_dir))==NULL ) 
		perror("could not open directory"); //TODO: output dirname
		// fprintf(stderr,"pfind: cannot open %s, error: %d \n", start_dir,errno);
		
	else
	{
		do{
			errno = 0;		/*always clear errno before the readdir system call*/
			if ((direntp = readdir(dir_ptr)) != NULL){
				subpath = malloc(strlen(start_dir) + strlen(direntp->d_name) + 1);
				strcpy(subpath,start_dir);

				// if((char *)start_dir[strlen(start_dir)-1],back )
				if(strcmp(start_dir+(strlen(start_dir)-1),back_slash)!=0)
					strcat(subpath,back_slash); //add a directory delimiter 
				strcat(subpath,direntp->d_name);	

				is_parent=!strcmp(direntp->d_name,".");
				is_grandparent=!strcmp(direntp->d_name,"..");		

				if(is_grandparent || is_parent)
					continue;
				
				/*check if it is a file*/
				if((criteria_flag==CR_TYPE) && match_type(subpath,type)) {
					//add to find results and increment index
					find_results[*current_index] = subpath;
					*current_index +=1;	
				}else if((criteria_flag==CR_NAME) && match_name(direntp->d_name,name)){
					find_results[*current_index] = subpath;
					*current_index +=1;
				}else if ((criteria_flag==CR_ALL) && match_name(direntp->d_name,name) && match_type(subpath,type)){
					find_results[*current_index] = subpath;
					*current_index +=1;
				}

				if(match_type(subpath,'d')){ //if this is a directory, traverse it
					// printf("subpath: %s\n", subpath);
					parse_dir_helper(subpath,name,type,find_results,current_index, criteria_flag);
					index++;
				}
			}
		}while(direntp != NULL);

		if (errno != 0)
        	perror("error reading directory");

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