#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *f;
    char w[1001], longest[1001], shortest[1001];
    int any = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: extremes FILE\n");
        return 2;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "extremes: %s: %s\n", argv[1], strerror(errno));
        return 2;
    }
    while (fscanf(f, "%1000s", w) == 1) {
        if (!any) {
            strcpy(longest, w);
            strcpy(shortest, w);
            any = 1;
        } else {
            if (strlen(w) > strlen(longest))
                strcpy(longest, w);
            if (strlen(w) < strlen(shortest))
                strcpy(shortest, w);
        }
    }
    fclose(f);
    if (!any) {
        fprintf(stderr, "extremes: no words\n");
        return 1;
    }
    printf("longest: %s\nshortest: %s\n", longest, shortest);
    return 0;
}
