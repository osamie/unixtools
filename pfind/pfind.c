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
#define		CR_NONE		99

#define		FALSE		0
#define		TRUE		1

#define		BUFF_SIZE	9000


void do_ls(char dirname[]);
void dostat(char *filename);
int match_type(char * entry_pathname,char type);
int match_name(char * entry_pathname, const char * pattern);
void searchdir(char * start_dir,char * name, const char type);
void parse_with_type(char * start_dir,char type);
void parse_with_name(char * start_dir,char * name);
int get_stat(const char *filename, struct stat * info);
void parsedir(char * start_dir,char * name,const char type,char * find_results[],int * current_index, int criteria_flag);
int compare(const void* str1, const void* str2);
void finalize_results(char * find_results[],int current_index,int free_start_dir_string);
void process_args(int argc, char * argv[]);
void show_usage();

static int * current_index;  /*holds the index of the results buffer*/

int main(int argc, char * argv[]){
		// struct stat * info;
		// printf("HERE:");
		process_args(argc,argv);
		return 0;
}

void process_args(int argc, char * argv[]){
	char * starting_dir, *name=NULL, * type;

	starting_dir =(argc>=2)? argv[1] : ".";
	if(argc<=2){
			/*pfind <dir> */
			searchdir(starting_dir,"*",' ');
	}else if(argc==4){/* pfind <dir> -name <pattern> OR pfind <dir> -type <filetype> */			
		if(!strcmp(argv[2],"-name")){
			name = argv[3];
			searchdir(starting_dir,name,' ');
		}else if(!strcmp(argv[2],"-type")){
			type = argv[3];
			searchdir(starting_dir,"",type[0]);
		}else
			show_usage();
	}else if(argc==6){
		/* */
		if(!strcmp(argv[2],"-name")){
			name = argv[3]; 
			if(!strcmp(argv[4],"-type")){
				type = argv[5];
				searchdir(starting_dir,name,type[0]);		
			} else {show_usage();}
		} else if(!strcmp(argv[2],"-type")){
			type = argv[3];
			if(!strcmp(argv[4],"-name")){
				name = argv[5];
				searchdir(starting_dir,name,type[0]);
			}else 
				show_usage();
		}
	}
}

int get_stat(const char *filename, struct stat * info)
{
	errno=0;  //clear errno before system call
	if (lstat(filename, info) == -1){
		/* cannot do lstat	 */

		if(errno==EACCES){
			printf("%s: Permissions issue\n",filename);
		}else{
			printf("%s: errno %d\n",filename,errno);
		}

		// perror( filename );			/* say why	 */
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


void searchdir(char * start_dir,char * name,const char type){
	DIR	*dir_ptr;		/* the directory */
	struct dirent *direntp;		/* each entry	 */
	char * find_results[BUFF_SIZE];  //holds the final search results
	int criteria_flag=CR_ALL, index=0, free_start_dir_string = TRUE, same_name, same_all,same_type;

	same_name = (criteria_flag==CR_NAME) && match_name(start_dir,name);
	same_type = (criteria_flag==CR_TYPE) && match_type(start_dir,type);
	same_all = (criteria_flag==CR_ALL) && match_name(start_dir,name) && match_type(start_dir,type);

	current_index = &index;

	if((strlen(name)==0) && (type==' '))
		criteria_flag = CR_NONE;
	else if((strlen(name)==0) && (type > 0)) {
		criteria_flag =CR_TYPE;
	}else if ((strlen(name)>0) && (type==' ')){
		criteria_flag=CR_NAME;
	}

	if(same_name||same_type||same_all) {		
		find_results[*current_index] = start_dir;
		*current_index +=1;	
		free_start_dir_string = FALSE;
	}

	parsedir(start_dir,name,type,find_results,current_index, criteria_flag);
	finalize_results(find_results,*current_index,free_start_dir_string);
}


void finalize_results(char * find_results[],int current_index,int free_start_dir_string){
	int i;
	/* sort find_results */
	int result_size = sizeof(find_results)/sizeof(char *);
	qsort(find_results,result_size,sizeof(char *),compare); /*sort the results*/
	for(i=0;i<current_index;i++){
		printf("%s\n",find_results[i]);
		if(free_start_dir_string)
			free(find_results[i]);
	}
}

int compare(const void* str1, const void* str2)
{
    const char *str_a = (const char *)str1;
    const char *str_b = (const char *)str2;
    return strcmp(str_a, str_b);
}


// parse_with_type(start_dir,type); parse_with_name(start_dir,name);
void parsedir(char * start_dir,char * name,const char type,char * find_results[],int * current_index, int criteria_flag){
	DIR	*dir_ptr;		/* the directory */
	struct dirent *direntp;		/* each entry	 */
	char * sub_directories[BUFF_SIZE];  //holds the final search results
	int index = 0,same_type, same_name, same_all, is_match, is_current_dir,is_parent_dir;
	char * subpath, * back_slash = "/";

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

				is_current_dir=!strcmp(direntp->d_name,".");
				is_parent_dir=!strcmp(direntp->d_name,"..");		

				if(is_parent_dir || is_current_dir)
					continue;
				
				same_type = ((criteria_flag==CR_TYPE) && match_type(subpath,type));
				same_name = ((criteria_flag==CR_NAME) && match_name(direntp->d_name,name));
				same_all = ((criteria_flag==CR_ALL) && match_name(direntp->d_name,name) && match_type(subpath,type));
				is_match = same_all || same_name || same_type;

				if(is_match) {
					find_results[*current_index] = subpath;
					*current_index +=1;	
				}

				if(match_type(subpath,'d')){ //if this is a directory, traverse it
					// printf("subpath: %s\n", subpath);
					parsedir(subpath,name,type,find_results,current_index, criteria_flag);
					index++;
				}
			}
		}while(direntp != NULL);

		if (errno != 0)
        	perror("error reading directory");

		closedir(dir_ptr);
	}
}

void show_usage(){
	printf ("pfind [path] [-name <expression>] [-type <char: file_type>]\n");
	printf ("where,");
	printf("\t\tpath\tThis is the root of the directory tree to be traversed. \n"); 
	printf("\t\t-name\tprintout files or directories that match the following expression. \n"); 
	printf("\t\t-type\tprintout files that match the following file type. \n"); 
	return;
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