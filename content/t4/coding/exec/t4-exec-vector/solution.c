#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    char *args[] = {"echo", "one", "two", "three", NULL};
    pid_t pid = fork();

    if (pid < 0)
        return 1;
    if (pid == 0) {
        execv("/bin/echo", args);
        exit(1);
    }
    wait(NULL);
    printf("done\n");
    return 0;
}
