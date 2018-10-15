#ifndef ARRAY_H
#define ARRAY_H

#define INITIAL_SIZE 20
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{

    int size;
    int capacity;
    char **data;


}Array;


void create_new_array(Array * array);

void append(Array * array, char * s);

char * get_at(Array * array, int index);

void expand_capacity(Array * array);

void free_array(Array * array);

int search_for(Array * array, char * s);
int getSize(Array *array);

#endif
