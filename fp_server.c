/**************************************************/
/**** CLIENT-SERVER ARCHITECTURE FINAL PROJECT ****/
/*                                                */
/****************  SERVER PROGRAM  ****************/
/*                                                */
/****************   TEAM MEMBERS   ****************/
/*                                                */
/********  HERRERA GODINA ADRIANA JOCELYN  ********/
/********   SANCHEZ TORRES SERGIO DANIEL   ********/
/**************************************************/

//Required headers
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

#define LENGTH 20000 //response
#define MAXDATASIZE 100
#define QLEN 2

int main(int argc, char *argv[]){
    FILE *output;
    char buffer[LENGTH];

    //For the command request
    char command_request[MAXDATASIZE];
    int len_comm;

    //For the response
    char buf[MAXDATASIZE];
    int numbytes;

    struct hostent *info_client;

    //Server and client file descriptors
    int server_fd, client_fd;

    //sockaddr_in structs: server and client
    struct sockaddr_in server;
    struct sockaddr_in client;

    //server and client length
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

    server.sin_family = AF_INET; //Machine byte sorting
    server.sin_port = htons(atoi(argv[1])); //Network byte sorting
    server.sin_addr.s_addr = INADDR_ANY; //Fill with IP address
    memset(&(server.sin_zero), '\0', 8); //Reset the rest of the structure to zero

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

        printf("Server: A CLIENT is connected from: %s\n", inet_ntoa(client.sin_addr));

        do {
            if((numbytes = recv(client_fd, command_request, sizeof(command_request), 0)) == -1) {
                perror("recv");
                exit(1);
            }

            printf("Client's command: %s\n", command_request);

            command_request[numbytes] = '\0';

            //fgets(buf, MAXDATASIZE, stdin);

            //Using popen() function
            output = popen(command_request, "r");

            if((output = popen(command_request, "r")) == -1) {
                perror("popen");
            }

            while (fgets(buf, LENGTH-1, output) != NULL)
            {
                send(client_fd, buf, strlen(buf), 0);
            }

            //fgets(buf, LENGTH - 1, output);
            //send(client_fd, buf, strlen(buf), 0);

            pclose(output); //closing the resource

        }
        while(strcmp(command_request, "adios") != 0);

        printf("\nClient's connection has been ended\n");
        close(client_fd);
        //exit(0);

    }

    close(server_fd);
    shutdown(server_fd, SHUT_RDWR);
    exit(0);
}