#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <ls_arg> <grep_arg>\n", argv[0]);
        exit(1);
    }

    int pipe_fd[2];

    if (pipe(pipe_fd) < 0)
    {
        fprintf(stderr, "Could not create pipe\n");
        exit(1);
    }

    pid_t pid = fork();

    if (pid > 0)
    {
        close(pipe_fd[READ]);

        dup2(pipe_fd[WRITE], STDOUT_FILENO);

        execlp("ls", "ls", argv[1], "-laR", NULL);
        exit(2);
    }
    else if (pid == 0)
    {
        close(pipe_fd[WRITE]);

        int pipe_fd2[2];

        if (pipe(pipe_fd2) < 0)
        {
            fprintf(stderr, "Could not create pipe\n");
            exit(1);
        }

        pid_t pid2 = fork();

        if (pid2 > 0)
        {
            close(pipe_fd2[READ]);

            dup2(pipe_fd[READ], STDIN_FILENO);
            dup2(pipe_fd2[WRITE], STDOUT_FILENO);

            execlp("grep", "grep", argv[2], NULL);
            exit(3);
        }
        else if (pid == 0)
        {
            close(pipe_fd2[WRITE]);

            dup2(pipe_fd2[READ], STDIN_FILENO);

            execlp("sort", "sort", NULL);
            exit(4);
        }
    }

    exit(0);
}