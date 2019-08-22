/*
 * y86emul.h
 * Anirudh Tunoori netid: at813
 */
#ifndef y86dis_h_included
#define y86dis_h_included

//Status Codes:
#define AOK "Everything is fine.\n"
#define HLT "Halt instruction has been encountered.\n"
#define ADR "Invalid address encountered...\n"
#define INS "Invalid instruction encountered...\n"


//Define all 8 registers:
#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

struct BITFIELD_{
    unsigned char highHalf: 4; /*The upper half of the bitfield (the first four bits)*/
    unsigned char lowHalf: 4; /*The lower half of the bitfield (the last four bits)*/
};  typedef struct BITFIELD_ bitField;

struct CPU_ {
    int registers[9];   //array of registers
    int pc; //program counter
    int size;
    int run;
    
    //flags:
    int overflow;
    int zero;
    int negative;
};  typedef struct CPU_ cpu;

//FUNCTION PROTOTYPES: //
//int hexadecimal_to_decimal(char* hexadecimalValue); //hex to decimal converter
//int allocateMemory(char* data); //allocate memory into the memory image

int loadBss(char *byte, int address); //load the contents of .bss
int loadBytes(char *byte, int address); //load the contents of .byte
int loadString(char *longString, int index); //load the contents of .string
int loadLong (char *longString, int index);   //load the contents of .long
int text(char *arg, int i); //load the contents of .text

int push(int reg);                //Executes the push instruction
int pop(int reg);                 //Executes the pop instruction
int movl(unsigned char* instruction); //Executes all of the move instructions
int ALU(unsigned char* instruction); //Executes the Arithmetic and Logical instructions
int jump(unsigned char* instruction); //Executes the jump instructions
int read(unsigned char* instruction); //Executes the read instructions
int write(unsigned char* instruction); //Executes the write instructions
int fetch();                                        //fetch cycle
int decode(unsigned char* instruction);          //decode cycle


#endif
