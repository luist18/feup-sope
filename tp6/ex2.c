#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define STDERR 2

int N = 50000;

void *thrfunc(void *arg)
{
    fprintf(stderr, "Starting thread %s\n", (char *)arg);
    void *ret = malloc(sizeof(int));

    int count = 0;

    while (N-- > 0)
    {
        write(STDERR, arg, 1);
        count++;
    }

    *(int *)ret = count;

    return ret;
}
int main()
{
    void *ra, *rb;

    pthread_t ta, tb;

    pthread_create(&ta, NULL, thrfunc, "1");
    pthread_create(&tb, NULL, thrfunc, "2");
    pthread_join(ta, &ra);
    pthread_join(tb, &rb);

    free(ra);
    free(rb);

    fprintf(stderr, "\nThe number of characters written by thread 1 were %d and by the thread 2 were %d (%d + %d = %d)\n", *(int *)ra, *(int *)rb, *(int *)ra, *(int *)rb, *(int *)ra + *(int *)rb);
    return 0;
}