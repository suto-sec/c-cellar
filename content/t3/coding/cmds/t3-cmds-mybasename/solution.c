#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char path[4096];
    char *base;
    size_t len;

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "usage: mybasename PATH [SUFFIX]\n");
        return 2;
    }
    snprintf(path, sizeof(path), "%s", argv[1]);
    len = strlen(path);
    while (len > 1 && path[len - 1] == '/')
        path[--len] = '\0';
    if (strcmp(path, "/") == 0) {
        puts("/");
        return 0;
    }
    base = strrchr(path, '/');
    base = base ? base + 1 : path;
    if (argc == 3) {
        size_t bl = strlen(base), sl = strlen(argv[2]);
        if (sl < bl && strcmp(base + bl - sl, argv[2]) == 0)
            base[bl - sl] = '\0';
    }
    puts(base);
    return 0;
}
