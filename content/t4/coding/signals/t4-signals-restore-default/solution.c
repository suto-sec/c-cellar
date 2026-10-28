#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

static void handler(int sig)
{
    const char *msg = "first\n";

    (void)sig;
    write(STDOUT_FILENO, msg, strlen(msg));
    signal(SIGINT, SIG_DFL);
}

int main(void)
{
    pid_t pid = fork();
    int status;

    if (pid < 0)
        return 1;
    if (pid == 0) {
        signal(SIGINT, handler);
        raise(SIGINT);
        raise(SIGINT);
        exit(0);
    }
    wait(&status);
    if (WIFEXITED(status))
        printf("child survived\n");
    else
        printf("child died on the second SIGINT\n");
    return 0;
}
