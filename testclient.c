#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>

#define SIZE 1024

int main(int argc, char** argv){
    char name[SIZE];
    struct sockaddr_in addr;
    int _sockfd, n;
    char buffer[SIZE];

    fgets(name, SIZE, stdin);
    name[strlen(name)-1] = '\0';

    if((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        printf("Cannot be create socket\n");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2324);
    inet_pton(AF_INET, argv[1], &addr.sin_addr);

    if(connect(_sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1){
        printf("Error with connect\n");
        exit(EXIT_FAILURE);
    }

    write(_sockfd, name, strlen(name));

    while((n = read(_sockfd, buffer, SIZE)) > 0){
        write(STDIN_FILENO, buffer, n);
    }

    close(_sockfd);
}