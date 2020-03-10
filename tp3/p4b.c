#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();
    
    if (pid > 0) {
        // Parent
        write(STDOUT_FILENO, "Hello ", 6);
    } else {
        // Child
        write(STDOUT_FILENO, "world!\n", 7);
    }

    return 0;
}