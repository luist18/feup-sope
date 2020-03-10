#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    int status;
    if (pid > 0) {
        // Parent
        waitpid(pid, &status, 0);
        write(STDOUT_FILENO, "world!\n", 7);
    } else {
        // Child
        write(STDOUT_FILENO, "Hello ", 6);
    }

    return 0;
}