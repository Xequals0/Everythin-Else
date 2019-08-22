/* CS 214: Systems Programming Spring 2018
 *  Professor Francisco
 *  Assignment 2: Keyspace Construction
 *  Richard Farrington netid: raf172
 *  Anirudh Tunoori netid: at813
 */

//  list.c
#include <ctype.h>
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


void freestuff(List* head){
     List* ptr = head;
     List* next = NULL;
     while(ptr){
         next = ptr -> next;
         free(ptr);
         ptr = next; 
     }head = ptr;
     return;
}

void printIndex(List* l, char* outputName) {
     int out = open(outputName, O_RDWR);
     List* ptr = l;
     char* word;
     char* fname;
     char buf[4] = {0};
     //write(out, "Hello World!\n", 13);
     if(ptr == NULL){
         write(out, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", 39);      
         write(out, "<fileIndex>\n", 12);
         while(ptr != NULL) {
              printf("hi\n");
              fname = strndup(ptr -> file, strlen(ptr -> file));
              if(ptr -> prev == NULL || (strcmp(ptr -> word, ptr -> prev -> word) != 0)){
                   word = strndup(ptr -> word, strlen(ptr -> word));
                   write(out, "\t<word text=\"", 14); write(out, word, strlen(word));
                   write(out, "\">\n", 4);
                   write(out, "\t<file name=\"", 14); write(out, fname, strlen(fname));
                   write(out, "\">", 3);
                   memset(buf, 0, sizeof (buf));
                   snprintf(buf, sizeof (ptr -> count), "%d", (ptr -> count));
                   write(out, buf, strlen(buf)); write(out, "</file>\n", 9);
              } else if(strcmp(ptr -> word, ptr -> prev -> word) == 0){
                   write(out, "\t\t<file name=\"", 15); write(out, fname, strlen(fname));
                   write(out, "\">", 3);
                   memset(buf, 0, sizeof (buf));
                   snprintf(buf, sizeof (ptr -> count), "%d", (ptr -> count));
                   write(out, buf, strlen(buf)); write(out, "</file>\n", 9);
              }if (ptr -> next == NULL || (strcmp(ptr -> word, ptr -> next -> word) != 0)){
                    write(out, "\t</word>\n", 10);
              }
              ptr = ptr -> next;
         }
         write(out, "</fileIndex>\n", 13);
     }
     close(out);
     freestuff(l);
     return;
}

void makeNode(List* head, char* token, char* filename){
     List* ptr = head;
     List* curr = NULL;
     List* start = NULL;
     List* end = NULL;
     List* newNode = NULL;
     while(ptr != NULL){
         if(strcmp((ptr -> word), token) == 0) {
              if(strcmp((ptr -> file), filename) == 0) {
                  ptr -> count = ptr -> count + 1;
                  curr = ptr -> prev;
                  if(ptr -> count <= curr -> count){
                       return;
                  }else {
                       start = curr -> prev;
                       end = ptr -> next;
                       ptr -> prev = start; 
                       end -> prev = curr;
                       curr -> prev = ptr;
                       ptr -> next = curr;
                       curr -> next = end;
                       start -> next = ptr;
                       ptr = head;
                       return;
                  }
              } 
         }
         if(strcmp((ptr -> word), token) > 0) {
              break;
         }         
         ptr = ptr -> next; 
     }ptr = head;

     newNode = malloc(sizeof(List));
     newNode -> word = token;
     newNode -> file = filename;
     newNode -> count = 1;     
     newNode -> prev = NULL;
     newNode -> next = NULL;
     insertToList(head, newNode);

     return;
}

void insertToList(List* head, List* node){
     if(node == NULL) {    //As a precaution
          return;
     }
     if(head == NULL) { //This is the first node in the list
          head = node;
          return;
     }

     List* ptr = head;
     List* curr = NULL;
     while(ptr != NULL){
         if(strcmp((ptr -> word), (node -> word)) < 0) {
              ptr = ptr -> next;
         } else if(strcmp((ptr -> word), (node -> word)) == 0){
              while((ptr -> count > node -> count) && (strcmp((ptr -> word), (node -> word)) == 0)){
                   if(ptr -> count == node -> count){
                        while(strcmp((ptr -> file), (node -> file)) < 0){
                            if(strcmp((ptr -> file), (node -> file)) > 0){
                                node -> next = ptr;
                                node -> prev = ptr -> prev;
                                curr = ptr -> prev;
                                curr -> next = node;
                                ptr -> prev = node;
                                ptr = head;
                                return;   
                            }
                            ptr = ptr -> next;
                        }
                   } else if(ptr -> count < node -> count){
                        node -> next = ptr;
                        node -> prev = ptr -> prev;
                        curr = ptr -> prev;
                        curr -> next = node;
                        ptr -> prev = node;
                        ptr = head;
                        return;
                   }
                   ptr = ptr -> next;
              }
         }
         if(strcmp((ptr -> word), (node -> word)) > 0) {
              break;
         }          
     }
     if(ptr == NULL){
         curr = ptr -> prev;
         curr -> next = node;
         node -> prev = curr;
         node -> next = NULL;
         ptr = head;
         return;  
     }else{
         curr = ptr -> prev;
         node -> prev = curr;
         node -> next = curr -> next;          
         ptr -> prev = node;
         curr -> next = node;
         ptr = head;
         return;
     }
     return;
}

void bufferstuff(int fd, char* buff, int* calnum){




     return;
}

