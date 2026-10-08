int is_prime(int n)
{
    if (n < 2)
        return 0;
    for (long d = 2; d * d <= n; d++)
        if (n % d == 0)
            return 0;
    return 1;
}
