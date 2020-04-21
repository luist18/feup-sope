#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define STDERR 2

int N = 200000; // Shared variable

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *thrfunc(void *arg)
{
    fprintf(stderr, "Starting thread %s\n", (char *)arg);
    void *ret = malloc(sizeof(int));

    int count = 0;

    while (1)
    {
        pthread_mutex_lock(&mut);

        if (N-- <= 0)
        {
            *(int *)ret = count;

            pthread_mutex_unlock(&mut);
            return ret;
        }

        pthread_mutex_unlock(&mut);

        write(STDERR, arg, 1);
        count++;
    }
}

int main()
{
    void *ra, *rb;

    pthread_t ta, tb;

    pthread_create(&ta, NULL, thrfunc, "1");
    pthread_create(&tb, NULL, thrfunc, "2");
    pthread_join(ta, &ra);
    pthread_join(tb, &rb);

    fprintf(stderr, "\nThe number of characters written by thread 1 were %d and by the thread 2 were %d (%d + %d = %d)\n", *(int *)ra, *(int *)rb, *(int *)ra, *(int *)rb, *(int *)ra + *(int *)rb);

    free(ra);
    free(rb);

    return 0;
}