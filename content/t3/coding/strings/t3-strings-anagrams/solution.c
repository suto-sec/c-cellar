#include <stdio.h>
#include <string.h>

int main(void)
{
    char a[128], b[128];
    int count[26] = {0};

    if (scanf("%127s %127s", a, b) != 2)
        return 1;
    for (char *p = a; *p; p++)
        count[*p - 'a']++;
    for (char *p = b; *p; p++)
        count[*p - 'a']--;
    for (int i = 0; i < 26; i++)
        if (count[i] != 0) {
            printf("not anagram\n");
            return 0;
        }
    printf("anagram\n");
    return 0;
}
