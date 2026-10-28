#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXF 16

int main(int argc, char *argv[])
{
    int rd[MAXF], n = argc - 1, i;
    long total = 0, count;

    if (n < 1 || n > MAXF)
        return 2;
    for (i = 0; i < n; i++) {
        int p[2];
        pid_t pid;

        if (pipe(p) < 0)
            return 1;
        pid = fork();
        if (pid < 0)
            return 1;
        if (pid == 0) {
            FILE *f = fopen(argv[i + 1], "r");
            int c;

            close(p[0]);
            count = 0;
            if (f == NULL) {
                count = -1;
            } else {
                while ((c = fgetc(f)) != EOF)
                    if (c == '\n')
                        count++;
                fclose(f);
            }
            write(p[1], &count, sizeof count);
            close(p[1]);
            exit(0);
        }
        close(p[1]);
        rd[i] = p[0];
    }
    for (i = 0; i < n; i++) {
        if (read(rd[i], &count, sizeof count) != sizeof count)
            count = -1;
        close(rd[i]);
        if (count < 0) {
            printf("%s: error\n", argv[i + 1]);
        } else {
            printf("%s: %ld\n", argv[i + 1], count);
            total += count;
        }
    }
    while (wait(NULL) > 0)
        ;
    printf("total: %ld\n", total);
    return 0;
}
