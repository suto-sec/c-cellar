long power(int base, int exp)
{
    long r = 1;

    for (int i = 0; i < exp; i++)
        r *= base;
    return r;
}
