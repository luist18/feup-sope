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
    int fd_req = init_fifo(REQUEST_FIFO, 0660);

    // 2. Waits for a client
    int fd_ans;
    do
    {
        fd_ans = open(ANSWER_FIFO, O_WRONLY);

        if (fd_ans == -1)
        {
            printf("Waiting for a client...\n");
            sleep(1);
        }

    } while (fd_ans == -1);

    // 3. Retrives the data from the client
    int numbers[2];

    read(fd_req, numbers, 2 * sizeof(int));
    printf("Data recieved. Computing data...\n");

    // 4. Computes the values

    int sum = numbers[0] + numbers[1];
    int difference = numbers[0] - numbers[1];
    int product = numbers[0] * numbers[1];

    float quocient = numbers[0] * 1.0 / numbers[1];

    write(fd_ans, &sum, sizeof(sum));
    write(fd_ans, &difference, sizeof(difference));
    write(fd_ans, &product, sizeof(product));

    char type[TYPE_SIZE];

    if (!numbers[1])
    {
        strcpy(type, INVALID);
    }
    else if (numbers[0] % numbers[1] == 0)
    {
        strcpy(type, INTEGER);
    }
    else
    {
        strcpy(type, FLOAT);
    }

    write(fd_ans, &type, TYPE_SIZE);
    write(fd_ans, &quocient, sizeof(quocient));

    printf("Data computed and sent...\n");

    // X. Shutsdown the program
    close(fd_ans);
    close(fd_req);
    unlink(REQUEST_FIFO);
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