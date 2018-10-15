/*
name: Hamza Bana
email: hbana3@uwo.ca

commandParser.c contains all the methods for processing the user input
*/

#include "processCommand.h"
#include "array.h"

void process(char *command, char *argument){
  printf("command is %s and argument is %s\n", command, argument );
  printf("process method called\n");
  pid_t pid; //process id
  pid = fork(); //fork new process
  if (pid < 0){
					perror("Problem forking");
					exit(1);
	}
  if (pid ==0 ){
    printf("child process\n");
    //child process
    if (execlp(command, argument, NULL) < 0) //process command
      printf("command error\n");
  }
}

// array, index is the index at which the output redirection is located
//find index of > in the array
//pass in the file  right after that index in the array
//pass in the entire argument of the command to use in the command process method
void output(char *fileoutput, char *command, char *argument){
  int file; //define file descrpption
  file = open(fileoutput, O_WRONLY|O_CREAT|O_TRUNC); //open file to output
  if (file < 0) //if error
    perror("cant open output file");
  int tempFDWrite = dup(STDOUT_FILENO); //get whats already in the fd write end, save it
  if (dup2(file, STDOUT_FILENO) < 0){
    //redirect output to file from command execuation
    perror("cant dupe");
    exit(1);
  }
  process(command, argument);

}
