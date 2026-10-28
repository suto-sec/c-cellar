#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    if (argc != 2)
        return 2;
    if (chdir(argv[1]) < 0) {
        fprintf(stderr, "cannot enter %s\n", argv[1]);
        return 1;
    }
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        execlp("pwd", "pwd", NULL);
        exit(1);
    }
    wait(NULL);
    return 0;
}
