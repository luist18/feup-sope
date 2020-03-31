#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define INTEGER "d"
#define FLOAT "f"
#define INVALID "i"
#define TYPE_SIZE 2
#define REQUEST_FIFO "/tmp/fifo_req"
#define ANSWER_FIFO "/tmp/fifo_ans"

int init_fifo(char *path, int perms);

int main(void)
{
    // 1. Initiliazes the FIFO
    int fd_ans = init_fifo(ANSWER_FIFO, 0660);

    // 2. Checks if the server is closed or not
    int fd_req = open("/tmp/fifo_req", O_WRONLY);
    if (fd_req == -1)
    {
        printf("The server is now closed!\n");
        close(fd_req);
        unlink(ANSWER_FIFO);
        exit(1);
    } else {
        close(fd_ans);
    }

    // 2. Reads the two numbers from the console
    printf("Digit two numbers: ");

    int numbers[2];

    scanf("%d %d", &numbers[0], &numbers[1]);

    // 3. Sends a request to the server
    write(fd_req, numbers, sizeof(int) * 2);

    // 4. Retrives the answer from the server and prints
    fd_ans = open(ANSWER_FIFO, O_RDONLY);

    int sum, difference, product;

    float quocient;

    char type[TYPE_SIZE];

    read(fd_ans, &sum, sizeof(sum));
    read(fd_ans, &difference, sizeof(difference));
    read(fd_ans, &product, sizeof(product));
    read(fd_ans, &type, TYPE_SIZE);
    read(fd_ans, &quocient, sizeof(quocient));

    printf("Sum: %d\n", sum);
    printf("Difference: %d\n", difference);
    printf("Product: %d\n", product);

    if (!strcmp(type, INVALID))
    {
        printf("Quocient: invalid\n");
    }
    else if (!strcmp(type, INTEGER))
    {
        printf("Quocient: %d\n", (int)quocient);
    }
    else if (!strcmp(type, FLOAT))
    {
        printf("Quocient: %.2f\n", quocient);
    }

    // X. Shutsdown the program
    close(fd_req);
    close(fd_ans);
    unlink(ANSWER_FIFO);
    exit(0);
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

    if ((fd = open(path, O_RDONLY | O_NONBLOCK)) != -1)
        printf("FIFO '%s' openned in READONLY mode\n", path);
    else
    {
        perror("Could not open FIFO");
        exit(2);
    }

    return fd;
}