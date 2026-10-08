#include <stdio.h>

int main(void)
{
    char name[32];
    int age;

    if (scanf("%31s %d", name, &age) != 2)
        return 1;
    printf("%s is %d years old.\n", name, age);
    return 0;
}
