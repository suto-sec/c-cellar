#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int n = 10;
    long total = 0;
    char line[1024], **ring;

    if (argc == 2 && argv[1][0] == '-')
        n = atoi(argv[1] + 1);
    else if (argc != 1)
        return 2;
    if (n <= 0)
        return 0;
    ring = calloc((size_t) n, sizeof(char *));
    if (ring == NULL)
        return 1;
    while (fgets(line, sizeof(line), stdin) != NULL) {
        free(ring[total % n]);
        ring[total % n] = strdup(line);
        total++;
    }
    for (long i = total > n ? total - n : 0; i < total; i++)
        fputs(ring[i % n], stdout);
    for (int i = 0; i < n; i++)
        free(ring[i]);
    free(ring);
    return 0;
}
