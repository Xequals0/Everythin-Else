/*
 * tokenizer.c
 * Anirudh Tunoori netid: at813
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
    char *tkString;
    int index;
    int size;
 };

// Token identities.
enum tokenId {decimal, octal, hexadecimal, floating_point, undefined,
              mal,oct_hex_float_dec,unsigned_float, float_int};
enum tokenId id = undefined;    //initial state of the token
char errorChar = '0';           //error character is used for outputting escape values
typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {
    /* char arr[size];
    arr[size - 1] = '\0';
    strncpy(arr, ts, size);*/
    int size = strlen(ts); char arr[size];  //create a character array that has the size of the length of the input string
    strcpy(arr,ts); int arrSize = strlen(arr);  //copy the string into the array so that the input is not dependent on staying immutable
    TokenizerT *tkPtr = malloc(sizeof(struct TokenizerT_)); //dynamically allocate space for a tokenizer struct; must be freed by the destroy function which is called by the caller of this function.
    {
        tkPtr->index = 0;   //assign initial values to the members of the tokenizer struct
        tkPtr->size = arrSize;
        tkPtr->tkString = strdup(arr);  //copy the string
    }
    if(tkPtr != NULL){  //return the struct iff. it is not null and all of the members of the struct are defined.
        return tkPtr;
    }
  return NULL;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
    free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {
    int startPos, finalPos, indexBoundary;
    startPos = tk->index;   //obtain the starting index
    indexBoundary = (tk->size)-1;   //obtain the index boundary so that the "scope" stays within the bounds
    char *ptr = tk->tkString;
    int tokenSize;

    if(startPos > indexBoundary) return 0;   //ensure that the index is in bounds
    while((startPos < indexBoundary) && isspace(ptr[startPos])){    //increment the index past the whitespace
        startPos++;
    }if((startPos == indexBoundary) && isspace(ptr[startPos]))  //the input string contains only whitespace
        return 0;
    finalPos = startPos+1;  //initialize the final position
    while((finalPos <= indexBoundary) && !isspace(ptr[finalPos])){    //increment the final position to the end of the token
        finalPos++;
    }
    
    
    tokenSize = finalPos - startPos;    //determine the length/size of the token
    tk->index = finalPos + 1; int i;
    char *newToken = malloc ((tokenSize+2)*sizeof(char));       //dynamic allocation; is freed in the main function
    for(i = 0; i < tokenSize; i++){     //obtain the new token
        newToken[i] = ptr[i + startPos];
    }newToken[i] = '\0';                //add the nullbyte at the final index position
    if(newToken != 0){
        return newToken;                //return the token if sucessfully obtained
    }
  return 0;
}

int malFormed(char c){  //this function(state) deals with any malformed characters that are encountered. (includes escape values and invalid chars)
    errorChar = c;
    return 0;
}
int state0(char c){ //typically the first step of the fsm
    if(c == '0'){                    //checks if the char is a zero
        id = oct_hex_float_dec;
    }else if ((c > '0' && c <= '9')||(c=='-')) {    //checks if the char is between 0 and 9 or if the token is signed
        id = decimal;
    }
    else if (c == '.') {              //checks to see if the char/token is/has a decimal point
        id = floating_point;
    }
    else {
        id = mal;
        malFormed(c);
    }
    return 0;
}

int state1(char c){ //makes initial classifications (oct_hex_float_dec)
    if(c >= '0' && c <= '7'){             //checks if the char is between 0 and 7 and is placed in the octal loop
        id = octal;
    }else if ((c == 'X' || c == 'x')) {  //check if the character is an uppercase or lowercase x (hex loop)
        id = hexadecimal;
    }
    else if (c == '.') {                //check for a decimal point (float loop)
        id = floating_point;
    }
    else {
        id = mal;
        malFormed(c);
    }
    return 0;
}

int state2(char c){//determines whether the token is an octal or remains as an octal
    if(c >= '0' && c <= '7'){             //check if the character is between 0 and 7
        id = octal; //placed in the octal loop
    }
    else{id = mal;
        malFormed(c);}
    return 0;
}
    
int state3(char c){ //determines whether the token is a hexadecimal or remains as a hexadecimal
    if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')){  //check if the charais between 0-9 or a-f (A-F)
        id = hexadecimal;   //placed in the "hex loop"
    }
    else {
        id = mal;
        malFormed(c);
    }
    return 0;
}

int state4(char c){
    if (c >= '0' && c <= '9') {          //checks to see if c is between 0 and 9
        id = floating_point;
    }else if (c == 'E' || c == 'e') {    //checks the char is an E or e
        id = unsigned_float;
    }
    else{id = mal;
        malFormed(c);}
        return 0;
}

int state5(char c){ //for unsigned tokens (floats or decimals(integers))
    if ((c >= '0' && c <= '9') || (c == '-' || c == '+')) {  //check if the character is between 0-9 or a sign
        id = float_int; //indeterminate state either a float or an int
    }else{
        id = mal;
        malFormed(c);
    }
    return 0;
}

int state6(char c){
    if ((c >= '0' && c <= '9')) {  //checks if the char is between 0-9 or a sign
        id = float_int; //indeterminate state either a float or an int
    }else{
        id = mal;
        malFormed(c);
    }
    return 0;
}

int state7(char c){ //makes the final distinction (decimal) integer or a float
    if (c == '.') {                //check for a decimal point
        id = floating_point;    //placed in the float loop
    }else if (c == 'E' || c == 'e') {                //check for E or e
        id = unsigned_float;
    }
    else if (c >= '0' && c <= '9'){                //check for a decimal point
        id = decimal;   //placed in the decimal loop
    }
    else{
        id = mal;
        malFormed(c);
    }
    return 0;
}
/*This is the "State Transition Algorithm",  it obtains the token and
 *goes through each individual character and depending on the current
 *state of the identification process the current character is passed to
 *the appropriate state functions.
 *
 *Please see the comments for each state to better understand how the
 *finite state machine is being implemented through eight state functions
 *
 *This function does not return anything significant it merely updates
 *the id in the state methods for each token from undefined to any other
 *case.
*/
int stateTransitionAlgorithm(char *token){
    while(*token != '\0'){
         switch (id){
            case(mal): {    //once a token is identified as mal at any characters no further identification is needed
                return 0;
                break;
            }case(undefined): { //this state occurs at the beginning of the fsm
                state0(*token);
                break;
            }case(oct_hex_float_dec): {//this is the undetermined state where the id is still largely unknowl
                state1(*token);
                break;
            }case(octal): { //this is the "octal loop" of the fsm, once a token is identified as octal it must remain valid for all of the remaining characters
                state2(*token);
                break;
            }case(hexadecimal): {  //this is the "hexadecimal loop" of the fsm ensures that all the remaining characters of the token can keep the classification constant
                state3(*token);
                break;
            }case(floating_point): {//this is the floaing point loop of the fsm ensures that the rest of the token behaves like a float
                state4(*token);
                break;
            }case(unsigned_float): {//unsigned float the token does not begin with a sign
                state5(*token);
                break;
            }case(float_int): { //at this point the token can either be a float or an int (decimal)
                state6(*token);
                break;
            }case(decimal): {   //integer(decimal) loop
                state7(*token);
                break;
            }
            default:
                break;
        }
        token++;
    }
    return 0;
}


/*Passes the token into the "State Transition Algorithm", obtains
 *the Identity of the token and returns a string that can be output
 *as the identity.
 */
char *identify(char *token) {
    stateTransitionAlgorithm(token); //pass the token into the state transition algorithm so that the identity can be read and later returned as a string
    switch (id){
        case(mal): {    //malcases results in an error
            return "Error";
            break;
        }case(oct_hex_float_dec): { //if the identity is still undetermined at this point "unrecognized" is returned
            return "Unrecognized";
            break;
        }case(octal): { //octal case
            return "octal";
            break;
        }case(hexadecimal): {   //hexadecimal case
            return "hexadecimal";
            break;
        }case(floating_point): case(unsigned_float): case(float_int):{  //all three floating point cases should return "float"
            return "float";
            break;
        }case(decimal): {//standard decimal usually integers
            return "decimal";
            break;
        }default:
        break;
    }

    return NULL;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
    if (argv[1][0] == '\0') {	//ensures that the string argument is not empty
        return 0;
    }
    /*int *alert = '\a';        ////PLEASE IGNORE \\\\\\\
    char *backsp = '\b';
    char *form = '\f';
    char *newL = '\n';
    char *car = '\r';
    char *horz = '\t';
    char *vert = '\v';
    char *backsl = '\\';
    char *singquote = '\'';
    char *doubquote = '\"';
    char *quest = '\?';*/
    
    TokenizerT *tk = TKCreate(argv[1]); //call the create function, obtains a tokenizer struct that is dynamically allocated
    char *token = TKGetNextToken(tk);   //obtain the first token
    char *identity; char *comp;
    if(token != 0){ //this isn't really necessary; used as a precaution
        while(token != 0){
            identity = identify(token); //identify token, calls the identify function which runs through the state transition algorithm and returns a string with the appropriate id
            if(token != 0){
                if (id == mal) {    //deals with escape characters
                    comp = token;
                    comp++;//advance the pointer
                    if(*comp == 'a')
                    {//alert
                        errorChar = '\a';
                    }else if(*comp == 'b')
                    {//backspace
                        errorChar = '\b';
                    }else if(*comp == 'f')
                    {//formfeed
                        errorChar = '\f';
                    }else if(*comp == 'n')
                    {//newline
                        errorChar = '\n';
                    }else if(*comp == 'r')
                    {//carriage return
                        errorChar = '\r';
                    }else if(*comp == 't')
                    {//horizontal tab
                        errorChar = '\t';
                    }else if(*comp == 'v')
                    {//vertical tab
                        errorChar = '\v';
                    }else if(*comp == '"')
                    {//double quote
                        errorChar = '\"';
                    }else if(*comp == '?')
                    {//question mark
                        errorChar = '\?';
                    }else {
                        errorChar = '0';    //failsafe to check that invalid but non-escape sequences are not processed
                    }
                    
                        if(errorChar != '0'){
                            printf("Error: %s [0x0%x]\n",token,errorChar);  //this output occurs when an escape sequence is found; the appropriate hex value is returned
                        }else{
                            printf("Error: invalid %s \n",token);   //for invalid tokens, not whitespace, legitimate, or an escape character
                        }
                        
                    
                }else{
                    printf("%s",identity);  //output the legitimate identity "decimal, octal, float etc"
                    printf(" %s \n",token); //output the correct appropriate token followed by a newline (for the next token)
                }
            }
            id = undefined; //return the id back to undefined for the next token that is processed
            token = TKGetNextToken(tk); //obtain the next token and run through the loop once more if one exists
        }
    }
    free(token);    //frees the pointer to the token (token exists outside the scope of this function as well)
   TKDestroy(tk);   //the call to destroy will ensure that this struct is dynamically freed
  return 0;
}
