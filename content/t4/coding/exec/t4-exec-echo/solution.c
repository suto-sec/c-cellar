#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0)
        return 1;
    if (pid == 0) {
        execlp("echo", "echo", "hello", "world", NULL);
        exit(1);
    }
    wait(NULL);
    printf("done\n");
    return 0;
}
