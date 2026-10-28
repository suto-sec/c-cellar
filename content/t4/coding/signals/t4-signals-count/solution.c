#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static volatile sig_atomic_t received = 0;

static void handler(int sig)
{
    (void)sig;
    received++;
}

int main(int argc, char *argv[])
{
    int n, i;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    signal(SIGUSR1, handler);
    for (i = 0; i < n; i++)
        raise(SIGUSR1);
    printf("received %d\n", (int)received);
    return 0;
}
