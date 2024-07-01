#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

int x = 0;

void* ProcessA(void* arg){
    while (1)
    {
        x = x+1;
        if(x==20) x = 0;
        printf("A: %d\n", x);
    }
}

void* ProcessB(void* arg){
    while (1)
    {
        x = x+1;
        if(x==20) x = 0;
        printf("B: %d\n", x);
    }
 
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, ProcessA, NULL);
    pthread_create(&t2, NULL, ProcessB, NULL);

    while (1)
    { }
    
    return 0;
}