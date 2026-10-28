#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int p[2];
    pid_t pid;
    FILE *in;
    char line[1000];

    if (argc < 2)
        return 2;
    if (pipe(p) < 0)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        close(p[0]);
        dup2(p[1], STDOUT_FILENO);
        close(p[1]);
        execvp(argv[1], &argv[1]);
        exit(127);
    }
    close(p[1]);
    in = fdopen(p[0], "r");
    while (fgets(line, sizeof line, in) != NULL)
        printf("> %s", line);
    fclose(in);
    wait(NULL);
    return 0;
}
