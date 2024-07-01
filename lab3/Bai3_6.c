#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>


int main(int argc, char** argv){

    if(argc != 2){
        printf("Please enter one number\n");
        exit(0);
    }

    const char *shm_name = "OS";
    int shm_fd;
    int *buffer;

    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);

    ftruncate(shm_fd, 4096);

    buffer = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    int ip1 = atoi(argv[1]);
    if(ip1 < 0){
        printf("Please enter positive number\n");
        exit(0);
    }

    
    for (int i = 0; i < ip1; i++) buffer[i] = 0;
    
    pid_t pid = fork();

    if(pid == 0){
        int i = 1;
        buffer[0] = ip1;
        while(ip1 != 1){
            if(ip1 % 2 == 0) ip1 /= 2;
            else  ip1 = 3*ip1 + 1;
            buffer[i] = ip1;
            i++;
        }
    }
    else{
        wait(NULL);
        int i = 0;
            while(buffer[i] != 0){
                printf("%d ", buffer[i]);
                i++;
            } 
    }
}