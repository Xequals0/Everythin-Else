/*
 * calc.h
 * Anirudh Tunoori netid: at813
 */
#ifndef calc_h_included
#define calc_h_included

typedef enum id { binary, hexadecimal, octal, decimal,undefined } identity; //enum identity

struct number_ { //number struct
    char* numStr;
    int sign;
    int value;
    char initialForm;
    enum id identity;
};
typedef struct number_ NumberN;

//Function definitions:
NumberN* NCreate(char * numStr);

void numDestroy(NumberN* num1, NumberN* num2, char* result);

int dec_octal_binary_ToInt(NumberN *numPtr);

int hexToInt(NumberN *numPtr);

int power(int base, int exponent);

int isBinary(char* num);

int isDecimal(char* num);

int isOctal(char* num);

int isHexadecimal(char* num);

int checkInput(char* argv[]);

int reverse(char* sequence);

int add(int first, int second);

int subtract(int first, int second);

int multiply(int first, int second);    //Performs multiplication

char* toBinaryDecimalOctal(int solution, char output);

char* toHexadecimal(int solution);

int checkOutput(NumberN* num1, NumberN* num2, char op, int answer);

#endif
