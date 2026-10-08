int *find_max(int *v, int n)
{
    int *best = v;

    for (int *p = v + 1; p < v + n; p++)
        if (*p > *best)
            best = p;
    return best;
}
