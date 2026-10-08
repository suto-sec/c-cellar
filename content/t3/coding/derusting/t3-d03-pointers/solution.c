void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void minmax(const int *v, int n, int *min, int *max)
{
    *min = *max = v[0];
    for (int i = 1; i < n; i++) {
        if (v[i] < *min)
            *min = v[i];
        if (v[i] > *max)
            *max = v[i];
    }
}
