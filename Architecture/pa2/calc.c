/*
 * calc.c
 * Anirudh Tunoori netid: at813
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "calc.h"
//define the bases of each number system:
#define binary_Base 2
#define octal_Base 8
#define decimal_Base 10
#define hexadecimal_Base 16

int startDecimal, possiblyDecimal = 0; //0 is false, 1 is true

//Power function that returns an int (as opposed to a double)
// takes in a base and a power and returns
// the exponentiated value
int power(int base, int exponent){
    int result = base;
    if(exponent > 0){   //positive exponent case (most likely to be used)
        int index = 1;
        for(; index < exponent; index++){
            result *= base;
        }
    }
    if(exponent < 0){   //negative exponent case
        int index = 0;
        for(; (index-1) > exponent; index--){
            result = result * (base);
        } result = 1/result;
    }
    if (exponent == 0) {    //zero exponent case; return 1
        return 1;
    }
    return result;
}

/*Obtain the numeric (integer) value of NumberN
 * This function is called if it is determined that
 * the number is a binary, decimal, or octal
 */
int dec_octal_binary_ToInt(NumberN *numPtr) {
    char* str = numPtr -> numStr; int temp = 0;
    int base = 0; int base10Equiv = 0;
    //set the base for this function call; determined by the initialForm field.
    if (numPtr -> initialForm == 'B' || numPtr -> initialForm == 'b') base = binary_Base;
    else if (numPtr -> initialForm == 'D' || numPtr -> initialForm == 'd') base = decimal_Base;
    else if (numPtr -> initialForm == 'O' || numPtr -> initialForm == 'o') base = octal_Base;
    int index = 0;
    for (; *str != '\0'; index++) {
        if (numPtr -> sign == 1) { //negative case
            base10Equiv = (base10Equiv * base) + ((-1) * (*str - '0'));
            if (temp < base10Equiv) return 1;   //error case if the number is negative
        }else if (numPtr -> sign == 0) { //positive case
            base10Equiv = (base10Equiv * base) + (*str - '0');
            if (temp > base10Equiv) return 1;   //error case if the number is positive
        } temp = base10Equiv; str++;
    }
    numPtr -> value = base10Equiv;  //Assign the appropriate value to the value field of this
    return 0;                       //NumberN struct
}
/*Obtain the numeric (integer) value of NumberN
 * This function is called if it is determined that
 * the number is a hexadecimal
 */
int hexToInt(NumberN *numPtr) {
    char* str = numPtr -> numStr;   //obtain the string representation
    int base10Equiv = 0; int size = 0;
    int temp = 0; int hexVal = 0;
    
    while (*str != '\0') {
        size++; str++;
    }str--; //account for null bit
    int index = 0;
    for (; index < size; index++) {
        if (isdigit(*str)) hexVal = *str - '0'; //Obtain the appropriate numeric value for A-F or a-f
        else if(*str == 'A' || *str == 'a') hexVal = 10;
        else if(*str == 'B' || *str == 'b') hexVal = 11;
        else if(*str == 'C' || *str == 'c') hexVal = 12;
        else if(*str == 'D' || *str == 'd') hexVal = 13;
        else if(*str == 'E' || *str == 'e') hexVal = 14;
        else if(*str == 'F' || *str == 'f') hexVal = 15;
        
        if (numPtr -> sign == 1) { //negative case
            base10Equiv += ((-1) * hexVal * (int)power(hexadecimal_Base,index));  //calls the power function hexadecimal_base is defined as 16
            if (temp < base10Equiv) return 1;   //error case for a negative hexadecimal number
        }else if (numPtr -> sign == 0) { //positive case
            base10Equiv += (hexVal * (int)power(hexadecimal_Base,index));   //calls the power function hexadecimal_base is defined as 16
            if (temp > base10Equiv) return 1;   //error case for a positive hexadecimal number
        }temp = base10Equiv; str--;
    }
    numPtr -> value = base10Equiv;  //Assign the appropriate value to the value field of this NumberN struct
    return 0;
}

//Create the Number structs for both numbers and fill in all of the fields if appropriate.
//Returns the struct if everything is valid and null otherwise. The struct is dynamically
//allocated and is freed by the main function (the caller)
// if the number is negative the sign is 1 and 0 if it is positive
NumberN* NCreate(char* numStr) {
    NumberN* newNum = malloc(sizeof(struct number_));   //dynamically allocate memory for a NumberN struct
    newNum -> value = 0;
    if (numStr[0] == '-') { //Negative numbers; Fill out the fields of the struct
        newNum -> sign = 1;
        newNum -> initialForm = numStr[1];
        newNum -> numStr = strdup(&numStr[2]);
    }else{                  //Positive numbers; Fill out the fields of the struct
        newNum -> sign = 0;
        newNum -> initialForm = numStr[0];          //the initial form is assigned based on the input and
        newNum -> numStr = strdup(&numStr[1]);      //it was already checked by one of the "is_" functions
        
    }
    //Now obtain a numerical value for this number, as all other fields of the struct have been filled.
    if(newNum -> initialForm == 'B' || newNum -> initialForm == 'b' ||newNum -> initialForm == 'D'
     ||newNum -> initialForm == 'd' ||newNum -> initialForm == 'O' ||newNum -> initialForm == 'o'){
        if (dec_octal_binary_ToInt(newNum) == 0) {  //obtain the correct numerical (integer) value of a octal/binary/decimal string
            return newNum;
        }else{
            newNum -> identity = undefined;
            return NULL;    //one of the error cases from the dec_octal_binary_ToInt function
        }
    }else if(newNum -> initialForm == 'X' || newNum -> initialForm == 'x'){
        if (hexToInt(newNum) == 0) {    //obtain the correct numerical (integer) value of a hexadecimal string
            return newNum;
        }else{             //one of the error cases from the hexToInt function
            newNum -> identity = undefined;
            return NULL;
        }
    }
    return newNum;  //return the number struct
}

//Clear the dynamically allocated memory; primarily the number struct:
void numDestroy(NumberN* num1, NumberN* num2, char* result){
    free(num1);
    free(num2);
    free(result);
}

//Number Checks (binary, decimal, hexadecimal, octal):
int isBinary(char* num){    //checks a string to see if it is a representation of a binary number
    while (*num != '\0') {
        if (*num == '0' || *num == '1') num++;
        else return 1;  //error case detected
    }
    return 0;
}

int isDecimal(char* num){   //checks a string to see if it is a representation of a decimal number
    while (*num != '\0') {  //go through each chacter/digit should ony be 0-9
        if (startDecimal == 1){
            if (*num <= '9' && *num >= '1'){ possiblyDecimal = 1;   //first digit can't be 0
                break;
            }else if(strlen(num) == 1 && isdigit(*num)){    //one digit case
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
            if (*num <= '9' && *num >= '0') break;
            else{
                possiblyDecimal = 0;
                return 1;   //not a decimal
            }
        } num++;
    }
    possiblyDecimal = 0;   //reset for the next call
    return 0;
}

int isOctal(char* num){  //checks a string to see if it is a representation of a octal number
    while (*num != '\0') {
        if (*num <= '7' && *num >= '0') num++;  //contains only 0-7
        else return 1;  //error case
    }
    return 0;
}

int isHexadecimal(char* num){   //checks a string to see if it is a representation of a hexadeimal number
    while (*num != '\0') {  //should only contain 0-9, a-f, or A-F
        if ((*num <= '9' && *num >= '0') || (*num <= 'f' && *num >= 'a') || (*num <= 'F' && *num >= 'A')) num++;
        else return 1;  //error case
    }
    return 0;
}
//This function takes a string and returns it upon
//reversing it.
int reverse(char* sequence){    
    int index = 0; char current;
    int size = strlen(sequence) -1;
    while(index < size){    //simple switching of characters (runs size/2 times)
        current = sequence[index];
        sequence[index] = sequence[size];
        sequence[size] = current;
        size--; index++;
    }
    return 0;
}

//The three operations:
int add(int first, int second){ //adds two integers and returns the sum
    return(first + second);
}
int subtract(int first, int second){ //subtracts two integers and returns the difference
    return(first - second);
}
int multiply(int first, int second){ //multiplies two integers and returns the product
    return(first * second);
}
//This function converts an integer to a string in the desired output form.
//This function specifically processes the binary, decimal, and octal cases
char* toBinaryDecimalOctal(int solution, char output){
    int index = 0; int base = 0; int sign = 0;
    char* outputStr = malloc(33*sizeof(char));  //32 chars plus one null bit
    
    if (solution < 0) sign = 1; //for negative solutions
    //Set the base (numerically) of the output
    if (output == 'B' || output == 'b') {
        base = binary_Base;
        output = 'b';
    }else if (output == 'D' || output == 'd') {
        base = decimal_Base;
        output = 'd';
    }else if (output == 'O' || output == 'o') {
        base = octal_Base;
        output = 'o';
    }
    
    if (solution == 0) {    //zero case
        outputStr[0] = output; outputStr[1] = '0'; outputStr[2] = '\0';
        return outputStr;
    }
    
    while (solution != 0) { //conversion process:
        int precision = solution % base;
        if (precision < 0)  precision *= -1;
        outputStr[index] = precision + '0'; //add an int to a string
        index++; solution /= base;
    }
    
    //A few modifications:
    outputStr[index] = output; index++;
    if (sign == 1) {
        outputStr[index] = '-'; index++;    //add the sign
    }outputStr[index] = '\0';   //add the null-terminating bit
    reverse(outputStr); //reverse
    
    return outputStr;   //returns the correct output string
}

char* toHexadecimal(int solution){
    int index = 0; int sign = 0;
    char* outputStr = malloc(33*sizeof(char));  //32 chars plus one null bit
    
    if (solution == 0) {    //zero case
        outputStr[0] = 'x'; outputStr[1] = '0'; outputStr[2] = '\0';
        return outputStr;
    }if (solution < 0) {    //for negative solutions
        sign = 1;
        solution *= -1;
    }
    
    while (solution != 0) { //conversion process:
        int precision = solution % hexadecimal_Base;
        if (precision < 0)  precision *= -1;
        if (precision == 10) outputStr[index] = 'a';    //Assigns the correct hex letter for the appropriate number
        else if (precision == 11) outputStr[index] = 'b';
        else if (precision == 12) outputStr[index] = 'c';
        else if (precision == 13) outputStr[index] = 'd';
        else if (precision == 14) outputStr[index] = 'e';
        else if (precision == 15) outputStr[index] = 'f';
        else outputStr[index] = precision + '0';    //add an int to a string
        index++; solution /= hexadecimal_Base;
    }
    
    //A few modifications:
    outputStr[index] = 'x'; index++;
    if (sign == 1) {
        outputStr[index] = '-'; index++;    //add the sign
    }outputStr[index] = '\0';               //add the null-terminating bit
    reverse(outputStr); //reverse
    
    return outputStr;   //returns the correct output string
}
//Check/process the output for errors
int checkOutput(NumberN* num1, NumberN* num2, char op, int answer){
    int temp = 0;
    //Cases are the operations
    if (op == '+') {
        if ((num1 -> sign) == 1 && (num2 -> sign) == 1) {
            if ((answer > num1 -> value) || (answer > num2 -> value)) return 1; //error case
        }else if ((num1 -> sign) == 0 && (num2 -> sign) == 0) {
            if ((answer < num1 -> value) || (answer < num2 -> value)) return 1; //error case
        }
    }
    if (op == '-') {
        if ((num1 -> sign) == 0 && (num2 -> sign) == 1) {
            if (answer < num1 -> value) return 1;   //error case
        }else if ((num1 -> sign) == 1 && (num2 -> sign) == 0) {
            if (answer > num1 -> value) return 1;   //error case
        }
    }
    if (op == '*') {
        temp = answer / (num1 -> value);
        if(temp != num2 -> value) return 1; //error case
    }
    return 0;
}

//This function checks the input for errors also calls the "is_" functions where appropriate
int checkInput(char* argv[]){
    int badNum1 = 0; int badNum2 = 0; char type1 = '\0'; char type2 = '\0';
    char* num1 = NULL; char* num2 = NULL;
    if (strlen(argv[1]) != 1 || (argv[1][0] != '+' && argv[1][0] != '-' && argv[1][0] != '*')) {    //check operation
        fprintf(stdout,"Invalid operation, must be: '+', '-', or '*' \n");
    }
    //Check numbers: argv[2] and argv[3]
    if(argv[2][0] == '-' && strlen(argv[2]) <= 2) badNum1 = 1;
    else if(strlen(argv[2]) <= 1) badNum1 = 1;
    else if(argv[2][0] == '-'){
        type1 = argv[2][1];
        num1 = strdup(&argv[2][2]);
    }
    else{
        type1 = argv[2][0];
        num1 = strdup(&argv[2][1]);
    }

    if(argv[3][0] == '-' && strlen(argv[3]) <= 2) badNum2 = 1;
    else if(strlen(argv[3]) <= 1) badNum2 = 1;
    else if(argv[3][0] == '-'){
        type2 = argv[3][1];
        num2 = strdup(&argv[3][2]);
    }
    else{
        type2 = argv[3][0];
        num2 = strdup(&argv[3][1]);
    }
    //Now call sthe specific "is_" functions:
    //for the first number:
    if (type1 == 'B' || type1 == 'b') {
        if (isBinary(num1) != 0) badNum1 = 1;
    }if (type1 == 'O' || type1 == 'o') {
        if (isOctal(num1) != 0) badNum1 = 1;
    }if (type1 == 'D' || type1 == 'd') {
        if (isDecimal(num1) != 0) badNum1 = 1;
    }if (type1 == 'X' || type1 == 'x') {
        if (isHexadecimal(num1) != 0) badNum1 = 1;
    }
    //and for the secondnumber:
    if (type2 == 'B' || type2 == 'b') {
        if (isBinary(num2) != 0) badNum2 = 1;
    }if (type2 == 'O' || type2 == 'o') {
        if (isOctal(num2) != 0) badNum2 = 1;
    }if (type2 == 'D' || type2 == 'd') {
        if (isDecimal(num2) != 0) badNum2 = 1;
    }if (type2 == 'X' || type2 == 'x') {
        if (isHexadecimal(num2) != 0) badNum2 = 1;
    }
    //Do the error reporting:
    if(badNum1 == 1 && badNum2 == 1) fprintf(stdout," Firat and Second numbers are invalid inputs");
    else if(badNum1 == 1) fprintf(stdout,"ERROR: The firat number is an invalid input");
    else if(badNum2 == 1) fprintf(stdout,"ERROR: The second number is an invalid input");
    
    if (strlen(argv[4]) != 1 || (argv[4][0] != 'b' && argv[4][0] != 'o' && argv[4][0] != 'd' && argv[4][0] != 'x')) {     //check output_format
        fprintf(stdout,"ERROR: Invalid output format, must be: 'b' for binary, 'o' for octal, 'd' for decimal or 'x' for hexadecimal \n");
    }
    free(num1); free(num2);
    return 0;
}

int main(int argc, char* argv[]){
    int numericalResult = 0;
    char* result;
    if(argc != 5){
        fprintf(stderr,"ERROR: Invalid number of arguments... should have 4 \n"); return 0;
    }

    /*extract arguments
     *   //char* operation, num1, num2, outputFormat, result;
     *   //char* result = (char*)malloc(sizeof(char));
     *  char* outputFormat = (char*)malloc(strlen(argv[4]));
     *  strcpy(outputFormat,argv[4]);
     */
    if(checkInput(argv) != 0){
        return 0;
    }
    
    //Create two NumberN's one for each number:
        NumberN* num1 = NCreate(argv[2]);
        NumberN* num2 = NCreate(argv[3]);
    //Check the size of the numbers, post-conversion:
    if (num1 == NULL) fprintf(stderr,"ERROR: The size of the first number is greater than 32 bits \n");
    if (num2 == NULL) fprintf(stderr,"ERROR: The size of the second number is greater than 32 bits \n");
    if (num1 == NULL || num2 == NULL) return 0; //error cases
    
    //Perform the calculation:
    int x = num1 -> value;  //obtain the numbers
    int y = num2 -> value;
    if ((x == 0 || y == 0) && (argv[1][0] == '*')) {
        printf("%c0 \n",argv[4][0]); return 0;
    }
    if(argv[1][0] == '+') numericalResult = add(x,y);   //call the specific operation fucntion
    else if(argv[1][0] == '-') numericalResult = subtract(x,y);
    else if(argv[1][0] == '*') numericalResult = multiply(x,y);
    
    //Check the size or the reult:
    if (checkOutput(num1, num2, argv[1][0], numericalResult) != 0) {
        fprintf(stderr,"ERROR: The size of the answer is greater than 32 bits"); return 0;
    }
    
    //Convert the result into the required output base.
    if(argv[4][0] == 'B' || argv[4][0] == 'D' || argv[4][0] == 'O' ||
       argv[4][0] == 'b' || argv[4][0] == 'd' || argv[4][0] == 'o')
    {
        result = toBinaryDecimalOctal(numericalResult, argv[4][0]); //obtain the binary/octal/decimal result
    }else result = toHexadecimal(numericalResult);  //obtain the hex result
    printf("%s \n",result); //Print the output
    numDestroy(num1,num2,result);   //Free dynamically allocated memory
    
    return 0;
}
