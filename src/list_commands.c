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
    if((search_res=search(rlists,commands[1]))!=NULL && size==4){
        result=*((r_list**)search_res); int idxs[2];
        idxs[0]=atoi(commands[2]); idxs[1]=atoi(commands[3]);
        if(idxs[0]==result->size || idxs[0]>idxs[1]){
            resp=bulk_arr(NULL); return resp;
        }
        for(int i=0;i<2;i++){
            if(idxs[i]>result->size) idxs[i]=result->size-1;
            if(idxs[i]<-result->size) idxs[i]=0;
        }
        for(int i=0;i<2;i++){
            if(idxs[i]<0) idxs[i]= result->size + idxs[i] ;
        }
        int s=0; int e=0;

        if(idxs[0]<idxs[1]){
            s=idxs[0]; e=idxs[1];
        }else{
            s=idxs[1]; e=idxs[0];
        }
        int length=e-s + 1;
        keys=malloc(sizeof(char*)*length);
        r_elem* iter=result->head;
        for(int i=0;i<s;i++){
            iter=iter->next;
        }
        for(int i=0;i<length;i++){
            keys[i]=malloc(sizeof(char)*strlen(iter->key)+1);
            strcpy(keys[i],iter->key); iter=iter->next;
        }
        char* ret=bulk_arr2(keys,length);  return ret;
    }else{
        resp=bulk_arr(NULL); return resp;
    }
}
