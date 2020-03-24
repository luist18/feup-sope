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

        int numbers[2];

        scanf("%d %d", &numbers[0], &numbers[1]);

        close(pipe_fd[READ]);
        write(pipe_fd[WRITE], numbers, 2 * sizeof(int));
        close(pipe_fd[WRITE]);
    }
    else if (pid == 0)
    {
        int numbers[2];

        close(pipe_fd[WRITE]);

        read(pipe_fd[READ], numbers, 2 * sizeof(int));

        printf("Sum: %d\n", numbers[0] + numbers[1]);
        printf("Difference: %d\n", numbers[0] + numbers[1]);
        printf("Product: %d\n", numbers[0] * numbers[1]);

        if (!numbers[1])
            printf("Quocient: invalid\n");
        else
            printf("Quocient: %.2f\n", numbers[0] * 1.0 / numbers[1]);

        close(pipe_fd[READ]);
    }

    exit(0);
}