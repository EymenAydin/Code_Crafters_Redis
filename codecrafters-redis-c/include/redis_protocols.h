#ifndef REDIS_P
#define REDIS_P

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "methods.h"

char* bulk_arr2(char**,int);
char* bulk_arr(char*);
char* bulk_str(char*);
char* smp_str(const char*);
char* int_str(int msg);
char* null_str();

#endif
