#include "commands.h"
#include "database.h"
#include "methods.h"
#include <stdio.h>
#include <string.h>

#define COMMANDS_SIZE 4
const char* COMMANDS[]={"echo","ping","set","get"};
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

char* response(char* command,list* data_list,ExpiryList* expiry_l){
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
                resp=malloc(sizeof(char)*6);
                strcpy(resp, "$-1\r\n");
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
                goto fail;
            }
            strcpy(ptr,"OK"); resp=smp_str(ptr);
            break;
        case 4:
            gotten=get_elem(data_list,commands[1]);
            if(!gotten){
                resp=malloc(sizeof(char)*6);
                strcpy(resp, "$-1\r\n");
            }else{
                resp=bulk_str(gotten);
            }
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

char* bulk_str(char* res_str){
    int length_i = strlen(res_str);
    int digits = (length_i == 0) ? 1 : count_of_digits(length_i);
    int resp_size = length_i + digits + 6;
    char* resp=malloc(sizeof(char)*resp_size);
    resp[0]='$'; char* pos=resp+1;
    snprintf(pos,digits+1,"%d",length_i); pos=pos+digits;
    memcpy(pos,"\r\n",2); pos+=2;
    memcpy(pos,res_str,length_i); pos=pos+length_i;
    memcpy(pos,"\r\n",2);
    resp[resp_size-1]='\0';
    return resp;
}

char* smp_str(const char* msg){
    int length=strlen(msg);
    int resp_size=length+4;
    char* resp=malloc(sizeof(char)*resp_size); char* pos=resp;
    memcpy(pos++,"+",1); memcpy(pos,msg,length); pos+=length;
    memcpy(pos,"\r\n\0",3);
    return resp;
}
