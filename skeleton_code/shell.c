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
#include<sys/wait.h>
#include "processCommand.h"

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
*******************************************************************spacedArray***********/
short execute_commands(char* line)
{
	// signal(SIGINT, handle_SIGINT); //start signal to watch for control + c entered by user

	short status;
	//enter your code here
//create array of pointers, each pointer points to a subarray of the parsed array
//each subarray will contain one commands
//dynamically create array of pointers and fork as many processes as there are indexes
//iterate and read from pipe, perform function, output to pipe

	if ((strcmp(line, "exit")) == 0)
		exit(0);

	int i = 0;
	while (line[i])
		i++;
	//
	int size = i;
	// // printf("%d\n", size);
	//
	char *copyOfLine = calloc(size, sizeof(char));
	//create copy of line
	copyArray(line, copyOfLine);
	// i=0;
	//
	int count = 0; //keep track of how many pipes or redirections there are
	int input = -1; //boolean variable to check whether there is input redirections
	int output = -1; //bool var to check if there is output redirection
	parsePipeFilter(copyOfLine, &count, &input, &output); //get count of how mabny pipes there are
	// // printf("count: %d\n", count);

	Array array; //create new object array
	create_new_array(&array); //constructor to create and initialize array
	Array spacedArray;
	create_new_array(&spacedArray);

	//initialize indicies
  char * pnch =  strtok(line, " ");

  while(pnch){ //separate everything by spaces and pipes
		//each index in array contains tokens separated bvy spaces
      append(&spacedArray, pnch);
      pnch = strtok(NULL, " ");
  }

	//parse for pipe
	pnch =  strtok(copyOfLine, "|");

  while(pnch){ //separate everything by spaces
		//each index in array contains tokens separated bvy spaces
      append(&array, pnch);
      pnch = strtok(NULL, "|");
  }

	if ((input != -1) || (output != -1)){ //if there are redirections, then feed the new parsed Array
		//and find the indicies for redirection command
		int output = search_for(&array, ">"); //get index of output redirection
		int input = search_for(&array, "<"); //get index of output redirection
	}


	//multi process
	pid_t pid; //keep track of process id of forks
	int fd[2]; //hold fds of both ends of the pipe

	//create pipe
	if (pipe(fd) < 0)
		perror("pipe error");

	// printf("index %d in array is %s\n", 2, get_at(&array, 2));

	int j = 0; //counter to keep track of indices in array
	// printf("array size is  is %d\n", getSize(&array));

	i = 0;

	if (count > 0){ //if there are pipes in the command
		for (i = 0; i < getSize(&array); i++)
		{
			printf("\n" );
			printf("i: %d sizeof array : %d j: %d\n", i, getSize(&array), j);
			printf("\n" );
			if (i==0){ //if we're at the command command
			//	printf("hey first command \n");
				close(fd[0]); //close pipe
				if (dup2(fd[1], STDOUT_FILENO) < 0){//redirect output to pipe
						perror("canot dupe frist command");
						exit(1);
				}
				//printf("hey first command \n");
				//since it sthe first command, no need to read from the pipe
				int size = 0;
				while ((j < getSize(&spacedArray)) && (strcmp(get_at(&spacedArray, j), "|") != 0)){
					j++;
					size++;
				}
				// j++;
				printf("j: %d. size of command: %d\n", j, size);
				printf("\n" );
				//make exec call, output will be written to fd[1]
				//calculate siize of command and call pass appropriate params into the process call
				if (size > 1)
					process(get_at(&spacedArray, j-2), get_at(&spacedArray, j-1));
			 	else
				 	process(get_at(&array, i), get_at(&array, i));//call process method to process the command if no pipes exist
			}

			else if (i == getSize(&array)-1){// last command
				printf("last command \n");
				printf("\n" );
				//close writing fd
				//will write to standard output
				close(fd[1]); //close writing end
				if (dup2(fd[0], STDIN_FILENO) < 0){
					perror("canot dupe last command");
					exit(1);
				}

				//write to standardout output

				//calculate siize of command and call pass appropriate params into the process call
				int size = 0;
				while ((j < getSize(&spacedArray)) && (strcmp(get_at(&spacedArray, j), "|") != 0)){
					j++;
					size++;
				}
				// j++;
				// printf("j: %d. size : %d\n", j, size);
				//make exec call, output will be written to fd[1]
				if (size > 1)
					process(get_at(&spacedArray, j-2), get_at(&spacedArray, j-1));
			 	else
				 	process(get_at(&array, i), get_at(&array, i));//call process method to process the command if no pipes exist
			}

			else{
				// printf("middle commands\n");
				//not the first or last commands
				//read from fd[0]
				//output to fd[1]


				close(fd[1]); //close writing end
				if (dup2(fd[0], STDIN_FILENO) < 0){
					perror("canot dupe");
					exit(1);
				}

				//calculate size of the command make apporpriate call
				int size = 0;
				while (strcmp(get_at(&spacedArray, j), "|") != 0){
					j++;
					size++;
				}
				j++;
				//make exec call, output will be written to fd[1] and read from fd[0]
				if (size > 1)
					process(get_at(&spacedArray, j-2), get_at(&spacedArray, j-1));
			 	else
				 	process(get_at(&array, i), get_at(&array, i));//call process method to process the command if no pipes exist

			}
		}
	}

	else{ //if no pipes are in the command
		printf("one command %s\n", get_at(&array, 0));
		if ((input != -1) || (output != -1)){
			printf("redirections present\n");
			//if there are redirections in the command
			if ((input != -1) && (output == -1)){ //if there is only input
				printf("only input\n");
				int index = search_for(&spacedArray, "<"); //find index for < in the spaced array
				inputhandling (get_at(&spacedArray, index+1), get_at(&spacedArray, index-1),  get_at(&spacedArray, index-1));
			}

			if ((input == -1) && (output != -1)){ //if there is only output
				printf("only output\n");
				int index = search_for(&spacedArray, ">"); //find index for > in the spaced array
				if (getSize(&spacedArray) >= 4){
					outputhandling (get_at(&spacedArray, index+1), get_at(&spacedArray, index-2), get_at(&spacedArray, index-1));
				}else //only one command before and after the redirect
					outputhandling (get_at(&spacedArray, index+1), get_at(&spacedArray, index-1), get_at(&spacedArray, index-1));
			}
			exit(0);//exit
		}
		else{
			if (getSize(&spacedArray) > 1)
				process(get_at(&spacedArray, 0), get_at(&spacedArray, 1));
			else
				process(get_at(&spacedArray, 0), get_at(&spacedArray, 0));//call process method to process the command if no pipes exist

		}
		//ls -a
		//command would be ls, argument would be -a
		// printf("%d\n", getSize(&spacedArray));
	}
	printf("\n");
	return status;
}
