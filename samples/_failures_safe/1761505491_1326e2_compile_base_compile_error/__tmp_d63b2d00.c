//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
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

int read_matrix(struct Matrix *m) {
    if (scanf("%d %d", &m->rows, &m->cols) != 2) return 0;
    if (m->rows <= 0 || m->cols <= 0) return 0;
    if (m->rows > 100 || m->cols > 100) return 0;
    
    m->data = malloc(m->rows * sizeof(int*));
    if (m->data == NULL) return 0;
    
    for (int i = 0; i < m->rows; i++) {
        m->data[i] = malloc(m->cols * sizeof(int));
        if (m->data[i] == NULL) {
            for (int j = 0; j < i; j++) free(m->data[j]);
            free(m->data);
            return 0;
        }
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                for (int k = 0; k <= i; k++) free(m->data[k]);
                free(m->data);
                return 0;
            }
        }
    }
    return 1;
}

void free_matrix(struct Matrix *m) {
    if (m == NULL || m->data == NULL) return;
    for (int i = 0; i < m->rows; i++) free(m->data[i]);
    free(m->data);
}

int add_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!validate_matrix(a) || !validate_matrix(b)) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    result->data = malloc(result->rows * sizeof(int*));
    if (result->data == NULL) return 0;
    
    for (int i = 0; i < result->rows; i++) {
        result->data[i] = malloc(result->cols * sizeof(int));
        if (result->data[i] == NULL) {
            for (int j = 0; j < i; j++) free(result->data[j]);
            free(result->data);
            return 0;
        }
        for (int j = 0; j < result->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                for (int k = 0; k <= i; k++) free(result->data[k]);
                free(result->data);
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int subtract_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!validate_matrix(a) || !validate_matrix(b)) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    result->data = malloc(result->rows * sizeof(int*));
    if (result->data == NULL) return 0;
    
    for (int i = 0; i < result->rows; i++) {
        result->data[i] = malloc(result->cols * sizeof(int));
        if (result->data[i] == NULL) {
            for (int j = 0; j < i; j++) free(result->data[j]);
            free(result->data);
            return 0;
        }
        for (int j = 0; j < result->cols; j++) {
            if ((b->data[i][j] > 0 && a->data[i][j] < INT_MIN + b->data[i][j]) ||
                (b->data[i][j] < 0 && a->data[i][j] > INT_MAX + b->data[i][j])) {
                for (int k = 0; k <= i; k++) free(result->data[k]);
                free(result->data);
                return 0;
            }
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return 1;
}

int multiply_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!validate_matrix(a) || !validate_matrix(b)) return 0;
    if (a->cols