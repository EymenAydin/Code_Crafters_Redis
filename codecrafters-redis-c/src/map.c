#include "../include/map.h"

struct node* create_node(struct hashMap*hmap,char* key,void* value){
    struct node* new_n=malloc(sizeof(struct node));
    int length=strlen(key)+1; new_n->key=malloc(sizeof(char)*length);
    new_n->value=malloc(hmap->value_size);
    memcpy(new_n->value,value,hmap->value_size); free(value);
    strcpy(new_n->key,key); new_n->next=NULL; return new_n;
}

struct hashMap* create_hmap(size_t value_size,void destroy_fn(void* elem)){
    struct hashMap* hmap=malloc(sizeof(struct hashMap)); hmap->destroy_value=destroy_fn;
    hmap->value_size=((value_size + sizeof(size_t) - 1) / sizeof(size_t)) *
			sizeof(size_t);
    hmap->numofElements=0;  hmap->capacity=64;
    hmap->arr=calloc(hmap->capacity,sizeof(struct node*));
    return hmap;
}

int hashfunc(struct hashMap* hmap,char* key){
    unsigned int fnv_prime=0x01000193; unsigned int fnv_offset=0x811c9dc5;
    unsigned int hash=fnv_offset;
    for(int i=0;key[i]!='\0';i++){
        hash=hash ^ (unsigned char)key[i];
        hash=hash * fnv_prime;
    }
    int index= hash & (unsigned int)(hmap->capacity-1);
    return index;
}

void insert(struct hashMap** map,char* key,void* value){
    struct node* n_elem=create_node(*map,key,value); (*map)->numofElements++;
    int index=hashfunc(*map,key);
    if(!(*map)->arr[index]){
        (*map)->arr[index]=n_elem;
    }else{
        n_elem->next=(*map)->arr[index];
        (*map)->arr[index]=n_elem;
    }
    if((1.0)*(*map)->numofElements/(*map)->capacity>0.5)  *map=rehash(*map);
}

struct hashMap* rehash(struct hashMap* map){
    struct hashMap* new_map=malloc(sizeof(struct hashMap));
    new_map->capacity=map->capacity*2; new_map->numofElements=0;
    new_map->arr=calloc(new_map->capacity,sizeof(struct node*));
    for(int i=0;i<map->capacity;i++){
        struct node* iter=map->arr[i];
        while(iter){
            insert(&new_map,iter->key,iter->value);
            iter=iter->next;
        }
    }
    destroy(map); return new_map;
}

void remove_elem(struct hashMap* map,char* key){
    int s_index=hashfunc(map,key);
    struct node* iter=map->arr[s_index];
    struct node* prev=NULL; struct node* to_removed=NULL;
    while(iter){
        if(strcmp(iter->key,key)==0){
            if(iter==map->arr[s_index]){
                map->arr[s_index]=iter->next;
            }else{
                prev->next=iter->next;
            }
            to_removed=iter;
            break;
        }
        prev=iter; iter=iter->next;
    }
    if(to_removed){
        free(to_removed->key);
        if(map->destroy_value!=NULL) map->destroy_value(to_removed->value);
        free(to_removed->value);
        free(to_removed);
        map->numofElements--;
    }
}
 void* search(struct hashMap* map,char* key){
     int s_index=hashfunc(map,key);
     struct node* iter=map->arr[s_index];
     while(iter){
         if(strcmp(iter->key,key)==0){
             return iter->value;
         }
         iter=iter->next;
     }
     return NULL;
 }

void destroy(struct hashMap* map){
    struct node* iter=NULL; struct node* temp=NULL;
    for(int i=0;i<map->capacity;i++){
        iter=map->arr[i];
        if(!iter) free(iter);
        else {
            while(iter){
                temp=iter; iter=iter->next; free(temp->key);
                if(map->destroy_value!=NULL) map->destroy_value(temp->value);
                free(temp->value);
                free(temp);
            }
        }
    }
    free(map->arr);
    free(map);
}
