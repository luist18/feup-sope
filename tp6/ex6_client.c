#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define INTEGER 1
#define FLOAT 2
#define INVALID 3
#define ANSWER_FIFO "/tmp/fifo_ans"

struct query
{
    int first_operand;
    int second_operand;
    char fifo_ans[64];
};

struct response
{
    int sum;
    int subtraction;
    int product;
    int quocient_op;
    float quocient;
};

int init_fifo(char *path, int perms);

int main(void)
{
    // 1. Initiliazes the FIFO
    char fifo_ans[64];
    sprintf(fifo_ans, "%s_%d", ANSWER_FIFO, getpid());
    int fd_ans = init_fifo(fifo_ans, 0660);

    // 2. Checks if the server is closed or not
    int fd_req = open("/tmp/fifo_req", O_WRONLY);
    if (fd_req == -1)
    {
        printf("The server is now closed!\n");
        close(fd_req);
        unlink(fifo_ans);
        exit(1);
    }
    else
    {
        close(fd_ans);
    }

    // 2. Reads the two numbers from the console
    printf("Digit two numbers: ");

    struct query req_query;

    strcpy(req_query.fifo_ans, fifo_ans);

    scanf("%d %d", &req_query.first_operand, &req_query.second_operand);

    // 3. Sends a request to the server
    write(fd_req, &req_query, sizeof(req_query));

    // 4. Retrives the answer from the server and prints
    fd_ans = open(fifo_ans, O_RDONLY);

    struct response ans_response;

    read(fd_ans, &ans_response.sum, sizeof(ans_response.sum));
    read(fd_ans, &ans_response.subtraction, sizeof(ans_response.subtraction));
    read(fd_ans, &ans_response.product, sizeof(ans_response.product));
    read(fd_ans, &ans_response.quocient_op, sizeof(ans_response.quocient_op));
    read(fd_ans, &ans_response.quocient, sizeof(ans_response.quocient));

    printf("Sum: %d\n", ans_response.sum);
    printf("Subtraction: %d\n", ans_response.subtraction);
    printf("Product: %d\n", ans_response.product);

    if (ans_response.quocient_op == INVALID)
    {
        printf("Quocient: invalid\n");
    }
    else if (ans_response.quocient_op == INTEGER)
    {
        printf("Quocient: %d\n", (int)ans_response.quocient);
    }
    else if (ans_response.quocient_op == FLOAT)
    {
        printf("Quocient: %.2f\n", ans_response.quocient);
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