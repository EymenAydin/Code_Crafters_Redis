#include "../include/list_commands.h"
#include <string.h>

char* r_push_comm(struct hashMap* rlists,char** commands,int size){
    r_list* result=NULL; char* resp=NULL;
    if((result=search(rlists,commands[1]))!=NULL){
        if(size>2){
           r_list* res_rl=*((r_list**)result);
           for(int i=2;i<size;i++){
               r_push(res_rl,commands[i]);
           }
           int l_size=res_rl->size;
           resp=int_str(l_size);
        }
        else{
            resp=null_str();
        }
    }
    else{
        r_list* new_rl=create_rlist();
        insert(&rlists,commands[1],&new_rl);
        for(int i=2;i<size;i++){
            r_push(new_rl,commands[i]);
        }
        int l_size=new_rl->size;
        resp=int_str(l_size);
    }
    return resp;
}

char* l_range(struct hashMap* rlists,char** commands,int size){
    void* search_res=NULL; r_list* result=NULL;
    char* resp=NULL; char** keys=NULL;
    if((search_res=search(rlists,commands[1]))!=NULL || size==4){
        result=*((r_list**)search_res);
        int st_idx=atoi(commands[2]); int end_idx=atoi(commands[3]);
        printf("%d %d\n",st_idx,end_idx);
        if(st_idx==result->size || st_idx>end_idx){
            resp=bulk_arr(NULL); return resp;
        }
        if(end_idx>result->size) end_idx=result->size-1;
        int length=end_idx-st_idx + 1;
        keys=malloc(sizeof(char*)*length);
        r_elem* iter=result->head;
        for(int i=0;i<st_idx;i++){
            iter=iter->next;
        }
        for(int i=0;i<length;i++){
            printf("%s\n",iter->key);
            keys[i]=malloc(sizeof(char)*strlen(iter->key)+1);
            strcpy(keys[i],iter->key); iter=iter->next;
        }
        char* ret=bulk_arr2(keys,length);  return ret;
    }else{
        resp=null_str(); return resp;
    }
}
