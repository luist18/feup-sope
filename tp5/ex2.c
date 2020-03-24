#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAXLINE 4096
#define READ 0
#define WRITE 1
#define INTEGER "d"
#define FLOAT "f"
#define INVALID "i"
#define TYPE_SIZE 2

int main(void)
{
    int pipe_fd[2];

    int extra_pipe_fd[2];

    if (pipe(pipe_fd) < 0)
    {
        fprintf(stderr, "Pipe error");
        exit(1);
    }

    if (pipe(extra_pipe_fd) < 0)
    {
        fprintf(stderr, "Pipe error");
        exit(1);
    }

    pid_t pid = fork();

    if (pid > 0)
    {
        printf("Digit two numbers: ");

        int numbers[2];

        scanf("%d %d", &numbers[0], &numbers[1]);

        close(pipe_fd[READ]);
        write(pipe_fd[WRITE], numbers, 2 * sizeof(int));
        close(pipe_fd[WRITE]);

        close(extra_pipe_fd[WRITE]);

        int sum, difference, product;

        float quocient;

        char type[TYPE_SIZE];

        read(extra_pipe_fd[READ], &sum, sizeof(sum));
        read(extra_pipe_fd[READ], &difference, sizeof(difference));
        read(extra_pipe_fd[READ], &product, sizeof(product));
        read(extra_pipe_fd[READ], &type, TYPE_SIZE);
        read(extra_pipe_fd[READ], &quocient, sizeof(quocient));

        close(extra_pipe_fd[READ]);

        printf("Sum: %d\n", sum);
        printf("Difference: %d\n", difference);
        printf("Product: %d\n", product);

        if (!strcmp(type, INVALID))
        {
            printf("Quocient: invalid\n");
        }
        else if (!strcmp(type, INTEGER))
        {
            printf("Quocient: %d\n", (int) quocient);
        }
        else if (!strcmp(type, FLOAT))
        {
            printf("Quocient: %.2f\n", quocient);
        }
    }
    else if (pid == 0)
    {
        int numbers[2];

        close(pipe_fd[WRITE]);

        read(pipe_fd[READ], numbers, 2 * sizeof(int));

        int sum = numbers[0] + numbers[1];
        int difference = numbers[0] - numbers[1];
        int product = numbers[0] * numbers[1];

        float quocient = numbers[0] * 1.0 / numbers[1];

        close(extra_pipe_fd[READ]);
        write(extra_pipe_fd[WRITE], &sum, sizeof(sum));
        write(extra_pipe_fd[WRITE], &difference, sizeof(difference));
        write(extra_pipe_fd[WRITE], &product, sizeof(product));

        char type[TYPE_SIZE];

        if (!numbers[1])
        {
            strcpy(type, INVALID);
        }
        else if (numbers[0] % numbers[1] == 0)
        {
            strcpy(type, INTEGER);
        }
        else
        {
            strcpy(type, FLOAT);
        }

        write(extra_pipe_fd[WRITE], &type, TYPE_SIZE);
        write(extra_pipe_fd[WRITE], &quocient, sizeof(quocient));
        close(extra_pipe_fd[WRITE]);

        close(pipe_fd[READ]);
    }

    exit(0);
}