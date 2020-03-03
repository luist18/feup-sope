#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    FILE *file_to_open = fopen(argv[1], "r");
    FILE *file_to_write = fopen(argv[2], "w");

    char buffer[BUFFER_SIZE];

    size_t bytes;
    while ((bytes = fread(buffer, 1, BUFFER_SIZE, file_to_open)) != 0)
        if (fwrite(buffer, 1, bytes, file_to_write) != bytes)
            return 1;

    fclose(file_to_open);

    fclose(file_to_write);

    return 0;
}
