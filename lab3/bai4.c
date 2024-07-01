#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#define BUFFER_SIZE 10

int main() {
    const char *shm_name = "OS";
    int shm_fd;
    int *buffer;

    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);

    ftruncate(shm_fd, 4096);

    buffer = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (buffer == MAP_FAILED) {
        perror("Error mapping shared memory");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }

    int *control = buffer + BUFFER_SIZE; 
    *control = 1; // Set control variable to 1 (continue)

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Error forking process");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Consumer process
        int sum = 0; 
        for (int i = 0; i < BUFFER_SIZE; i++) {
            while (buffer[i] == 0) {
                usleep(1000);
            }
            sum += buffer[i];
            printf("Consumer read: %d\n", buffer[i]);
            if(sum > 100) {
                printf("Total sum: %d\n", sum);
                *control = 0; // Set control variable to 0 (stop)
                exit(0);
            }
        }
        printf("Total sum: %d\n", sum);
        exit(0);
    } else {
        // Producer process
        srand(time(NULL));
        for (int i = 0; i < BUFFER_SIZE; i++) {
            if (*control == 0) {
                break;
            }
            int random_value = rand() % 11 + 10;
            buffer[i] = random_value;
            printf("Buffer[%d]: %d\n", i, buffer[i]);
            usleep(100000);
        }
        wait(NULL);
    }

    munmap(buffer, (BUFFER_SIZE + 1) * sizeof(int));
    close(shm_fd);
    shm_unlink(shm_name);

    return 0;
}
