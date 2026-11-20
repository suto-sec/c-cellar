#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0)
        return 1;
    if (pid == 0)
        _exit(0);
    sleep(1);
    printf("before wait: %s\n", kill(pid, 0) == 0 ? "still there" : "gone");
    waitpid(pid, NULL, 0);
    printf("after wait: %s\n", kill(pid, 0) == 0 ? "still there" : "gone");
    return 0;
}
