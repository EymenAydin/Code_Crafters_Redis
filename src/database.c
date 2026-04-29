#include "database.h"
#include <stdlib.h>
#include <string.h>

list* create_list(){
    list* new_list=malloc(sizeof(list));
    new_list->elems=malloc(sizeof(elem*));
    new_list->size=0; new_list->capacity=1;
    return new_list;
}

bool add_elem(list* list,const char* key,const char* value){
    elem* new_elem=malloc(sizeof(elem)); if(!new_elem) return false;
    int key_length=strlen(key); int value_len=strlen(value);
    new_elem->key=malloc(sizeof(key_length)+1); if(!new_elem->key) return false; strcpy(new_elem->key,key);
    new_elem->value=malloc(sizeof(value_len)+1); if(!new_elem->value) return false;  strcpy(new_elem->value,value);
    list->elems[list->size]=new_elem; list->size++;
    if(list->size==list->capacity) list->capacity=list->capacity*2;
    list->elems=realloc(list->elems,sizeof(elem*)*list->capacity);
    return true;
}

bool set_elem(list* list,const char* key,const char* value){
    bool found=false;
    for(int i=0;i<list->size;i++){
        if(strcmp(list->elems[i]->key, key)==0){
            int val_length=strlen(value);
            list->elems[i]->value=realloc(list->elems[i]->value,sizeof(char)*(val_length+1)); if(!list->elems[i]->value) return false;
            strcpy(list->elems[i]->value,value); found=true;
        }
    }
    if(!found){
        if(!add_elem(list, key, value)) return false;
        else return true;
    }else return true;
}

char* get_elem(list* list,const char* key){
    for(int i=0;i<list->size;i++){
        if(strcmp(list->elems[i]->key, key)==0){
            return list->elems[i]->value;
        }
    }
    return NULL;
}

void destroy_list(list* list){
    for(int i=0;i<list->size;i++){
        free(list->elems[i]->key); free(list->elems[i]->value);
        free(list->elems[i]);
    }
    free(list->elems); free(list);
}
