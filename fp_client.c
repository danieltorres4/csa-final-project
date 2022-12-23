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
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
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

    int sockfd;
    struct hostent *he;
    struct sockaddr_in client;

    if(argc != 3) {
        fprintf(stderr, "usage client hostname port\n");
        exit(1);
    }

    if((he = gethostbyname(argv[1]) == NULL)) {
        perror("gethostbyname");
        exit(1);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    client.sin_family = AF_INET;
    client.sin_port = htons(atoi(argv[2]));
    client.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(client.sin_zero), '\0', 8);

    if(connect(sockfd, (struct sockaddr *)&client, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    while(strcmp(buf, "bye\n") != 0) {
        fgets(command, MAXDATASIZE-1, stdin);
        len_command = strlen(command) - 1;
        command[len_command] = '\0';

        printf("Command: %s\n",command);

        if(send(sockfd, command, len_command, 0) == -1) {
            perror("send");
            exit(1);
        } else {
            printf("Command has been sent");
        }

        if((numbytes = recv(sockfd, buf, MAXDATASIZE_RESP-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';
        printf("Received:\n%s\n",buf);

        close(sockfd);

    }

    return 0;
    
}
