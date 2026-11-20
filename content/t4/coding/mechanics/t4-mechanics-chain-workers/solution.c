#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int n, i, j, v;
    int (*p)[2];

    if (argc < 2)
        return 2;
    n = atoi(argv[1]);
    if (n < 1)
        return 2;
    p = malloc((n + 1) * sizeof *p);
    if (p == NULL)
        return 1;
    for (i = 0; i <= n; i++)
        if (pipe(p[i]) < 0)
            return 1;
    for (i = 1; i <= n; i++) {
        pid_t pid = fork();

        if (pid < 0)
            return 1;
        if (pid == 0) {
            for (j = 0; j <= n; j++) {
                if (j != i - 1)
                    close(p[j][0]);
                if (j != i)
                    close(p[j][1]);
            }
            while (read(p[i - 1][0], &v, sizeof v) == sizeof v) {
                v += i;
                write(p[i][1], &v, sizeof v);
            }
            _exit(0);
        }
    }
    for (j = 0; j <= n; j++) {
        if (j != n)
            close(p[j][0]);
        if (j != 0)
            close(p[j][1]);
    }
    for (v = 1; v <= 5; v++)
        write(p[0][1], &v, sizeof v);
    close(p[0][1]);
    while (read(p[n][0], &v, sizeof v) == sizeof v)
        printf("%d\n", v);
    close(p[n][0]);
    for (i = 0; i < n; i++)
        wait(NULL);
    free(p);
    return 0;
}
