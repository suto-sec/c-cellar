#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int p[2], value;
    pid_t pid;

    if (pipe(p) < 0)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        close(p[0]);
        value = 6 * 7;
        write(p[1], &value, sizeof value);
        close(p[1]);
        exit(0);
    }
    close(p[1]);
    if (read(p[0], &value, sizeof value) != sizeof value)
        return 1;
    close(p[0]);
    wait(NULL);
    printf("answer: %d\n", value);
    return 0;
}
