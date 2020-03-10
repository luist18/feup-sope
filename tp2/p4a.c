#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 512

int main(void)
{
    write(STDOUT_FILENO, "Para terminar a execucao pressione ENTER\n\n", 42);

    char buffer[BUFFER_SIZE];

    int file_to_write = open("students.txt", O_CREAT | O_TRUNC | O_WRONLY);

    ssize_t size_name;

    while (write(STDOUT_FILENO, "Aluno: ", 7) > 0 && (size_name = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0 && buffer[0] != '\n')
    {
        write(file_to_write, buffer, size_name - 1);

        write(STDOUT_FILENO, "Classificacao: ", 15);

        memset(buffer, 0, BUFFER_SIZE);

        ssize_t size_grade = read(STDOUT_FILENO, buffer, BUFFER_SIZE);

        write(file_to_write, ": ", 2);
        write(file_to_write, buffer, size_grade - 1);
        write(file_to_write, "\n", 1);

        memset(buffer, 0, BUFFER_SIZE);
    }

    close(file_to_write);
}