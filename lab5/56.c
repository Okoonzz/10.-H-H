#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

sem_t v_sem, w_sem, y_sem, z_sem;
int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5, x6 = 6;
int v, w, y, z, ans;

void* thread_a(void* arg) {
        w = x1 * x2;
        printf("W(A): %d\n", w);
        sem_post(&w_sem);
        sem_post(&w_sem);
        return NULL;
}

void* thread_b(void* arg) {
        v = x3 * x4;
        printf("V(B): %d\n", v);
        sem_post(&v_sem);
        sem_post(&v_sem);
        return NULL;
}

void* thread_c(void* arg) {
        sem_wait(&v_sem);
        y = v * x5;
        printf("Y(C): %d\n", y);
        sem_post(&y_sem);
        return NULL;
}

void* thread_d(void* arg) {
        sem_wait(&v_sem);
        z = v * x6;
        printf("Z(D): %d\n", z);
        sem_post(&z_sem);
        return NULL;
}

void* thread_e(void* arg) {

        sem_wait(&w_sem);
        sem_wait(&y_sem);
        y = w * y;
        printf("Y(E): %d\n", y);
        sem_post(&y_sem);
        return NULL;
}

void* thread_f(void* arg) {

        sem_wait(&w_sem);
        sem_wait(&z_sem);
        z = w * z;
        printf("Z(F): %d\n", z);
        sem_post(&z_sem);
        return NULL;
}

void* thread_g(void* arg) {
        sem_wait(&y_sem);
        sem_wait(&z_sem);
        ans = y + z;
        printf("ans = %d\n", ans);
        sem_post(&y_sem);
        sem_post(&z_sem);

    return NULL;
}

int main() {
    sem_init(&v_sem, 0, 0);
    sem_init(&w_sem, 0, 0); 
    sem_init(&y_sem, 0, 0);
    sem_init(&z_sem, 0, 0);

    pthread_t threads[7];
    pthread_create(&threads[0], NULL, thread_a, NULL);
    pthread_create(&threads[1], NULL, thread_b, NULL);
    pthread_create(&threads[2], NULL, thread_c, NULL);
    pthread_create(&threads[3], NULL, thread_d, NULL);
    pthread_create(&threads[4], NULL, thread_e, NULL);
    pthread_create(&threads[5], NULL, thread_f, NULL);
    pthread_create(&threads[6], NULL, thread_g, NULL);

    while (1)
    {
        /* code */
    }
    



    sem_destroy(&v_sem);
    sem_destroy(&w_sem);
    sem_destroy(&y_sem);
    sem_destroy(&z_sem);

    return 0;
}