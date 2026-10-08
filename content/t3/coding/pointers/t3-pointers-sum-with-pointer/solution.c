int sum(const int *v, int n)
{
    int s = 0;
    const int *end = v + n;

    while (v < end)
        s += *v++;
    return s;
}
