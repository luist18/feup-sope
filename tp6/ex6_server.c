#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define INTEGER 1
#define FLOAT 2
#define INVALID 3
#define REQUEST_FIFO "/tmp/fifo_req"

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

struct query_response
{
    struct query *q;
    struct response *r;
};

void *sum_thread(void *arg)
{
    struct query_response *q_r = (struct query_response *)arg;

    q_r->r->sum = q_r->q->first_operand + q_r->q->second_operand;

    return NULL;
}

void *sub_thread(void *arg)
{
    struct query_response *q_r = (struct query_response *)arg;

    q_r->r->subtraction = q_r->q->first_operand - q_r->q->second_operand;

    return NULL;
}

void *product_thread(void *arg)
{
    struct query_response *q_r = (struct query_response *)arg;

    q_r->r->product = q_r->q->first_operand * q_r->q->second_operand;

    return NULL;
}

void *quocient_thread(void *arg)
{
    struct query_response *q_r = (struct query_response *)arg;

    if (!q_r->q->second_operand)
        q_r->r->quocient_op = INVALID;
    else if (q_r->q->first_operand % q_r->q->second_operand == 0)
        q_r->r->quocient_op = INTEGER;
    else
        q_r->r->quocient_op = FLOAT;

    if (!q_r->q->second_operand)
        q_r->r->quocient = 0;
    else
        q_r->r->quocient = q_r->q->first_operand * 1.0 / q_r->q->second_operand;

    return NULL;
}

int init_fifo(char *path, int perms);

int main(void)
{
    unlink(REQUEST_FIFO);

    // 1. Initiliazes the FIFO
    int fd_req = init_fifo(REQUEST_FIFO, 0660);

    // 2. Retrives the data from the client
    struct query req_query;

    struct response ans_response;

    read(fd_req, &req_query, sizeof(req_query));
    printf("Data recieved. Computing data...\n");

    // 3. Checks if the client is accepting the result
    int fd_ans = open(req_query.fifo_ans, O_WRONLY);
    if (fd_ans == -1)
    {
        printf("The server is now closed!\n");
        close(fd_req);
        unlink(req_query.fifo_ans);
        exit(1);
    }
    else
    {
        close(fd_req);
    }

    // 4. Computes the values
    struct query_response q_r = {&req_query, &ans_response};

    pthread_t tid1, tid2, tid3, tid4;

    pthread_create(&tid1, NULL, sum_thread, &q_r);
    pthread_create(&tid2, NULL, sub_thread, &q_r);
    pthread_create(&tid3, NULL, product_thread, &q_r);
    pthread_create(&tid4, NULL, quocient_thread, &q_r);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    write(fd_ans, &ans_response, sizeof(ans_response));

    printf("Data computed and sent...\n");

    // X. Shutsdown the program
    close(fd_ans);
    close(fd_req);
    unlink(REQUEST_FIFO);
    unlink(req_query.fifo_ans);
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

    if ((fd = open(path, O_RDONLY)) != -1)
        printf("FIFO '%s' openned in READONLY mode\n", path);
    else
    {
        perror("Could not open FIFO");
        exit(2);
    }

    return fd;
}