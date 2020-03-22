#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

pid_t pid;

void alarm_handler(int signo)
{
    kill(pid, SIGKILL);
}

int main(int argc, char *argv[])
{
    int timeout;

    if (argc < 3 || (timeout = atoi(argv[1])) == 0)
    {
        printf("Usage: %s <int> <program> <program args>\n", argv[0]);
        exit(1);
    }

    pid = fork();

    if (pid == 0)
    {
        execv(argv[2], &argv[2]);
    }
    else
    {
        struct sigaction action;
        action.sa_handler = alarm_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;

        if (sigaction(SIGALRM, &action, NULL) < 0)
        {
            fprintf(stderr, "Unable to install alarm handler\n");
            exit(1);
        }

        alarm(timeout);

        int status;
        wait(&status);
        if (WIFEXITED(status))
            printf("The child ended normally\n");
        else
            printf("The child did not end normally\n");
    }

    exit(0);
}