#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    char before[4096], now[4096], *slash;
    pid_t pid;

    if (argc < 2)
        return 2;
    if (getcwd(before, sizeof before) == NULL)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        if (chdir(argv[1]) < 0) {
            printf("child: cannot go to %s: %s\n", argv[1], strerror(errno));
            exit(1);
        }
        if (getcwd(now, sizeof now) == NULL)
            exit(1);
        slash = strrchr(now, '/');
        printf("child: %s\n", slash ? slash + 1 : now);
        exit(0);
    }
    wait(NULL);
    if (getcwd(now, sizeof now) == NULL)
        return 1;
    printf("parent: %s\n", strcmp(before, now) == 0 ? "unchanged" : "changed");
    return 0;
}
