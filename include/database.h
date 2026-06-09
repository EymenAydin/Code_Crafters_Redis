#ifndef DATABASE_H
#define DATABASE_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "map.h"


// Global Redis Variables

typedef struct Element{
   char*  key;
   char* value;
}elem;

typedef struct Linked_List{
    elem** elems;
    int size;
    int capacity;
}list;

//Redis List


typedef struct RedisElem{
    char* key;
    struct RedisElem* next;
    struct RedisElem* prev;
}r_elem;

typedef struct RedisList{
    int size;
    r_elem* head;
    r_elem* back;
}r_list;

//Expiry Struct

typedef struct Expiry{
    char* key;
    int64_t expiry_time;
}expiry;

typedef struct ExpiryList{
    expiry* elems;
    int size;
    int capacity;
}ExpiryList;

typedef struct expiry_thread{
    ExpiryList* expiry_l;
    list* data_list;
}expiry_thread;


//List Methods

list* create_list();
bool add_elem(list* list,const char* key,const char* value);
bool set_elem(list* list,ExpiryList* expiry_l,const char* key,const char* value,bool ex_flag,int wait_time);
void delete_elem(list* list,char* key);
char* get_elem(list* list,const char* key);
void destroy_list(list* list);

//Expiry List Methods

ExpiryList* create_expiry();
void add_expiry(ExpiryList* expiry_l,char* key,int waiting);
void delete_expiry(ExpiryList* expiry_l,int index);
void* check_expiry(void* arg);
void destroy_expiry(ExpiryList* expiry_l);

//Redis linked list methods

r_elem* create_elem(char* key);
r_list* create_rlist();
void r_push(r_list* list,char* elem);
void l_push(r_list* list,char* elem);
char* r_pop(r_list* list);
char* l_pop(r_list* list);
void destroy_rlist(r_list* list);

void destroy_rlist_wrapper(void* p);

#endif
