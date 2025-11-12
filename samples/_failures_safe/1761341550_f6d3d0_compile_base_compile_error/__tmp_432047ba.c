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
    double **data;
    int rows;
    int cols;
};

int allocate_matrix(struct Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * sizeof(double*));
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

void free_matrix(struct Matrix *m) {
    if (m && m->data) {
        for (int i = 0; i < m->rows; i++) free(m->data[i]);
        free(m->data);
    }
}

int read_matrix(struct Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) return 0;
    if (m->rows <= 0 || m->cols <= 0) return 0;
    if (!allocate_matrix(m, m->rows, m->cols)) return 0;
    printf("Enter %d elements row by row:\n", m->rows * m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(struct Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i][j]);
        }
        printf("\n");
    }
}

int add_matrices(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!allocate_matrix(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int subtract_matrices(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!allocate_matrix(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return 1;
}

int multiply_matrices(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!allocate_matrix(result, a->rows, b->cols)) return 0;
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

int transpose_matrix(struct Matrix *a, struct Matrix *result) {
    if (!allocate_matrix(result, a->cols, a->rows)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[j][i] = a->data[i][j];
        }
    }
    return 1;
}

double determinant_matrix(struct Matrix *a) {
    if (a->rows != a->cols) return NAN;
    if (a->rows == 1) return a->data[0][0];
    if (a->rows == 2) {
        return a->data[0][0] * a->data[1][1] - a->data[0][1] * a->data[1][0];
    }
    double det = 0.0;
    for (int col = 0; col < a->cols; col++) {
        struct Matrix sub;
        if (!allocate_matrix(&sub, a->rows - 1, a->cols - 1)) return NAN;
        for (