#define STOCK_SIZE 5

/**
 * The name of the shared memory region of the stock
 */
const char SHM_NAME[] = "/shm_stock";

/**
 * The name of the not full semaphore
 */
const char SEM_NOT_FULL_NAME[] = "/sem_notfull";

/**
 * The name of the not empty semaphore
 */
const char SEM_NOT_EMPTY_NAME[] = "/sem_notempty";

/**
 * The name of the mutex
 */
const char MUTEX_NAME[] = "/mutex";

/**
 * A structure to represent the shared memory container. Later more fields may be added
 */
typedef struct shared_memory
{
    /**
     * The array containing the stock
     */
    int stock[STOCK_SIZE];

    /**
     * The number of items to produce and consume.
     */
    int number_of_items;
} shared_memory;
