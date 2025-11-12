//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum MatrixOp {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

struct Matrix {
    int rows;
    int cols;
    double *data;
};

int matrix_init(struct Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(double));
    return m->data != NULL;
}

void matrix_free(struct Matrix *m) {
    free(m->data);
    m->data = NULL;
}

int matrix_get(struct Matrix *m, int row, int col, double *val) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) return 0;
    *val = m->data[row * m->cols + col];
    return 1;
}

int matrix_set(struct Matrix *m, int row, int col, double val) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) return 0;
    m->data[row * m->cols + col] = val;
    return 1;
}

int matrix_add(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            double val1, val2;
            if (!matrix_get(a, i, j, &val1) || !matrix_get(b, i, j, &val2)) {
                matrix_free(result);
                return 0;
            }
            matrix_set(result, i, j, val1 + val2);
        }
    }
    return 1;
}

int matrix_subtract(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            double val1, val2;
            if (!matrix_get(a, i, j, &val1) || !matrix_get(b, i, j, &val2)) {
                matrix_free(result);
                return 0;
            }
            matrix_set(result, i, j, val1 - val2);
        }
    }
    return 1;
}

int matrix_multiply(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                double val1, val2;
                if (!matrix_get(a, i, k, &val1) || !matrix_get(b, k, j, &val2)) {
                    matrix_free(result);
                    return 0;
                }
                sum += val1 * val2;
            }
            matrix_set(result, i, j, sum);
        }
    }
    return 1;
}

int matrix_transpose(struct Matrix *a, struct Matrix *result) {
    if (!matrix_init(result, a->cols, a->rows)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            double val;
            if (!matrix_get(a, i, j, &val)) {
                matrix_free(result);
                return 0;
            }
            matrix_set(result, j, i, val);
        }
    }
    return 1;
}

double matrix_determinant(struct Matrix *a) {
    if (a->rows != a->cols) return 0.0;
    int n = a->rows;
    if (n == 1) {
        double val;
        matrix_get(a, 0, 0, &val);
        return val;
    }
    if (n == 2) {
        double a1, a2, a3, a4;
        matrix_get(a, 0, 0, &a1);
        matrix_get(a, 0, 1, &a2);
        matrix_get(a, 1, 0, &a3);
        matrix_get(a, 1, 1, &a4);
        return a1 * a4 - a2 * a3;
    }
    double det = 0.0;
    for (int col = 0; col < n; col++) {
        struct Matrix sub;
        if (!matrix_init(&sub, n-1, n-1)) return 0.0;
        for (int i = 1