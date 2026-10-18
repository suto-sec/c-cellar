#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: perms PATH...\n");
        return 2;
    }
    for (int i = 1; i < argc; i++) {
        if (access(argv[i], F_OK) == -1) {
            printf("%s: missing\n", argv[i]);
            continue;
        }
        printf("%s: %c %c %c\n", argv[i],
               access(argv[i], R_OK) == 0 ? 'r' : '-',
               access(argv[i], W_OK) == 0 ? 'w' : '-',
               access(argv[i], X_OK) == 0 ? 'x' : '-');
    }
    return 0;
}
