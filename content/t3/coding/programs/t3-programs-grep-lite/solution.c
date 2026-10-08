#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int numbers = 0, first = 1, matched = 0, lineno = 0;
    FILE *f;
    char line[1024];

    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        numbers = 1;
        first = 2;
    }
    if (argc != first + 2)
        return 2;
    f = fopen(argv[first + 1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[first + 1]);
        return 2;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        lineno++;
        if (strstr(line, argv[first]) != NULL) {
            if (numbers)
                printf("%d:", lineno);
            fputs(line, stdout);
            matched = 1;
        }
    }
    fclose(f);
    return matched ? 0 : 1;
}
