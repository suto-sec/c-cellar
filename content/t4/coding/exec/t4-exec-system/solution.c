#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    char line[1024];
    pid_t pid;
    int status;

    if (fgets(line, sizeof line, stdin) == NULL)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", line, NULL);
        exit(127);
    }
    wait(&status);
    printf("status %d\n", WEXITSTATUS(status));
    return 0;
}
