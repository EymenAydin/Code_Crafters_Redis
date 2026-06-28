#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../include/redis_protocols.h"

#define PORT 6379


int main(){
    int status,read_bytes,client_fd=0;
    struct sockaddr_in serv_addr;
    if((client_fd= socket(AF_INET,SOCK_STREAM,0))<0){
        printf("Socket creation error\n");
        return EXIT_FAILURE;
    }
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_port=htons(PORT);

    if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0){
        printf("\nInvalid address / Address not supported.\n");
        return EXIT_FAILURE;
    }

    if((status = connect(client_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)){
        printf("Connection failed.");
        return EXIT_FAILURE;
    }

    char message[1024];
    char response[1024]={0};
    while(1){
       printf(">");
       if (fgets(message, sizeof(message), stdin) == NULL) break;
       message[strcspn(message, "\n")] = 0;

       char* formatted_msg = bulk_arr(message);
       if(send(client_fd, formatted_msg, strlen(formatted_msg), 0) < 0) {
           free(formatted_msg);
           break;
       }

       memset(response, 0, sizeof(response));
       read_bytes=read(client_fd, response, 1023);
       if (read_bytes <= 0) {
           free(formatted_msg);
           break;
       }
       printf(">%s \n",response);
       free(formatted_msg);
    }
    close(client_fd);
    return EXIT_SUCCESS;
}
