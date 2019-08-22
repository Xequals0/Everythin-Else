/* CS 214: Systems Programming Spring 2018
 *  Professor Francisco
 *  Assignment 1: To Build A Better Malloc
 *  Richard Farrington netid: raf172
 *  Anirudh Tunoori netid: at813
 */

//mymalloc.c
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "mymalloc.h"

static char mem[5000];

/*mem is the block
 the ptr is going to iterate through the block as needed
 our metadata will be comprised of a short (used to indicate number of blocks in an area) and a character(the character will be 'y' if
 the space is available and 'n'	if it is not this means that every time I allocate a new space, I am taking away only three bytes*/

void* mymalloc(int x, char* file, int linenum){
    if(x>4997)
    { //assuming it is available, the most space that can be given is 4997 bytes
        printf("User is asking for more space than what can be given. file: %s line no.: %d\n", file, linenum);
        return NULL;
    }
    else if(x==0)
    {
        printf("User is requesting a pointer with no bytes. file: %s line no.: %d\n", file, linenum);
        return NULL;
    }
   
    short z;
    z = (short)x; 
    
    if( (mem[0] != 'y') && (mem[0] != 'n') )
    {	//this means that malloc has been called for the first time. we create a chunk of metadata that will be in use and another available chunk of data
        mem[0] = 'n'; //this indicates that the first chunk of metadata is in use
        short * val1 = (short*)(&mem[1]); //val1 is a short pointer that will store the short portion of the metadata
        *val1 = z; //the first block is equal to z, which is what the user requested
        
        mem[z+3] = 'y'; //this is the next available chucnk of metadata
        
        short * val2 = (short*) (&mem[z+4]); //val2 is the short that is going to be stored in the next available chunk of metadata.
        *val2 = 4997-z-3; //4997 is derived from the fact that each chunk of metadata is 3 bytes, so we can have at most 4997 bytes available
       
        return mem+3; //we want to return the actual chunk of data that the user asked for
    }
    short * val3; //this is a temp pointer variable
    char * ptr = mem; //ptr is going to iterate through the block
    short* val4; //whenever a new chunk of data is going to be made, the short value will be *val4
    while(ptr < mem+5000)
    { //if ptr <= mem+5000, that means that you have gone past the block
        val3 = (short*)(&ptr[1]); //ptr[1] is the beginning of the short value
        if(ptr[0] == 'n')
        {
            ptr = ptr+(*val3)+3; //move pointer to the next chunk of metadata since the this bit of data is in use
        }
        else if(ptr[0] == 'y')
        { //data is available
            short temp; 
            temp = *val3; //temp takes the value inside val3 because val3 may change
            if(temp >= z)
            {
                ptr[0] = 'n'; //you are giving the user some data, so it will be in use
                if(temp > (z+3) )
                { //another chunk of metadata can be made
                    *val3 = z; //val3 changes to z
                    ptr[z+3] = 'y'; //a new chunk of metadata is made with available data
                    val4 = (short*)(&ptr[z+4]); //val4 gets stored next to the new character
                    *val4 = temp-z-3; //the value in val4 is the amount of available bytes that remain
                }
                return ptr+3; //return the data the user asked for
            }
            else{
                ptr = ptr+(*val3)+3; //the available data was smaller than what the user requested
            }
        }
        else{
            printf("something is not right\n"); //debugging statement
        }
        
    }
    printf("There is no available space. file: %s line no.: %d\n", file, linenum); //ptr has gone through the entire block
    return NULL;
}


void myfree(void* x, char* file, int linenum){
	if(x == NULL)
	{
		printf("error\n");
		return;
	}  
 char* ptr = (char*)x; //you have to cast a pointer to a char pointer since you cannot dereference a void pointer
    if(ptr >= mem+5000)
    { //user error
        printf("User has exceeded the bounds of the block file: %s line no.: %d\n", file, linenum);
        //return;
        
    }
    ptr = ptr-3; //this leads to the metadata of the pointer that is to be freed.
    
    if((ptr[0] != 'y') && (ptr[0] != 'n'))
    {	//the pointer is in an improper place
        printf("This is not a proper address. file: %s line no.: %d\n", file, linenum);
        return;
    }
    if(ptr[0] == 'y')
    {   //either the pointer refers to an available space that has never been in use or has already been freed.
        printf("User is attempting to free the same pointer twice or is trying to free space that has not received a recent malloc call. ");
        printf("file: %s line no.: %d\n", file, linenum);
        return;
    }
    else if(ptr[0] == 'n')
    {   //the data is now available
        ptr[0] = 'y';
    }
    int merger; //this integer starts off as 1. if the bits of metadata cannot be merged, the value will become zero
    merger = 1;
    short* count1; //this is the short pointer stored in the chunk of metadata that referred to the pointer that was freed
    short* count2; //this is the short stored in the next chunk of metadata
    short temp; //this preserves the value stored inside count2
    
    temp = 0; //temp is initialised to avoid a compiler error
    count1 = (short*)(&ptr[1]);
    if((ptr+3+*count1) >= (mem+5000))
    { //this if statement is here just to ensure that ptr doesn't go out of bounds. it appears in the while loop below
        return;
    }
    while(merger != 0)
    {
        
        
        
        if(ptr[3+*count1] == 'y')
        { //the next chunk of metadata has available bytes.
            count2 = (short*)(&ptr[4+*count1]); //get the short in that chunk of metadata
            temp = *count2; //*count2 is now preserved
            ptr[3+*count1] = ptr[2+*count1]; //the previous character is empty. when removing the metadata, you want empty characters
            char * a = (char*)(&ptr[*count1+4]); //since a short is 2 bytes, two char pointers need to be declared in order to remove the short
            char * b = (char*)(&ptr[*count1+5]);
            *a = ptr[3+*count1]; //a and b now store empty characters
            *b = ptr[3+*count1];
            *count1 = *count1+temp+3; //the short in the original chunk of metadata is changed to indicate that there is a larger amount of available space
	    /*
	    *the below if statements ensure that the metadata doesn't say that there are more than 4997 bytes available
	    *or to ensure that if the pointer goes out of bounds, that we don't continue
	    */
            if(*count1 == 4997)
            { 
                return;
            }
            else if(*count1 > 4997)
            {
                *count1 = 4997;
                return;
            }
            if((ptr+3+*count1) >= (mem+5000))
            {
                return;
            }
            
        }
        else{ //the next chunk of metadata has bytes that are in use
            merger = 0;
        }
        
        
    }
    
    
    
    
    return;
}

