/*
name: Hamza Bana
email: hbana3@uwo.ca

commandParser.c contains all the methods for parsing the user input
*/

#include "commandParser.h"

void parsePipeFilter(char *arr, int *count, int *input, int *output){
  //iterate through array,
  //every time there is, pipe filter, replace it with '\0'
  int i = 0;
  while (arr[i]){
    if (arr[i] == '|'){
      (*count) ++;
    }
    else if (arr[i] == '<'){ //check if input redirection exists
      (*input) = 1;
    }
    else if (arr[i] == '>'){ //check if output redirection exists
      (*output) = 1;
    }
    // printf("hello: %c\n", arr[i]);
    i++;
  }
}

void copyArray(char *arr1, char *arr2){
  int i = 0;
	 //copy element of line[i] into new array
	while (arr1[i]){
		arr2[i] = arr1[i];
		i++;
	}
  // printf("arr 2: %c\n", arr2[1]);
}

// void current(char *commands, int *i, char *word){
//   *word = commands[*(i)];
// }
