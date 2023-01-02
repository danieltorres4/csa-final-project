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

//Required constants
#define LENGTH 20000 //response
#define MAXDATASIZE 100
#define QLEN 2

int main(int argc, char *argv[]){
    //This var will be used with popen()
    FILE *output;

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

    //Calling socket() function. It will return the server's file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    //Setting the socket options with the SOL_SOCKET level. setsockopt() function will receive
    //the server file descriptor, the level, the optional arguments like SO_REUSEADDR (to allow the reuse of local addresses)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1)
    {
        perror("Server-setsockopt() error!");
        exit(1);
    } else {
        printf("Server-setsockopt() is OK...\n");
    }

    //Initializing the struct attributes
    server.sin_family = AF_INET; //Machine byte sorting
    server.sin_port = htons(atoi(argv[1])); //Network byte sorting
    server.sin_addr.s_addr = INADDR_ANY; //Fill with IP address
    memset(&(server.sin_zero), '\0', 8); //Reset the rest of the structure to zero

    sin_size_server = sizeof(server);

    //Using bind() function to link an IP address
    if (bind(server_fd, (struct sockaddr *)&server, sin_size_server) == -1)
    {
        perror("bind");
        exit(1);
    }

    //Using listen() function
    if(listen(server_fd, 1) == -1){
            perror("listen");
            exit(1);
    }

    sin_size_client = sizeof(client);

    while (1){ //infinite loop

        //Using accept() function accept every client's connection. This function receives
        //server's file descriptor, client's struct and return the client's file descriptor
        if((client_fd = accept(server_fd, (struct sockaddr *)&client, &sin_size_client)) == -1) {
            perror("accept");
            exit(1);
        }

        printf("Server: A CLIENT is connected from: %s\n", inet_ntoa(client.sin_addr));

        do {
            //Using recv() function to "save" the client's requested command
            if((numbytes = recv(client_fd, command_request, sizeof(command_request), 0)) == -1) {
                perror("recv");
                exit(1);
            }

            printf("Client's command: %s\n", command_request); //printing the received command

            command_request[numbytes] = '\0';

            //Initializing output var with popen() function which receives the requested command and the flag (read)
            //popen() function will initiate pipe streams from a process
            //this function will execute the client's requested command
            output = popen(command_request, "r");

            //If popen() does not returns -1, the program will continue
            //Other way to see popen() function is the following: if a child process were created within popen() call 
            //using fork() and the child invoked execl() function
            if((output = popen(command_request, "r")) == -1) {
                perror("popen");
            }

            //Using a while loop to send the whole response
            while (fgets(buf, LENGTH-1, output) != NULL)
            {
                send(client_fd, buf, strlen(buf), 0);
            }

            pclose(output); //closing the resource

        } //As long as the word "adios" is not received, the following lines will be executed
        while(strcmp(command_request, "adios") != 0); //do-while condition

        printf("\nClient's connection has been ended\n");

        //Closing the client's file descriptor
        close(client_fd);
    }

    //Closing the server's file descriptor
    close(server_fd);
    shutdown(server_fd, SHUT_RDWR);
    exit(0);
}