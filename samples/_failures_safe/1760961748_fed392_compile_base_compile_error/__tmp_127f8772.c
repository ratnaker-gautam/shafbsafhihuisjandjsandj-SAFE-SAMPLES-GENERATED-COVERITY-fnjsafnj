//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

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
    int **data;
};

int validate_matrix(const struct Matrix *m) {
    if (m == NULL || m->data == NULL) return 0;
    if (m->rows <= 0 || m->cols <= 0) return 0;
    if (m->rows > 100 || m->cols > 100) return 0;
    return 1;
}

int allocate_matrix(struct Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > 100 || cols > 100) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * sizeof(int *));
    if (m->data == NULL) return 0;
    for (int i = 0; i < rows; i++) {
        m->data[i] = malloc(cols * sizeof(int));
        if (m->data[i] == NULL) {
            for (int j = 0; j < i; j++) free(m->data[j]);
            free(m->data);
            return 0;
        }
    }
    return 1;
}

void free_matrix(struct Matrix *m) {
    if (m == NULL || m->data == NULL) return;
    for (int i = 0; i < m->rows; i++) free(m->data[i]);
    free(m->data);
    m->data = NULL;
}

int read_matrix(struct Matrix *m) {
    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2) return 0;
    if (rows <= 0 || cols <= 0 || rows > 100 || cols > 100) return 0;
    if (!allocate_matrix(m, rows, cols)) return 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const struct Matrix *m) {
    if (!validate_matrix(m)) return;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!validate_matrix(a) || !validate_matrix(b)) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!allocate_matrix(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!validate_matrix(a) || !validate_matrix(b)) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!allocate_matrix(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((b->data[i][j] > 0 && a->data[i][j] < INT_MIN + b->data[i][j]) ||
                (b->data[i][j] < 0 && a->data[i][j] > INT_MAX + b->data[i][j])) {
                return 0;
            }
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!validate_matrix(a) || !validate_matrix(b)) return 0;
    if (a->cols != b->rows) return 0;
    if (!allocate_matrix(result, a->rows, b->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * b->