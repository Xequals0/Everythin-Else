/*
 * y86dis.c
 * Anirudh Tunoori netid: at813
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "y86dis.h"
//#include "mallocld.h"

char* memoryImage;
int pc;
int run;
int registers[9];
int overflow;
int zero;
int negative;

//fills in the contents of the .text directive to the memory
int text(char *arg, int i){
    int index = i;
    int ptr = 0; pc = -1;
    if (pc == -1)
        pc = index;
    else if(pc != -1){
        fprintf(stderr, INS);   //invalid instruction
        exit(1);
    }
    while (ptr < strlen(arg)) {
        bitField* b = (bitField *)(memoryImage + index);  //create a bitfield
        char *high = malloc(1);
        char *low = malloc(1);
        strncpy(high, arg + ptr + 1, 1);
        strncpy(low, arg + ptr, 1);
        b -> highHalf = strtol(high, NULL, 16);
        b -> lowHalf =  strtol(low, NULL, 16);
        free(high);
        free(low);
        ptr += 2;
        index++;
    }
    return 0;
}

//Executes the push instruction:
int push(int reg){
    int valueHeld =  registers[reg];
    int decrementStackFrame =  registers[ESP] - 4;
    int *newLoc = (int*)(memoryImage + decrementStackFrame);
    *newLoc = valueHeld;
    registers[ESP] = decrementStackFrame;
    pc += 2;
    return 0;
}

//Executes the pop instruction:
int pop(int reg){
    int ESPvalueHeld =  registers[ESP];
    int incrementStackFrame = ESPvalueHeld + 4;
    int *newLoc = (int*)(memoryImage + ESPvalueHeld);
    registers[ESP] = incrementStackFrame;
    registers[reg] = *newLoc;
    pc += 2;
    return 0;
}

//Executes all of the move instructions:
int movl(unsigned char* instruction){
    bitField * bit = (bitField *)(memoryImage + ( pc) + 1);
    switch (*instruction) {
        case 0x20:  //rrmovl
        {
            int regA = bit -> highHalf;
            int regB = bit -> lowHalf;
            if ((regA >= 0 && regA <= 8) && (regB >= 0 && regB <= 8)) {
                registers[regA] =  registers[regB];    //move contents of B onto A
                pc += 2;
            }else{
                fprintf(stderr, "ERROR: Invalid rrmovl!");
                exit(1);
            }
            break;
        }
        case 0x30:  //irmovl
        {
            int reg = bit -> highHalf;
            if ((reg >= 0 && reg <= 8) && (bit -> lowHalf == 0xf)) {
                int* immVal = (int*)(memoryImage + ( pc) + 2);
                registers[reg] = *immVal;    //move contents of B onto A
                pc += 6;
            }else{
                fprintf(stderr, "ERROR: Invalid irmovl!");
                exit(1);
            }
            break;
        }
        case 0x40:  //rmmovl
        {
            if ((bit -> highHalf >= 0 && bit -> highHalf <= 8) && (bit -> lowHalf >= 0 && bit -> lowHalf <= 8)) {
                int* Value = (int*)(memoryImage + ( pc) + 2);
                int regAVal =  registers[bit -> highHalf];
                int regBVal =  registers[bit -> lowHalf];
                int *memLoc = (int*)(memoryImage + (regAVal + *Value));
                *memLoc = regBVal;
                pc += 6;
            }else{
                fprintf(stderr, "ERROR: Invalid rmmovl!");
                exit(1);
            }
            break;
        }
        case 0x50:  //mrmovl
        {
            int regA = bit -> highHalf;
            int regB = bit -> lowHalf;
            if ((bit -> highHalf >= 0 && bit -> highHalf <= 8) && (bit -> lowHalf >= 0 && bit -> lowHalf <= 8)) {
                int* memVal = (int*)(memoryImage + ( pc) + 2);
                int regAVal =  registers[bit -> highHalf];
                int *memLoc = (int*)(memoryImage + (regAVal + *memVal));
                registers[bit -> lowHalf] = *memLoc;
                pc += 6;
            }else{
                fprintf(stderr, "ERROR: Invalid mrmovl!");
                exit(1);
            }
            break;
        }
        case 0xe0:  //movsbl
        {
            int regA = bit -> highHalf;
            int regB = bit -> lowHalf;
            if ((bit -> highHalf >= 0 && bit -> highHalf <= 8) && (bit -> lowHalf >= 0 && bit -> lowHalf <= 8)) {
                int* memVal = (int*)(memoryImage + ( pc) + 2);
                int regAVal =  registers[bit -> highHalf];
                int *memLoc = (int*)(memoryImage + (regAVal + *memVal));
                registers[bit -> lowHalf] = *memLoc;
                pc += 6;
            }else{
                fprintf(stderr, "ERROR: Invalid movsbl: likely trying to access a register that is unavailable!");
                exit(1);
            }
            break;
        }
        default:
        {
            fprintf(stderr, "ERROR: Invalid movl!");
            exit(1);
            break;
        }
    }return 0;
}

//Executes the Arithmetic and Logical instructions:
int ALU(unsigned char* instruction){
    bitField * bit = (bitField *)(memoryImage + ( pc) + 1);
    switch (*instruction) {
        case 0x60:  //addl operation
        {
            if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8) && (bit -> highHalf >= 0 && bit -> highHalf <= 8)) {
                int sum = ( registers[bit -> highHalf]) + ( registers[bit -> lowHalf]); //perform the addition operation
                registers[bit -> highHalf] = sum;
                
                //Set the status flags of the operation:
                if (sum == 0)    zero = 1;
                else  zero = 0;
                if (sum < 0)    negative = 1;
                else  negative = 0;
                if (( registers[bit -> highHalf] < 0 &&  registers[bit -> lowHalf] < 0 && sum > 0)
                    ||(( registers[bit -> highHalf] > 0 &&  registers[bit -> lowHalf] > 0 && sum < 0)))
                {
                    overflow = 1;
                    fprintf(stderr, "ERROR: An addition operation triggered an overflow (flag)!");
                    exit(1);
                }
                else  overflow = 0;
                pc += 2; //increment the counter
            }
            else{
                fprintf(stderr,INS);
                fprintf(stderr, "~ERROR: Invalid addl: likely trying to access a register that is unavailable!");
                exit(1);
            }
            break;
        }
            
        case 0x61:  //subl operation
        {
            if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8) && (bit -> highHalf >= 0 && bit -> highHalf <= 8)) {
                int difference = ( registers[bit -> highHalf]) - ( registers[bit -> lowHalf]); //perform the subtraction operation
                registers[bit -> highHalf] = difference;
                
                //Set the status flags of the operation:
                if (difference == 0)    zero = 1;
                else  zero = 0;
                if (difference < 0)    negative = 1;
                else  negative = 0;
                if (( registers[bit -> highHalf] < 0 &&  registers[bit -> lowHalf] > 0 && difference > 0)
                    ||(( registers[bit -> highHalf] > 0 &&  registers[bit -> lowHalf] < 0 && difference < 0)))
                {
                    overflow = 1;
                    fprintf(stderr, "ERROR: A subtraction operation triggered an overflow (flag)!");
                    exit(1);
                }
                else  overflow = 0;
                pc += 2; //increment the counter
            }
            else{
                fprintf(stderr,INS);
                fprintf(stderr, "~ERROR: Invalid subl: likely trying to access a register that is unavailable!");
                exit(1);
            }
            break;
        }
            
        case 0x62:  //andl logical operation
        {
            if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8) && (bit -> highHalf >= 0 && bit -> highHalf <= 8)) {
                int AND = ( registers[bit -> highHalf]) & ( registers[bit -> lowHalf]); //perform the and operation
                registers[bit -> highHalf] = AND;
                
                //Set the status flags of the operation:
                if (AND == 0)    zero = 1;
                else  zero = 0;
                if (AND < 0)    negative = 1;
                else  negative = 0;
                overflow = 0;    //no overflow associated with this operation.
                pc += 2; //increment the counter
            }
            else{
                fprintf(stderr,INS);
                fprintf(stderr, "~ERROR: Invalid andl: likely trying to access a register that is unavailable!");
                exit(1);
            }
            break;
        }
            
        case 0x63:  //xorl logical operation
        {
            if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8) && (bit -> highHalf >= 0 && bit -> highHalf <= 8)) {
                int XOR = ( registers[bit -> highHalf]) ^ ( registers[bit -> lowHalf]); //perform the xor operation
                registers[bit -> highHalf] = XOR;
                
                //Set the status flags of the operation:
                if (XOR == 0)    zero = 1;
                else  zero = 0;
                if (XOR < 0)    negative = 1;
                else  negative = 0;
                overflow = 0;    //no overflow associated with this operation.
                pc += 2; //increment the counter
            }
            else{
                fprintf(stderr,INS);
                fprintf(stderr, "~ERROR: Invalid xorl: likely trying to access a register that is unavailable!");
                exit(1);
            }
            break;
        }
            
        case 0x64:  //mull operation
        {
            if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8) && (bit -> highHalf >= 0 && bit -> highHalf <= 8)) {
                int product = ( registers[bit -> highHalf]) * ( registers[bit -> lowHalf]); //perform the multiplication operation
                registers[bit -> highHalf] = product;
                
                //Set the status flags of the operation:
                if (product == 0)    zero = 1;
                else  zero = 0;
                if (product < 0)    negative = 1;
                else  negative = 0;
                if (( registers[bit -> highHalf] < 0 &&  registers[bit -> lowHalf] > 0 && product > 0)
                    ||(( registers[bit -> highHalf] > 0 &&  registers[bit -> lowHalf] < 0 && product > 0))
                    ||( registers[bit -> highHalf] < 0 &&  registers[bit -> lowHalf] < 0 && product < 0)
                    ||(( registers[bit -> highHalf] > 0 &&  registers[bit -> lowHalf] > 0 && product < 0)))
                {
                    overflow = 1;
                    fprintf(stderr, "ERROR: A multiplication operation triggered an overflow (flag)!");
                    exit(1);
                }
                else  overflow = 0;
                pc += 2; //increment the counter
            }
            else{
                fprintf(stderr,INS);
                fprintf(stderr, "~ERROR: Invalid mull: likely trying to access a register that is unavailable!");
                exit(1);
            }
            break;
        }
            
        case 0x65:  //cmpl logical operation
        {
            if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8) && (bit -> highHalf >= 0 && bit -> highHalf <= 8)) {
                int cmp = ( registers[bit -> lowHalf]) - ( registers[bit -> highHalf]); //perform the cmp operation
                registers[bit -> highHalf] = cmp;
                
                //Set the status flags of the operation:
                if (cmp == 0)    zero = 1;
                else  zero = 0;
                if (cmp < 0)    negative = 1;
                else  negative = 0;
                overflow = 0;    //no overflow associated with this operation.
                pc += 2; //increment the counter
            }
            else{
                fprintf(stderr,INS);
                fprintf(stderr, "~ERROR: Invalid cmpl: likely trying to access a register that is unavailable!");
                exit(1);
            }
            break;
        }
            
        default:
        {
            fprintf(stderr,INS);
            exit(1);
            break;
        }
    }
    return 0;
}

//Executes the jump instructions:
int jump(unsigned char* instruction){
    int *jumpTo = (int*)(memoryImage + ( pc) + 1);
    switch (*instruction) {
        case 0x70:
            pc = *jumpTo;
            break;
        case 0x71:
            if ((( overflow ^  negative)| zero) == 1) {
                pc = *jumpTo;
            }else{
                pc += 5;
            }
            break;
        case 0x72:
            if (( overflow ^  negative) == 1) {
                pc = *jumpTo;
            }else{
                pc += 5;
            }
            break;
        case 0x73:
            if ( zero == 1) {
                pc = *jumpTo;
            }else{
                pc += 5;
            }
            break;
        case 0x74:
            if ( zero == 0) {
                pc = *jumpTo;
            }else{
                pc += 5;
            }
            break;
        case 0x75:
            if (( overflow ^  negative) == 0) {
                pc = *jumpTo;
            }else{
                pc += 5;
            }
            break;
        case 0x76:
            if ((( overflow ^  negative) &  zero) == 0) {
                pc = *jumpTo;
            }else{
                pc += 5;
            }
            break;
        default:
        {
            fprintf(stderr, INS);
            fprintf(stderr, "ERROR: Invalid jump!");
            exit(1);
            break;
        }
    }
    return 0;
}

//Executes the read instructions:
int read(unsigned char* instruction){
    bitField * bit = (bitField *)(memoryImage + ( pc) + 1);
    int *ptr = (int*)(memoryImage + (pc) + 2);
    unsigned char input;    int numValue;
    
    switch (*instruction) {
        case 0xc0:  //readb instruction
        {
            if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8) && (bit -> highHalf == 0xf)) {
                negative = 0;  overflow = 0;
                if (scanf("%c",&input) != EOF) {  //read in input (byte)
                    zero = 0;
                    unsigned char *readIn = (unsigned char*)(memoryImage + ( registers[bit -> lowHalf]) + *ptr);
                    *readIn = input;
                }else{
                    zero = 1;
                }
                pc += 6;
            }else{
                fprintf(stderr, INS);
                fprintf(stderr, "ERROR: Invalid readb instruction!");
                exit(1);
            }
            break;
        }
            
        case 0xc1:  //readl instruction
        {
            if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8) && (bit -> highHalf == 0xf)) {
                negative = 0;  overflow = 0;
                if (scanf("%d",&numValue) != EOF) {  //read in input (long)
                    zero = 0;
                    int *readLong = (int*)(memoryImage + ( registers[bit -> lowHalf]) + *ptr);
                    *readLong = numValue;
                }else{
                    zero = 1;
                }
                pc += 6;
            }else{
                fprintf(stderr, INS);
                fprintf(stderr, "ERROR: Invalid readl instruction!");
                exit(1);
            }
            break;
        }
            
        default:
        {
            fprintf(stderr, "ERROR: Invalid read instruction!");
            exit(1);
            break;
        }
    }
    return 0;
}

//Executes the write instructions:
int write(unsigned char* instruction){
    bitField * bit = (bitField *)(memoryImage + (pc) + 1);
    int *ptr = (int*)(memoryImage + (pc) + 2);
    
    switch (*instruction) {
        case 0xd0:  //writeb instruction
        {
            if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8) && (bit -> highHalf == 0xf)) {
                unsigned char *writeOut = (unsigned char*)(memoryImage + ( registers[bit -> lowHalf]) + *ptr);
                printf("%c",*writeOut); //print "long"
                pc += 6;
            }else{
                fprintf(stderr, INS);
                fprintf(stderr, "ERROR: Invalid writeb instruction!");
                exit(1);
            }
            break;
        }
            
        case 0xd1:  //writel instruction
        {
            if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8)&&(bit -> highHalf == 0xf)) {
                negative = 0;  overflow = 0;
                if ((bit -> lowHalf >= 0 && bit -> lowHalf <= 8) && (bit -> highHalf == 0xf)) {
                    int *writeOutLong = (int*)(memoryImage + ( registers[bit -> lowHalf]) + *ptr);
                    printf("%d",*writeOutLong); //print "byte"
                    pc += 6;
                }else{
                    zero = 1;
                }
                pc += 6;
            }else{
                fprintf(stderr, INS);
                fprintf(stderr, "ERROR: Invalid writel instruction!");
                exit(1);
            }
            break;
        }
            
        default:
        {
            fprintf(stderr, "ERROR: Invalid write instruction!");
            exit(1);
            break;
        }
    }
    return 0;
}

char * getReg(int reg){
    char* name;
    if (reg == 0) {
        name = "%EAX ";
    }
    else if (reg == 1) {
        name = "%ECX ";
    }
    else if (reg == 2) {
        name = "%EDX ";
    }
    else if (reg == 3) {
        name = "%EBX ";
    }
    else if (reg == 4) {
        name = "%ESP ";
    }
    else if (reg == 5) {
        name = "%EBP ";
    }
    else if (reg == 6) {
        name = "%ESI ";
    }
    else if (reg == 7) {
        name = "%EDI ";
    }else name = "";
    return name;
}


//Performs the decode portion of the cpu cycle:
int decode(unsigned char* instruction){
    bitField * bit = (bitField *)(memoryImage + (pc) + 1);
    int regB = bit -> highHalf;
    int regA = bit -> lowHalf;
    
    switch (*instruction) {
        case 0x00:  //Nop
            pc += 1;
            break;
        case 0x10:  //halt
        {    run = 0; printf("halt");
            printf(HLT); printf(AOK); exit(0);
            break;
        }
            //Move instructions:
        case 0x20:  //rrmovl
        {   printf("rrmovl ");
            printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x30: //irmovl
        {   printf("irmovl ");
            printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x40:  //rmmovl
        {   printf("rmmovl ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x50:  //mrmovl
        {   printf("mrmovl ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
            //Arithmetic/Logic instructions:
        case 0x60:  //addl
        {   printf("addl ");
            printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x61:  //subl
        {   printf("subl ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x62:  //andl
        {   printf("andl ");
            ALU(instruction);
            break;
        }
        case 0x63:  //xorl
        {   printf("xorl ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x64:  //mull
        {   printf("mull ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x65:  //cmpl
        {   printf("cmpl ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
            //Jump instructions (all are executed by the jump function):
        case 0x70:  //jmp
        {   printf("jmp ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x71:  //jle
        {   printf("jle ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x72:  //jl
        {   printf("jl ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x73:  //je
        {   printf("je ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x74:  //jne
        {   printf("jne ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x75:  //jge
        {   printf("jge ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x76:  //jg
        {   printf("jg ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
            //Call, return, push, and pop all utilize either the push or pop function:
        case 0x80:  //call
        {   printf("call ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0x90:  //return
        {   printf("return ");
            pop(8);
            pc =  registers[8];
            break;
        }
        case 0xa0:  //pushl
        {   printf("pushl ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0xb0:  //popl
        {   printf("popl ");
             printf("%s, %s\n",get(regA),get(regB));            break;
        }
            //Read and Write Instructions:
        case 0xc0:  //readb
        {   printf("readb ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0xc1:  //readl
        {   printf("readl ");
            printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0xd0:  //writeb
        {   printf("writeb ");
            printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0xd1:  //writel
        {   printf("writel ");
             printf("%s, %s\n",get(regA),get(regB));
            break;
        }
        case 0xe0:  //movsbl
        {    printf("%s, %s\n",get(regA),get(regB));
            movl(instruction);
            break;
        }
        default:    //invalid instruction encountered; exit.
        {
            fprintf(stderr, INS);
            exit(1);
            break;
        }
    }
    return 0;
}

//Performs the fetch portion of the cpu cycle:
int fetch(){
    while ( run == 1) {
        unsigned char * instruction = (unsigned char*) (memoryImage + pc);
        decode(instruction);   //decode the instruction
    }if ( run == 0) {
        printf(HLT); printf(AOK); exit(0);
    }else{
        fprintf(stderr, "ERROR: Invalid/Missing instruction or address.");
        exit(1);
    }
    return 0;
}

int main(int argc, const char *argv[]) {
    
    if (argc > 2 || argc < 2) { // "y86dis [-h] <y86 input file>" is the valid input format.
        fprintf(stderr, "ERROR: Invalid Number of Arguments (requires 1)\n");
        return 1;
    }if (strcmp(argv[1], "-h") == 0) {   //Usage Instruction
        printf("Usage: y86dis <y86 input filename here>\n");
        return 0;
    }if (strcmp(argv[1], "") == 0) { //no file name entered
        fprintf(stderr, "ERROR: No file name entered...\n");
        return 0;
    }if (strstr(argv[1], ".y86") == NULL) { //checks for a valid file type
        fprintf(stderr, "ERROR: This file is not .y86...\n");
        return 1;
    }
    
    int curr_line = 0;
    FILE* file = fopen(argv[1], "rb");  //open the file that was provided as input
    int dataLength = 0;
    if(file != NULL){
        char line[1500]; /*Initializes line array*/
        char str[6]; /*Initializes string array*/
        int size; /*Declares size*/
        int index; /*Declares index*/
        char *token; /*Declares token*/
        //cpu *cpu = malloc(sizeof cpu);   //Create a cpu struct
        size = 0;  run = 1;
        index = 0;
        for (; index < 9; index++) {
            registers[index] = 0;
        }
        
        while (fgets(line, 1500, file) != NULL) { /*While the line in the file is not null*/
            
            if (curr_line == 0) { /*If the line is the first line in the file*/
                sscanf(line, "%s %x\n", str, &size); /*Scans the line and assigns its members to variables*/
                memoryImage = (char *) malloc(sizeof(char)*size);
            }
            
            else { /*If the line is not the first line in the file*/
                
                token = strtok(line, " \t\n\f\v\r"); /*Splits the line's members into tokens, delimited by white-spaces*/
                
                if (strcmp(token, ".text") == 0) { /*If the directive is ".text"*/
                    token = strtok(NULL, " \t\n\f\v\r"); /*Moves to the next member and makes it a token*/
                    sscanf(token, "%x", &index); /*Assigns the next member to an int variable, "index"*/
                    pc = index;
                    token = strtok(NULL, " \t\n\f\v\r"); /*Moves to the next member and makes it a token*/
                    text(token, index);
                }
            }
            curr_line++;
        }
        fclose(file);
        fetch(); free(memoryImage); //free(cpu);
    }
}
