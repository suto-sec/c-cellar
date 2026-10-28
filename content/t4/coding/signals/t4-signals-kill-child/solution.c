#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();
    int status;

    if (pid < 0)
        return 1;
    if (pid == 0) {
        for (;;)
            pause();
    }
    kill(pid, SIGTERM);
    wait(&status);
    if (WIFEXITED(status))
        printf("child exited\n");
    else
        printf("child was killed\n");
    return 0;
}
