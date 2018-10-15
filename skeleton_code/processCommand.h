#ifndef PROCESS_COMMAND_H
#define PROCESS_COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> // for open

void process(char *command, char *argument);
void output(char *fileoutput, char *command, char *argument);
void input( char *fileinput, char *command, char * argument);

#endif
