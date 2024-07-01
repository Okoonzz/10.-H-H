#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char**argv) {
    pid_t child_pid;
    struct timeval start_time, end_time;

    child_pid = fork();

    if (child_pid == -1) {
        perror("Error forking process");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Child 
        gettimeofday(&start_time, NULL);
        execl("/bin/sh", "sh", "-c", argv[1], NULL);
        perror("Error executing command");
        exit(EXIT_FAILURE);
    } else {
        // Parent 
        wait(NULL); 
        gettimeofday(&end_time, NULL); 

        double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                              (end_time.tv_usec - start_time.tv_usec) / 1e6;

        printf("Thời gian thực thi: %.6f seconds\n", elapsed_time);
    }

    return 0;
}
