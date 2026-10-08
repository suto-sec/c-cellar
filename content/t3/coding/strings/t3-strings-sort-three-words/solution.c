#include <stdio.h>
#include <string.h>

int main(void)
{
    char w[3][64];

    for (int i = 0; i < 3; i++)
        if (scanf("%63s", w[i]) != 1)
            return 1;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2 - i; j++)
            if (strcmp(w[j], w[j + 1]) > 0) {
                char t[64];
                strcpy(t, w[j]);
                strcpy(w[j], w[j + 1]);
                strcpy(w[j + 1], t);
            }
    for (int i = 0; i < 3; i++)
        puts(w[i]);
    return 0;
}
