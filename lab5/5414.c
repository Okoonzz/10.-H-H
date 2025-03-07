#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 2
void *thread_print(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Hello IT007! I'm Thread #%ld ^_^!!!\n", getppid());
    sleep(100);
    pthread_exit(NULL);
}
int main()
{
    pthread_t threads[NUM_THREADS];
    int check;
    long tID;
    for (tID = 0; tID < NUM_THREADS; tID++)
    {
        printf("I'm Main Thread: create Thread: #%ld\n",
               getpid());
        check = pthread_create(
            &threads[tID],
            NULL,
            thread_print,
            (void *)tID);
        if (check != 0)
        {
            printf("ERROR!!! I'm Main Thread, can't create Thread #%ld ", tID);
            exit(-1);
        }
    }
    sleep(100);
    /* Last thing that main() should do */
    pthread_exit(NULL);
}
