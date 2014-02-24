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


#include <stdio.h>


int main(int argc, char * argv[]){
		char * starting_dir=NULL, *name=NULL;
		char type=NULL;

		if(argc==2){
			/*pfind <dir> */
			starting_dir =  argv[1];
			//walk tree hierarchy
		}else if(argc==4){
			/* pfind <dir> -name <pattern>
			   pfind <dir> -type <filetype>
			*/
			starting_dir =  argv[1];

			if(strcmp(argv[2],"-name")){
				name = argv[3]; 
				//walk tree hierarchy with name
			}else if(strcmp(argv[2],"-type")){
				type = argv[3];
				//walk tree hierarchy with type
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
		}

		return 0;
}