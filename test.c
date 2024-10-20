// #include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <x86_64-linux-gnu/asm/signal.h>

void handler(int signum) {
    printf("Caught signal %d\n", signum);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // Измените на 0 вместо 1

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("Press Ctrl+C to trigger the signal.\n");

    // Бесконечный цикл для ожидания сигнала
    while (1) {
        pause(); // Ожидает сигнала
    }

    return 0;
}
