#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int p[2];
    pid_t a, b;

    if (pipe(p) < 0)
        return 1;
    a = fork();
    if (a < 0)
        return 1;
    if (a == 0) {
        dup2(p[1], STDOUT_FILENO);
        close(p[0]);
        close(p[1]);
        execlp("ls", "ls", NULL);
        exit(127);
    }
    b = fork();
    if (b < 0)
        return 1;
    if (b == 0) {
        dup2(p[0], STDIN_FILENO);
        close(p[0]);
        close(p[1]);
        execlp("tr", "tr", "a-z", "A-Z", NULL);
        exit(127);
    }
    close(p[0]);
    close(p[1]);
    wait(NULL);
    wait(NULL);
    return 0;
}
