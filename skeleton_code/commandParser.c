/*
name: Hamza Bana
email: hbana3@uwo.ca

commandParser.c contains all the methods for parsing the user input
*/

#include "commandParser.h"

//parse spaces " " in user input to obtain the commands only
void parseSpaces(char *str, char **resultArr, int *i){
  // printf("\nspaces method called\n");
  //user char *strsep(char **stringp, const char *delim) from string.h library
  // printf("before loop result array at index %d is %s and str is %s\n", *i, resultArr[*i], str);
  char *token;
  token = strsep(&str, " ");
  while (token){
    resultArr[(*i)++] = token;
    // printf("i: %d, token is %s\n", *i, token);
    token = strsep(&str, " ");
  }
  resultArr[(*i)++] = "/0"; //mark with a 0 character to mark end of array
  // for (int j = 0; j < *i; j ++)
  //   printf("result arr %d: %s\n", j, resultArr[j]);
  // printf("spaces method end\n");
}


void parsePipeFilter(char *arr){
  //iterate through array,
  //every time there is, pipe filter, replace it with '\0'
  int i = 0;
  while (arr[i]){
    printf("hello: %s\n", &arr[i]);
    if (strcmp(&arr[i], "|") || strcmp(&arr[i], ">") || strcmp(&arr[i], "<"))
      arr = "\0";
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
  printf("arr 2: %s\n", &arr2[1]);
}

//parse and look for ">" io redirection in user input
void parseIORedirection(char *line){

}
