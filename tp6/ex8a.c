#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_line(char *string, char *pathname);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <string> <pathname>\n", argv[0]);
        exit(1);
    }

    int line = get_line(argv[1], argv[2]);

    if (line != -1)
        printf("Found %s at line %d\n", argv[1], line);
    else
        printf("Could not find that string...\n");

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