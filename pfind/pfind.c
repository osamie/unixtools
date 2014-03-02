/*
* 
* pfind (Pseudo Find) 
* 
*  Purpose: 
*	   This is a utility that recursively descends a given directory tree,
*	   evaluating each file in the tree with a given expression or expressions 
*	   (passed as commandline arguments).
* 
*  Arguments:
*	starting directory, filename-or-pattern, type-of-file
*	$ pfind starting_dir [-name filename-or-pattern] [-type {f|d|b|c|p|l|s}]
*		$ pfind /home/s/smith -name core
*		$ pfind . -name ’*.c’ -type f
*		$ pfind cscie215/hw
*	 
*	 
*   Author: Osazuwa Omigie	
*   Date: February 22,2014.		
*   CSCI-E28
*   Harvard University		
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

#define		BUFF_SIZE	9000		/*Initial buffer size*/


void dostat(char *filename);
int match_type(char * entry_pathname,char type);
int match_name(char * entry_pathname, const char * pattern);
void searchdir(char * start_dir,char * name, const char type);
int get_stat(const char *filename, struct stat * info);
void parsedir(char * start_dir,char * name,const char type,char * find_results[],int * buffsize,int * current_index, int criteria_flag);
int compare(const void* str1, const void* str2);
void finalize_results(char * find_results[],int current_index,int free_start_dir_string);
void process_args(int argc, char * argv[]);
void show_usage();
void add_item(int * current_index, char * item);

static int * current_index;  /*holds the index of the results buffer*/
int * buffersize;
char * find_results[BUFF_SIZE];  //holds the final search results

int main(int argc, char * argv[]){
	process_args(argc,argv);
	return 0;
}

/**
	Process the command line arguments and call the search directory 
	function based on the given commandline arguments
**/
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

/*
	Calls the system lstat function on a file. 
	If sucessful, modifies the info stat structure to reflect the 
	stat of the file. 
*/
int get_stat(const char *filename, struct stat * info)
{
	errno=0;  //clear errno before system call
	if (lstat(filename, info) == -1){
		/* cannot do lstat	 */
		fprintf(stderr, "pfind: %s: %s\n",filename, strerror(errno)); //print to stderr
		return -1;
	} 
	return 0;	
}

/**
	Returns 1 or TRUE if the given file is of the same type; 
	FALSE or 0 otherwise. 
	The supported type checks are f,d,b,c,p,l, and s.	
**/
int match_type(char * entry_pathname, char type){
	struct stat * file_info = malloc(sizeof(struct stat));
	int mode;

	if (get_stat(entry_pathname,file_info) < 0){
		return FALSE; //cannot get stat
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
	int criteria_flag=CR_ALL, index=0, buffsize=BUFF_SIZE, free_start_dir_string = TRUE, same_name, same_all,same_type;
	// is_wildcard = (criteria_flag == CR_NONE);

	current_index = &index;  /* set the current index of the buffer to zero*/
	buffersize = &buffsize;  /*keep a reference to the buffer current buffer size*/

	if((strlen(name)==0) && (type==' ')) /* no other commandline arguments*/
		criteria_flag = CR_NONE;
	else if((strlen(name)==0) && (type > 0)) { /* only type criterion specified on commandline optiont*/
		criteria_flag =CR_TYPE;
	}else if ((strlen(name)>0) && (type==' ')){ /*only name criterion specified on commandline option*/
		criteria_flag=CR_NAME;
	}

	/*check criteria*/
	same_name = (criteria_flag==CR_NAME) && match_name(start_dir,name);
	same_type = (criteria_flag==CR_TYPE) && match_type(start_dir,type);
	same_all = (criteria_flag==CR_ALL) && match_name(start_dir,name) && match_type(start_dir,type);

	/*Check criteria for current directory*/
	if(same_name||same_type||same_all||(criteria_flag==CR_NONE)){		
		find_results[*current_index] = start_dir; 
		*current_index +=1;	
		// add_item(current_index,start_dir);
		free_start_dir_string = FALSE;
	}
	/*Start traversing the file tree*/
	parsedir(start_dir,name,type,find_results,buffersize, current_index, criteria_flag);
	finalize_results(find_results,*current_index,free_start_dir_string); /*soprt and print out results*/
}

/**
	Adds a given item to a given buffer and then increments it index. 
	If the buffer is already full, the buffer is first incremented by 
	two thirds it original size.   
**/
void add_item(int * current_index, char * item){
	// int size = buffersize; // 
	int buffsize = (sizeof(find_results)/sizeof(char *));
	int newsize = (buffsize) + ((2/3) * buffsize);

	int index = *current_index;
	if(index >= buffsize-1){
		realloc(find_results,newsize); //increase the size of the results buffer
		// printf("New Buffer size: %d******\n",*buffersize);
	}
	find_results[index] = item;
	*current_index = index+1;
	// printf("added:%s , index=%d, buffersize=%d\n",item,(int)*current_index, *buffersize);
	return;
}

/*
	Sorts the buffer and dumps its content on the output console
*/

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

/* 
	Lexicographically compares two string. 
	Returns an integer greater than, equal to, or less than 0, 
	when the string str1 is greater than,
    equal to, or less than the string s2 respectively.
*/
int compare(const void* str1, const void* str2)
{
    const char *str_a = (const char *)str1;
    const char *str_b = (const char *)str2;
    return strcmp(str_a, str_b);
}


// parse_with_type(start_dir,type); parse_with_name(start_dir,name);
void parsedir(char * start_dir,char * name,const char type,char * find_results[], int * buffersize, int * current_index, int criteria_flag){
	DIR	*dir_ptr;		/* the directory */
	struct dirent *direntp;		/* each entry	 */
	int same_type, same_name, same_all, is_match, is_current_dir,is_parent_dir;
	char * subpath, * back_slash = "/";
	errno=0; /* clear errno before system call */

	if ((dir_ptr=opendir(start_dir))==NULL ) 
		fprintf(stderr, "pfind: %s : %s\n",start_dir, strerror(errno)); //print to stderr
	else
	{
		do{
			errno = 0;		/*always clear errno before the readdir system call*/
			if ((direntp = readdir(dir_ptr)) != NULL){
				is_current_dir=!strcmp(direntp->d_name,".");
				is_parent_dir=!strcmp(direntp->d_name,"..");		

				if(is_parent_dir || is_current_dir) /* skip "." and ".." entries*/
					continue;
				
				/*create a string large enough to hold the current directory path and the name of the current dirent.d_name */
				subpath = malloc(strlen(start_dir) + strlen(direntp->d_name) + 1);
				strcpy(subpath,start_dir); 
				// if((char *)start_dir[strlen(start_dir)-1],back )
				if(strcmp(start_dir+(strlen(start_dir)-1),back_slash)!=0)
					strcat(subpath,back_slash); /*add a directory delimiter if none*/
				strcat(subpath,direntp->d_name); /*form the complete sub directory or file path*/	

				same_type = ((criteria_flag==CR_TYPE) && match_type(subpath,type));
				same_name = ((criteria_flag==CR_NAME) && match_name(direntp->d_name,name));
				same_all = ((criteria_flag==CR_ALL) && match_name(direntp->d_name,name) && match_type(subpath,type));
				is_match = same_all || same_name || same_type || (criteria_flag==CR_NONE);

				if(is_match) { // add_item(find_results,buffersize,current_index,subpath);
					find_results[*current_index] = subpath;
					*current_index +=1;					
				}

				if(match_type(subpath,'d')){ //if this is a directory, traverse it now
					parsedir(subpath,name,type,find_results,buffersize, current_index, criteria_flag);
					// index++;
				}
			}
		}while(direntp != NULL);
		// if (errno != 0) fprintf(stderr, "pfind: %s: %s\n",start_dir, strerror(errno)); //print to stderr
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
