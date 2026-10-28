#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t a, b;
    int status;

    a = fork();
    if (a < 0)
        return 1;
    if (a == 0)
        exit(10);
    b = fork();
    if (b < 0)
        return 1;
    if (b == 0)
        exit(20);
    waitpid(b, &status, 0);
    printf("second: %d\n", WEXITSTATUS(status));
    waitpid(a, &status, 0);
    printf("first: %d\n", WEXITSTATUS(status));
    return 0;
}
