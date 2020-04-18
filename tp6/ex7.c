#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PATH_NAME_SIZE 128

typedef struct copy_information
{
    char src[257];
    char dest_dir[PATH_NAME_SIZE];
    long byte_size;
} copy_information;

void *thread_function(void *inf)
{
    copy_information *cpy_inf = (copy_information *)inf;

    char buffer[cpy_inf->byte_size];

    size_t bytes_read = 0;

    int fd_read = open(cpy_inf->src, O_RDONLY);
    if (fd_read < 0)
        pthread_exit(0);

    int fd_write = open(cpy_inf->dest_dir, O_CREAT | O_TRUNC | O_WRONLY);
    if (fd_write < 0)
        pthread_exit(0);

    bytes_read = read(fd_read, buffer, cpy_inf->byte_size);

    if (bytes_read == cpy_inf->byte_size)
        write(fd_write, buffer, bytes_read);

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <src_dir> <dest_dir>\n", argv[0]);
        exit(1);
    }

    // Checks if the source directory exists
    DIR *src;
    if ((src = opendir(argv[1])) == NULL)
    {
        exit(2);
    }

    // Checks if the destiny directory exists, if not creates it.
    DIR *dest;
    if ((dest = opendir(argv[2])) == NULL)
    {
        mkdir(argv[2], 0700);
    }

    struct dirent *dirent;

    int index = 0;
    copy_information cpy_inf[512];

    while ((dirent = readdir(src)) != NULL)
    {
        char entry_name[257];

        sprintf(entry_name, "%s/%s", argv[1], dirent->d_name);

        struct stat stat;

        lstat(entry_name, &stat);

        if (S_ISREG(stat.st_mode))
        {
            char dest_name[257];

            sprintf(dest_name, "%s/%s", argv[2], dirent->d_name);

            strcpy(cpy_inf[index].src, entry_name);
            strcpy(cpy_inf[index].dest_dir, dest_name);

            cpy_inf[index].byte_size = stat.st_size;

            pthread_t tid;
            pthread_create(&tid, NULL, thread_function, &cpy_inf[index++]);
        }
    }

    pthread_exit(0);
}