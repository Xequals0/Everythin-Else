/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2017                              *
 *  Author: Ulrich Kremer                    *
 *  Student Version                          *
 *  Anirudh Tunoori                          *
 *  netid: at813                             *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

//function that searches for a target within an integer array:
//returns the index for a success and -1 for a failure
int searchArray(int target, int *list, int size) {
    int index = 0;
    for (;index < size; index++)
        if (list[index] == target)
            return index;    //target was found in the list
    return -1;              //target was not found in the list
}



//This function deletes (dynamically frees) instructions that are not critical
//from the list
Instruction* delete(Instruction *ptr){
    Instruction *curr;
    curr = ptr -> prev;
    if (ptr == NULL)
        return ptr;
    if (ptr -> next != NULL) {
        ptr -> next -> prev = ptr -> prev;
    }
    if (ptr -> prev != NULL) {
        ptr -> prev -> next = ptr -> next;
    }
    free(ptr);
    return curr;
}

int main()
{
    Instruction *head, *ptr, *tail, *next;       //additional pointers to be used when necessary
    int listSize, index, numCritical, found, i;
    listSize = 0; index = 0; numCritical = 0; i = 0;
    
    head = ReadInstructionList(stdin);
    if (!head) {
        WARNING("No instructions\n");
        exit(EXIT_FAILURE);
    }
    /* YOUR CODE GOES HERE */
    ptr = head;
    tail = LastInstruction(head);
    while (ptr) {
        ptr = ptr -> next;
        listSize++;
    }ptr = head;
    
    /* Marking the instructions as critical requires four integer arrays
     two to keep track of output instructions (the register number and associated offsets)
     and two to keep track of registers already marked as critical
     (again the register number and the associated offsets)
     */
    int outputList[listSize], outputOffsets[listSize], marked[listSize], markedOffsets[listSize];
    for (; ptr != NULL; index++) {
        marked[index] = -1; markedOffsets[index] = -1;
        if (ptr) {  //storing to the array is done by the opcode
            if (ptr -> opcode == OUTPUTAI) {
                outputList[index] = ptr -> field1;
                outputOffsets[index] = ptr -> field2;
            }else if(ptr -> opcode == LOADI || ptr -> opcode == STOREAI){
                outputList[index] = ptr -> field2;
                outputOffsets[index] = ptr -> field3;
            }else if(ptr -> opcode == ADD || ptr -> opcode == SUB
                     || ptr -> opcode == MUL || ptr -> opcode == DIV ||
                     ptr -> opcode == LOADAI){
                outputList[index] = ptr -> field3;
                outputOffsets[index] = -1;  //negative one can be ignored
            }
        } else{
            ERROR("Illegal instruction(s) encountered!\n");
            exit(EXIT_FAILURE);
        }
        
        if (ptr -> opcode == OUTPUTAI) {
            /*mark all of the output instructions, store the register numbers containeing these instructions along with the associated offsets*/
            ptr -> critical = 'c';
            marked[numCritical] = ptr -> field1;
            markedOffsets[numCritical] = ptr -> field2;
            numCritical++;
        } ptr = ptr -> next;
    } ptr = tail; index = listSize;
   

    //the marking process is now conducted from the tail of the list to the head
    //once an instruction is marked as critical the instructions preceeding it that reference
    // that register need to be checked.
    while(ptr) {
        index--;
        found = searchArray(outputList[index], marked, numCritical);    //determine if the register can be found in the array of registers deemed critical
        if (found != -1) {
            ptr->critical = 'c';    //mark these instructions as critical
            switch(ptr->opcode) {
                case LOADI: case OUTPUTAI:
                    break;
                case ADD:   case SUB:   case MUL:   case DIV:
                    //Add the registers of the operands used in the operation
                    if (searchArray(ptr->field1, marked, numCritical) == -1)
                        marked[numCritical++] = ptr->field1;    //operand 1
                    if (searchArray(ptr->field2, marked, numCritical) == -1)
                        marked[numCritical++] = ptr->field2;    //operand 2
                    break;
                
                case STOREAI:
                    
                    i = 0;
                    while (i < listSize) {
                        if (markedOffsets[i] == outputOffsets[index]) {
                            ptr->critical = 'c';
                            markedOffsets[i] = -2; //remove from the array after a match is made
                            numCritical--;
                            if (searchArray(ptr->field1, marked, numCritical) == -1) {
                                marked[numCritical++] = ptr->field1;    //new critical register
                            }
                            break;
                        } else  ptr->critical = 'n';//not critical if the offsets in the
                                                    //...two arrays do not match
                        i++;
                    }
                    break;
                    
                case LOADAI:
                //add the critical registers and offsets that are not yet accounted for
                    if (searchArray(ptr->field1, marked, listSize) == -1 || searchArray(ptr->field2, markedOffsets, listSize) == -1) {
                        marked[numCritical] = ptr->field1;  //the target register
                        markedOffsets[found] = ptr->field2; //the associated offset
                        numCritical++;
                    }
                    break;
               /* default:
                    ERROR("Illegal instructions\n");    / This is already checked above...//
                    exit(EXIT_FAILURE);             */
            }
        }
        ptr = ptr->prev;
    }

    
    //Delete instructions that are not critical (call the delete function)
    ptr = head -> next;
    while (ptr) {
        if (ptr -> critical != 'c')
            delete(ptr);
        ptr = ptr -> next;
    }
    if (head)   //Print the optimized list of instructions
        PrintInstructionList(stdout, head);
    
    //dynamically free the list of instructions
    ptr = head;
    while (ptr) {
        next = ptr -> next;
        free(ptr);
        ptr = next;
    }head = ptr;
    return EXIT_SUCCESS;
}
