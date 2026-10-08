#include <stdio.h>

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");

        if (f != NULL) {
            fclose(f);
            printf("%s\n", argv[i]);
            return 0;
        }
    }
    printf("none readable\n");
    return 1;
}
