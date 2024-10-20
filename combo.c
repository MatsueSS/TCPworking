#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 1024

int main(void)
{
    char name[SIZE];
    fgets(name, SIZE, stdin);
    name[strlen(name)-1] = '\0';
    printf("\n%d", strlen(name));

    // char name[SIZE];
    // char psw[SIZE];
    // FILE* file = fopen("file.txt", "r");
    // char test[SIZE];
    // while(fgets(test, SIZE, file) != NULL){
    //     sscanf(test, "%s %s", name, psw);
    //     puts(name);
    //     puts(psw);
    //     puts(test);
    // }

    // time_t ticks = time(NULL);
    // char buffer[SIZE];
    // char temp[SIZE] = "test string in C";
    // snprintf(buffer, sizeof(buffer), "%.24s - ", ctime(&ticks));
    // strncat(buffer, temp, strlen(temp));

    // puts(buffer);

    return 0;
}