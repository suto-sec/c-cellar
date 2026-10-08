#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *f;
    char line[1024], best[1024] = "";
    size_t best_len = 0;
    int any = 0;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        if (!any || strlen(line) > best_len) {
            strcpy(best, line);
            best_len = strlen(line);
            any = 1;
        }
    }
    fclose(f);
    if (any)
        printf("%zu: %s\n", best_len, best);
    else
        printf("empty\n");
    return 0;
}
