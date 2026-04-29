#ifndef DATABASE_H
#define DATABASE_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Element{
   char*  key;
   char* value;
}elem;

typedef struct Linked_List{
    elem** elems;
    int size;
    int capacity;
}list;

list* create_list();
bool add_elem(list* list,const char* key,const char* value);
bool set_elem(list* list,const char* key,const char* value);
char* get_elem(list* list,const char* key);
void destroy_list(list* list);

#endif
