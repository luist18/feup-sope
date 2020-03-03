#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <source>\n", argv[0]);
        return 1;
    }

    int file_to_open = open(argv[1], O_RDONLY);

    if (file_to_open == -1) {
        perror(argv[1]);
        return -1;
    }

    ssize_t bytes, bytes_to_write;

    char buffer[BUFFER_SIZE];
    
    while ((bytes = read(file_to_open, buffer, BUFFER_SIZE)) > 0) {
        if ((bytes_to_write = write(STDOUT_FILENO, buffer, bytes)) <= 0 || bytes != bytes_to_write) {
            perror(argv[2]);
            close(file_to_open);
            return -1;
        }
    }

    close(file_to_open);
    
    return 0;
}
