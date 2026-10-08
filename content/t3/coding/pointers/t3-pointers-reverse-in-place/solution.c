void reverse(int *v, int n)
{
    int *a = v, *b = v + n - 1;

    while (a < b) {
        int t = *a;
        *a++ = *b;
        *b-- = t;
    }
}
