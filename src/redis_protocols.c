#include "../include/redis_protocols.h"

char* bulk_arr2(char** prompts , int size){
    int length_prompts=count_of_digits(size); int resp_size=length_prompts+3;
    char* resp=malloc(sizeof(char)*(length_prompts+4)); char* pos=resp;
    snprintf(pos,length_prompts+4,"*%d\r\n",size); pos+=length_prompts+3;
    for(int i = 0; i < size; i++){
        int len = strlen(prompts[i]);
        int len_digits = count_of_digits(len);
        int char_size = len + len_digits + 5;
        int offset = pos - resp;
        resp_size += char_size;
        char* tmp = realloc(resp, sizeof(char) * (resp_size + 1));
        if(!tmp){ free(resp); free(prompts); return NULL; }
        resp = tmp;
        pos = resp + offset;
        snprintf(pos, char_size + 1, "$%d\r\n%s\r\n", len, prompts[i]);
        pos += char_size;
    }
    resp[resp_size] = '\0';
    for(int i=0;i<size;i++){
        free(prompts[i]);
    }
    free(prompts); return resp;
}

char* bulk_arr(char* res_str){
    if(res_str==NULL){
        char* resp=malloc(sizeof(char)*5);
        strcpy(resp,"*0\r\n"); return resp;
    }
    char* token=NULL; int size=0;
    char** prompts=malloc(sizeof(char*));
    token = strtok(res_str, " ");
    while(token != NULL){
        prompts[size]=token; size++;
        char** tmp = realloc(prompts, sizeof(char*) * (size+1));
        if(!tmp){ free(prompts); return NULL; }
        prompts = tmp;
        token = strtok(NULL, " ");
    }
    int length_prompts=count_of_digits(size); int resp_size=length_prompts+3;
    char* resp=malloc(sizeof(char)*(length_prompts+4)); char* pos=resp;
    snprintf(pos,length_prompts+4,"*%d\r\n",size); pos+=length_prompts+3;
    for(int i = 0; i < size; i++){
        int len = strlen(prompts[i]);
        int len_digits = count_of_digits(len);
        int char_size = len + len_digits + 5;
        int offset = pos - resp;
        resp_size += char_size;
        char* tmp = realloc(resp, sizeof(char) * (resp_size + 1));
        if(!tmp){ free(resp); free(prompts); return NULL; }
        resp = tmp;
        pos = resp + offset;
        snprintf(pos, char_size + 1, "$%d\r\n%s\r\n", len, prompts[i]);
        pos += char_size;
    }
    resp[resp_size] = '\0';
    free(prompts); return resp;
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

char* int_str(int msg){
    int digit_count= (msg==0) ? 1: count_of_digits(msg);
    char* str_msg=malloc(sizeof(char)*(digit_count+4)); char* pos=str_msg;
    memcpy(pos++,":",1); snprintf(pos,digit_count+1,"%d",msg); pos=pos+digit_count;
    memcpy(pos,"\r\n\0",3); return str_msg;
}

char* null_str(){
    char* ret_msg=malloc(sizeof(char)*6);
    strcpy(ret_msg, "$-1\r\n"); return ret_msg;
}
