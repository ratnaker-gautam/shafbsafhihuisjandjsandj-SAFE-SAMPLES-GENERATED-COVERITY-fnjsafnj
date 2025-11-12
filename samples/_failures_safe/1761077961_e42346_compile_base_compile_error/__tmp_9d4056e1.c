//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

enum matrix_op {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

struct matrix {
    int rows;
    int cols;
    int *data;
};

int matrix_init(struct matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > 100 || cols > 100) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(int));
    return m->data != NULL;
}

void matrix_free(struct matrix *m) {
    free(m->data);
    m->data = NULL;
}

int matrix_get(const struct matrix *m, int row, int col) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) return 0;
    return m->data[row * m->cols + col];
}

void matrix_set(struct matrix *m, int row, int col, int value) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) return;
    m->data[row * m->cols + col] = value;
}

int matrix_read(struct matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) return 0;
    if (!matrix_init(m, m->rows, m->cols)) return 0;
    printf("Enter %d elements row-wise:\n", m->rows * m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i * m->cols + j]) != 1) {
                matrix_free(m);
                return 0;
            }
        }
    }
    return 1;
}

void matrix_print(const struct matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", matrix_get(m, i, j));
        }
        printf("\n");
    }
}

int matrix_add(const struct matrix *a, const struct matrix *b, struct matrix *result) {
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

int matrix_subtract(const struct matrix *a, const struct matrix *b, struct matrix *result) {
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

int matrix_multiply(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)matrix_get(a, i, k) * matrix_get(b, k, j);
                if (product > INT_MAX || product < INT_MIN) {
                    matrix_free(result);
                    return 0;
                }
                long long new_sum = (long long)sum + product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) {
                    matrix_free(result);
                    return 0;
                }
                sum = (int)new_sum;
            }
            matrix_set(result, i, j, sum);
        }
    }
    return 1;
}

int matrix_transpose(const struct matrix *m, struct matrix *result) {
    if (!