#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int n, i, status, sum = 0;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    for (i = 1; i <= n; i++) {
        pid_t pid = fork();

        if (pid < 0)
            return 1;
        if (pid == 0)
            exit(i);
    }
    while (wait(&status) > 0)
        if (WIFEXITED(status))
            sum += WEXITSTATUS(status);
    printf("sum of statuses: %d\n", sum);
    return 0;
}
