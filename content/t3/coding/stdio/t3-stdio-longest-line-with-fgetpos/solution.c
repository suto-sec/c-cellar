#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char line[256];
    fpos_t pos, best_pos;
    size_t best_len = 0;
    int have = 0;

    if (argc != 2)
        return 1;
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
        return 1;
    for (;;) {
        fgetpos(f, &pos);
        if (fgets(line, sizeof line, f) == NULL)
            break;
        size_t len = strcspn(line, "\n");

        if (!have || len > best_len) {
            best_len = len;
            best_pos = pos;
            have = 1;
        }
    }
    if (have) {
        fsetpos(f, &best_pos);
        if (fgets(line, sizeof line, f) != NULL) {
            line[strcspn(line, "\n")] = '\0';
            puts(line);
        }
    }
    fclose(f);
    return 0;
}
