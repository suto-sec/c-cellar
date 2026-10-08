#include <stdio.h>

enum day { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };

static const char *names[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

int main(void)
{
    int n;
    enum day d, next;

    if (scanf("%d", &n) != 1)
        return 1;
    if (n < MONDAY || n > SUNDAY) {
        printf("invalid\n");
        return 0;
    }
    d = (enum day) n;
    next = d == SUNDAY ? MONDAY : (enum day) (d + 1);
    printf("%s -> %s\n", names[d], names[next]);
    return 0;
}
