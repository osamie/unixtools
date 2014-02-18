/*
*
* 
* Unix user accounting tool
*	
* Author: Osazuwa Omigie	
*		
* CSCI-E28
* Harvard University
*	
*/
#include <stdio.h>

#include <utmp.h>

#ifndef __USE_GNU
#warning __USE_GNU undefined
#endif

#define _GNU_SOURCE
#include <utmpx.h>
#undef _GNU_SOURCE
 

#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>


#include <fcntl.h>
#include <time.h>
//#include "utmplib.h" /* include interface def for utmplib.c */

#define DEBUG 0 /*Debug mode. Enable:1,Disable:0*/

struct elem{
	struct utmpx value;
	struct elem * next;	
};
typedef struct elem linked_list; 

void show_usage();
double get_connect_time(char *username, char *filename);
time_t login_entry(char * username, struct utmpx * entry);
time_t get_logout_time(struct utmpx * login_entry); 
void dump_output(double connect_time,char * username);
void add_to_list(linked_list * head, struct utmpx entry);
void print_list(linked_list * head);


int main(int argc, char * argv[]){
	char * username = NULL; //= malloc(UT_NAMESIZE * sizeof(char));
	char * filename = NULL; //= malloc(32 * sizeof(char));

	if(argc==2){
		/* # aac <username>*/
		username = argv[1];
		dump_output(get_connect_time(username,filename),username);
	}else if(argc==4){
		//check for flag
		if(strcmp(argv[1],"-f")==0){
			filename = argv[2];
			username = argv[3];
		}else if(strcmp(argv[2],"-f")==0){
			filename = argv[3];
			username = argv[1];
		}else{
			show_usage();
			return 0;
		}
		dump_output(get_connect_time(username,filename),username);
	}else{
		show_usage();
		return 0;
	}
	return 0;
}


/*	
	time_t total=0;
	store all entries of username in list
	Iterate through list and find corresponding logouts..
	
	find corresponding logouts(login_utmp):	
		getutline(login_utmp)

	for each corresponding logout entry, 
		total + (logout-login)	 

*/




/*
* Gets the connection duration in seconds, converts
* it to decimal time and outputs it to the console
* 
*/
void dump_output(double connect_time,char * username){
	/*convert seconds to decimal hours*/
	connect_time = connect_time/3600; 

	printf("\ttotal\t  %.2f\n",connect_time);		
}


/**
* Returns the connect time of the  
*
*/
double get_connect_time(char *username, char *filename){
	if(username==NULL){
		printf("\nget_connect_time: username is NULL \n");
		return -1;
	}	
	if(filename!=NULL){
		utmpxname(filename); /*set the utmp file to be accessed*/
	}else{
		//use system default wtmpx file
		utmpxname(WTMP_FILE);
	}
	
	linked_list * curr;
	linked_list * head;
	//struct utmpx;
	head = NULL;
	struct utmpx * entry = malloc(sizeof(struct utmpx));
	struct utmpx * new_entry = malloc(sizeof(struct utmpx));
	
	setutxent(); /*set file pointer at the beginning*/
	
	//TODO: Build a list (called head) of login entries for this user 
	while(login_entry(username,entry)!=0){
			//printf("entry time: %.2f\n",(float)entry->ut_time);
		memcpy(new_entry,entry,sizeof(struct utmpx));
		//add_to_list(head,*entry); //adds to the user login entries
		linked_list *curr=(linked_list *)malloc(sizeof(linked_list));
		curr->value=*new_entry;
		curr->next=head;
		head=curr;
		//printf("new time: %.2f\n",(float)(head->value).ut_time);
		if(head==NULL)
			printf("empty list");
	}
	
	if(DEBUG) print_list(head);
	
	time_t logout_time,login_time;
	time_t total = 0;
	
	/*
	TODO: iterate through list of login entries,head, and 
	and get their corresponding logout times
	*/
	curr = head;
	while(curr!=NULL){
		struct utmpx * curr_value; 
		curr_value = &(curr->value);
		login_time=curr_value->ut_time;
		logout_time = get_logout_time(curr_value);
		if(logout_time==0){
			if(DEBUG){
				//printf("login:%f\tlogout:%f\n",(float)login_time,(float)logout_time);
			}
			/*use current time, if a logout entry is not found*/
			time(&logout_time);
		}else if(login_time > logout_time){
			curr = curr->next;		
			continue;
		}	
		total+= (logout_time - login_time); 
		curr = curr->next; 
	}
	/*
		TODO: For each login entry call logout_time and do
		total+=login_time-logout_time 
		If logout_time = 0, use current_time
	*/
	
	endutxent(); /*close utmpx file*/
	
	free(entry);
	return (double)total;
}



void print_list(linked_list * head){
	linked_list *curr;
	curr=head;
	while(curr){
		printf("%s logged in at %.2f on terminal %s\n",(curr->value).ut_user,(float)(curr->value).ut_time,(curr->value).ut_line);
		curr=curr->next;
	}
}

/*
* Adds a given utmpx to a linked list 
**/
void add_to_list(linked_list * head, struct utmpx entry){
	linked_list *curr=(linked_list *)malloc(sizeof(linked_list));
	curr->value=entry;
	curr->next=head;
	head=curr;
	printf("entry time: %.2f\n",(float)entry.ut_time);
	printf("new time: %.2f\n",(float)(head->value).ut_time);
	return;
}

/*
* Given a specific username, searches the utmp file for user's initial login time 
* Returns 0 if not found and -1 if error detected
*/
time_t login_entry(char * username, struct utmpx * entry){
	struct utmpx * temp_entry; 
	time_t login_time=0 ; //= malloc(sizeof(time_t)); 

	if (strlen(username)==0) {
		return -1;	
	}
	while((temp_entry=getutxent())!=NULL){
		*entry = *temp_entry;
		//check the username and entry type
		int same_username = (strcmp(entry->ut_user,username)==0);
		
		if((entry->ut_type==USER_PROCESS) && same_username){
			login_time = entry->ut_time;
			//printf("found entry! %.2f",(float)login_time);
			break;
		}
	}
	return login_time;
}

/*
* Given a specific username, searches the utmp file for user's initial logout time
* Returns 0 if not found and -1 if error detected
*/
time_t get_logout_time(struct utmpx * login_entry){
	setutxent(); /*set file pointer at the beginning*/
	struct utmpx * entry; 
	time_t logout_time=0 ; 
		
	while((entry=getutxent())!=NULL){
		//check if it is the same line
		int is_same_line = (strcmp(entry->ut_line,login_entry->ut_line)==0);
		if (login_entry->ut_time > entry->ut_time) {
			continue;
		}else if((entry->ut_type==DEAD_PROCESS) && is_same_line){
			logout_time = entry->ut_time;
			printf("");
			break;
		}
	}
	return logout_time;
}



void show_usage() {
	printf("\nUsage:\n\taac <username>\n");
	printf("\taac -f <filename> <username>\n");
	printf("\taac <username> -f <filename>\n");
}

