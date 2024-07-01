#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem_products;
sem_t sem_sells;

int products = 0;
int sells = 0;

void* processA(void* arg) {
    while (1) {
        sem_wait(&sem_products);
        sells++;
        printf("Sells: %d\n", sells);
        sem_post(&sem_sells);
    }
}

void* processB(void* arg) {
    while (1) {
        if (products - sells < 167) {
            products++;
            printf("Products: %d\n", products);
            sem_post(&sem_products);
        } else {
            printf("Too many products, waiting for more sells...\n");
        }
        sem_wait(&sem_sells);
    }
}

int main() {
    pthread_t t1, t2;
    sem_init(&sem_products, 0, 0);
    sem_init(&sem_sells, 0, 0);

    pthread_create(&t1, NULL, processA, NULL);
    pthread_create(&t2, NULL, processB, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem_products);
    sem_destroy(&sem_sells);

    return 0;
}
