#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

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
	 	printf("Listen failed: %s \n", strerror(errno));
	 	return 1;
	 }

	 printf("Waiting for a client to connect...\n");
	 client_addr_len = sizeof(client_addr);

	 int client=accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
	 if(client!=-1) 	 printf("Client connected\n");
	 else return 0;
	 void *command = malloc(sizeof(char) * 1024);
     const char *response = "+PONG\r\n";
     const char *command_ex = "*1\r\n$4\r\nPING\r\n";
     size_t command_ex_len = strlen(command_ex);
     ssize_t bytes_read;

     while ((bytes_read = read(client, command, 1023)) > 0) {
         void *search_pos = command;
         size_t remaining = bytes_read;
         void *pos;
         while ((pos = memmem(search_pos, remaining, command_ex, command_ex_len)) != NULL) {
             send(client, response, strlen(response), 0);
             remaining =  remaining - ((char *)pos - (char *)search_pos) - command_ex_len;
             search_pos = (char *)pos + command_ex_len;
         }

     }
     close(server_fd);
     free(command);
     return 0;
}
