/*
 * format.c
 * Anirudh Tunoori netid: at813
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "format.h"
#define LN10 2.3025850929940456840179914546844  //The natural log of 10 used in the log10 function

//This function determines if a string is binary (contains only 0's and 1's)
//It also checks that the string does not exceed (or is not less than)
//32 bits/characters
int isBinary(char* input){
    if(strlen(input) != 32) return 1;   //checks size
    while(*input != '\0'){
        if(*input != '0' && *input != '1') return 2;    //checks contents
        input++;
    }
    return 0;
}
/*This is a power function that raises a base to a power and returns the
 *result. It can handle any integer exponent (positive or negative)
 */
double power(int binaryBase, int exponent){
    double result = binaryBase;
    if(exponent > 0){   //for positive exponents
        int index = 1;
        for(; index < exponent; index++){
            result *= binaryBase;
        }
    }
    if(exponent < 0){   //for negative exponents
        int index = 0;
        for(; (index-1) > exponent; index--){
            result = result * (binaryBase);
        } result = 1/result;
    }
    if (exponent == 0) {    //for zero exponent
        return 1.0;
    }
    return result;
}
/*This is a natural log function. It is used to implement the log10 function
 *It can handle any integer exponent (positive or negative)
*/
double ln(double x)
{  // printf("ln entry");
    double target = 0.0;
    double diffX = (x - 1) / (x + 1);
    double diffX_squared = diffX * diffX;
    double denominator = 1.0;
    double fraction = diffX;
    double sum = diffX;
    
    while (sum != target)
    {
        target = sum;
        denominator += 2.0;
        fraction *= diffX_squared;
        sum += fraction / denominator;
    }
    return (2.0 * sum); //returns the natural log of x.
}
//This function returns the log (base 10) of a number
double log10(double x) {
    return ln(x) / LN10;    //Change of log base formula
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

/*This function handles all of the special cases of an input
 *including 0.0e0, -0.0e0, not a number, positive infinity and negative infinity
 */
int specialCases(int sign, int exponent, double mantissa){
    if(exponent == 255){
        if(mantissa != 1.0){ printf("NaN \n"); exit(0); }   //NaN case
        else{
            if(sign == 1){ printf("ninf \n"); exit(0); }    //ninf case
            else{ printf("pinf \n"); exit(0); }             //pinf case
        }
    }
    else if(exponent == 0 && mantissa == 0.0){
        if(sign == 0){ printf("0.0e0 \n"); exit(0); }   //positive 0
        else{ printf("-0.0e0 \n"); exit(0); }           //negative 0
    }
    return 0;
}

//Returns the integer value (as a string) of the input bit sequence (two's complement).
char* toInt(char* bitStr){
    int sign, value, index = 0;
    char* intStr = malloc(sizeof(char)*(32 + 2));   //dynamically allocate memory, 32 chars, sign bit, null bit
    
    if(bitStr[0] == '1'){   //for negative integers
        sign = 1; reverse(bitStr);  //reverse the string
        char* ptr = bitStr;
        char* flipBit = bitStr; //follow the rules of the complements:
        for(;(*ptr != '1' && *ptr != '\0'); ptr++){
            *ptr = '1';
        }if(*ptr == '1') *ptr = '0';
        reverse(bitStr);
        
        for(;*flipBit != '\0'; flipBit++){  //flips the bits
            if(*flipBit == '1') *flipBit = '0';
            else *flipBit = '1';
        }
    }
    //Obtain integer values from the binary string
    for(;*bitStr != '\0'; bitStr++){
        if(sign == 1){
            value = (2 * value) + ((*bitStr - '0')*(-1));   //use everything as an int (- '0')
        } else value = (2 * value) + ((*bitStr - '0')*(1)); //use everything as an int (- '0')
    }
  
    
    if(value == 0){  //If the input's representation is zero
        intStr[0] = '0';
        intStr[1] = '\0';
        return intStr;  //return a 0 followed by the null-terminating bit
    }
    while(value != 0){  //Convert int to string (while maintaining precision):
        int precision  = value%10;
        if(precision < 0) precision *= -1;
        intStr[index] = precision + '0';
        index++; value /= 10;
    }
    
    if(sign == 1){   //Add the sign of the integer if necessary
        intStr[index] = '-'; index++;
    }
    
    intStr[index] = '\0';   //Add the null-terminating bit
    reverse(intStr);        //Reverse the string
    return intStr;          //returns the desired string (integer representation)
}
//Returns the floating_point value (as a string) of the input bit sequence (IEEE 754 single precision).
char* toFloat(char* bitStr){
    int exponent = 0;
    int sign = bitStr[0] - '0'; //hold the sign as an int
    double magnitude = 1.0;
    int startMantissa = 1;  //23 of the bits are reserved for the magnitude according to IEEE754
    int endMantissa = 23;   //establish the end boundary
    char* mantissaPtr = &bitStr[9];
    
    char* floatString = malloc(16*sizeof(char));    //dynamically allocate memory for a string that will hold a floating point representation
    char* ieee754String = floatString;
    int i = 1; // an index
    for(; i < 9; i++){
        exponent = (2*exponent) + (bitStr[i] - '0');    //does an operation using a char and stores it to an int
    }if(exponent == 0){ startMantissa = 0; endMantissa = 22; magnitude = 0.0;}
    int index = startMantissa;
    for(; index <= endMantissa; index++){
        if(*mantissaPtr == '1') magnitude = magnitude + power(2,(-1*index));
        mantissaPtr++;
    }
    specialCases(sign, exponent, magnitude);    //calls the special cases function
    double convertedMagnitude = magnitude*power(2,(exponent - 127));
    exponent = log10(convertedMagnitude);   //call the log10 function to obtain the appropriate exponent
    if(exponent < 0){   //negative exponent case:
        exponent--; int temp = exponent;
        while (temp != 0) {
            convertedMagnitude *= 10;   //for each power of 10 obtained from "exponent" multiply by 10.
            temp++;
        }
    }else{  //positive exponent case:
        int temp = exponent;
        while (temp != 0) {
            convertedMagnitude /= 10;
            temp--;
        }
    }
    
    if (sign == 1) {    //for negative numbers
        *floatString = '-';
        floatString++;
    }
    int precisionIndex = 0;
    while (precisionIndex < 7 && convertedMagnitude != 0) { //8 bits are reserved for the exponent
        if (precisionIndex == 1) {
            *floatString = '.'; //add the decimal after the first digit (one's place)
            floatString++;
        }
       /* convertedMagnitude = convertedMagnitude%10
        printf("test: %f \n", convertedMagnitude);*/
        int trunc = convertedMagnitude + 1e-9;
        //printf("test2: %d \n", test);
        *floatString = (trunc)%10 + '0';
        convertedMagnitude = 10*(convertedMagnitude - (*floatString - '0'));
        floatString++;
        precisionIndex++;
    }*floatString = 'e'; floatString++;
    
    if (exponent == 0) {    //zero case
        *floatString = '0'; floatString++;
    }if (exponent < 0) {    //add the negative to the string
        *floatString = '-'; floatString++;
    }
    
    char* reverseString = floatString;
    while(exponent != 0){   //use precision to ensure that each digit has the appropriate significance
        int precision  = exponent%10;   //obtain the "precision" value
        if(precision < 0) precision *= -1;
        *floatString = precision + '0'; //store an int to a string
        floatString++; exponent /= 10;
    }
    
    reverse(reverseString); //reverse the string
    *floatString = '\0';    //add the null terminating bit
    return ieee754String;   //returns the desired string (floating_point representation)
}

/*The main function: reads in the input checks it for errors
 *and calls the appropriate function (toFloat or toInt) depending
 *on the required output type. It then takes the returned string
 *representation and prints/returns it.
 */
int main(int argc, char** argv){
    char* output;

    if(argc != 3){
        fprintf(stderr,"ERROR: Invalid number of arguments... should be 2 \n"); return 0;
    }if(isBinary(argv[1]) == 1){
        fprintf(stderr,"ERROR: Invalid size (not 32 bits) \n"); return 0;
    }if(isBinary(argv[1]) == 2){
        fprintf(stderr,"ERROR: Bit sequence is invalid (not 0's and 1's) \n"); return 0;
    }
    
    if(strcasecmp(argv[2],"int") == 0){ //int case
        output = toInt(argv[1]);
    }else if(strcasecmp(argv[2],"float") == 0){ //float case
        output = toFloat(argv[1]);
    }else{
        fprintf(stderr,"ERROR: Invalid (type) format (must be int or float) \n"); return 0;
    }
    
    printf("%s \n",output);
    free(output);   //free the string
    return 0;
}
