#include "../include/redis_protocols.h"

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
