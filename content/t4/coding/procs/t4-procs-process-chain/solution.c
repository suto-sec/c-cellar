#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void level(int k, int n)
{
    printf("down %d\n", k);
    fflush(stdout);
    if (k < n) {
        pid_t pid = fork();

        if (pid < 0)
            exit(1);
        if (pid == 0) {
            level(k + 1, n);
            exit(0);
        }
        wait(NULL);
    }
    printf("up %d\n", k);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 2;
    level(1, atoi(argv[1]));
    return 0;
}
