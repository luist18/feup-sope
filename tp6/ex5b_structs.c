#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct container
{
    int first;
    int second;
};

void *sum_thread(void *arg)
{
    struct container numbers = *(struct container *)arg;

    void *sum = malloc(sizeof(int));

    *(int *)sum = numbers.first + numbers.second;

    return sum;
}

void *sub_thread(void *arg)
{
    struct container numbers = *(struct container *)arg;

    void *sub = malloc(sizeof(int));

    *(int *)sub = numbers.first - numbers.second;

    return sub;
}

void *product_thread(void *arg)
{
    struct container numbers = *(struct container *)arg;

    void *product = malloc(sizeof(int));

    *(int *)product = numbers.first * numbers.second;

    return product;
}

void *quocient_thread(void *arg)
{
    struct container numbers = *(struct container *)arg;

    void *quocient = malloc(sizeof(float));

    *(float *)quocient = numbers.first * 1.0f / numbers.second;

    return NULL;
}

int main(void)
{
    struct container numbers;

    printf("Type two numbers: ");
    scanf("%d %d", &numbers.first, &numbers.second);

    pthread_t tid1, tid2, tid3, tid4;

    void *r1, *r2, *r3, *r4;

    pthread_create(&tid1, NULL, sum_thread, &numbers);
    pthread_create(&tid2, NULL, sub_thread, &numbers);
    pthread_create(&tid3, NULL, product_thread, &numbers);
    pthread_create(&tid4, NULL, quocient_thread, &numbers);

    phtread_join(tid1, r1);
    phtread_join(tid2, r2);
    phtread_join(tid3, r3);
    phtread_join(tid4, r4);

    printf("Sum: %d\n", *(int *)r1);
    printf("Subtraction: %d\n", *(int *)r2);
    printf("Product: %d\n", *(int *)r3);
    printf("Quocient: %.2f\n", *(int *)r4);

    free(r1);
    free(r2);
    free(r3);
    free(r4);

    pthread_exit(0);
}