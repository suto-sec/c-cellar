#include <stdio.h>

int main(void)
{
    char c;

    if (scanf(" %c", &c) != 1)
        return 1;
    switch (c) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
        case 'A': case 'E': case 'I': case 'O': case 'U':
            printf("vowel\n");
            break;
        default:
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                printf("consonant\n");
            else
                printf("not a letter\n");
    }
    return 0;
}
