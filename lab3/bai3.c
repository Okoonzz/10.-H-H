#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main() {
    pid_t child_pid;
    int status;

    printf("Welcome to IT007, I am 22520167!\n");

    child_pid = fork();

    if (child_pid == -1) {
        perror("Error fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        execl("./count.sh", "./count.sh", "120", NULL);
        perror("Error command");
        exit(EXIT_FAILURE);
    } else {
        // Tiến trình cha
        signal(SIGINT, intHandler);
        while (keepRunning) {
            wait(&status);
        }
        printf("\ncount.sh has stopped\n");
    }

    return 0;
}
