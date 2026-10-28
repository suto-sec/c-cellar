#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int x = 10;
    pid_t pid = fork();

    if (pid < 0)
        return 1;
    if (pid == 0) {
        x = 20;
        printf("child: x = %d\n", x);
        exit(0);
    }
    wait(NULL);
    printf("parent: x = %d\n", x);
    return 0;
}
