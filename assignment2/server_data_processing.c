#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pwd.h>
#include <assert.h>

int main(int argc,char *args[])
{
    printf("Now in executable called by execvp() \n");

    int new_socket, valread;
    struct sockaddr_in address;
    char buffer[1024] = {0};
    int addrlen = sizeof(address);
    char *hello = "Hello from server";
    int server_fd = atoi(args[0]);
    int PORT = atoi(args[1]);

    printf("server: %d \n", server_fd);
    printf("port: %d \n", PORT);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
						   (socklen_t*)&addrlen))<0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	valread = read( new_socket , buffer, 1024);
	printf("%s\n",buffer );
	send(new_socket , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");

    return 0;
}