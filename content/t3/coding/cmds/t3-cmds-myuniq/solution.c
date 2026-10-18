#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char line[1024], prev[1024];
    int count = 0, with_count = 0;

    if (argc == 2 && strcmp(argv[1], "-c") == 0)
        with_count = 1;
    else if (argc != 1) {
        fprintf(stderr, "usage: myuniq [-c]\n");
        return 2;
    }
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (count > 0 && strcmp(line, prev) == 0) {
            count++;
            continue;
        }
        if (count > 0) {
            if (with_count)
                printf("%7d ", count);
            fputs(prev, stdout);
        }
        strcpy(prev, line);
        count = 1;
    }
    if (count > 0) {
        if (with_count)
            printf("%7d ", count);
        fputs(prev, stdout);
    }
    return 0;
}
