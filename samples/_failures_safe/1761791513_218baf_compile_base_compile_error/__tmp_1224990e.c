//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
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
    size_t rows;
    size_t cols;
    int *data;
};

int matrix_init(struct matrix *m, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0 || rows > SIZE_MAX / cols) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = calloc(rows * cols, sizeof(int));
    return m->data != NULL;
}

void matrix_free(struct matrix *m) {
    if (m != NULL) free(m->data);
}

int matrix_get(const struct matrix *m, size_t row, size_t col) {
    if (row >= m->rows || col >= m->cols) return 0;
    return m->data[row * m->cols + col];
}

void matrix_set(struct matrix *m, size_t row, size_t col, int value) {
    if (row < m->rows && col < m->cols) m->data[row * m->cols + col] = value;
}

int matrix_read(struct matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%zu %zu", &m->rows, &m->cols) != 2) return 0;
    if (m->rows == 0 || m->cols == 0 || m->rows > SIZE_MAX / m->cols) return 0;
    if (!matrix_init(m, m->rows, m->cols)) return 0;
    printf("Enter %zu elements row-wise:\n", m->rows * m->cols);
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            int value;
            if (scanf("%d", &value) != 1) return 0;
            matrix_set(m, i, j, value);
        }
    }
    return 1;
}

void matrix_print(const struct matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%6d", matrix_get(m, i, j));
        }
        printf("\n");
    }
}

int matrix_add(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
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
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
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
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            int sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                int val_a = matrix_get(a, i, k);
                int val_b = matrix_get(b, k, j);
                int prod = val_a * val_b;
                if (val_a != 0 && prod / val_a != val_b) {
                    matrix_free(result);
                    return 0;
                }
                if ((prod > 0 && sum > INT_MAX - prod) || (prod < 0 && sum < INT_MIN - prod)) {
                    matrix_free(result);
                    return 0;
                }
                sum += prod;
            }
            matrix_set(result, i, j