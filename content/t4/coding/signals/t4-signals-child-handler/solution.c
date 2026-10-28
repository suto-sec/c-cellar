#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

static void handler(int sig)
{
    const char *msg = "child: got SIGUSR1\n";

    (void)sig;
    write(STDOUT_FILENO, msg, strlen(msg));
    exit(0);
}

int main(void)
{
    pid_t pid;
    int status;

    signal(SIGUSR1, handler);
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        for (;;)
            pause();
    }
    kill(pid, SIGUSR1);
    waitpid(pid, &status, 0);
    printf("parent: child exited with %d\n", WEXITSTATUS(status));
    return 0;
}
