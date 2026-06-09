#include "../include/database.h"
#include "../include/timing.h"


//Linked List Methods

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
            return false;
        }
        index=list->size-1;
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



//Expiry List Methods

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
    ExpiryList* expiry_l = ((expiry_thread*)arg)->expiry_l;
    list* data_list = ((expiry_thread*)arg)->data_list;
    free(arg);
    while (1) {
        usleep(10000);
        for(int i=0;i<expiry_l->size;i++){
            if(expiry_l->elems[i].expiry_time<get_millis()){
                char* key=expiry_l->elems[i].key;
                delete_elem(data_list,key);
                delete_expiry(expiry_l, i); i-=1;
            }
        }
    }
    return NULL;
}

void destroy_expiry(ExpiryList* expiry_l){
    for(int i=0;i<expiry_l->size;i++){
        free(expiry_l->elems[i].key);
    }
    free(expiry_l);
}


//Redis List Methods

r_elem* create_elem(char* key){
    r_elem* new_el=malloc(sizeof(r_elem));
    int size=strlen(key)+1;
    new_el->key=malloc(sizeof(char)*size);
    strcpy(new_el->key,key);
    return new_el;
}

r_list* create_rlist(){
    r_list* new_rl=malloc(sizeof(r_list));
    new_rl->size=0; new_rl->back=NULL; new_rl->head=NULL;
    return new_rl;
}

void r_push(r_list* list,char* elem){
    r_elem* new_el=create_elem(elem);
    new_el->next=NULL;
    if(list->head==NULL){
       list->head=new_el;
       new_el->prev=NULL;
    }
    else{
         list->back->next=new_el;
         new_el->prev=list->back;
    }
    list->back=new_el; list->size++;
}

void l_push(r_list* list,char* elem){
    r_elem* new_el=create_elem(elem);
    new_el->prev=NULL;
    if(list->back==NULL){
        list->back=new_el;
        new_el->next=NULL;
    }else{
        list->head->prev=new_el;
        new_el->next=list->head;
    }
    list->head=new_el; list->size++;
}

char* r_pop(r_list* list){
    r_elem* to_be_popped=list->back;
    list->back=list->back->prev;
    int el_size=strlen(to_be_popped->key)+1;
    char* ret_str=malloc(sizeof(char)*el_size); strcpy(ret_str,to_be_popped->key);
    free(to_be_popped->key); free(to_be_popped);
    return ret_str;
}

char* l_pop(r_list* list){
    r_elem* to_be_popped=list->head;
    list->head=list->head->next;
    int el_size=strlen(to_be_popped->key)+1;
    char* ret_str=malloc(sizeof(char*)*el_size); strcpy(ret_str,to_be_popped->key);
    free(to_be_popped->key); free(to_be_popped); return ret_str;
}

void destroy_rlist(r_list* list){
    r_elem* iter=list->head; r_elem* temp=NULL;
    while(iter!=NULL){
        temp=iter; iter=iter->next;
        free(temp->key); free(temp);
    }
     free(list);
}

void destroy_rlist_wrapper(void* p){
    destroy_rlist(*((r_list**)(p)));
}
