#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <string.h>
//Node definition

struct node{
    char* key;
    void* value;
    struct node* next;
};

//General Destroyer
typedef void (*destroy_fn)(void *);

//Hash Map definition

struct hashMap{
    destroy_fn destroy_value;
    int numofElements,capacity;
    struct node** arr;
    size_t value_size;
};

// Hash Map Methods
struct node* create_node(struct hashMap* map,char* key,void* value);
struct hashMap* create_hmap(size_t value_size,void destroy_fn(void* elem));
int hashfunc(struct hashMap* map,char* key);
void insert(struct hashMap** map,char* key,void* value);
struct hashMap* rehash(struct hashMap* map);
void remove_elem(struct hashMap* map,char* key);
void* search(struct hashMap* map,char* key);
void destroy(struct hashMap* map);
#endif
