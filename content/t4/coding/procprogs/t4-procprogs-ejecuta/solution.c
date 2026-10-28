#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int status;

    if (argc < 2) {
        fprintf(stderr, "usage: ejecuta CMD [ARGS...]\n");
        return 2;
    }
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        execvp(argv[1], &argv[1]);
        fprintf(stderr, "error: cannot run %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }
    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        printf("the command finished correctly\n");
    else
        printf("the command did not run correctly\n");
    return 0;
}
