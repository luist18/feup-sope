#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define MAX_LINE 256
#define FIFO "/tmp/fifo_chg"

int init_fifo(char *path, int perms);

int user_count = 0;

void alarm_handler(int signo)
{
    printf("Total user count: %d\n", user_count);
    unlink(FIFO);
    exit(0);
}

int main(void)
{
    int seconds = 30;

    int fifo = init_fifo(FIFO, 0660);

    alarm(seconds);

    struct sigaction action;
    action.sa_handler = alarm_handler;
    action.sa_flags = 0;
    sigfillset(&action.sa_mask);

    sigaction(SIGALRM, &action, NULL);

    while (1)
    {
        char line[MAX_LINE];

        int bytes_read = read(fifo, line, MAX_LINE);

        if (bytes_read > 0)
        {
            printf("The user %s has arrived\n", line);
            user_count++;
        }
    }
}

int init_fifo(char *path, int perms)
{
    if (mkfifo(path, perms) < 0)
    {
        perror("Can't create FIFO");
        exit(1);
    }
    else
    {
        printf("FIFO '%s' sucessfully created\n", path);
    }

    int fd;

    if ((fd = open(path, O_RDONLY)) != -1)
        printf("FIFO '%s' openned in READONLY mode\n", path);
    else
    {
        perror("Could not open FIFO");
        exit(2);
    }

    return fd;
}