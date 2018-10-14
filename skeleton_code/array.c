#include "array.h"


void create_new_array(Array * array){

    array->size = 0;
    array->capacity = INITIAL_SIZE;


    array->data = (char **)malloc(sizeof(char) * array->capacity);

}

void append(Array * array, char * s){

    if(array->size >= array->capacity){
            expand_capacity(array);
    }

    array->data[array->size] = s;
    array->size++;
}

char* get_at(Array * array, int index){

    if(index >= array->size || index < 0){
        printf("Index out of bounds");
    }
      else{

        return array->data[index];

      }

}

void expand_capacity(Array * array){

    array->capacity *=2;

    array->data = realloc(array->data, sizeof(char) * array->capacity);



}


void free_array(Array * array){

    free(array->data);

}

char * search_for(Array * array, char * s){


   for(int i = 0; i < array->size; i++){

        if(strstr(array->data[i], s) != NULL) return s;
   }

    return NULL;
}

int getSize(Array *array){
  return array->size;
}

char parse(char *command, char **parsedCommand){
  int i = 0;
  char *token = strtok_r(command, "|<> ");//parse for command separatred by | or < or >
  while (token){//loop to separate the entire command and store each parsed command in one index
    parsedCommand[i] = token;
    token = strtok_r(NULL, "|<> ");
    i++;
  }
}

/**
int main(){

    Array array;

    create_new_array(&array);

    char * s = "Hello";
    char * t = "lol";
    char * l = "meow";

    append(&array, s);
    append(&array, t);
    append(&array, l);

   printf("%d\n", array.size);
   char *m = get_at(&array, 0);

   printf("%s\n", array.data[0]);


   // Array array;

}
**/
