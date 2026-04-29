#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "commands.h"
#include "database.h"


typedef struct thread_args{
    list* data_list;
    int* client_fd;
}thread_args;


void* handle_client(void* arg){
    int client=*(((thread_args*)arg)->client_fd); list* data=((thread_args*)arg)->data_list;
    free(((thread_args*)arg)->client_fd); free(arg);
    char *command = malloc(sizeof(char) * 1024);
    ssize_t bytes_read;
    while ((bytes_read = read(client, command, 1023)) > 0) {
        command[bytes_read]='\0';
        char* resp=response(command,data);
        send(client, resp, strlen(resp), 0);
        free(resp);
    }
    destroy_list(data); free(command); close(client);
    return NULL;
}

int main() {
	// Disable output buffering
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	// You can use print statements as follows for debugging, they'll be visible when running tests.
	printf("Logs from your program will appear here!\n");


	 int server_fd;
	 unsigned int client_addr_len;
	 struct sockaddr_in client_addr;

	 server_fd = socket(AF_INET, SOCK_STREAM, 0);
	 if (server_fd == -1) {
	 	printf("Socket creation failed: %s...\n", strerror(errno));
	 	return 1;
	 }

	 // Since the tester restarts your program quite often, setting SO_REUSEADDR
	 // ensures that we don't run into 'Address already in use' errors
	 int reuse = 1;
	 if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
	 	printf("SO_REUSEADDR failed: %s \n", strerror(errno));
	 	return 1;
	 }

	 struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
	 								 .sin_port = htons(6379),
	 								 .sin_addr = { htonl(INADDR_ANY) },
	 								};

	 if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
	 	printf("Bind failed: %s \n", strerror(errno));
	 	return 1;
		 }

	 int connection_backlog = 5;
	 if (listen(server_fd, connection_backlog) != 0) {
	 	printf("Listen fax  iled: %s \n", strerror(errno));
	 	return 1;
	 }
	 printf("Waiting for a client to connect...\n");
	 client_addr_len = sizeof(client_addr);
	 int client;
	 while(1){
		 int* client_fd=malloc(sizeof(int));
		 *client_fd=accept(server_fd,(struct sockaddr*)&client_addr,&client_addr_len);
		 if(*client_fd==-1){
			    printf("Accept failed: %s\n", strerror(errno));
				free(client_fd);
				continue;
		 }
         thread_args* args=malloc(sizeof(thread_args));
		 args->data_list=create_list();
	 	 args->client_fd=client_fd;
		 printf("Client connected\n");
		 pthread_t thread;
		 if (pthread_create(&thread,NULL,handle_client,args)!=0){
			    printf("Thread creation failed: %s\n",strerror(errno));
				close(*client_fd);
				free(client_fd); free(args->data_list); free(args);
				continue;
		}
		pthread_detach(thread);
	 }

     close(server_fd);
     return 0;
}
