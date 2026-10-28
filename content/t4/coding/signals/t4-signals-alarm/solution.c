#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

static void handler(int sig)
{
    const char *msg = "alarm!\n";

    (void)sig;
    write(STDOUT_FILENO, msg, strlen(msg));
}

int main(void)
{
    signal(SIGALRM, handler);
    printf("waiting\n");
    fflush(stdout);
    alarm(1);
    pause();
    printf("done\n");
    return 0;
}
