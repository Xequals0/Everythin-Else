/*  Professor Francisco
*  Assignment 1: To Build A Better Malloc
*  Richard Farrington netid: raf172
*  Anirudh Tunoori netid: at813
*/

//mymalloc.h
#ifndef mymalloc_h
#define mymalloc_h

//Libraries:
#include <stdlib.h>
#include <stdio.h>

//Macros for malloc() and free():
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

//Define the function prototypes:
//these functions are found in the mymalloc "library"
void* mymalloc(int x, char* file, int linenum); //the malloc() function
void myfree(void* x, char* file, int linenum);  //the free()   function

#endif
