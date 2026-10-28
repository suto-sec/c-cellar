#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t parent = getpid();
    pid_t pid = fork();
    int status;

    if (pid < 0)
        return 1;
    if (pid == 0)
        exit(getppid() == parent ? 0 : 1);
    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        printf("child sees the right parent: yes\n");
    else
        printf("child sees the right parent: no\n");
    return 0;
}
