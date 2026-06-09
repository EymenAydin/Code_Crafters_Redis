#include "../include/commands.h"
#include "../include/database.h"
#include "../include/methods.h"

#define COMMANDS_SIZE 8
const char* COMMANDS[]={"echo","ping","set","get","rpush","lrange","lpush","llen"};
const char* SET_OPTIONS[]={"px"};

char** command_parser(char* command,int* commands_in_size){
    char* pos=strchr(command,'*');
    if(!pos) return NULL;
    char* end_ptr;
    int commands_size=strtol(pos+1,&end_ptr,10);
    if(commands_size<0) return NULL;
    char** commands=malloc(sizeof(char*)*commands_size);
    if(commands==NULL) return NULL;
    int command_size=0;
    for(int i=0;i<commands_size;i++){
        pos=strchr(pos,'$');
        if(!pos) goto fail; pos+=1;
        command_size=strtol(pos,&end_ptr,10);
        commands[i]=malloc(sizeof(char)*(command_size+1));
        if(!commands[i]) goto fail;
        pos=strstr(pos,"\r\n"); if(!pos) goto fail; pos=pos+2;
        strncpy(commands[i], pos, command_size);
        commands[i][command_size]='\0';
        pos=pos+command_size+2;
    }
    *commands_in_size=commands_size;
    return commands;

fail :
    for(int j=0;j< commands_size;j++) free(commands[j]);
    free(commands);
    return NULL;
}


int command_rec(char* command){
    for(int i=0;i<COMMANDS_SIZE;i++){
        if(str_cmp(command,COMMANDS[i])==1) return i+1;
    }
    return 0;
}

char* response(char* command,lists* ls){
    list* data_list=ls->l; ExpiryList* expiry_l=ls->el;
    struct hashMap* rlists=ls->rlists; void* result=NULL;
    int size=0; char ptr[100]; char* gotten=NULL;
    bool px_flag=false; int waiting_time; char* end_ptr;
    char** commands=command_parser(command,&size);
    if (!commands || size == 0) return NULL;
    char* resp; int cmd = command_rec(commands[0]);
    switch(cmd){
        case 1:
            if(size>1){
                resp= bulk_str(commands[1]);
            }else{
                resp=null_str();
            }
            break;
        case 2:
            strcpy(ptr,"PONG"); resp=smp_str("PONG");
            break;
        case 3:
            if(size==5 && str_cmp(commands[3], SET_OPTIONS[0])==1){
                px_flag=true; waiting_time=strtol(commands[4], &end_ptr,10);
            }
            if(!set_elem(data_list,expiry_l, commands[1], commands[2],px_flag,waiting_time)){
                resp = NULL;
                goto fail;
            }
            strcpy(ptr,"OK"); resp=smp_str(ptr);
            break;
        case 4:
            gotten=get_elem(data_list,commands[1]);
            if(!gotten){
                resp=null_str();
            }else{
                resp=bulk_str(gotten);
            }
            break;
        case 5:
            resp=r_push_comm(rlists,commands,size);
            break;
        case 6:
            resp=l_range(rlists,commands,size);
            break;
        case 7:
            resp=l_push_comm(rlists,commands, size);
            break;
        case 8:
            resp=l_len(rlists,commands,size);
            break;
        default:
            resp=malloc(sizeof(char)*6); strcpy(resp, "$-1\r\n");
            break;
    }
fail:
    for(int i=0;i<size;i++){
        free(commands[i]);
    }
    free(commands);
    return resp;
}
