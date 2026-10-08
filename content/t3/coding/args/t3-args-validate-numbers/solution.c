#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        char *end;
        long v = strtol(argv[i], &end, 10);

        if (end == argv[i] || *end != '\0')
            printf("invalid %s\n", argv[i]);
        else
            printf("ok %ld\n", v);
    }
    return 0;
}
