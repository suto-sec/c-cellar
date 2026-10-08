#include <stdio.h>

int main(void)
{
    double coffee = 3.50, bread = 2.25;
    double total = 2 * coffee + bread;

    printf("%-10s%5s%8s\n", "Item", "Qty", "Price");
    printf("%-10s%5d%8.2f\n", "Coffee", 2, coffee);
    printf("%-10s%5d%8.2f\n", "Bread", 1, bread);
    printf("%-10s%5s%8.2f\n", "Total", "", total);
    return 0;
}
