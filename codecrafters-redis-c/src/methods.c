#include "../include/methods.h"

int count_of_digits(int n){
    if(n==0){
        return 0;
    }else{
        return 1+count_of_digits(n/10);
    }
}

int str_cmp(char* str1,const char* str2){
    if(strlen(str1)!=strlen(str2)) return 0;
    int a=0; int b=0;
    for(;str1[a]!='\0';a++,b++){
        if(tolower(str1[a])!=tolower(str2[b])) return 0;
    }
    return 1;
}
