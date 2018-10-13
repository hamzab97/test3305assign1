/*
name: Hamza Bana
email: hbana3@uwo.ca

commandParser.c contains all the methods for parsing the user input
*/

#include "commandParser.h"

void parsePipeFilter(char *arr, int *count, char **commands){
  //iterate through array,
  //every time there is, pipe filter, replace it with '\0'
  int i = 0;
  while (arr[i]){
    if (arr[i] == '|' || arr[i] == '>'  || arr[i] == '<' ){
      (*count) ++;
    }
    // printf("hello: %c\n", arr[i]);
    i++;
  }
    //each index of arr will have 1 command
  i = 0;
  char *token;
  while ((token = strtok_r(arr, "|<> ", &arr))){
    // printf("token: %s\n", token);
    commands[i] = token; //assign entire command to one index of arr
    // printf("commands at %d is %s\n", i, commands[i]);
    i++;
  }

  // free(arr);
  // arr [(*count)+1];//allocate spacce to hold the command words for each argument
  // while ((token = strtok_r(&commands, " ", commands))){
  //   printf("token: %s\n", token);
  //   arr[i] = token; //assign entire command to one index of arr
  //   // printf("commands at %d is %s\n", i, commands[i]);
  //   i++;
  // }
  // // printf("command 1 is %s\n", commands[1]);
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
