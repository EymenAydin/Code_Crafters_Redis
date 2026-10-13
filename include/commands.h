#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "database.h"
#include "map.h"
#include "redis_protocols.h"
#include "list_commands.h"

#define COMMANDS_SIZE 10
static const char* COMMANDS[]={"echo","ping","set","get","rpush","lrange","lpush","llen","lpop","rpop"};
static const char* SET_OPTIONS[]={"px"};

typedef struct Lists{
    list* l;
    struct hashMap* rlists;
    ExpiryList* el;
}lists;

char** command_parser(char*,int*);
int command_rec(char*);
char* response(char*,lists*);
char* get(const char*);
char* set(const char*);
#endif
