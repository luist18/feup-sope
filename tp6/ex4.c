#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER_OF_THREADS 10
#define SLEEP_TIME 1

void *thread_function(void *thread_number)
{
    pthread_t tid = pthread_self();

    printf("TID of current thread: %ld\n", (long)tid);

    sleep(SLEEP_TIME);

    void *return_value = malloc(sizeof(int));
    *(int *)return_value = *(int *)thread_number;
    return return_value;
}

int main(void)
{
    pthread_t tids[NUMBER_OF_THREADS];

    int args[NUMBER_OF_THREADS];
    for (int i = 0; i < NUMBER_OF_THREADS; ++i)
    {
        args[i] = i;
        pthread_create(&tids[i], NULL, thread_function, &args[i]);
    }

    void *threads_number[NUMBER_OF_THREADS];

    for (int i = 0; i < NUMBER_OF_THREADS; ++i)
    {
        pthread_join(tids[i], &threads_number[i]);
        printf("Thread number: %d\n", *(int *)threads_number[i]);

        free(threads_number[i]);
    }

    pthread_exit(0);
}