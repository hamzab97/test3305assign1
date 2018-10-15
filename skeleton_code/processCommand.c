/*
name: Hamza Bana
email: hbana3@uwo.ca

commandParser.c contains all the methods for processing the user input
*/

#include "processCommand.h"
#include "array.h"
#include <sys/wait.h>

void process(char *command, char *argument){
  printf("command is %s and argument is %s\n", command, argument );
  printf("\n" );
  // printf("process method called\n");
  pid_t pid; //process id
  pid = fork(); //fork new process
  if (pid < 0){
					perror("Problem forking");
					exit(1);
	}
  if (pid ==0 ){
    // printf("child process\n");
    //child process
      if (strcmp(command, argument) == 0)//if there are no additional arguments
        {
          printf("only one command passed %s\n", command);
          if (execlp(command, command, NULL) < 0); //process command
          {
            printf("command error\n");
          }
        }
      else
      {
        printf("paramteres are more\n");
        if (execlp(command, command, argument, NULL) < 0); //process command
      //paramteres are  command, command, arguments
          {printf("command error\n");}}
  }
  else
    wait(NULL);
}

// array, index is the index at which the output redirection is located
//find index of > in the array
//pass in the file  right after that index in the array
//pass in the entire argument of the command to use in the command process method
void outputhandling(char *fileoutput, char *command, char *argument){
  int file; //define file descrpption
  file = open(fileoutput, O_WRONLY|O_CREAT|O_TRUNC); //open file to output
  if (file < 0) //if error
    perror("cant open output file");
  int tempFDWrite = dup(1); //get whats already in the fd write end, save it
  if (dup2(file, STDOUT_FILENO) < 0){
    //redirect output to file from command execuation
    perror("cant dupe");
    exit(1);
  }
  process(command, argument);
  // dup2(tempFDWrite, STDOUT_FILENO); //restore previous data
}

void inputhandling( char *fileinput, char *command, char * argument){
  int file;
  file = open(fileinput, O_RDONLY);
  if (file < 0) //perror
    perror("cant open input file");
  int tempFDRead = dup(0);
  if (dup2(file, STDIN_FILENO) < 0){
    //redirect input using dup2
    perror("canot dupe");
    exit(1);
  }
  process(command, argument);
  dup2(tempFDRead, STDIN_FILENO); //restore previous data
}
