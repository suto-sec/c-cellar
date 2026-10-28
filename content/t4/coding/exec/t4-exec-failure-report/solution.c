#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int status;

    if (argc < 2)
        return 2;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        execvp(argv[1], &argv[1]);
        exit(127);
    }
    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
        printf("command not found\n");
    else if (WIFEXITED(status))
        printf("exit status %d\n", WEXITSTATUS(status));
    return 0;
}
