#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int p[2], n = 0;
    pid_t pid;
    FILE *f;
    char line[100];

    if (pipe(p) < 0)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        close(p[0]);
        f = fdopen(p[1], "w");
        fprintf(f, "alpha\nbeta\ngamma\n");
        fclose(f);
        exit(0);
    }
    close(p[1]);
    f = fdopen(p[0], "r");
    while (fgets(line, sizeof line, f) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        printf("%d: %s\n", ++n, line);
    }
    fclose(f);
    wait(NULL);
    return 0;
}
