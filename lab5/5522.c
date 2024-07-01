#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define MAX_SIZE 100

int a[MAX_SIZE];
int size = 0;
sem_t sem_add;
sem_t sem_remove;

void* add_random(void* arg) {
    while (1) {
        sem_wait(&sem_remove);
        if (size < MAX_SIZE) {
            a[size] = rand() % 100;
            printf("Added: %d, Size: %d\n", a[size], size+1);
            size++;
            sem_post(&sem_add);
        }
    }
}

void* remove_random(void* arg) {
    while (1) {
        sem_wait(&sem_add);
        if (size > 0) {
            int index = rand() % size;
            printf("Removed: %d, Size: %d\n", a[index], size-1);
            a[index] = a[size-1];
            size--;
            sem_post(&sem_remove);
        } else {
            printf("Nothing in array a\n");
            sem_post(&sem_remove);
        }
    }
}

int main() {
    srand(time(NULL));

    pthread_t t1, t2;
    sem_init(&sem_add, 0, 0);
    sem_init(&sem_remove, 0, 1);

    pthread_create(&t1, NULL, add_random, NULL);
    pthread_create(&t2, NULL, remove_random, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem_add);
    sem_destroy(&sem_remove);

    return 0;
}
