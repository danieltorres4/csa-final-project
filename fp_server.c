/**************************************************/
/**** CLIENT-SERVER ARCHITECTURE FINAL PROJECT ****/
/*                                                */
/****************   TEAM MEMBERS   ****************/
/*                                                */
/********  HERRERA GODINA ADRIANA JOCELYN  ********/
/********   SANCHEZ TORRES SERGIO DANIEL   ********/
/**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define LENGTH 20000

int main(int argc, char *argv[]){
    int numbytes;
    char buf[150];

    int server_fd, client_fd;

    struct sockaddr_in server;
    struct sockaddr_in client;

    int sin_size_server;
    int sin_size_client;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1)
    {
        perror("Server-setsockopt() error!");
        exit(1);
    } else {
        printf("Server-setsockopt() is OK...\n");
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = INADDR_ANY;
    memset(&(server.sin_zero), '\0', 8);

    sin_size_server = sizeof(server);

    if (bind(server_fd, (struct sockaddr *)&server, sin_size_server) == -1)
    {
        perror("bind");
        exit(1);
    }

    if(listen(server_fd, 1) == -1){
            perror("listen");
            exit(1);
        }

    sin_size_client = sizeof(client);

    while (1){
        if((client_fd = accept(server_fd, (struct sockaddr *)&client, &sin_size_client)) == -1) {
            perror("accept");
            exit(1);
        }

        printf("Server: Client is connected from: %s\n", inet_ntoa(client.sin_addr));

        if((numbytes = recv(client_fd, buf, 100-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
    }
    
    
}