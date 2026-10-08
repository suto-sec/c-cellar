#include <stdio.h>

struct student {
    char name[32];
    int age;
    double grade;
};

int main(void)
{
    struct student s;

    if (scanf("%31s %d %lf", s.name, &s.age, &s.grade) != 3)
        return 1;
    printf("%s (%d): %.2f\n", s.name, s.age, s.grade);
    return 0;
}
