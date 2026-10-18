#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char line[1024];
    int delim = -1, field = 0;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-d", 2) == 0 && strlen(argv[i]) == 3)
            delim = (unsigned char) argv[i][2];
        else if (strncmp(argv[i], "-f", 2) == 0)
            field = atoi(argv[i] + 2);
        else
            delim = -2;
    }
    if (delim < 0 || field <= 0) {
        fprintf(stderr, "usage: mycut -dC -fN\n");
        return 2;
    }
    while (fgets(line, sizeof(line), stdin) != NULL) {
        char *start = line, *end;
        size_t len = strlen(line);

        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        if (strchr(line, delim) == NULL) {
            puts(line);
            continue;
        }
        for (int k = 1; k < field && start != NULL; k++) {
            start = strchr(start, delim);
            if (start != NULL)
                start++;
        }
        if (start == NULL) {
            puts("");
            continue;
        }
        end = strchr(start, delim);
        if (end != NULL)
            *end = '\0';
        puts(start);
    }
    return 0;
}
