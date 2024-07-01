#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MAX_SIZE 100

int a[MAX_SIZE];
int size = 0;

void* add_random(void* arg) {
    while (1) {
        if (size < MAX_SIZE) {
            a[size] = rand() % 100;
            printf("Added: %d, Size: %d\n", a[size], size+1);
            size++;
            sleep(1);
        }
    }
}

void* remove_random(void* arg) {
    while (1) {
        if (size > 0) {
            int index = rand() % size;
            printf("Removed: %d, Size: %d\n", a[index], size-1);
            a[index] = a[size-1];
            size--;
            sleep(1);
        } else {
            printf("Nothing in array a\n");
            sleep(1);
        }
    }
}

int main() {
    srand(time(NULL));

    pthread_t t1, t2;

    pthread_create(&t1, NULL, add_random, NULL);
    pthread_create(&t2, NULL, remove_random, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
