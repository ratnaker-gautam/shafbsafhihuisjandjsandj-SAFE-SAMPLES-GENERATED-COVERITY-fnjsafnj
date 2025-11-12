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

int matrix_input(struct Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) return 0;
    if (m->rows <= 0 || m->cols <= 0) return 0;
    if (!matrix_init(m, m->rows, m->cols)) return 0;
    
    printf("Enter %d elements row-wise:\n", m->rows * m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i * m->cols + j]) != 1) {
                matrix_free(m);
                return 0;
            }
        }
    }
    return 1;
}

void matrix_print(struct Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

int matrix_add(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            double val1, val2;
            matrix_get(a, i, j, &val1);
            matrix_get(b, i, j, &val2);
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
            matrix_get(a, i, j, &val1);
            matrix_get(b, i, j, &val2);
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
                matrix_get(a, i, k, &val1);
                matrix_get(b, k, j, &val2);
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
            matrix_get(a, i, j, &val);
            matrix_set(result, j, i, val);
        }
    }
    return 1;
}

double matrix_determinant(struct Matrix *a) {
    if (a->rows != a->cols) return 0.0