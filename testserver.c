#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

// A = 65, z = 122

#define SIZE 1024

// void generate_string(char *buffer){
//     char temp[24];
//     temp[0] = ' ';
//     temp[1] = '-';
//     temp[2] = ' ';
//     for(int i = 3; i < 23; i++){
//         int rand_value = rand()%58 + 65;
//         temp[i] = rand_value;
//     }
//     temp[23] = '\0';
//     strncat(buffer, temp, strlen(temp));
// }

void write_string(int sockfd){
    char temp[SIZE];
    char name[SIZE];
    read(sockfd, name, SIZE);
    FILE* file = fopen("file.txt", "r");
    while(fgets(temp, SIZE, file) != NULL){
        char fname[SIZE], psw[SIZE];
        sscanf(temp, "%s %s", fname, psw);
        if(strcmp(name, fname) == 0){
            strcpy(name, psw);
            break;
        }
    }

    time_t ticks = time(NULL);
    char buffer[SIZE];
    snprintf(buffer, sizeof(buffer), "%.24s ", ctime(&ticks));
    // generate_string(buffer);

    strncat(buffer, name, 21);

    write(sockfd, buffer, strlen(buffer));
}

void sig_chld(int signo){
    pid_t pid;
    int stat;
    char buffer[SIZE];

    while( (pid = waitpid(-1, &stat, WNOHANG)) > 0 ){
        snprintf(buffer, SIZE, "child %d terminated\n", pid);
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    return;
}

int main(void){
    srand(time(NULL));
    struct sockaddr_in addr, cliaddr;
    int server_fd, new_socket;
    socklen_t clilen;
    pid_t pid;

    if( (server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        // printf("Error with create socket\n");
        // exit(EXIT_FAILURE);
        perror("Error with create socket\n");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2324);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));

    if(listen(server_fd, 10) == -1){
        printf("Cannot be listened\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("listening...\n");
    }

    signal(SIGCHLD, sig_chld);
    
    for(;;){
        clilen = sizeof(cliaddr);
        if( (new_socket = accept(server_fd, (struct sockaddr *)&cliaddr, &clilen)) == -1 ){
            if(errno == EINTR) continue;
            else { printf("Error with accept\n"); exit(EXIT_FAILURE); }
        }
        if( (pid = fork()) == 0 ){
            close(server_fd);
            //work
            write_string(new_socket);
            exit(0);
        }
        close(new_socket);
    }
}