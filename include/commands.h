#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "database.h"
#include "map.h"
#include "redis_protocols.h"
#include "list_commands.h"


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
