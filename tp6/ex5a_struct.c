#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct container {
    int first;
    int second;
};

void *sum_thread(void *arg)
{
    struct container numbers = *(struct container *)arg;

    int sum = numbers.first + numbers.second;

    printf("Sum: %d\n", sum);

    return NULL;
}

void *sub_thread(void *arg)
{
    struct container numbers = *(struct container *)arg;

    int sub = numbers.first - numbers.second;

    printf("Subtraction: %d\n", sub);

    return NULL;
}

void *product_thread(void *arg)
{
    struct container numbers = *(struct container *)arg;

    int product = numbers.first * numbers.second;

    printf("Product: %d\n", product);

    return NULL;
}

void *quocient_thread(void *arg)
{
    struct container numbers = *(struct container *)arg;

    float quocient = numbers.first * 1.0f / numbers.second;

    printf("Quocient: %.2f\n", quocient);

    return NULL;
}

int main(void)
{
    struct container numbers;

    printf("Type two numbers: ");
    scanf("%d %d", &numbers.first, &numbers.second);

    pthread_create(NULL, NULL, sum_thread, &numbers);
    pthread_create(NULL, NULL, sub_thread, &numbers);
    pthread_create(NULL, NULL, product_thread, &numbers);
    pthread_create(NULL, NULL, quocient_thread, &numbers);

    pthread_exit(0);
}