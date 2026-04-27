#include <ctype.h>
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

const char* COMMANDS[]={"echo","ping"};


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
char** command_parser(char* command,int* commands_in_size){
    char* pos=strchr(command,'*');
    if(!pos) return NULL;
    char* end_ptr;
    int commands_size=strtol(pos+1,&end_ptr,10);
    if(commands_size<0) return NULL;
    char** commands=malloc(sizeof(char*)*commands_size);
    if(commands==NULL) return NULL;
    int command_size=0;
    for(int i=0;i<commands_size;i++){
        pos=strchr(pos,'$');
        if(!pos) goto fail; pos+=1;
        command_size=strtol(pos,&end_ptr,10);
        commands[i]=malloc(sizeof(char)*(command_size+1));
        if(!commands[i]) goto fail;
        pos=strstr(pos,"\r\n"); if(!pos) goto fail; pos=pos+2;
        strncpy(commands[i], pos, command_size);
        commands[i][command_size]='\0';
        pos=pos+command_size+2;
    }
    *commands_in_size=commands_size;
    return commands;

fail :
    for(int j=0;j< commands_size;j++) free(commands[j]);
    free(commands);
    return NULL;
}
int command_rec(char* command){
    if(str_cmp(command,COMMANDS[0])==1) return 1;
    else if(str_cmp(command,COMMANDS[1])==1) return 2;
    else return 0;
}
char* response(char* command){
    int size=0;
    char** commands=command_parser(command,&size);
    /*
    if (!commands || size == 0) return NULL;
    char* res_str;
    int cmd = command_rec(commands[0]);
        if (cmd == 1 && size > 1)   res_str = commands[1];
        else if (cmd == 2)          res_str = "+PONG";

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
    */
    int resp_size=strlen(commands[1]);
    char* resp=malloc(sizeof(char)*(resp_size+5));
    char* pos=resp; memcpy(pos,"+",1); pos+=1;
    memcpy(pos,commands[1],resp_size); pos+=resp_size;
    memcpy(pos,"\r\n",2);  resp[resp_size]='\0';
    for(int i=0;i<size;i++){
        free(commands[i]);
    }
    free(commands);
    return resp;
}

void* handle_client(void* arg){
    int client=*(int*)arg;
    free(arg);
    char *command = malloc(sizeof(char) * 1024);
    ssize_t bytes_read;
    while ((bytes_read = read(client, command, 1023)) > 0) {
        command[bytes_read]='\0';
        char* resp=response(command);
        send(client, resp, strlen(resp), 0);
        free(resp);
    }

    free(command);
    close(client);
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
		 int *client_fd=malloc(sizeof(int));
		 *client_fd=accept(server_fd,(struct sockaddr*)&client_addr,&client_addr_len);
		 if(*client_fd==-1){
			    printf("Accept failed: %s\n", strerror(errno));
				free(client_fd);
				continue;
		 }

		 printf("Client connected\n");
		 pthread_t thread;
		 if (pthread_create(&thread,NULL,handle_client,client_fd)!=0){
			    printf("Thread creation failed: %s\n",strerror(errno));
				close(*client_fd);
				free(client_fd);
				continue;
		}
		pthread_detach(thread);
	 }

     close(server_fd);
     return 0;
}
