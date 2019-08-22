/*
 * mystery.c
 * Anirudh Tunoori netid: at813
 */
#include <stdlib.h>
#include <stdio.h>

#include "mystery.h"
int num[200];   //an input of 199 provides a value, while 200 produces "0"

//This program takes an in integer n and returns the nth fibonnachi number in the
//fibonnachi sequence

/*This is the add function found in mystery.s
 *It simply adds the two numbers it receives as
 *inputs and returns the sum
 */
int add(int num1, int num2){
    return (num1 + num2);
}
/*This is the dothething function found in mystery.s
 *It obtains the nth fibbonachi number by first recursively
 *obtaining the n-2 and the n-1 fibbonachi number
 *and then by calling the add function on these numbers and returning the sum
 */
int dothething(int n){
    if (num[n] == -1) {
        if (n == 0) num[n] = 0;
        else if (n == 1) num[n] = 1;
        else num[n] = add(dothething(n-2),dothething(n-1));
    }
    return num[n];
}
/*This is the main function found in mystery.s
 *It holds the input as an int (nth fibonnachi number),
 *fills an int array with -1, calls the dothething function
 *and outputs the result
 */
int main(int argc, char** argv){
   /* Unnecessary according to mystery.s
    if(argc != 2){
        fprintf(stderr,"ERROR: Invalid number of inputs! (only accepts a single integer) \n"); return 0;
    }*/
    int n = atoi(argv[1]);  //store a string(integer) input as an int
    int index = 0;
    for (;index <= 199;index++) { //the loop condition is jle in mystery.s -> (index <= 199)
        num[index] = -1;
    }printf("Value:   %d\n",dothething(n)); //call to the dothething function followed by printf
    
    return 0;
}
