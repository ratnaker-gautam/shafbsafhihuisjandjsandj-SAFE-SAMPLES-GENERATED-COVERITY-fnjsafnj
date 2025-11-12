//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum matrix_op {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

struct matrix {
    size_t rows;
    size_t cols;
    int *data;
};

int matrix_alloc(struct matrix *m, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0 || rows > SIZE_MAX / cols) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(int));
    return m->data != NULL;
}

void matrix_free(struct matrix *m) {
    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

int matrix_get(const struct matrix *m, size_t row, size_t col) {
    if (row >= m->rows || col >= m->cols) return 0;
    return m->data[row * m->cols + col];
}

void matrix_set(struct matrix *m, size_t row, size_t col, int value) {
    if (row < m->rows && col < m->cols) {
        m->data[row * m->cols + col] = value;
    }
}

int matrix_add(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_alloc(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            int val_a = matrix_get(a, i, j);
            int val_b = matrix_get(b, i, j);
            if ((val_a > 0 && val_b > INT_MAX - val_a) || (val_a < 0 && val_b < INT_MIN - val_a)) {
                matrix_free(result);
                return 0;
            }
            matrix_set(result, i, j, val_a + val_b);
        }
    }
    return 1;
}

int matrix_subtract(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_alloc(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            int val_a = matrix_get(a, i, j);
            int val_b = matrix_get(b, i, j);
            if ((val_b > 0 && val_a < INT_MIN + val_b) || (val_b < 0 && val_a > INT_MAX + val_b)) {
                matrix_free(result);
                return 0;
            }
            matrix_set(result, i, j, val_a - val_b);
        }
    }
    return 1;
}

int matrix_multiply(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_alloc(result, a->rows, b->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            int sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                int val_a = matrix_get(a, i, k);
                int val_b = matrix_get(b, k, j);
                long long prod = (long long)val_a * val_b;
                if (prod > INT_MAX || prod < INT_MIN) {
                    matrix_free(result);
                    return 0;
                }
                int prod_int = (int)prod;
                if ((prod_int > 0 && sum > INT_MAX - prod_int) || (prod_int < 0 && sum < INT_MIN - prod_int)) {
                    matrix_free(result);
                    return 0;
                }
                sum += prod_int;
            }
            matrix_set(result, i, j, sum);
        }
    }
    return 1;
}

int matrix_transpose(const struct matrix *a, struct matrix *result) {
    if (!matrix_alloc(result, a->cols, a->rows)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            matrix_set(result, j, i, matrix_get(a, i, j));
        }
    }
    return 1;
}

int matrix_determinant(const struct matrix *a, int *result) {
    if (a->rows != a->cols || a->rows == 0) return 0;
    if (a->rows == 1) {
        *result = matrix_get(a, 0, 0);
        return 1;
    }
    if (a->rows == 2) {
        int a11 = matrix_get(a, 0, 0);
        int a12 = matrix_get(a, 0, 1);
        int a21 = matrix_get(a,