/******************************************************************************
*
* Name: 	Zaid Albirawi
* Email: 	zalbiraw@uwo.ca
*
* shell.c processes the users input, passes it to a tokenizer, and then passes
* the tokens obtained to the process_commands function which will process and
* execute the commands.
*
******************************************************************************/

#include "shell.h"
#include "commandParser.h"
#include <unistd.h>
#include "array.h"
#include <signal.h>
#include <fcntl.h>

/******************************************************************************
* Processes the input and determine whether it is a user interface operation
* or a set of commands that will need to be executed.
******************************************************************************/
void shell(char* filename)
{

	/**************************************************************************
	* short			special_char 	determines whether the character to be
	*								processed is special or not.
	* int 			len 			keeps track of the current line length
	* char 			line 			holds current line
	**************************************************************************/
	short special_char = FALSE;
	int status, len = 0;
	char ch, *line = (char*)malloc(MAX_LEN);
	FILE *fp = NULL;

	if (filename != NULL)
	{
		fp = fopen(filename, READ);

		if (fp == NULL) printf("Unable to open %s\n", filename);

	}

	/**************************************************************************
	* Loops until the user exits the program.
	**************************************************************************/
	print_user();
	while(TRUE)
	{

		ch = getch(fp);

		if (special_char)
		{
			special_char = FALSE;
			ch = switch_keypad(ch);
		}

		/**********************************************************************
		* switchs arround possible cases depending on the read character
		**********************************************************************/
		switch(ch)
		{
			/******************************************************************
			* handles the ascii translation of arrow characters
			******************************************************************/
			case '\033':
				getch(NULL);
				special_char = TRUE;
				continue;
				break;

			/******************************************************************
			* ignore arrow characters and tab
			******************************************************************/
			case KEYLEFT:
			case KEYRIGHT:
			case KEYUP:
			case KEYDOWN:
			case '\t':
				break;

			/******************************************************************
			* handles backspacing
			******************************************************************/
			case DELETE:
			{
				if (len > 0)
					delete(--len, line);
				break;
			}

			/******************************************************************
			* if the maximum line length is not exceeded the program will print
			* the character. if the character is not a new line then continue.
			* Else, terminate line, pass it to the execute_commands methond,
			* allocate a new line, and reset the line length.
			******************************************************************/
			default:
			{
				if (len < MAX_LEN)
				{

					if (ch != '\n')
					{
						printf("%c", ch);
						line[len++] = ch;
					}

					else if (ch == '\n' && len > 0)
					{
						printf("%c", ch);
						line[len] = '\0';
						status = execute_commands(line);

						free(line);
						if (status == TERMINATE) exit(SUCCESS);

						line = (char*)malloc(MAX_LEN);
						len = 0;

						print_user();
					}
				}
				break;
			}
		}
	}

	fclose(fp);
}

/******************************************************************************
* execute_commands will process and execute the commands in the variable line.
******************************************************************************/
short execute_commands(char* line)
{
	// signal(SIGINT, handle_SIGINT); //start signal to watch for control + c entered by user
	while(1){ //keep loop running

		short status;
		//enter your code here
	//create array of pointers, each pointer points to a subarray of the parsed array
	//each subarray will contain one commands
	//dynamically create array of pointers and fork as many processes as there are indexes
	//iterate and read from pipe, perform function, output to pipe

		int i = 0;
		while (line[i])
			i++;

		int size = i;
		// printf("%d\n", size);

		char *copyOfLine = calloc(size, sizeof(char));

		copyArray(line, copyOfLine);
		i=0;

		int count = 0; //keep track of how many pipes or redirections there are
		int input = 0; //boolean variable to check whether there is input redirections
		int output = 0; //bool var to check if there is output redirection
		parsePipeFilter(copyOfLine, &count, &input, &output); //get count of how mabny pipes there are
		// printf("count: %d\n", count);

		Array array; //create new object array
		create_new_array(&array); //constructor to create and initialize array

		//initialize indicies
	  char * pnch =  strtok(line, " ");

	  while(pnch){ //separate everything by spaces
			//each index in array contains tokens separated bvy spaces
	      append(&array, pnch);
	      pnch = strtok(NULL, " ");
	  }

		//multi process
		pid_t pid; //keep track of process id of forks
		int finalfd[2]; //hold fds of both ends of the pipe
		int tempfd[2]; //hold temporary fd to keep track of IO redirection

		//create pipe
		if (pipe(fd) < 0)
			perror("pipe error");

		// printf("index %d in array is %s\n", 2, get_at(&array, 2));

		int j = 0; //counter to keep track of indices in array
		printf("array size is  is %d\n", getSize(&array));
		int commands;
		if (count == 0){
			commands = 1;
		}
		else{
			commands = count +2;
		}
		i = 0;
		while (i < getSize(&array)){
			if ((strcmp(get_at(&array, i),"|") == 0) || (strcmp(get_at(&array, i), ">") == 0)|| (strcmp(get_at(&array, i), "<") == 0)){
				if (strcmp(get_at(&array, i), ">") == 0){
					//output command
					//open outfile
					int file;
					//use file descriptors to redirect standard output to file rather than shared memory
					file = open(get_at(&array, i+1), O_WRONLY|O_CREAT|O_TRUNC);
					if (file < 0){
						perror("cant open output file"); //error check
					}
					dup2(file, STDOUT_FILENO);//create dup2 to output to file
					close(file); //close file

					pid = fork(); //fork new process to execute command
					if (pid < 0){
						perror("Problem forking");
						exit(1);
					}
					else if (pid == 0){
						//fork successful, in child process
						execlp(get_at(&array, i-1), get_at(&array, i-1), get_at(&array, i+1),NULL) //parameters are command, command, file, NULL
					}
					i = i + 2;
				}
				else if (strcmp(get_at(&array, i), "<") == 0){
					//output command
					//open outfile
					int file;
					//use file descriptors to redirect standard output to file rather than shared memory
					file = open(get_at(&array, i+1), O_WRONLY|O_CREAT|O_TRUNC);
					if (file < 0){
						perror("cant open input file"); //error check
						exit(1);
					}
					dup2(file, STDIN_FILENO);//create dup2 to input from file
					close(file); //close file

					//if redirecting input is the case, then read the file after the '<' @ position i+1 and redirect it to i-1
					pid = fork(); //fork new process to execute command
					if (pid < 0){
						perror("Problem forking");
						exit(1);
					}
					else if (pid == 0){
						//fork successful, in child process
						execlp(get_at(&array, i-1), get_at(&array, i-1), get_at(&array, i+1),NULL) //parameters are command, command, file, NULL
					}
					i = i + 2;
				}
				else{
					//char at i is "|"
					i++; //increment by one to go to the next argument
				}
			}
			else{
				// printf("at index %d is %s\n", i, get_at(&array, i));
				// printf("commands: %d i: %d, element at index i is %s\n", commands, i, get_at(&array, i));
				pid = fork();
				if (pid < 0){
					perror("Problem forking");
					exit(1);
				}
				else if (pid == 0){
					//child process
					printf("\n" );
					printf("i am child process\n");
					close(fd[0]); //close reading pipe
					if (dup2(fd[1], STDOUT_FILENO)<0){ //write to pipe
						perror("cant dupe");
						exit(1);
					}
					//how to create new subarray with the entire command?
					if (strcmp(get_at(&array, i+1), "|") == 0)
						execlp(get_at(&array, i), get_at(&array, i), NULL); //process current index only
					if ((strcmp(get_at(&array, i+1), "<") == 0) || (strcmp(get_at(&array, i+1), ">") == 0)) //if next is IORedirection, continue
						continue; //no exec call needed
					// execlp(get_at(&array, 2), get_at(&array, 2), NULL); //test command
					}
					// else{ //parent process
					// 	printf("\n" );
					// 	printf("i am parent process\n");
					//
					// 	close(fd[1]);
					// 	if (dup2(fd[0], STDIN_FILENO) < 0){
					// 		perror("cant dupe");
					// 		exit(1);
					// 	}
					// 	execlp("sort", "sort", NULL);
					// }
				i++;
			}
		}
		return status;
	}
}
