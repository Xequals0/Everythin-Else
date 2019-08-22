/*
 * y86emul.c
 * Anirudh Tunoori netid: at813
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "y86emul.h"
//#include "mallocld.h"

char* memoryImage;
int pc;
int run;
int registers[9];
int overflow;
int zero;
int negative;

//Conversion functions were modified from calc/pa2
/*int hexadecimal_to_decimal(char* hexadecimalValue){
    char *binaryValue = malloc(4 * strlen(hexadecimalValue) + 1);
    binaryValue[4 * strlen(hexadecimalValue)] = '\0';
    int pos = 0; char* digit = "";
    
    for (; pos < strlen(hexadecimalValue); pos++) {
        if (hexadecimalValue[pos] == '0') digit = "0000";
        else if(hexadecimalValue[pos] == '1') digit = "0001";
        else if(hexadecimalValue[pos] == '2') digit = "0010";
        else if(hexadecimalValue[pos] == '3') digit = "0011";
        else if(hexadecimalValue[pos] == '4') digit = "0100";
        else if(hexadecimalValue[pos] == '5') digit = "0101";
        else if(hexadecimalValue[pos] == '6') digit = "0110";
        else if(hexadecimalValue[pos] == '7') digit = "0111";
        else if(hexadecimalValue[pos] == '8') digit = "1000";
        else if(hexadecimalValue[pos] == '9') digit = "1001";
        
        else if(hexadecimalValue[pos] == 'A' || hexadecimalValue[pos] == 'a') digit = "1010";
        else if(hexadecimalValue[pos] == 'B' || hexadecimalValue[pos] == 'b') digit = "1011";
        else if(hexadecimalValue[pos] == 'C' || hexadecimalValue[pos] == 'c') digit = "1100";
        else if(hexadecimalValue[pos] == 'D' || hexadecimalValue[pos] == 'd') digit = "1101";
        else if(hexadecimalValue[pos] == 'E' || hexadecimalValue[pos] == 'e') digit = "1110";
        else if(hexadecimalValue[pos] == 'F' || hexadecimalValue[pos] == 'f') digit = "1111";
        else{   fprintf(stderr,("ERROR: Found at least one irregular value in the input\n"));
            digit = "";
        }//printf("%c    %s \n", hexadecimalValue[pos], digit); testing purposes...
        binaryValue = strcat(binaryValue, digit);
    } pos = 0; int decimalValue = 0;
    
    int power = strlen(binaryValue) - 1;
    for (; pos < strlen(binaryValue); pos++) {
        if (binaryValue[pos] == '1') {
            decimalValue += (int) pow(2,power);
        }
        power--;
    }
    return decimalValue;
}*/

//fills in the contents of the .bss directive to the memory
int loadBss(char *byte, int address) {
    int num;    char numChar;
    sscanf(byte, "%2x", &num);
    numChar = num;
    memoryImage[address] = numChar;
    return 0;
}

//fills in the contents of the .byte directive to the memory
int loadBytes(char *byte, int address) {
    int num;    char numChar;
    sscanf(byte, "%2x", &num);
    numChar = num;
    memoryImage[address] = numChar;
    return 0;
}

//fills in the contents of the .string directive to the memory
int loadString(char *longString, int index) {
    while (*longString != '\0') { /*Goes through the string character by character*/
        memoryImage[index] = *longString;
        index++;
        longString++;
    }return 0;
}

//fills in the contents of the .long directive to the memory
int loadLong (char *longString, int index) {
    int num;
    sscanf(longString, "%d", &num);
    memoryImage[index] = num;    return 0;
}

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
    bitField * bit = (bitField *)(memoryImage + ( pc) + 1);
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

//Performs the decode portion of the cpu cycle:
int decode(unsigned char* instruction){
    switch (*instruction) {
        case 0x00:  //Nop
             pc += 1;
            break;
        case 0x10:  //halt
        {    run = 0; //printf("halt");
            printf(HLT); printf(AOK); exit(0);
            break;
        }
        //Move instructions:
        case 0x20:  //rrmovl
        {  // printf("rrmovl");
            movl(instruction);
            break;
        }
        case 0x30: //irmovl
        {   //printf("irmovl");
            movl(instruction);
            break;
        }
        case 0x40:  //rmmovl
        {   //printf("rmmovl");
            movl(instruction);
            break;
        }
        case 0x50:  //mrmovl
        {   //printf("mrmovl");
            movl(instruction);
            break;
        }
        //Arithmetic/Logic instructions:
        case 0x60:  //addl
        {   //printf("addl");
            ALU(instruction);
            break;
        }
        case 0x61:  //subl
        {  // printf("subl");
            ALU(instruction);
            break;
        }
        case 0x62:  //andl
        {   //printf("andl");
            ALU(instruction);
            break;
        }
        case 0x63:  //xorl
        {  // printf("xorl");
            ALU(instruction);
            break;
        }
        case 0x64:  //mull
        {   //printf("mull");
            ALU(instruction);
            break;
        }
        case 0x65:  //cmpl
        {   //printf("cmpl");
            ALU(instruction);
            break;
        }
        //Jump instructions (all are executed by the jump function):
        case 0x70:  //jmp
        {  // printf("jmp");
            jump(instruction);
            break;
        }
        case 0x71:  //jle
        {  // printf("jle");
            jump(instruction);
            break;
        }
        case 0x72:  //jl
        { //  printf("jl");
            jump(instruction);
            break;
        }
        case 0x73:  //je
        { //  printf("je");
            jump(instruction);
            break;
        }
        case 0x74:  //jne
        { //  printf("jne");
            jump(instruction);
            break;
        }
        case 0x75:  //jge
        {  // printf("jge");
            jump(instruction);
            break;
        }
        case 0x76:  //jg
        { //  printf("jge");
            jump(instruction);
            break;
        }
       //Call, return, push, and pop all utilize either the push or pop function:
        case 0x80:  //call
        { //  printf("call");
            int *functionPtr = (int*)(memoryImage + ( pc) + 1);
             registers[8] = ( pc) + 5;
            push(8);
             pc = *functionPtr;
            break;
        }
        case 0x90:  //return
        { //  printf("return");
            pop(8);
             pc =  registers[8];
            break;
        }
        case 0xa0:  //pushl
        {//printf("pushl");
         bitField *pushBit = (bitField *)(memoryImage + ( pc) + 1);
         int reg = pushBit -> lowHalf;
            if ((pushBit -> highHalf == 0xf) && (reg >= 0 && reg <= 8)) {
                push(reg); //execute the push instruction using the given register
            }else{
                fprintf(stderr, INS);
                fprintf(stderr, "ERROR: Invalid push!");
                exit(1);
            }
            break;
        }
        case 0xb0:  //popl
        {//printf("popl");
            bitField *popBit = (bitField *)(memoryImage + ( pc) + 1);
            int reg = popBit -> lowHalf;
            if ((popBit -> highHalf == 0xf) && (reg >= 0 && reg <= 8)) {
                pop(reg); //execute the push instruction using the given register
            }else{
                fprintf(stderr, INS);
                fprintf(stderr, "ERROR: Invalid pop!");
                exit(1);
            }
            break;
        }
        //Read and Write Instructions:
        case 0xc0:  //readb
        {   //printf("readb");
            read(instruction);
            break;
        }
        case 0xc1:  //readl
        {  // printf("readl");
            read(instruction);
            break;
        }
        case 0xd0:  //writeb
        {  // printf("writeb");
            write(instruction);
            break;
        }
        case 0xd1:  //writel
        {  // printf("writel");
            write(instruction);
            break;
        }
        case 0xe0:  //movsbl
        {  // printf("movsbl");
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
    
    if (argc > 2 || argc < 2) { // "y86emul [-h] <y86 input file>" is the valid input format.
        fprintf(stderr, "ERROR: Invalid Number of Arguments (requires 1)\n");
        return 1;
    }if (strcmp(argv[1], "-h") == 0) {   //Usage Instruction
        printf("Usage: y86emul <y86 input filename here>\n");
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
                
                else if (strcmp(token, ".byte") == 0) { /*If the directive is ".byte"*/
                    token = strtok(NULL, " \t\n\f\v\r"); /*Moves to the next member and makes it a token*/
                    sscanf(token, "%x", &index); /*Assigns the next member to an int variable, "index"*/
                    token = strtok(NULL, " \t\n\f\v\r"); /*Moves to the next member and makes it a token*/
                    loadBytes(token, index);
                    
                }
                else if (strcmp(token, ".bss") == 0) { /*If the directive is ".byte"*/
                    token = strtok(NULL, " \t\n\f\v\r"); /*Moves to the next member and makes it a token*/
                    sscanf(token, "%x", &index); /*Assigns the next member to an int variable, "index"*/
                    token = strtok(NULL, " \t\n\f\v\r"); /*Moves to the next member and makes it a token*/
                    loadBss(token, index);
                }
                
                else if (strcmp(token, ".long") == 0) { /*If the directive is ".long"*/
                    token = strtok(NULL, " \t\n\f\v\r"); /*Moves to the next member and makes it a token*/
                    sscanf(token, "%x", &index); /*Assigns the next member to an int variable, "index"*/
                    token = strtok(NULL, " \t\n\f\v\r"); /*Moves to the next member and makes it a token*/
                    loadLong(token, index);
                }
                
                else if (strcmp(token, ".string") == 0) {
                    
                    token = strtok(NULL, " \t\n\f\v\r"); /*Moves to the next member and makes it a token*/
                    sscanf(token, "%x", &index); /*Assigns the next member to an int variable, "index"*/
                    token = strtok(NULL, " \t\n\f\v\r"); /*Moves to the next member and makes it a token*/
                    token++;
                    
                    while (token != NULL) { /*Goes through the entire string*/
                        char *nextToken = strtok(NULL, " \t\n\f\v\r");
                        int len;
                        if (nextToken != NULL) {
                            len = strlen(token); /*Assigns the length of the string to a variable*/
                            loadString(token, index);
                            index += len;
                            memoryImage[index] = ' ';
                            index++;    token = nextToken;
                        }
                        else {
                            int lastChar = strlen(token)-1;
                            token[lastChar] = '\0';len = strlen(token);
                            loadString(token, index);
                            index += len;   index++;    memoryImage[index] = '\0';
                            break;
                        }
                    }
                }
                else {
                    fprintf(stderr, "ERROR: Invalid directive.\n");
                    exit(1);
                }
            }
            curr_line++;
        }
        fclose(file);
        fetch(); free(memoryImage); //free(cpu);
    }
}
