//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

enum MatrixOp {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

struct Matrix {
    double *data;
    int rows;
    int cols;
};

int matrix_init(struct Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(double));
    return m->data != NULL;
}

void matrix_free(struct Matrix *m) {
    if (m->data) free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

int matrix_read(struct Matrix *m) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i * m->cols + j]) != 1) return 0;
        }
    }
    return 1;
}

void matrix_print(const struct Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

int matrix_add(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i * a->cols + j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
        }
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i * a->cols + j] = a->data[i * a->cols + j] - b->data[i * a->cols + j];
        }
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const struct Matrix *a, struct Matrix *result) {
    if (!matrix_init(result, a->cols, a->rows)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[j * result->cols + i] = a->data[i * a->cols + j];
        }
    }
    return 1;
}

double matrix_determinant(const struct Matrix *m);

double matrix_determinant(const struct Matrix *m) {
    if (m->rows != m->cols) return NAN;
    if (m->rows == 1) return m->data[0];
    if (m->rows == 2) {
        return m->data[0] * m->data[3] - m->data[1] * m->data[2];
    }
    double det = 0.0;
    for (int col = 0; col < m->cols; col++) {
        struct Matrix sub;
        if (!matrix_init(&sub, m->rows - 1, m->cols - 1)) return NAN;
        for (int i = 1; i < m->rows; i++) {
            int sub_col = 0;
            for (int j = 0; j < m->cols; j++) {
                if (j == col) continue;
                sub.data[(i-1) * sub.cols + sub_col] = m->data[i * m->cols + j];
                sub_col++;
            }
        }
        double sign = (col % 2 == 0) ? 1.0 : -1.