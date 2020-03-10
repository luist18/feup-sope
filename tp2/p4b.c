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
    write(STDOUT_FILENO, "Press ENTER to exit\n\n", 21);

    char buffer[BUFFER_SIZE];

    int file_to_write = open("students.txt", O_CREAT | O_TRUNC | O_WRONLY);

    ssize_t size_name;

    char name[50], grade[50];
    while (write(STDOUT_FILENO, "Name: ", 6) > 0 && (size_name = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0 && buffer[0] != '\n')
    {
        strncpy(name, buffer, size_name - 1);
        
        write(STDOUT_FILENO, "Grade: ", 7);

        ssize_t size_grade = read(STDOUT_FILENO, buffer, BUFFER_SIZE);

        strncpy(grade, buffer, size_grade - 1);

        struct Student stud;
        strncpy(stud.name, name, strlen(name));
        stud.grade = atoi(grade);

        write(file_to_write, &stud, sizeof(stud));
    }

    close(file_to_write);
}