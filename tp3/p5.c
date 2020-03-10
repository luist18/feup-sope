#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pidp = fork();

    int statusp;

    if (pidp > 0){
        // Parent
        waitpid(pidp, &statusp, 0);
        write(STDOUT_FILENO, "friends!\n", 9);
    } else {
        // Child
        pid_t pidc = fork();

        int statusc;

        if (pidc > 0) {
            // Parent
            waitpid(pidc, &statusc, 0);
            write(STDOUT_FILENO, " my ", 4);
        } else {
            // Child
            write(STDOUT_FILENO, "Hello", 5);
        }
    }
}