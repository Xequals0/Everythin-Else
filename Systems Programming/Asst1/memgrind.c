/* CS 214: Systems Programming Spring 2018
 *  Professor Francisco
 *  Assignment 1: To Build A Better Malloc
 *  Richard Farrington netid: raf172
 *  Anirudh Tunoori netid: at813
 */

//memgrind.c
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "mymalloc.h"

//Workload functions:

//Workload A: malloc() 1 byte and immediately free it - (x150)
void workloadA(){
    //printf("A\n");
    char* allocate; //A pointer
    int i;
    for (i = 0; i < 150; i++) { //This loop calls malloc for 1 byte and immediately frees it
	//printf("this is the %dth time we call malloc\n", i+1);
        allocate = (char *) malloc(1);  //Allocate the byte
	
        free(allocate); //Free the byte
	if(i==150)
	{
		break;
	}
    }
}

//Workload B:  malloc() 1 byte, store the pointer in an array
//then free() the 150 1 byte pointers one by one - (x150)
void workloadB(){
    //printf("B\n");
    char* arr[150]; //Create an array of size 150
    char* ptr;      //A pointer
    int i;
    for (i = 0; i < 150; i++) { //This loop allocates 1 byte and stores the pointers in an array
        ptr = (char *) malloc(1);   //Malloc 1 byte
        arr[i] = ptr;               //Store the pointer
    }
    for (i = 0; i < 150; i++) { //This loop frees the 150 1 byte pointers one by one
	//printf("we have done this %d time(s)\n",i+1);        
	free(arr[i]);   //Free each 1 byte pointer one by one
    }
}

//Workload C: : Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer  - (x150)
void workloadC(){
    //printf("C\n");
    srand(time(NULL));
    int i, j, random, counter, numFree;
    counter = 0;    //Used to keep track of the "net" number of times 1 byte is allocated
    numFree = 0;    //Used to keep track of the number of times free() is called
    char* ptr[150];
    for(i = 0; i < 150; i++){
        ptr[i] = 0;
    }
    
    for (i = 0; i < 150;) { //This main loop will iterate untill we malloc() 150 bytes
        random = rand() % 2;    //Generate a random number between 0 and 1 (inclusive)
        
        if (random == 0) {  //If the number is 0, malloc() 1 byte
            ptr[i] = (char*) malloc(1);  //malloc() 1 byte
            i++;    //Increment the main loop's counter
            counter++;  //Increment the counter
            //printf("A: Index: %d Counter: %d\n",i,counter);
        }
        
        else if(random == 1) { //If the number is 1, free() 1 byte
            if (counter > numFree && ptr[numFree] != NULL) {  //Do not free if there is nothing to free        
                //printf("Value: %c Trying to free index: %d\n",ptr[numFree][1], counter);            
                if(ptr[numFree][1] == 'n'){ 
                    free(ptr[numFree]); //Free 1 byte
                    //printf("Freed?: %c\n",ptr[counter - 1][1]);
                    counter--;  //Decrement the counter 
                    numFree++;  //Increment the number of Frees    
                    //printf("B: Counter is now: %d\n",counter);
                }
            }
        }
    }
    for(j = 0; j < 150; j++) {
        if (ptr[j][1] == 'n') {
              free(ptr[j]); //Ensure that all pointers are eventually freed.
        }
        counter--;
    }
    
}

//Workload D: Randomly choose between a randomly-sized malloc() or free()ing a pointer
void workloadD(){
    printf("D\n");
    srand(time(NULL));
    int i, j, random, size, counter, totalSize, numFree;
    i = 0;     //This index keeps track of the number of malloc() calls up to 150
    counter = 0; //Used to keep track of the "net" number of times malloc() is called
    totalSize = 0; //Used to keep track of the total number of bytes allocated
    numFree = 0;    //Used to keep track of the number of times free() is called
    //int append = 0;    //Used to determine the index of the metadata
    
    char* ptr[150];
    for(i = 0; i < 150; i++){
        ptr[i] = 0;
    }
    //Main loop:
    for(i = 0; i < 150;){
       random = rand() % 2;
       if (random == 0) {  //If the number is 0, malloc() a random (1-64) number of bytes
          size = rand() % 64; size++; //Generate a random size (between 1 and 64 bytes)
            if((totalSize + size) < 4990){  //Ensure that there is sufficient memory to allocate the number of bytes
               ptr[i] = (char *) malloc(size);  //malloc() a random number of bytes
               if(ptr[i] != NULL){
                    i++;           //Increment the counters: i goes closer to 150 (controls the main loop)
                    counter++;     //Counter keeps track of the net number of malloc() calls; increment with each malloc() and decrement with each free()
                    totalSize += size;        //Increment the that number of bytes that are allocated
                } else if (ptr[i] == NULL){ //If the current pointer is already NULL
                    totalSize -= ptr[i][strlen(ptr[i]) - 1]; //Decrement the totalSize to account for the newly available memory
                    free(ptr[numFree]); //Free the pointer
                    ptr[numFree] = NULL;
                    numFree++;   //Increment the free() call counter
                }
             } else printf("The User is attempting to exceed the available capacity of memory.\n");
       }

       else if(random == 1) { //If the number is 1, call free()
             totalSize -= ptr[i][strlen(ptr[i]) - 1]; //Decrement the totalSize to account for the newly available memory
             free(ptr[numFree]); //Free the pointer
             ptr[numFree] = NULL;
             numFree++;   //Increment the free() call counter
       }
    }   //End of main loop


    for(j = numFree; j < 150; j++) {    //Free all remaining pointers
        //if (ptr[j][1] == 'n') {
             // printf("freeing: %d\n",j);
          free(ptr[j]); //Ensure that all pointers are eventually freed.
          ptr[j] = NULL; 
          counter--;
    }  




/*
    srand(time(NULL));
    int i, j, k, random, size, counter, totalSize, numFree, append;
    counter = 0; //Used to keep track of the "net" number of times malloc() is called
    totalSize = 0; //Used to keep track of the total number of bytes allocated
    numFree = 0;    //Used to keep track of the number of times free() is called
    append = 0;    //Used to determine the index of the metadata
    char* ptr[150];
    for(i = 0; i < 150; i++){
        ptr[i] = 0;
    }

    for (i = 0; i < 150;) { //This main loop will iterate untill we malloc() 150 times
        random = rand() % 2;    //Generate a random number between 0 and 1 (inclusive)
        if (random == 0) {  //If the number is 0, malloc() a random (1-64) number of bytes
            size = rand() % 64; size++; //Generate a random size
            printf("total size: %d\n",(totalSize + size));
            if ((totalSize + size) < 5000) {
                ptr[i] = (char *) malloc(size);  //malloc() a random number of bytes  

                if(ptr[i][size] == 'y'){
                    totalSize+= size;     //Increment the total size allocated
                    printf("1: Index: %d Total Size: %d\n",i,totalSize);
                    counter++;         //Keep track of the number of times malloc() is called
                    i++;    //Increment the main loop's counter
                }
            } else printf("There is no available space.\n");
        }
        
        else if(random == 1) { //If the number is 1, call free()
            if (counter > numFree && ptr[numFree] != NULL) {  //Do not free if there is nothing to free
               for(k = 0; k <= 64; k++){
                  if(ptr[numFree][k] == 'n'){
                     append = k;   
                     printf("index: %d value: %c\n",k, ptr[numFree][k]);
                     break;
                  }
               } 
                
                printf("index: %d BP1: %c\n",append,ptr[numFree][append]);
                if(ptr[numFree][append] == 'n'){
                    printf("BP2: \n"); 
                    totalSize -= append; //Decrement the totalSize
                    printf("size: %d\n",totalSize); 
                    free(ptr[numFree]); //Free the pointer
                    counter--;  //Decrement the counter 
                    numFree++;  //Increment the number of Frees    
                    printf("2: Total Size: %d\n",totalSize);
                }
            }
        }
    }
    
    for(j = 0; j < 150; j++) {
        //if (ptr[j][1] == 'n') {
              printf("freeing: %d\n",j);
              free(ptr[j]); //Ensure that all pointers are eventually freed.
        //}
        counter--;
    }*/

}

//Workload E: In a nested proccess Allocate n bytes one at a time and then free them; increasing n with each cycle
void workloadE(){
    //printf("E\n");
    int i, iteration = 0;
    char* ptr[1000]; //Pointer array
    for(i = 0; i < 150; i++){
        ptr[i] = 0; //Initialize
    }

    for(i = 1; i < 1000; i++){ //Outer loop: increases the number of bytes for the next cycle
        for(iteration = 1; iteration < i; iteration++){
           ptr[i] = (char *) malloc(iteration);  //call malloc()
           free(ptr[i]); //call free()
        }
    }
}

//Workload F: Allocate n bytes and then free them; with increasing n
void workloadF(){
    //printf("F\n");
    int i;          //Index
    char* ptr[1000]; //Pointer array
    for(i = 0; i < 150; i++){
        ptr[i] = 0;     //Initialize
    }

    for(i = 1; i < 1000; i++){ //Outer loop: increases the number of bytes for the next cycle
         ptr[i] = (char *) malloc(i);  //call malloc()
         free(ptr[i]); //call free()
    }
}


int main(int argc, char **argv) {
    int runs;
    struct timeval startTime, endTime;
    long ti,tf = 0;
    double totalRuntime_A, totalRuntime_B, totalRuntime_C, totalRuntime_D, totalRuntime_E, totalRuntime_F = 0.0;
    //This is the main loop that executes the worrkload cycle (A-F) 100 times
    for (runs = 1; runs <= 100; runs++) {
        //printf("Run: %d\n",runs);
        
        //Workload A:
        gettimeofday(&startTime,NULL);  //Start the runtime counter
        workloadA();    //Call Workload A
        gettimeofday(&endTime,NULL);    //End the runtime counter
        ti = (startTime.tv_sec * 1000000) + startTime.tv_usec;
        tf = (endTime.tv_sec * 1000000) + endTime.tv_usec;
        totalRuntime_A += tf - ti;  //increment the total runtime of workload A
        
        //Workload B:
        gettimeofday(&startTime,NULL);  //Start the runtime counter
        workloadB();    //Call Workload B
        gettimeofday(&endTime,NULL);    //End the runtime counter
        ti = (startTime.tv_sec * 1000000) + startTime.tv_usec;
        tf = (endTime.tv_sec * 1000000) + endTime.tv_usec;
        totalRuntime_B += tf - ti;  //increment the total runtime of workload B
        
        //Workload C:
        gettimeofday(&startTime,NULL);  //Start the runtime counter
        workloadC();    //Call Workload C
        gettimeofday(&endTime,NULL);    //End the runtime counter
        ti = (startTime.tv_sec * 1000000) + startTime.tv_usec;
        tf = (endTime.tv_sec * 1000000) + endTime.tv_usec;
        totalRuntime_C += tf - ti;  //increment the total runtime of workload C
        
        //Workload D:
        gettimeofday(&startTime,NULL);  //Start the runtime counter
        workloadD();    //Call Workload D
        gettimeofday(&endTime,NULL);    //End the runtime counter
        ti = (startTime.tv_sec * 1000000) + startTime.tv_usec;
        tf = (endTime.tv_sec * 1000000) + endTime.tv_usec;
        totalRuntime_D += tf - ti;  //increment the total runtime of workload D
        
        //Workload E:
        gettimeofday(&startTime,NULL);  //Start the runtime counter
        workloadE();    //Call Workload E
        gettimeofday(&endTime,NULL);    //End the runtime counter
        ti = (startTime.tv_sec * 1000000) + startTime.tv_usec;
        tf = (endTime.tv_sec * 1000000) + endTime.tv_usec;
        totalRuntime_E += tf - ti;  //increment the total runtime of workload E
        
        //Workload F:
        gettimeofday(&startTime,NULL);  //Start the runtime counter
        workloadF();    //Call Workload F
        gettimeofday(&endTime,NULL);    //End the runtime counter
        ti = (startTime.tv_sec * 1000000) + startTime.tv_usec;
        tf = (endTime.tv_sec * 1000000) + endTime.tv_usec;
        totalRuntime_F += tf - ti;  //increment the total runtime of workload F
    }   //End of 100 workload cycles.
    //Divide the total runtimes of each workload by 100 to obtain the means and output them one by one:
    totalRuntime_A = totalRuntime_A/100;
    totalRuntime_B = totalRuntime_B/100;
    totalRuntime_C = totalRuntime_C/100;
    totalRuntime_D = totalRuntime_D/100;
    totalRuntime_E = totalRuntime_E/100;
    totalRuntime_F = totalRuntime_F/100;
    printf("The mean runtime of each workload: \n Workload A: %f microseconds\n Workload B: %f microseconds\n Workload C: %f microseconds\n Workload D: %f microseconds\n Workload E: %f microseconds\n Workload F: %f microseconds\n",totalRuntime_A,totalRuntime_B,totalRuntime_C,totalRuntime_D,totalRuntime_E,totalRuntime_F);
    
    return 0;
}
