void apply(int *v, int n, int (*f)(int))
{
    for (int i = 0; i < n; i++)
        v[i] = f(v[i]);
}
