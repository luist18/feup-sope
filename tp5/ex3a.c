#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s\n", argv[0]);
        exit(1);
    }

    int pipe_fd[2];

    if (pipe(pipe_fd) < 0)
    {
        fprintf(stderr, "Could not create pipe\n");
        exit(1);
    }

    int fd_open = open(argv[1], O_RDONLY);

    if (fd_open < 0)
    {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        exit(1);
    }

    char buffer[BUFFER_SIZE];

    int bytes_read = read(fd_open, buffer, BUFFER_SIZE);

    if (bytes_read < 1)
        exit(1);

    write(pipe_fd[WRITE], buffer, bytes_read);
    close(pipe_fd[WRITE]);

    dup2(pipe_fd[READ], STDIN_FILENO);
    close(pipe_fd[READ]);

    execlp("sort", "sort", NULL);

    exit(0);
}