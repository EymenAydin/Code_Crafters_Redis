#ifndef LIST_COMM
#define LIST_COMM

#include "map.h"
#include "database.h"
#include "redis_protocols.h"

char* r_push_comm(struct hashMap* r_lists,char** commands,int size);
char* l_range(struct hashMap* rlists,char** commands,int size);
char* l_push_comm(struct hashMap* rlists,char** commands,int size);
char* l_len(struct hashMap* rlists,char** commands,int size);
char* l_pop_comm(struct hashMap *rlists,char** commands,int size);

#endif
