#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define FIFO "/tmp/fifo_chg"

int main(int argc, char *argv[])
{
    if (argc != 2){
        printf("Usage: %s <username>\n", argv[0]);
        exit(1);
    }

    int fd_sv = open(FIFO, O_WRONLY);
    if (fd_sv == -1)
    {
        printf("The server is currently closed!\n");
        exit(1);
    }

    write(fd_sv, argv[1], strlen(argv[1]));

    close(fd_sv);
    exit(0);
}