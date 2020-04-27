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
 * This method opens a shared memory region
 * 
 * @param shared_mem a pointer to a shared_memory container
 * 
 * @return the file descriptor of the shared memory region
 */
int open_shared_memory_region(shared_memory *shared_mem);

void open_semaphores();

void close_semaphores();

int main()
{
    // Open shared memory region
    shared_memory shared_mem;
    int shm_fd = open_shared_memory_region(&shared_mem);

    // Open semaphores
    sem_t sem_notfull;
    sem_t sem_notempty;
    sem_t sem_mutex;

    open_semaphores(&sem_notfull, &sem_notempty, &sem_mutex);

    // Consuming items
    int number_of_items = shared_mem.number_of_items;

    for (int item = 0; item < number_of_items; ++item)
    {
        sem_wait(&sem_notempty);

        sem_wait(&sem_mutex);

        int index;
        index = sem_getvalue(&sem_notempty, &index);

        int current_item = shared_mem.stock[index];

        sem_post(&sem_mutex);

        sem_post(&sem_notfull);

        printf("Read %d from %d\n", current_item, index);
    }

    // Close sempahores
    close_semaphores(&sem_notfull, &sem_notempty, &sem_mutex);
}

int open_shared_memory_region(shared_memory *shared_mem)
{
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0700);
    if (shm_fd < 0)
    {
        perror("Could not open the shared memory region");
    }

    shared_mem = mmap(0, sizeof(shared_memory), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_mem == MAP_FAILED)
    {
        perror("Could not map the shared memory region");
        exit(1);
    }

    return shm_fd;
}

void open_semaphores(sem_t *sem_notfull, sem_t *sem_notempty, sem_t *sem_mutex)
{
    sem_notfull = sem_open(SEM_NOT_FULL_NAME, 0);
    if (sem_notfull == SEM_FAILED)
    {
        perror("Could not open the not full semaphore");
        exit(8);
    }

    sem_notempty = sem_open(SEM_NOT_EMPTY_NAME, 0);
    if (sem_notempty == SEM_FAILED)
    {
        perror("Could not open the not empty semaphore");
        exit(8);
    }

    sem_mutex = sem_open(MUTEX_NAME, 0);
    if (sem_mutex == SEM_FAILED)
    {
        perror("Could not open the mutex semaphore");
        exit(8);
    }
}

void close_semaphores(sem_t *sem_notfull, sem_t *sem_notempty, sem_t *sem_mutex)
{
    if (sem_close(sem_notfull))
        perror("Could not close the not full semaphore");

    if (sem_close(sem_notempty))
        perror("Could not close the not empty semaphore");

    if (sem_close(sem_mutex))
        perror("Could not close the mutex semaphore");
}