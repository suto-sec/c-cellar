#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int p[2];
    pid_t pid;
    const char *msg = "hello from parent";
    char buf[100];
    ssize_t n;

    if (pipe(p) < 0)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        close(p[1]);
        n = read(p[0], buf, sizeof buf - 1);
        close(p[0]);
        if (n < 0)
            exit(1);
        buf[n] = '\0';
        printf("child read: %s\n", buf);
        exit(0);
    }
    close(p[0]);
    write(p[1], msg, strlen(msg));
    close(p[1]);
    wait(NULL);
    return 0;
}
