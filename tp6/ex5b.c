#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *sum_thread(void *arg)
{
    int *numbers = (int *)arg;

    int sum = *numbers + *(numbers + 1);

    printf("Sum: %d\n", sum);

    return NULL;
}

void *sub_thread(void *arg)
{
    int *numbers = (int *)arg;

    int sub = *numbers - *(numbers + 1);

    printf("Subtraction: %d\n", sub);

    return NULL;
}

void *product_thread(void *arg)
{
    int *numbers = (int *)arg;

    int product = *numbers * *(numbers + 1);

    printf("Product: %d\n", product);

    return NULL;
}

void *quocient_thread(void *arg)
{
    int *numbers = (int *)arg;

    float quocient = *numbers * 1.0f / *(numbers + 1);

    printf("Quocient: %.2f\n", quocient);

    return NULL;
}

int main(void)
{
    int numbers[2];

    printf("Type two numbers: ");
    scanf("%d %d", &numbers[0], &numbers[1]);

    pthread_create(NULL, NULL, sum_thread, &numbers);
    pthread_create(NULL, NULL, sub_thread, &numbers);
    pthread_create(NULL, NULL, product_thread, &numbers);
    pthread_create(NULL, NULL, quocient_thread, &numbers);

    pthread_exit(0);
}