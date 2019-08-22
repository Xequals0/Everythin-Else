/* CS 214: Systems Programming Spring 2018
 *  Professor Francisco
 *  Assignment 2: Keyspace Construction
 *  Richard Farrington netid: raf172
 *  Anirudh Tunoori netid: at813
 */

//list.h
#ifndef list_h
#define list_h

//Libraries:
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//Define the doubly linked list data structure that will hold the contents of the index:

typedef struct Index List;

struct Index {
    char* word;
    char* file;
    int count;  //keeps track of the number of occurrences of a unique token within a unique file
    List *prev;	/* previous node */
    List *next;	/* next node */
};

//Define the function prototypes:
//these functions are found in list.c
void freestuff(List* head);
void printIndex(List* l, char* outputName);
void makeNode(List* head, char* token, char* filename);
void insertToList(List* head, List* node);
void bufferstuff(int fd, char* buff, int* calnum);


#endif





