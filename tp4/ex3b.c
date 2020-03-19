#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

int d = 1;

void usr_handler(int signo)
{
    if (signo == SIGUSR1)
        d = 1;
    else if (signo == SIGUSR2)
        d = -1;
}

int main(void)
{
    time_t t;

    srand((unsigned) time(&t));

    struct sigaction action;
    action.sa_handler = usr_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGUSR1, &action, NULL) < 0)
    {
        fprintf(stderr, "Unable to install SIGUSR1 handler\n");
        exit(1);
    }
    if (sigaction(SIGUSR2, &action, NULL) < 0)
    {
        fprintf(stderr, "Unable to install SIGUSR2 handler\n");
        exit(1);
    }

    pid_t pid = fork();

    int v = 0;

    for (int i = 0; i < 5; ++i)
    {
        if (pid == 0)
        {
            printf("v = %d\n", v);

            v += d;
        }
        else
        {
            int random = (int) rand() % 2;

            int signal = random ? SIGUSR1 : SIGUSR2;

            kill(pid, signal);
        }

        sleep(1);
    }

    if (pid > 0) 
        wait(NULL);

    exit(0);
}