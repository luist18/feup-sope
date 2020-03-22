#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TIMES 5
#define DELAY 5

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <message>\n", argv[0]);
        exit(0);
    }

    char *message = argv[1];

    printf("%s\n", message);

    for (int i = 0; i < TIMES - 1; ++i)
    {
        sleep(DELAY);
        printf("%s\n", message);
    }

    exit(0);
}