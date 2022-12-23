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

void main(int argc, char const *argv[])
{
    char command[MAXDATASIZE];
    int len_command;

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
    
}
