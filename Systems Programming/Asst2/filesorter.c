/* CS 214: Systems Programming Spring 2018
 *  Professor Francisco
 *  Assignment 2: Keyspace Construction
 *  Richard Farrington netid: raf172
 *  Anirudh Tunoori netid: at813
 */

//filesorter.c
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
#include <ctype.h>
#include "filesorter.h"
#include "list.h"


int readdirect(char* dirname, List* head){
	int dread; //directory reader
	DIR* fd = opendir(dirname); 
	struct dirent* dir = readdir(fd); //this line and the next line call readdir to read through the two dots
	dir = readdir(fd);
	char* sourcepath;
	while((dir=readdir(fd)) != NULL)
	{ //while there are still files in this directory
		
		int ps = strlen(dirname)+strlen(dir->d_name)+2; //need room for the directory name, the file name, a slash and the null terminator ps = pathsize
		sourcepath = (char*)malloc(sizeof(char)*ps); //the actual path
		bzero(sourcepath,ps);
		strncpy(sourcepath,dirname,strlen(dirname));
		sourcepath[strlen(dirname)] = '/';
		strncat(sourcepath,dir->d_name,strlen(dir->d_name));
		printf("sourcepath:%s\n",sourcepath);
		int fr; //filereader
		int fi = open(sourcepath,O_RDWR);
		if(fi < 0)
		{	
			
			int e = errno;
			printf("%s\n",strerror(e));
			if(e == 21)
			{ //is a directory, recursive call
			
				dread = readdirect(sourcepath, head);
				if(dread != 0)
				{
					free(sourcepath);
					sourcepath = NULL;
					return -1;
				}
			}

		}
		else{
			char* currfile = (char*)malloc(sizeof(char*)*(strlen(dir->d_name)+1));//currfile gets passed into readfile to make linked list 
			currfile[strlen(dir->d_name)]='\0';
			strncpy(currfile,dir->d_name,strlen(dir->d_name));
			fr = readfile(fi,currfile, head);
			close(fi);
			if(fr != 0)
			{
				free(currfile);
				currfile = NULL;
				free(sourcepath);
				sourcepath = NULL;
				return -1;
			}
		}
		free(sourcepath);
		sourcepath = NULL;
	}
	
	return 0;
}

int readfile(int fd, char* filename, List* head){
	int bufsize = 50; //size of buffer
	char* buffer = (char*)malloc(sizeof(char)*bufsize);//buffer
	bzero(buffer,bufsize);
	int consecalnum=0, i; //consecalnum = consecutive alphanumeric characters, i is a counter
	int nc; //nullcheck
	char* tok; //token
	int r = 0; //return value of read(fd,buffer,bufize-1)
	int filesize = lseek(fd,0,SEEK_END); //amount of bytes in file
	lseek(fd,0,SEEK_SET); //set pointer back to the beginning of the file
	int bytesread = 0; //amount of bytes read
	do
	{		
		r = read(fd,buffer,bufsize-1);
		if(r<=0 && bytesread<filesize)
		{ //read is being fickle but you have not read every byte
			lseek(fd,bytesread,0); //this is to ensure that everything gets read
			r = read(fd,buffer,bufsize-1);
		}
		else if(r<=0 && bytesread == filesize)
		{ //reached actual end of file
			break;
		}
		
		for(i=0; i<bufsize-1; i++)
		{ // you gotta fix this
			bytesread += 1; 
		
			if( (i ==0 && isalpha(buffer[i]) )|| (i>0 && isalnum(buffer[i]) ) )
			{ //the conditional character checks if the first char is a letter or if the following characters are alphanumeric
				buffer[i] = tolower(buffer[i]); //change the char to lowercase if it is a letter
				consecalnum += 1;	
			}
			
			else{
								
				
				
				if(consecalnum > bufsize-1)
				{ //the token is going to be larger than the buffer size
					char* temp = tok; //placeholder
					tok = -1;
					tok = realloc(temp,sizeof(char)*consecalnum+i);
					nc = nullcheck(tok);
					if(nc == -1)
					{	// this will eventually lead to all malloced space being destroyed.
						free(buffer);
						buffer = NULL;
						return -1; //error code
					}
					tok[consecalnum+i-1] = '\0';
					strncat(tok,buffer,i);
					printf("this is tok:%s\n",tok);
					printf("make that list fool\n");
					makeNode(head, tok, filename);
				}
				else if(consecalnum == bufsize-1)
				{	//the token is the same size as the buffer the token gets set outside of the loop
					
					printf("make the node. print consecalnum=%d\n",consecalnum);
					printf("this is tok:%s\n",tok);
					printf("make the list fool\n");
					makeNode(head, tok, filename);
				}
				else if(consecalnum < bufsize-1 && consecalnum != 0)
				{ //a token needs to be made and it is smaller than the size of a buffer
					tok = (char*)malloc(sizeof(char)*i+1);
					tok[i] = '\0';
					strncpy(tok,buffer,consecalnum);
					
					printf("this is tok:%s\n",tok);
					printf("make that list fool\n");
					makeNode(head, tok, filename);
				}
				consecalnum = 0; // gotta start this over again
				bzero(buffer,bufsize); //clean the buffer
				break;
			}			
			
		} 
		lseek(fd,bytesread,0); //move pointer to index bytesread from the beginning of the file
		if(consecalnum == bufsize-1)
		{ //you need to make a string here becuase the entire buffer was read
			tok = (char*)malloc(sizeof(char)*bufsize);
			tok[bufsize-1] = '\0';
			strncpy(tok,buffer,bufsize-1);
			
		}	
		else if(consecalnum > bufsize-1)
		{ //the entire buffer has been read at least twice, so you have to realloc over here
			char* temp = tok;
			tok = -1;
			tok = realloc(temp,sizeof(char)*consecalnum+1);
			nc = nullcheck(tok);
			if(nc != 0)
			{
				free(buffer);
				buffer = NULL;
				return -1; //error code
			}
			tok[consecalnum] = '\0';
			strncat(tok,buffer,strlen(buffer)+1);
			
		}
		bzero(buffer,bufsize);
	}while(r>0);

	free(buffer);
	buffer = NULL;
	
	return 0;
}


int nullcheck(char* token){
	if(token == -1)
	{
		int i =0;
		while(i<100 && token==-1)
		{
			i++;
		}
		if(token == -1)
		{
			printf("Error: Realloc has taken too long. Space called on the heap is being freed.\n");
			return -1;
		}
		else if(token == NULL)
		{
			printf("Error: Realloc returned a null pointer. Space called on the heap is being freed.\n");
			return -1;
		}
		else{
			return 0;
		}
	}
	else if(token == NULL)
	{
		printf("Error: Realloc returned a null pointer. Space called on the heap is being freed.\n");
		return -1;
	}
	else{
		return 0;
	}
}


