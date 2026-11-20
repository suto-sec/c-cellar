#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static void nothing(int sig)
{
    (void)sig;
}

static void run(const char *label, int restore)
{
    int status;
    pid_t pid = fork();

    if (pid < 0)
        exit(1);
    if (pid == 0) {
        if (restore)
            signal(SIGINT, SIG_DFL);
        execlp("sh", "sh", "-c", "kill -INT $$; exit 5", (char *)NULL);
        _exit(127);
    }
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status))
        printf("%s: child killed by signal %d\n", label, WTERMSIG(status));
    else
        printf("%s: child exited with %d\n", label, WEXITSTATUS(status));
}

int main(void)
{
    signal(SIGINT, SIG_IGN);
    run("ignored", 0);
    signal(SIGINT, nothing);
    run("caught", 0);
    signal(SIGINT, SIG_IGN);
    run("restored", 1);
    return 0;
}
