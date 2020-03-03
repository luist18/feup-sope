#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 512

struct Student
{
    char name[50];
    int grade;
};

int main(void)
{

    int file_to_read = open("students.txt", O_RDONLY);

    struct Student stud;
    while (read(file_to_read, &stud, sizeof(stud)) > 0)
    {
        write(STDOUT_FILENO, stud.name, strlen(stud.name));
        write(STDOUT_FILENO, ": ", 2);

        char grade[50];
        sprintf(grade, "%d", stud.grade);

        write(STDOUT_FILENO, grade, strlen(grade));
        write(STDOUT_FILENO, "\n", 1);
    }

    close(file_to_read);

    return 0;
}