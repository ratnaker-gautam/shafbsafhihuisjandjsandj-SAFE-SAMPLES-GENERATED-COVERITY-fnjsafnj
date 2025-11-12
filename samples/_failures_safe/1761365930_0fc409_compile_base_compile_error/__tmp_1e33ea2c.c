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

int allocate_matrix(struct Matrix *mat, int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * sizeof(double*));
    if (!mat->data) return 0;
    for (int i = 0; i < rows; i++) {
        mat->data[i] = malloc(cols * sizeof(double));
        if (!mat->data[i]) {
            for (int j = 0; j < i; j++) free(mat->data[j]);
            free(mat->data);
            return 0;
        }
    }
    return 1;
}

void free_matrix(struct Matrix *mat) {
    if (mat && mat->data) {
        for (int i = 0; i < mat->rows; i++) free(mat->data[i]);
        free(mat->data);
    }
}

int read_matrix(struct Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%lf", &mat->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(struct Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i][j]);
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
        for (int i = 1; i < a->rows; i++) {
            int subcol = 0;
            for (int j = 0; j < a->cols; j++) {
                if (j == col) continue;
                sub.data[i-1][subcol++] = a->data[i][j];
            }
        }
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        double sub