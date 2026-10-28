#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    if (argc != 2)
        return 2;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        execlp("ls", "ls", "-1", argv[1], NULL);
        exit(1);
    }
    wait(NULL);
    printf("listing finished\n");
    return 0;
}
