/* CS 214: Systems Programming Spring 2018
*  Professor Francisco
*  Assignment 0: String Sorting
*  Richard Farrington netid: raf172
*  Anirudh Tunoori netid: at813
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct StringsorterS_ { //Define the Stringsorter struct contains a list of words and the number of words in that list.
    char **list;
    int numWords;
}components;

//Function definitions:
int isLetter(char c);
void swap(int x, int y);
void quiksort(int low, int high);
void fill(int startindex, int endindex, char* input);
int schism(int lo, int hi);
void freethings(char** wlist);


components* order; // this is a global struct that will be modified in multiple functions. numWords is the number of words and list is the list of words
char** copiedlist; // copied list is a copy of the list field from the global struct order. copied list will be the one that is sorted
int listsize; //order->numWords

//This function takes in a character and determines if it is a letter by comparing its ascii value
//with that of other letters
int isLetter(char c){
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return 1;
    } else return 0;
}

/* The fill function copies an input string (a) word into a list of words (global struct)
 * that can later be sorted. The function also adds a null trminator to each word and increments 
 * the total number of words in the list.
 */
void fill(int startindex, int endindex, char* input){
	int wordlen = (endindex-startindex)+2; //wordlen is the size of the string that is going into the list of words
	char* nuword = (char*) malloc(sizeof(char)*wordlen);    //create a new word (dynamically)
	nuword[wordlen-1] = '\0';   //add the null terminaor to the end of the word/string
	strncpy(nuword,input+startindex,endindex-startindex+1); 
	/*a certain number of characters is being copied from the input string to the new string. one is added because the difference between
	endindex and startindex is put in such a way that the last character of the substring would never make it into the new string*/
	if(order->numWords == 0)
	{   //if the list is currently empty
		order->list[0] = nuword;
		order->numWords++;
	}
	else{   //if the list already contains other words
		order->numWords++;
		order->list = (char**) realloc(order->list,(sizeof(char*)*order->numWords));  //Dynamically reallocate memory for each new word in the list. This is necessary because we do not know the number of words or the length of the words.
		order->list[order->numWords-1] = nuword;
	}
	return;
}

/* The function below is a quicksort function that deals with strings. */
 void quiksort(int low, int high){
	if(low >= high)
	{
		// printf("low and high %d %d\n",low,high);
		return;
	}
    else{
		//char* pivot = list[low]; //the pivot is initially going to be the leftmost location
		int pivotloc;
		pivotloc = schism(low,high); //this will disclose the location of the pivot so that you already know that it is sorted
		//printf("pivotloc: %d\n",pivotloc);
		quiksort(low,pivotloc-1);   //recursively apply quicksort to everything that comes before the pivot
        quiksort(pivotloc+1,high); //recursively apply quicksort to everything that comes after the pivot
		return;
	}
}

/*below is the partition function that is essential for any quicksort algorithm. the term schism is used
because it is (sort of) a synonym for the word partition. this function returns the index for the pivot's final location
*/ 
int schism(int lo, int hi){
	int i,j; //iterators. j will iterate through copiedlist until it reaches hi. i will increment when certain conditions are met
	i=lo-1;
	j=lo;
	char* pivot = copiedlist[lo];
	// printf("lo, hi: %d %d \n", lo, hi);
	while(j<=hi)
	{
		if(strcmp(pivot,copiedlist[j]) >= 0)
		{
			//this means that copiedlist[j] should appear before pivot
			i++;
			swap(i,j);
		}
		j++;
		//printf("%d %d\n",i,j);
	}
	//printf("%d\n",i);
	swap(lo,i); //swap the two words in the list (indexed)
	return i;
}

/*the swap method takes two strings at index i and j from the global copiedlist char**
and it swaps them using a temp "string" */
void swap(int x, int y){
	char* temp = copiedlist[x];
	copiedlist[x] = copiedlist[y];
	copiedlist[y] = temp;
	return;
}

//This function will free the list of words, whose memory/space was dynamically allocated
void freethings(char** wlist){
	int i = 0;
	while(i<listsize)
	{   //free each word
		free(wlist[i]);
		wlist[i] = NULL;
		//printf("the word at index %d has been freed\n",i);
		i++;
	} //free the now empty list
	free(wlist);
	wlist = NULL;
	return;
}


int main(int argc, char **argv) {
    if (argc != 2) {    //print a usage message if the wrong number of inputs is provided
        printf("Usage: ./stringsorter \"Enter (a single) Input String Here\"\n");
        return 1;
    }
    if (argv[1][0] == '\0') {	//ensures that the string argument is not empty
        return 0;
    }
    char* a = argv[1];
    int length = strlen(a); //length of the input
    int nonletter = 1;  //a boolean represented by an integer
    //printf("arg length: %d\n", length);
    int i;
    for (i = 0; i <= length; i++) {
        if (isLetter(argv[1][i])) {
            nonletter = 0;  //check to see if the input argument has at least one letter.
        }
    }
    if (nonletter == 1) {
        return 1;   //do nothing if the input has no letters
    }
    
    //dynamically allocate space for the list of words
    order = (components*) malloc(sizeof(components));
    order->numWords = 0;    //Set the initial number of words to 0
    order->list = (char**) malloc(sizeof(char*)); //locate space for the word, this string is reused/recycled
    
    //Parse
    int start, end, counter; //counter changes as you iterate through the input string. 
	/*in the fill method, every time we have a word, start will represent the start index, end will represent the end index*/
    counter = 0; start = -1; end = 0;
    while (a[counter] != '\0')  //ensures that we havent reached the end of the input
    {   //this loop is used to parse the input string into individual words.
        if(isLetter(a[counter]))
        {
            if(start < end)
            {
                start = counter; // this is to ensure that start always changes.
            }
        }
        else {
                if(counter != 0)
                {
                    // the above conditional should prevent a runtime error
                    if(isLetter(a[counter-1]))
                    {
                        end = counter-1;
                        //printf("start and end %d %d\n",start, end);
                        fill(start,end,a); // call fill method
                        if(start == end)
                        {
                            start--;    //decrement the starting index
                        }
                    }
                }
            }
        counter++;
    }

    if(isLetter(a[counter-1]))
    {
        end = counter-1;
        fill(start,end,a); /*call fill method*/
        /*printf("start and end %d %d\n",start, end);
        printf("call fill outside of loop\n");*/
    }

    i = 0;
    copiedlist = (char**) malloc(sizeof(char*)*order->numWords);
    while(i<order->numWords)
    {   //Copy the list of words into a new list that can now be sorted
        copiedlist[i] = (char*)malloc(sizeof(char)*(strlen(order->list[i])+1));
        strcpy(copiedlist[i],order->list[i]);
        //printf("%s\n",copiedlist[i]);
        i++;
    }
    
    quiksort(0,(order->numWords)-1); //Commence the quicksort algorithm on the list of
    i = 0;                           //words
    while(i<order->numWords)
    {   //Print all the words that are now in sorted order:
        printf("%s\n",copiedlist[i]);
        i++;
    } listsize = order->numWords;   //used for testing purposes
    //printf("listsize = %d\n",listsize);
    
    //Free everything that was allocated
    freethings(order->list);
    freethings(copiedlist);
    free(order);
    order = NULL; 
    return 0;
}
