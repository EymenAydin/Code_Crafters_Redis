#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "commands.h"
#include "database.h"
#include "map.h"

typedef struct thread_args{
    lists* lists;
    int* client_fd;
}thread_args;


void* handle_client(void* arg);
#endif
