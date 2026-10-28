#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int p1[2], p2[2], i;
    pid_t pid;

    if (pipe(p1) < 0 || pipe(p2) < 0)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        dup2(p1[1], STDOUT_FILENO);
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
        execlp("ls", "ls", NULL);
        exit(127);
    }
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        dup2(p1[0], STDIN_FILENO);
        dup2(p2[1], STDOUT_FILENO);
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
        execlp("sort", "sort", "-r", NULL);
        exit(127);
    }
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        dup2(p2[0], STDIN_FILENO);
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
        execlp("head", "head", "-n", "2", NULL);
        exit(127);
    }
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    for (i = 0; i < 3; i++)
        wait(NULL);
    return 0;
}
