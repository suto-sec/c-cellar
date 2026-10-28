#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

static void handler(int sig)
{
    const char *msg = "got SIGUSR1\n";

    (void)sig;
    write(STDOUT_FILENO, msg, strlen(msg));
}

int main(void)
{
    signal(SIGUSR1, handler);
    raise(SIGUSR1);
    printf("after\n");
    return 0;
}
