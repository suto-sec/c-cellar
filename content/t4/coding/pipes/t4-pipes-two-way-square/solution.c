#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int up[2], down[2], n, sq;
    pid_t pid;

    if (pipe(up) < 0 || pipe(down) < 0)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        close(up[1]);
        close(down[0]);
        while (read(up[0], &n, sizeof n) == sizeof n) {
            sq = n * n;
            write(down[1], &sq, sizeof sq);
        }
        close(up[0]);
        close(down[1]);
        exit(0);
    }
    close(up[0]);
    close(down[1]);
    while (scanf("%d", &n) == 1) {
        write(up[1], &n, sizeof n);
        if (read(down[0], &sq, sizeof sq) != sizeof sq)
            break;
        printf("%d squared is %d\n", n, sq);
    }
    close(up[1]);
    close(down[0]);
    wait(NULL);
    return 0;
}
