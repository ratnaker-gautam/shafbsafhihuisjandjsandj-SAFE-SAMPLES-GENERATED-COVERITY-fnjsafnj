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

int matrix_get(struct Matrix *m, int row, int col) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) return 0;
    return 1;
}

double matrix_at(struct Matrix *m, int row, int col) {
    if (!matrix_get(m, row, col)) return 0.0;
    return m->data[row * m->cols + col];
}

void matrix_set(struct Matrix *m, int row, int col, double val) {
    if (matrix_get(m, row, col)) {
        m->data[row * m->cols + col] = val;
    }
}

void matrix_print(struct Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", matrix_at(m, i, j));
        }
        printf("\n");
    }
}

int matrix_read(struct Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (scanf("%lf", &m->data[i * m->cols + j]) != 1) return 0;
        }
    }
    return 1;
}

int matrix_add(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            matrix_set(result, i, j, matrix_at(a, i, j) + matrix_at(b, i, j));
        }
    }
    return 1;
}

int matrix_subtract(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            matrix_set(result, i, j, matrix_at(a, i, j) - matrix_at(b, i, j));
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
                sum += matrix_at(a, i, k) * matrix_at(b, k, j);
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
            matrix_set(result, j, i, matrix_at(a, i, j));
        }
    }
    return 1;
}

double matrix_determinant(struct Matrix *a) {
    if (a->rows != a->cols) return 0.0;
    if (a->rows == 1) return matrix_at(a, 0, 0);
    if (a->rows == 2) {
        return matrix_at(a, 0, 0) * matrix_at(a, 1, 1) - 
               matrix_at(a, 0, 1) * matrix_at(a, 1, 0);
    }
    return 0.0;
}

int main() {
    struct Matrix mat1, mat2, result;
    int op_choice;
    int rows1, cols1, rows2, cols2;

    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols