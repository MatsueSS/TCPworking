#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#define SIZE 1024

// void sig_chld(int signo){
//     pid_t pid;
//     int stat;

//     pid = wait(&stat);
//     char buffer[SIZE];
//     snprintf(buffer, SIZE, "child %d terminated\n", pid);
//     write(STDOUT_FILENO, buffer, strlen(buffer));
//     // printf("child %d terminated\en", pid);
//     return;
// }

void sig_chld(int signo){
    pid_t pid;
    int stat;
    char buffer[SIZE];

    while( (pid = waitpid(-1, &stat, WNOHANG)) > 0){
        snprintf(buffer, SIZE, "child %d terminated\n", pid);
        write(STDOUT_FILENO, buffer, strlen(buffer));
        // printf("child %d terminated\en", pid);
    }

    return;
}

void str_echo(int sockfd){
    ssize_t n;
    char buff[SIZE];

    while ((n = read(sockfd, buff, SIZE)) > 0) {
        write(sockfd, buff, n);
    }
}

void accept_message(int sockfd){
    ssize_t n;
    char temp[SIZE];

    while( (n = read(sockfd, temp, SIZE)) > 0){
        char buff[SIZE] = "Message was received: ";
        strncat(buff, temp, n);
        write(sockfd, buff, strlen(buff));
    }
}

int main(int argc, char** argv){
    int listenfd, connfd, n;
    struct sockaddr_in cliaddr, servaddr;
    pid_t childpid;
    socklen_t clilen;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(2323);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, 3);

    signal(SIGCHLD, sig_chld);

    for(;;){
        clilen = sizeof(cliaddr);
        if( (connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0){
            if(errno == EINTR) continue;
            else { printf("accept error"); exit(EXIT_FAILURE); }
        }

        if ((childpid = fork()) == 0){
            close(listenfd);
            accept_message(connfd);
            exit(0);
        }
        close(connfd);
    }
}