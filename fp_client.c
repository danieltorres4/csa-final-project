/**************************************************/
/**** CLIENT-SERVER ARCHITECTURE FINAL PROJECT ****/
/*                                                */
/****************  CLIENT PROGRAM  ****************/
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
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

//Required constants
#define MAXDATASIZE 100
#define MAXDATASIZE_RESP 20000

int main(int argc, char const *argv[])
{
    //Command request
    char command[MAXDATASIZE];
    int len_command;

    //Comamand response
    int numbytes;
    char buf[MAXDATASIZE_RESP];

    int sockfd; //client file descriptor
    struct hostent *he;
    struct sockaddr_in client; //address information

    if(argc != 3) {
        fprintf(stderr, "usage client hostname port\n");
        exit(1);
    }

    //Getting information about the host server
    if((he = gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname");
        exit(1);
    }

    //Calling socket() function. It will return the  client's file descriptor
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    //Initializing the struct attributes
    client.sin_family = AF_INET; //Machine byte sorting
    client.sin_port = htons(atoi(argv[2])); //Network byte sorting
    client.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(client.sin_zero), '\0', 8); //Reset the rest of the structure to zero

    //Trying to connect with the server.
    if(connect(sockfd, (struct sockaddr *)&client, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    //As long as the word "adios" is not received, the following lines will be executed
    while(strcmp(command, "adios") != 0) {
        fgets(command, sizeof(command), stdin); //the command is read ... MAXDATASIZE-1
        len_command = strlen(command) - 1; //the last character is left empty
        command[len_command] = '\0';

        printf("Command entered: %s\n",command); //the command entered is printed

        //The command is sent to the server. If there's an error, the program will finish
        if(send(sockfd, command, len_command, 0) == -1) {
            perror("send");
            exit(1);
        } else { //the command has been received successfully
            printf("The command has been sent\n");
        }

        //If recv() doesn't return an error, the response will be read
        if((numbytes = recv(sockfd, buf, MAXDATASIZE_RESP-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';
        printf("\nResponse received: \n%s\n",buf);

    }

    //Client's file descriptor is closed
    close(sockfd);

    return 0;
    
}