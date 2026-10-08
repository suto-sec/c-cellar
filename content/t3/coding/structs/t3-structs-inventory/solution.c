#include <stdio.h>

struct item {
    char name[32];
    int qty;
    double price;
};

int main(void)
{
    struct item v[50];
    int n, best = 0;
    double total = 0;

    if (scanf("%d", &n) != 1 || n < 1 || n > 50)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%31s %d %lf", v[i].name, &v[i].qty, &v[i].price) != 3)
            return 1;
    for (int i = 0; i < n; i++) {
        total += v[i].qty * v[i].price;
        if (v[i].price > v[best].price)
            best = i;
    }
    printf("total=%.2f\nmost expensive: %s\n", total, v[best].name);
    return 0;
}
