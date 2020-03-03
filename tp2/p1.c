#include <termios.h>
#include <unistd.h>
#include <string.h>

#define MAX_PASSWD_LEN 32

int main(void)
{
    struct termios backup, new;
    char password[MAX_PASSWD_LEN + 1];
    int index = 0;

    tcgetattr(STDIN_FILENO, &backup);

    new = backup;
    new.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);

    write(STDOUT_FILENO, "\nType a password: ", 18);

    char ch;
    
    while (index < MAX_PASSWD_LEN && read(STDIN_FILENO, &ch, 1) && ch != '\n')
    {
        password[index++] = ch;
        write(STDOUT_FILENO, "*", 1);
    }

    password[index] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &backup);

    write(STDOUT_FILENO, "\nThe password that you typed was: ", 34);
    write(STDOUT_FILENO, password, strlen(password));
    write(STDOUT_FILENO, "\n", 1);

    return 0;
}
