/*
 * format.h
 * Anirudh Tunoori netid: at813
 */
#ifndef format_h_included
#define format_h_included

//Function definitions:
int isBinary(char* input);

char* toInt(char* bitStr);

double power(int binaryBase, int exponent);

double ln(double x);

double log10(double x);

int specialCases(int sign, int exponent, double mantissa);

char* toFloat(char* bitStr);

int reverse(char* sequence);

#endif
