#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 100

int main() {
    FILE *output;
    char buffer[BUFFER];
    char comm[BUFFER];
    int counter = 0;

    printf("Starting system call...\n");
    
    // output = popen("date","r");
    // printf("Donde with system call\n\n");

    // if(output == NULL) {
    //     fputs("POPEN: Failed to execute command\n", stderr);
    // } else {
    //     int count = 1;

    //     while(fgets(buffer, BUFFER - 1, output) != NULL) {
    //         printf("OUTPUT[%d]: %s\n", count, buffer);
    //         count++;
    //     }
    // }
    
    //closing the resource
    //pclose(output); 

    // while(counter < 4) {
    //     printf("Please insert a command to be executed: ");
    //     scanf("%s", comm);

    //     output = popen(comm, "r");
    //     printf("Donde with system call\n\n");

    //     if(output == NULL) {
    //         fputs("POPEN: Failed to execute command\n", stderr);
    //     } else {
    //         int count = 1;

    //         while(fgets(buffer, BUFFER - 1, output) != NULL) {
    //             printf("OUTPUT[%d]: %s\n", count, buffer);
    //             count++;
    //         }
    //     }

    //     counter++;
    //     pclose(output); //closing the resource
    // }

    do
    {
        printf("Please insert a command to be executed: ");
        scanf("%s", comm);

        output = popen(comm, "r");
        printf("Donde with system call\n\n");

        if(output == NULL) {
            fputs("POPEN: Failed to execute command\n", stderr);
        } else {
            int count = 1;

            while(fgets(buffer, BUFFER - 1, output) != NULL) {
                printf("OUTPUT[%d]: %s\n", count, buffer);
                count++;
            }
        }
    } while (strcmp(comm, "adios") != 0);
    

    return 0;
}