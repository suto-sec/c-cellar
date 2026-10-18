#include <stdio.h>
#include <stdlib.h>

struct matrix {
    int rows, cols;
    long *data;
};

static struct matrix create(int rows, int cols)
{
    struct matrix m = { rows, cols, calloc((size_t) rows * (size_t) cols, sizeof(long)) };
    return m;
}

static void release(struct matrix *m)
{
    free(m->data);
    m->data = NULL;
}

static int read_matrix(struct matrix *m)
{
    int rows, cols;

    if (scanf("%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0)
        return -1;
    *m = create(rows, cols);
    if (m->data == NULL)
        return -1;
    for (int i = 0; i < rows * cols; i++)
        if (scanf("%ld", &m->data[i]) != 1)
            return -1;
    return 0;
}

static struct matrix multiply(const struct matrix *a, const struct matrix *b)
{
    struct matrix c = create(a->rows, b->cols);

    for (int i = 0; i < a->rows; i++)
        for (int j = 0; j < b->cols; j++)
            for (int k = 0; k < a->cols; k++)
                c.data[i * c.cols + j] += a->data[i * a->cols + k] * b->data[k * b->cols + j];
    return c;
}

int main(void)
{
    struct matrix a, b, c;

    if (read_matrix(&a) == -1 || read_matrix(&b) == -1) {
        fprintf(stderr, "matmul: bad input\n");
        return 1;
    }
    if (a.cols != b.rows) {
        fprintf(stderr, "matmul: dimension mismatch\n");
        return 1;
    }
    c = multiply(&a, &b);
    for (int i = 0; i < c.rows; i++) {
        for (int j = 0; j < c.cols; j++)
            printf(j ? " %ld" : "%ld", c.data[i * c.cols + j]);
        putchar('\n');
    }
    release(&a);
    release(&b);
    release(&c);
    return 0;
}
