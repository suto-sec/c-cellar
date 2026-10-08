#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *path, *copy, *dir, *save;
    char full[4096];
    int found = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: %s NAME\n", argv[0]);
        return 2;
    }
    path = getenv("PATH");
    if (path == NULL)
        return 1;
    copy = strdup(path);
    if (copy == NULL)
        return 1;
    for (dir = strtok_r(copy, ":", &save); dir != NULL; dir = strtok_r(NULL, ":", &save)) {
        snprintf(full, sizeof(full), "%s/%s", dir, argv[1]);
        if (access(full, X_OK) == 0) {
            puts(full);
            found = 1;
        }
    }
    free(copy);
    return found ? 0 : 1;
}
