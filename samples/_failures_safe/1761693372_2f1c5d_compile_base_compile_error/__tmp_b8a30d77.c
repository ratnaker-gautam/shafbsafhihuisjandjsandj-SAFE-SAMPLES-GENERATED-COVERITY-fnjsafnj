//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum MatrixOp {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE
};

struct Matrix {
    size_t rows;
    size_t cols;
    double *data;
};

int matrix_init(struct Matrix *m, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(double));
    return m->data != NULL;
}

void matrix_free(struct Matrix *m) {
    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

int matrix_get(const struct Matrix *m, size_t row, size_t col, double *val) {
    if (row >= m->rows || col >= m->cols) return 0;
    *val = m->data[row * m->cols + col];
    return 1;
}

int matrix_set(struct Matrix *m, size_t row, size_t col, double val) {
    if (row >= m->rows || col >= m->cols) return 0;
    m->data[row * m->cols + col] = val;
    return 1;
}

int matrix_add(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            double val1, val2;
            if (!matrix_get(a, i, j, &val1) || !matrix_get(b, i, j, &val2)) {
                matrix_free(result);
                return 0;
            }
            if (!matrix_set(result, i, j, val1 + val2)) {
                matrix_free(result);
                return 0;
            }
        }
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            double val1, val2;
            if (!matrix_get(a, i, j, &val1) || !matrix_get(b, i, j, &val2)) {
                matrix_free(result);
                return 0;
            }
            if (!matrix_set(result, i, j, val1 - val2)) {
                matrix_free(result);
                return 0;
            }
        }
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                double val1, val2;
                if (!matrix_get(a, i, k, &val1) || !matrix_get(b, k, j, &val2)) {
                    matrix_free(result);
                    return 0;
                }
                sum += val1 * val2;
            }
            if (!matrix_set(result, i, j, sum)) {
                matrix_free(result);
                return 0;
            }
        }
    }
    return 1;
}

int matrix_transpose(const struct Matrix *a, struct Matrix *result) {
    if (!matrix_init(result, a->cols, a->rows)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            double val;
            if (!matrix_get(a, i, j, &val)) {
                matrix_free(result);
                return 0;
            }
            if (!matrix_set(result, j, i, val)) {
                matrix_free(result);
                return 0;
            }
        }
    }
    return 1;
}

void matrix_print(const struct Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            double val;
            if (matrix_get(m, i, j, &val)) {
                printf("%8.2f", val);
            }
        }
        printf("\n");
    }
}

int main(void) {
    struct Matrix mat1, mat2, result;
    size_t rows1, cols1, rows2, cols2;
    int op_choice;
    
    printf("Enter dimensions for matrix 1 (rows cols): ");
    if (scanf("%zu %zu", &rows1, &