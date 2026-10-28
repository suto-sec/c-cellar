#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int p[2], x, sum = 0;
    pid_t pid;

    if (pipe(p) < 0)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        close(p[1]);
        while (read(p[0], &x, sizeof x) == sizeof x)
            sum += x;
        close(p[0]);
        printf("sum: %d\n", sum);
        exit(0);
    }
    close(p[0]);
    while (scanf("%d", &x) == 1)
        write(p[1], &x, sizeof x);
    close(p[1]);
    wait(NULL);
    return 0;
}
