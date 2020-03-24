#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAXLINE 4096
#define READ 0
#define WRITE 1

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

        char first[MAXLINE], second[MAXLINE];

        scanf("%s %s", first, second);

        close(pipe_fd[READ]);
        write(pipe_fd[WRITE], first, MAXLINE);
        write(pipe_fd[WRITE], second, MAXLINE);
        close(pipe_fd[WRITE]);
    }
    else if (pid == 0)
    {
        char first_string[MAXLINE], second_string[MAXLINE];

        close(pipe_fd[WRITE]);

        read(pipe_fd[READ], first_string, MAXLINE);
        read(pipe_fd[READ], second_string, MAXLINE);

        int first, second;

        first = atoi(first_string);
        second = atoi(second_string);

        printf("Sum: %d\n", first + second);
        printf("Difference: %d\n", first + second);
        printf("Product: %d\n", first * second);

        if (!second)
            printf("Quocient: invalid\n");
        else
            printf("Quocient: %.2f\n", first * 1.0 / second);

        close(pipe_fd[READ]);
    }

    exit(0);
}