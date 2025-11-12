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
    int *data;
};

int matrix_init(struct Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > 100 || cols > 100) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(int));
    return m->data != NULL;
}

void matrix_free(struct Matrix *m) {
    free(m->data);
    m->data = NULL;
}

int matrix_get(struct Matrix *m, int row, int col) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) return 0;
    return m->data[row * m->cols + col];
}

void matrix_set(struct Matrix *m, int row, int col, int value) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) return;
    m->data[row * m->cols + col] = value;
}

int matrix_read(struct Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) return 0;
    if (!matrix_init(m, m->rows, m->cols)) return 0;
    printf("Enter %d elements row-wise:\n", m->rows * m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            int val;
            if (scanf("%d", &val) != 1) {
                matrix_free(m);
                return 0;
            }
            matrix_set(m, i, j, val);
        }
    }
    return 1;
}

void matrix_print(struct Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", matrix_get(m, i, j));
        }
        printf("\n");
    }
}

int matrix_add(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int val_a = matrix_get(a, i, j);
            int val_b = matrix_get(b, i, j);
            if ((val_a > 0 && val_b > INT_MAX - val_a) || (val_a < 0 && val_b < INT_MIN - val_a)) {
                matrix_free(result);
                return 0;
            }
            matrix_set(result, i, j, val_a + val_b);
        }
    }
    return 1;
}

int matrix_subtract(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int val_a = matrix_get(a, i, j);
            int val_b = matrix_get(b, i, j);
            if ((val_b > 0 && val_a < INT_MIN + val_b) || (val_b < 0 && val_a > INT_MAX + val_b)) {
                matrix_free(result);
                return 0;
            }
            matrix_set(result, i, j, val_a - val_b);
        }
    }
    return 1;
}

int matrix_multiply(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int val_a = matrix_get(a, i, k);
                int val_b = matrix_get(b, k, j);
                long long prod = (long long)val_a * val_b;
                if (val_a != 0 && prod / val_a != val_b) {
                    matrix_free(result);
                    return 0;
                }
                if ((prod > 0 && sum > INT_MAX - prod) || (prod < 0 && sum < INT_MIN - prod)) {
                    matrix_free(result);
                    return 0;
                }
                sum += (int)prod;
            }
            matrix_set(result, i, j, sum);
        }
    }
    return 1;
}

int matrix_trans