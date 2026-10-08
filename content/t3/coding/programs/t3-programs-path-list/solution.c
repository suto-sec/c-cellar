#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *path = getenv("PATH"), *copy, *save, *dir;
    int n = 0;

    if (path == NULL) {
        printf("no PATH\n");
        return 1;
    }
    copy = strdup(path);
    if (copy == NULL)
        return 1;
    for (dir = strtok_r(copy, ":", &save); dir != NULL; dir = strtok_r(NULL, ":", &save))
        printf("%d %s\n", ++n, dir);
    free(copy);
    return 0;
}
