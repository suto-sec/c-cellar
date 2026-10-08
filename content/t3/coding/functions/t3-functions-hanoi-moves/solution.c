long hanoi(int n)
{
    if (n == 0)
        return 0;
    return 2 * hanoi(n - 1) + 1;
}
