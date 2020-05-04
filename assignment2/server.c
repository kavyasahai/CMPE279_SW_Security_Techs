// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pwd.h>
#include <assert.h>
#define PORT 8080 

void fork_process(int server_fd, int new_socket, int valread,char buffer[1024],
char *hello,struct sockaddr_in address,int addrlen, int argc);
void process_data(char *args[]);

int main(int argc, char const *argv[]) 
{ 
    if(argc>1){
        process_data(argv);
    }
    else{
        printf("argc: %d\n",argc);
        int server_fd, new_socket, valread; 
        struct sockaddr_in address; 
        int opt = 1; 
        int addrlen = sizeof(address); 
        char buffer[1024] = {0}; 
        char *hello = "Hello from server"; 
        
        // Creating socket file descriptor 
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
        { 
            perror("socket failed"); 
            exit(EXIT_FAILURE); 
        } 
        
        // Forcefully attaching socket to the port 8080 
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, 
                                                    &opt, sizeof(opt))) 
        { 
            perror("setsockopt"); 
            exit(EXIT_FAILURE); 
        } 
        address.sin_family = AF_INET; 
        address.sin_addr.s_addr = INADDR_ANY; 
        address.sin_port = htons( PORT ); 
        
        // Forcefully attaching socket to the port 8080 
        if (bind(server_fd, (struct sockaddr *)&address,  
                                    sizeof(address))<0) 
        { 
            perror("bind failed"); 
            exit(EXIT_FAILURE); 
        } 
        if (listen(server_fd, 3) < 0) 
        { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        } 
        
        fork_process(server_fd, new_socket, valread, buffer, hello, address, addrlen,argc);
    }
    return 0; 
} 

void fork_process(int server_fd, int new_socket, int valread,char buffer[1024],
char *hello,struct sockaddr_in address,int addrlen,int argc) 
{ 
    pid_t childPID,wpid;

    childPID = fork();
    int status = 0;

    if(childPID >= 0) // fork successful
    {
        printf("Fork successful\n");
        if(childPID == 0) // child process
        {
            printf("Inside child process\n");
            //drop privileges here
            struct passwd *pw= getpwnam("nobody");
            assert(pw != NULL);
        
            if(pw == NULL)
            {
                printf("Failed to drop privileges since pw is null\n");
                exit(EXIT_FAILURE);
            }
        
            if(setuid(pw->pw_uid) != 0)
            {
                printf("Failed to drop privileges since setuid not 0\n");
                exit(EXIT_FAILURE);
            }
            else{
            printf("Successfully dropped privileges\n");
            }
            
            char fd_arg [10];
            char port_arg [10];
            snprintf(fd_arg,10, "%d", server_fd);
            snprintf(port_arg,10,"%d",PORT);
            
            char *args[]={fd_arg, port_arg, NULL};

            execv("./server",args);

        }
    }
    else // fork failed
    {
        printf("\n Fork has failed\n");
        exit(EXIT_FAILURE);
    }
    printf("server main ending\n");
    
}

void process_data(char *args[]){
    int new_socket, valread;
    struct sockaddr_in address;
    char buffer[1024] = {0};
    int addrlen = sizeof(address);
    char *hello = "Hello from server";
    int server_fd = atoi(args[0]);
    int p = atoi(args[1]);

    printf("server: %d \n", server_fd);
    printf("port: %d \n", p);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( p );

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
						   (socklen_t*)&addrlen))<0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	valread = read( new_socket , buffer, 1024);
	printf("%s\n",buffer );
	send(new_socket , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
}