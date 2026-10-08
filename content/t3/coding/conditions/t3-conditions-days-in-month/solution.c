#include <stdio.h>

int main(void)
{
    int m, y, days;

    if (scanf("%d %d", &m, &y) != 2)
        return 1;
    switch (m) {
        case 4: case 6: case 9: case 11:
            days = 30;
            break;
        case 2:
            days = ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) ? 29 : 28;
            break;
        default:
            days = 31;
    }
    printf("%d\n", days);
    return 0;
}
