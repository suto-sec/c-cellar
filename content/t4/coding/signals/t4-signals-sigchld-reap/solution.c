#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static volatile sig_atomic_t reaped = 0;

static void handler(int sig)
{
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        reaped++;
}

int main(int argc, char *argv[])
{
    int n, i;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    signal(SIGCHLD, handler);
    for (i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0)
            return 1;
        if (pid == 0)
            exit(0);
    }
    while (reaped < n)
        sleep(1);
    printf("reaped %d\n", (int)reaped);
    return 0;
}
