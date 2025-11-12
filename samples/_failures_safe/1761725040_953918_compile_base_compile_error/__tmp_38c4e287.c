//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

enum matrix_op {
    MATRIX_ADD,
    MATRIX_SUBTRACT,
    MATRIX_MULTIPLY,
    MATRIX_TRANSPOSE,
    MATRIX_DETERMINANT
};

struct matrix {
    double **data;
    int rows;
    int cols;
};

int matrix_init(struct matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * sizeof(double *));
    if (!m->data) return 0;
    for (int i = 0; i < rows; i++) {
        m->data[i] = malloc(cols * sizeof(double));
        if (!m->data[i]) {
            for (int j = 0; j < i; j++) free(m->data[j]);
            free(m->data);
            return 0;
        }
    }
    return 1;
}

void matrix_free(struct matrix *m) {
    if (m && m->data) {
        for (int i = 0; i < m->rows; i++) free(m->data[i]);
        free(m->data);
        m->data = NULL;
        m->rows = 0;
        m->cols = 0;
    }
}

int matrix_read(struct matrix *m) {
    if (!m || !m->data) return 0;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void matrix_print(const struct matrix *m) {
    if (!m || !m->data) return;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (!a || !b || !result || !a->data || !b->data) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int matrix_subtract(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (!a || !b || !result || !a->data || !b->data) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return 1;
}

int matrix_multiply(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (!a || !b || !result || !a->data || !b->data) return 0;
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

int matrix_transpose(const struct matrix *a, struct matrix *result) {
    if (!a || !result || !a->data) return 0;
    if (!matrix_init(result, a->cols, a->rows)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[j][i] = a->data[i][j];
        }
    }
    return 1;
}

double matrix_determinant(const struct matrix *m) {
    if (!m || !m->data) return NAN;
    if (m->rows != m->cols) return NAN;
    if (m->rows == 1) return m->data[0][0];
    if (m->rows == 2) {
        return m->data[0][0] * m->data[1][1] - m