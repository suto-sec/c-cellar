#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status;
    pid_t pid;

    if (argc != 2)
        return 2;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0)
        exit(atoi(argv[1]));
    wait(&status);
    if (WIFEXITED(status))
        printf("child exited with %d\n", WEXITSTATUS(status));
    return 0;
}
