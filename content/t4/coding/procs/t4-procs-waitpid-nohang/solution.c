#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();
    int status;

    if (pid < 0)
        return 1;
    if (pid == 0) {
        sleep(1);
        exit(5);
    }
    if (waitpid(pid, &status, WNOHANG) == 0)
        printf("child still running\n");
    waitpid(pid, &status, 0);
    printf("child exited with %d\n", WEXITSTATUS(status));
    return 0;
}
