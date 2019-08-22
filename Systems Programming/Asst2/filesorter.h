/* CS 214: Systems Programming Spring 2018
 *  Professor Francisco
 *  Assignment 2: Keyspace Construction
 *  Richard Farrington netid: raf172
 *  Anirudh Tunoori netid: at813
 */

//filesorter.h
#ifndef filesorter_h
#define filesorter_h

#include "list.h"
//Define the function prototypes:
//these functions are found in filesorter.c


int readfile(int fd, char* filename, List* l);
int readdirect(char* dirname, List* l);
int nullcheck(char* token);

#endif

