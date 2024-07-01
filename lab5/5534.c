#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

int x = 0;
pthread_mutex_t mutex;

void* ProcessA(void* arg){
    while (1)
    {
        pthread_mutex_lock(&mutex);
        x = x+1;
        if(x==20) x = 0;
        printf("A: %d\n", x);
        pthread_mutex_unlock(&mutex);
    }
 
}

void* ProcessB(void* arg){
    while (1)
    {
        pthread_mutex_lock(&mutex);
        x = x+1;
        if(x==20) x = 0;
        printf("B: %d\n", x);
        pthread_mutex_unlock(&mutex);
    }
 
}

int main() {

    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&t1, NULL, ProcessA, NULL);
    pthread_create(&t2, NULL, ProcessB, NULL);

    while (1)
    { }
    
    return 0;
}
