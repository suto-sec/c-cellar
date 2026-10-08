#include <stdio.h>

#define MAX 100

int main(void)
{
    int n, v[MAX];

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%d", &v[i]) != 1)
            return 1;
    int x, lo = 0, hi, pos = -1;

    if (scanf("%d", &x) != 1)
        return 1;
    hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (v[mid] == x) {
            pos = mid;
            break;
        }
        if (v[mid] < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    printf("%d\n", pos);
    return 0;
}
