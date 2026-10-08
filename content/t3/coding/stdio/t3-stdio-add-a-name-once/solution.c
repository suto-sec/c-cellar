#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char line[256];

    if (argc != 3)
        return 1;
    FILE *f = fopen(argv[1], "a+");
    if (f == NULL)
        return 1;
    rewind(f);
    while (fgets(line, sizeof line, f) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, argv[2]) == 0) {
            puts("already there");
            fclose(f);
            return 0;
        }
    }
    fprintf(f, "%s\n", argv[2]);
    fclose(f);
    puts("added");
    return 0;
}
