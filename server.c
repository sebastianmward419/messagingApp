#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 

void error (const char *msg) {
    perror(msg);
    exit(1);
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Port number not provided, Goodbye \n");
        exit(1); 
    }

    int sockFd, newSockFd, portNumber, n;
    char buffer[256];

    struct sockaddr_in server_addr, client_addr;
    socklen_t clilen;

    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd < 0) {
        error("Error opening socket");
    }

    bzero((char *) &server_addr, sizeof(server_addr));
    portNumber = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portNumber);

    if (bind(sockFd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        error("Binding Failed");
    }

    listen(sockFd, 5);
    clilen = sizeof(client_addr);

    newSockFd = accept(sockFd, (struct sockaddr *) &client_addr, &clilen);

    if (newSockFd < 0) {
        error("Error on Accept");
    }

    while(1) {
        bzero(buffer, 255);
        n = read(newSockFd, buffer, 255);

        if (n < 0) {
            error("Error on read");
        }

        printf("Client: %s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = write(newSockFd, buffer, strlen(buffer));

        if (n < 0) {
            error("Error on write");
        }

        int i = strncmp("Bye", buffer, 3);

        if (i == 0) {
            break;
        }
    }

    close(newSockFd);
    close(sockFd);
    
    return 0;
}