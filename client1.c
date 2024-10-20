#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define SIZE 1024

void str_cli(FILE* fp, int sockfd){
    char sendline[SIZE], recvline[SIZE];
    while(fgets(sendline, SIZE, fp) != NULL){
        write(sockfd, sendline, strlen(sendline));
        char recvline[SIZE];
        read(sockfd, recvline, SIZE);
        fputs(recvline, stdout);
        memset(recvline, 0, SIZE);
    }
}

int main(int argc, char *argv[]){
    struct sockaddr_in addr;
    int client_fd;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_port = htons(2323);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &addr.sin_addr);

    connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));

    str_cli(stdin, client_fd);

    exit(0);
}