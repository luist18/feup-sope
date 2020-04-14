#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 10
void *PrintHello(void *threadnum)
{
    printf("Hello from thread no. %d!\n", *(int *)threadnum);
    pthread_exit(NULL);
}

/**
 * The exit() function can be called before the thread starts and so there is a chance that a message is not printed.
 * The pthread_exit() only closes the current thread and allows the other threads to access the global data before they end. 
 */
int main()
{
    pthread_t threads[NUM_THREADS];
    int t;
    for (t = 0; t < NUM_THREADS; t++)
    {
        printf("Creating thread %d\n", t);
        pthread_create(&threads[t], NULL, PrintHello, (void *)&t);
        //pthread_join(threads[t], NULL);
    }
    exit(0);
}