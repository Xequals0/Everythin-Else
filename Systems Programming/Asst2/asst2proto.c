/* CS 214: Systems Programming Spring 2018
 *  Professor Francisco
 *  Assignment 2: Keyspace Construction
 *  Richard Farrington netid: raf172
 *  Anirudh Tunoori netid: at813
 */

//asst2proto.c
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "filesorter.h"
#include "list.h"

int main(int argc, char** argv){
if(argc != 3)
{ //first arg is output file, second is file/directory to run through argc should be 3. fix this later
	printf("Usage: ./invertedIndex <inverted-index filename> <file path to a directory or a file name> \n");
	return -1;
}
List* l = NULL;
int outputfile, e;
outputfile = open(argv[1],O_CREAT| O_RDWR | O_EXCL,00766);
if(outputfile<0)
{
	e = errno;
	if(e == 17)
	{
		int* answer;
		printf("This file already exists. Would the user like to overwrite the file? (Answer 0 for no and 1 for yes)\n");
		scanf("%d",answer);
		while(*answer != 0 || *answer != 1)
		{
			printf("Enter an integer (0 for no, 1 for yes).\n");
			scanf("%d",answer);
		}
		if(answer == 0)
		{
			return 1;
		}
		else{
			outputfile = open(argv[1],O_CREAT | O_RDWR | O_TRUNC, 00766);
			if(outputfile<0)
			{
				e = errno;
				printf("There was an error in overwriting the file. This is your error: %s.\n",strerror(e));
				return -1;
			}
		}
		
	}
	else{
		printf("The user has committed this error: %s.\n",strerror(e));	
		return 1;	
	}

}



int f = open(argv[2],O_RDWR);
int dreader; //directory reader

if(f<0)
{
	e = errno;
	close(f);
	if(e == 21)
	{ //is directory
		dreader = readdirect(argv[2], l);
		if(dreader != 0)
		{
			return -1;
		}
	}
	else if(e == 2)
	{ //does not exist
		printf("The file/directory does not exist.\n");
		return -1;
	}	
	else{ //uhh
		printf("User has committed the following error: %s.\n",strerror(e));
		return -1;
	}


}

else{
	int fr; //filereader
	char* filename = (char*)malloc(sizeof(char)*(strlen(argv[2])+1));
	filename[strlen(argv[2])] = '\0';
	strncpy(filename,argv[2],strlen(argv[2]));
	fr = readfile(f,filename, l);
	//print stuff;
        char* outputName = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
	outputName[strlen(argv[1])] = '\0';
	strncpy(outputName,argv[1],strlen(argv[1]));
        printIndex(l, outputName);

	free(filename);
	filename = NULL;
        free(outputName);
        outputName = NULL;
	close(f);
        close(outputfile);
	if(fr == -1)
	{
		return -1;
	}
}


return 0;
}
