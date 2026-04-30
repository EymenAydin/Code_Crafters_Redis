#include "database.h"
#include "timing.h"



list* create_list(){
    list* new_list=malloc(sizeof(list));
    new_list->elems=malloc(sizeof(elem*));
    new_list->size=0; new_list->capacity=1;
    return new_list;
}

bool add_elem(list* list,const char* key,const char* value){
    elem* new_elem=malloc(sizeof(elem)); if(!new_elem) return false;
    int key_length=strlen(key); int value_len=strlen(value);
    new_elem->key=malloc(key_length+1); if(!new_elem->key) return false; strcpy(new_elem->key,key);
    new_elem->value=malloc(value_len+1); if(!new_elem->value) return false;  strcpy(new_elem->value,value);
    list->elems[list->size]=new_elem; list->size++;
    if(list->size==list->capacity) list->capacity=list->capacity*2;
    list->elems=realloc(list->elems,sizeof(elem*)*list->capacity); printf("add %ld",get_millis());
    return true;
}

void delete_elem(list* list,char* key){
    elem* to_be_deleted=NULL; int index=0;
    for(int i=0;i<list->size;i++){
        if(strcmp(list->elems[i]->key, key)==0){
            to_be_deleted=list->elems[i]; index=i;
        }
    }
    for(int i=index;i<list->size-1;i++){
        list->elems[i]=list->elems[i+1];
    }
    list->elems[list->size-1]=NULL; list->size--;
    free(to_be_deleted->key); free(to_be_deleted->value); free(to_be_deleted);
}
bool set_elem(list* list,ExpiryList* expiry_l,const char* key,const char* value,bool ex_flag,int wait_time){
    bool found=false; int index=0;
    for(int i=0;i<list->size;i++){
        if(strcmp(list->elems[i]->key, key)==0){
            int val_length=strlen(value); index=i;
            list->elems[i]->value=realloc(list->elems[i]->value,sizeof(char)*(val_length+1)); if(!list->elems[i]->value) return false;
            strcpy(list->elems[i]->value,value); found=true;
        }
    }
    if(!found){
        if(!add_elem(list, key, value)) {
            index=list->size-1;
            return false;
        }
    }
    if(ex_flag) add_expiry(expiry_l, list->elems[index]->key , wait_time);
    return true;
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


ExpiryList* create_expiry(){
    ExpiryList* new_expiry=malloc(sizeof(ExpiryList));
    new_expiry->capacity=1; new_expiry->size=0;
    new_expiry->elems=malloc(sizeof(expiry)*new_expiry->capacity);
    return new_expiry;
}

void add_expiry(ExpiryList* expiry_l,char* key,int waiting){
    expiry_l->size++; expiry_l->elems[expiry_l->size-1].expiry_time=get_millis()+waiting;
    int key_length=strlen(key); char* ex_key=malloc(sizeof(char)*(key_length+1));
    strcpy(ex_key,key);  expiry_l->elems[expiry_l->size-1].key=ex_key;
    if(expiry_l->size==expiry_l->capacity){
        expiry_l->capacity=expiry_l->capacity*2;
        expiry_l->elems=realloc(expiry_l->elems,sizeof(expiry)*expiry_l->capacity);
    }
}

void delete_expiry(ExpiryList* expiry_l,int index){
    free(expiry_l->elems[index].key);
    for(int i=index;i<expiry_l->size-1;i++){
        expiry_l->elems[i]=expiry_l->elems[i+1];
    }

    expiry_l->size--;
}

void* check_expiry(void* arg){
    usleep(1000);
    ExpiryList* expiry_l = ((expiry_thread*)arg)->expiry_l;
    list* data_list = ((expiry_thread*)arg)->data_list;
    free(arg);
    for(int i=0;i<expiry_l->size;i++){
        if(expiry_l->elems[i].expiry_time<get_millis()){
            printf("%ld",get_millis());
            char* key=expiry_l->elems[i].key;
            delete_elem(data_list,key);
            delete_expiry(expiry_l, i); i-=1;
        }
    }
    return NULL;
}

void destory_expiry(ExpiryList* expiry_l){
    for(int i=0;i<expiry_l->size;i++){
        free(expiry_l->elems[i].key);
    }
    free(expiry_l);
}
