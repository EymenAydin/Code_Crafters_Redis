#include "../include/map.h"
#include "../include/database.h"
#include <stdio.h>

int main(){
    struct hashMap* new_m=create_hmap(sizeof(r_list*),destroy_rlist_wrapper);
    char key1[1000]; strcpy(key1,"eymen"); r_list* new_l=create_rlist();
    insert(&new_m,key1,&new_l); r_list* ret_val=*(r_list**)search(new_m,key1);
    r_push(new_l,"asd");
    printf("%d",new_l->size);
    r_push(new_l,"dsa");
    printf("%d",new_l->size);
    destroy(new_m);
    return EXIT_SUCCESS;
}
