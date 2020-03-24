#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAXLINE 4096
#define READ 0
#define WRITE 1

struct numbers_struct
{
    int first, second;
};

int main(void)
{
    int pipe_fd[2];

    if (pipe(pipe_fd) < 0)
    {
        fprintf(stderr, "Pipe error");
        exit(1);
    }

    pid_t pid = fork();

    if (pid > 0)
    {
        printf("Digit two numbers: ");

        struct numbers_struct numbers;

        scanf("%d %d", &numbers.first, &numbers.second);

        close(pipe_fd[READ]);
        write(pipe_fd[WRITE], &numbers, sizeof(numbers));
        close(pipe_fd[WRITE]);
    }
    else if (pid == 0)
    {
        struct numbers_struct numbers;

        close(pipe_fd[WRITE]);

        read(pipe_fd[READ], &numbers, sizeof(numbers));

        printf("Sum: %d\n", numbers.first + numbers.second);
        printf("Difference: %d\n", numbers.first + numbers.second);
        printf("Product: %d\n", numbers.first * numbers.second);

        if (!numbers.second)
            printf("Quocient: invalid\n");
        else
            printf("Quocient: %.2f\n", numbers.first * 1.0 / numbers.second);

        close(pipe_fd[READ]);
    }

    exit(0);
}