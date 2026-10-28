#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int up[2], down[2], i;
    pid_t pid;
    char line[1024];
    FILE *in, *out;

    if (pipe(up) < 0 || pipe(down) < 0)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        close(up[1]);
        close(down[0]);
        in = fdopen(up[0], "r");
        out = fdopen(down[1], "w");
        while (fgets(line, sizeof line, in) != NULL) {
            for (i = 0; line[i] != '\0'; i++)
                line[i] = toupper((unsigned char)line[i]);
            fputs(line, out);
            fflush(out);
        }
        fclose(in);
        fclose(out);
        exit(0);
    }
    close(up[0]);
    close(down[1]);
    out = fdopen(up[1], "w");
    in = fdopen(down[0], "r");
    while (fgets(line, sizeof line, stdin) != NULL) {
        fputs(line, out);
        fflush(out);
        if (fgets(line, sizeof line, in) == NULL)
            break;
        fputs(line, stdout);
    }
    fclose(out);
    fclose(in);
    wait(NULL);
    return 0;
}
