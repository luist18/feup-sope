#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct string_pathname
{
    char string[512];
    char pathname[512];
} string_pathname;

int get_line(char *string, char *pathname);

void *thread_function(void *path_struct)
{
    string_pathname *info = (string_pathname *)path_struct;

    void *result = malloc(sizeof(int));

    int line = get_line(info->string, info->pathname);

    *(int *)result = line;

    return result;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <string> <pathname...>\n", argv[0]);
        exit(1);
    }

    int files = argc - 2;

    string_pathname info[files];
    pthread_t tids[files];

    for (int i = 0; i < files; i++)
    {
        strcpy(info[i].string, argv[1]);
        strcpy(info[i].pathname, argv[i + 2]);

        pthread_create(&tids[i], NULL, thread_function, (void *)&info[i]);
    }

    void *results[files];

    for (int i = 0; i < files; i++)
    {
        pthread_join(tids[i], &results[i]);

        int result = *(int *)results[i];

        free(results[i]);

        if (result != -1)
            printf("Found %s at line %d in file %s\n", info[i].string, result, info[i].pathname);
        else
            printf("Could not find that string in file %s...\n", info[i].pathname);
    }

    exit(0);
}

int get_line(char *string, char *pathname)
{
    FILE *file;

    if ((file = fopen(pathname, "r")) == NULL)
    {
        fprintf(stderr, "Could not open the file %s\n", pathname);
        exit(2);
    }

    char *line;
    size_t size;

    int current_line = 1;

    int read;
    while ((read = getline(&line, &size, file)) != -1)
    {
        if (strstr(line, string) != NULL)
        {
            fclose(file);
            return current_line;
        }

        current_line++;
    }

    fclose(file);

    return -1;
}