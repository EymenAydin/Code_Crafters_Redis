#include "../include/map.h"
#include <stdio.h>

int main(){
    struct hashMap* new_m=create_hmap(sizeof(int),NULL);
    char key1[1000]; strcpy(key1,"eymen"); int val=1;
    insert(&new_m,key1,&val); int* ret_val=search(new_m,key1);
    printf("%d\n",*ret_val);
    destroy(new_m);
    return EXIT_SUCCESS;
}
