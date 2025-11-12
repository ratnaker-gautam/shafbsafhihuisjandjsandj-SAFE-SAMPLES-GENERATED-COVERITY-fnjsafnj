//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct Matrix {
    size_t rows;
    size_t cols;
    double *data;
};

struct MatrixOps {
    struct Matrix (*create)(size_t rows, size_t cols);
    void (*destroy)(struct Matrix *m);
    int (*set)(struct Matrix *m, size_t row, size_t col, double value);
    double (*get)(const struct Matrix *m, size_t row, size_t col);
    int (*multiply)(const struct Matrix *a, const struct Matrix *b, struct Matrix *result);
    int (*transpose)(const struct Matrix *src, struct Matrix *dst);
    void (*print)(const struct Matrix *m);
};

struct Matrix matrix_create(size_t rows, size_t cols) {
    struct Matrix m = {0, 0, NULL};
    if (rows == 0 || cols == 0 || rows > SIZE_MAX / cols / sizeof(double)) {
        return m;
    }
    m.data = malloc(rows * cols * sizeof(double));
    if (m.data == NULL) {
        return m;
    }
    m.rows = rows;
    m.cols = cols;
    memset(m.data, 0, rows * cols * sizeof(double));
    return m;
}

void matrix_destroy(struct Matrix *m) {
    if (m != NULL) {
        free(m->data);
        m->data = NULL;
        m->rows = 0;
        m->cols = 0;
    }
}

int matrix_set(struct Matrix *m, size_t row, size_t col, double value) {
    if (m == NULL || m->data == NULL) {
        return 0;
    }
    if (row >= m->rows || col >= m->cols) {
        return 0;
    }
    m->data[row * m->cols + col] = value;
    return 1;
}

double matrix_get(const struct Matrix *m, size_t row, size_t col) {
    if (m == NULL || m->data == NULL) {
        return 0.0;
    }
    if (row >= m->rows || col >= m->cols) {
        return 0.0;
    }
    return m->data[row * m->cols + col];
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }
    if (a->cols != b->rows) {
        return 0;
    }
    if (result->rows != a->rows || result->cols != b->cols) {
        return 0;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += matrix_get(a, i, k) * matrix_get(b, k, j);
            }
            if (!matrix_set(result, i, j, sum)) {
                return 0;
            }
        }
    }
    return 1;
}

int matrix_transpose(const struct Matrix *src, struct Matrix *dst) {
    if (src == NULL || dst == NULL) {
        return 0;
    }
    if (dst->rows != src->cols || dst->cols != src->rows) {
        return 0;
    }
    for (size_t i = 0; i < src->rows; i++) {
        for (size_t j = 0; j < src->cols; j++) {
            if (!matrix_set(dst, j, i, matrix_get(src, i, j))) {
                return 0;
            }
        }
    }
    return 1;
}

void matrix_print(const struct Matrix *m) {
    if (m == NULL || m->data == NULL) {
        printf("Invalid matrix\n");
        return;
    }
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.2f", matrix_get(m, i, j));
        }
        printf("\n");
    }
}

struct MatrixOps init_ops(void) {
    struct MatrixOps ops;
    ops.create = matrix_create;
    ops.destroy = matrix_destroy;
    ops.set = matrix_set;
    ops.get = matrix_get;
    ops.multiply = matrix_multiply;
    ops.transpose = matrix_transpose;
    ops.print = matrix_print;
    return ops;
}

int main(void) {
    struct MatrixOps ops = init_ops();
    struct Matrix a = ops.create(2, 3);
    struct Matrix b = ops.create(3, 2);
    struct Matrix c = ops.create(2, 2);
    struct Matrix d = ops.create(3, 2);
    
    if (a.data == NULL || b.data == NULL || c.data == NULL || d.data == NULL) {
        printf("Matrix creation failed\n");
        ops.destroy(&a);
        ops.destroy(&b);
        ops.destroy(&c);
        ops.destroy(&d);
        return 1;
    }
    
    ops.set(&a, 0, 0, 1.0);
    ops.set(&a, 0, 1, 2.0);
    ops.set(&a, 0,