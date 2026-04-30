#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "database.h"
char** command_parser(char*,int*);
int command_rec(char*);
char* response(char*,list*,ExpiryList*);
char* bulk_str(char*);
char* get(const char*);
char* set(const char*);
char* smp_str(const char*);
#endif
