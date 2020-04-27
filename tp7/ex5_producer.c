#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "ex5_utils.c"

/**
 * This method creates a shared memory region
 * 
 * @param shared_mem a pointer to a shared_memory struct where
 * the shared memory region is going to be allocated
 * 
 * @return the file descriptor of the shared memory region
 */
int create_shared_memory_region(shared_memory *shared_mem);

/**
 * This method closes a shared memory region
 * 
 * @param shm the shared memory region
 */
void close_shared_memory_region(shared_memory *shm);

/**
 * This method creates all the semaphores
 * 
 * @param sem_notfull the not full semaphore pointer
 * @param sem_notemtpy the not empty semaphore pointer
 * @param sem_mutex the mutex semaphore pointer
 * @param max_size the max size for the not full semaphore
 */
void create_semaphores(sem_t *sem_notfull, sem_t *sem_notempty, sem_t *sem_mutex, int max_size);

/**
 * This method closes all the semaphores
 * 
 * @param sem_notfull the not full semaphore pointer
 * @param sem_notemtpy the not empty semaphore pointer
 * @param sem_mutex the mutex semaphore pointer
 */
void close_semaphores(sem_t *sem_notfull, sem_t *sem_notempty, sem_t *sem_mutex);

int main(int argc, char *argv[])
{
    // Argument check
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <number of items>\n", argv[0]);
        exit(1);
    }

    int number_of_items = atoi(argv[1]);

    // Check for argument type
    if (number_of_items == 0)
    {
        fprintf(stderr, "%s is not a valid number...\n", argv[1]);
        exit(2);
    }

    // Create shared memory region
    shared_memory shared_mem;
    int shm_fd = create_shared_memory_region(&shared_mem);

    shared_mem.number_of_items = number_of_items;

    // Create semaphores
    sem_t sem_notfull;
    sem_t sem_notempty;
    sem_t sem_mutex;

    create_semaphores(&sem_notfull, &sem_notempty, &sem_mutex, number_of_items);

    // Producing items
    for (int item = 0; item < number_of_items; ++item)
    {
        sem_wait(&sem_notfull);

        sem_wait(&sem_mutex);

        int index;
        sem_getvalue(&sem_notempty, &index);

        int current_item = rand() % (number_of_items + 1);
        shared_mem.stock[index] = current_item;

        sem_post(&sem_mutex);

        sem_post(&sem_notempty);

        printf("Produced %d at %d\n", current_item, index);
    }

    // Delete semaphores
    close_semaphores(&sem_notfull, &sem_notempty, &sem_mutex);

    // Delete shared memory region
    close_shared_memory_region(&shared_mem);

    exit(0);
}

int create_shared_memory_region(shared_memory *shared_mem)
{
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDONLY, 0700);

    if (shm_fd < 0)
    {
        perror("Could not create the shared memory region");
        exit(3);
    }

    if (ftruncate(shm_fd, sizeof(shared_memory)))
    {
        perror("Could not truncate the shared memory region");
        exit(4);
    }

    shared_mem = mmap(0, sizeof(shared_memory), PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shared_mem == MAP_FAILED)
    {
        perror("Could not map the shared memory region");
        exit(5);
    }

    return shm_fd;
}

void close_shared_memory_region(shared_memory *shm)
{
    if (munmap(shm, sizeof(shared_memory)) < 0)
    {
        perror("WRITER failure in munmap()");
        exit(6);
    }
    if (shm_unlink(SHM_NAME) < 0)
    {
        perror("WRITER failure in shm_unlink()");
        exit(7);
    }
}

void create_semaphores(sem_t *sem_notfull, sem_t *sem_notempty, sem_t *sem_mutex, int max_size)
{
    sem_notfull = sem_open(SEM_NOT_FULL_NAME, O_CREAT, 0700, max_size);
    if (sem_notfull == SEM_FAILED)
    {
        perror("Could not create the not full semaphore");
        exit(8);
    }

    sem_notempty = sem_open(SEM_NOT_EMPTY_NAME, O_CREAT, 0700, 0);
    if (sem_notempty == SEM_FAILED)
    {
        perror("Could not create the not empty semaphore");
        exit(8);
    }

    sem_mutex = sem_open(MUTEX_NAME, O_CREAT, 0700, 1);
    if (sem_mutex == SEM_FAILED)
    {
        perror("Could not create the mutex semaphore");
        exit(8);
    }
}

void close_semaphores(sem_t *sem_notfull, sem_t *sem_notempty, sem_t *sem_mutex)
{
    if (!sem_close(sem_notfull))
    {

        if (sem_unlink(SEM_NOT_FULL_NAME))
            perror("Could not unlink not full semaphore");
    }
    else
        perror("Could not close not full semaphore");

    if (!sem_close(sem_notempty))
    {
        if (sem_unlink(SEM_NOT_EMPTY_NAME))
            perror("Could not unlink not full semaphore");
    }
    else
        perror("Could not close not empty semaphore");

    if (!sem_close(sem_mutex))
    {
        if (sem_unlink(MUTEX_NAME))
            perror("Could not unlink not full semaphore");
    }
    else
        perror("Could not close mutex semaphore");
}