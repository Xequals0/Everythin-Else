/*
 * formula.c
 * Anirudh Tunoori netid: at813
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>

#include "nCr.h"
#include "formula.h"


int startDecimal, possiblyDecimal = 0; //0 is false, 1 is true
//This function checks the input to make sure that it is a positive integer.
//I used my isDecimal funcction from calc.c in pa2
int checkInput(char* power){
    while (*power != '\0') {  //go through each chacter/digit should ony be 0-9
        if (startDecimal == 1){
            if (*power <= '9' && *power >= '1'){ possiblyDecimal = 1;   //first digit can't be 0
            }else if(strlen(power) == 1 && isdigit(*power)){    //one digit case
                startDecimal = 0; possiblyDecimal = 0;
                return 0;
            }
            else{
                startDecimal = 0; possiblyDecimal = 0;
                return 1;   //not a decimal
            }
        }
        if (possiblyDecimal == 1){  //if the first digit passes the test the remainder of the string is processed by this
            startDecimal = 0;       //block
            if (*power <= '9' && *power >= '0'){
            }
            else{
                possiblyDecimal = 0;
                return 1;   //not a decimal
            }
        } power++;
    }
    possiblyDecimal = 0;   //reset for the next call
    return 0;
}

int main(int argc, char** argv){
    int index = 1; int nextFactor; long executionTime;
    struct timeval startTime, endTime;
    gettimeofday(&startTime,NULL);  //Start the runtime counter

    if(argc != 2){  //check for valigd # of arguments
        fprintf(stderr,"ERROR: Invalid number of arguments... should have 1 \n"); return 0;
    }if (strcmp(argv[1],"-h") == 0) { //check for the help flag and print a usage message if necessary
        printf("Usage: formula <positive integer> \n");
        return 0;
    }startDecimal = 1;
    if(checkInput(argv[1]) != 0){   //check the input
        fprintf(stderr,"The entered input is not a positive integer \n");
        return 0;
    }startDecimal = 0;
    
    int n =  atoi(argv[1]); //store the power n as an int
    if (Factorial(n) == 0) {
        fprintf(stderr,"Overflow Detected...enter a smaller power \n");
        return 0;
    }
    printf("(1 + x)^%d = 1",n); //print the first factor
    while(index <= n) {
        nextFactor = nCr(n,index);  //obtain the coefficient/term/nextFactor
        printf(" + %d*x^%d",nextFactor,index);  //print the next factor
        index++;
    }
    gettimeofday(&endTime,NULL);    //End the runtime counter
    //Calculate the runtime:
    long t1 = (startTime.tv_sec * 1000000) + startTime.tv_usec;
    long t2 = (endTime.tv_sec * 1000000) + endTime.tv_usec;
    executionTime = t2 - t1;
    printf("\n Time Required = %ld microsecond \n",executionTime); //print the runtime
    
    return 0;
}
