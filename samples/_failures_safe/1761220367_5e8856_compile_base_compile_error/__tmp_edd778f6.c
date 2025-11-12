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
    if (rows == 0 || cols == 0 || rows > 100 || cols > 100) return 0;
    if (SIZE_MAX / rows < cols) return 0;
    m->data = malloc(rows * cols * sizeof(int));
    if (!m->data) return 0;
    m->rows = rows;
    m->cols = cols;
    return 1;
}

void matrix_free(struct matrix *m) {
    if (m && m->data) free(m->data);
}

int matrix_get(const struct matrix *m, size_t row, size_t col) {
    if (!m || !m->data || row >= m->rows || col >= m->cols) return 0;
    return m->data[row * m->cols + col];
}

int matrix_set(struct matrix *m, size_t row, size_t col, int value) {
    if (!m || !m->data || row >= m->rows || col >= m->cols) return 0;
    m->data[row * m->cols + col] = value;
    return 1;
}

int matrix_read(struct matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%zu %zu", &m->rows, &m->cols) != 2) return 0;
    if (!matrix_init(m, m->rows, m->cols)) return 0;
    printf("Enter %zu elements row-wise:\n", m->rows * m->cols);
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            int val;
            if (scanf("%d", &val) != 1) return 0;
            matrix_set(m, i, j, val);
        }
    }
    return 1;
}

void matrix_print(const struct matrix *m) {
    if (!m || !m->data) return;
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%6d", matrix_get(m, i, j));
        }
        printf("\n");
    }
}

int matrix_add(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (!a || !b || !result || a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            int aval = matrix_get(a, i, j);
            int bval = matrix_get(b, i, j);
            if ((aval > 0 && bval > INT_MAX - aval) || (aval < 0 && bval < INT_MIN - aval)) {
                matrix_free(result);
                return 0;
            }
            int sum = aval + bval;
            matrix_set(result, i, j, sum);
        }
    }
    return 1;
}

int matrix_subtract(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (!a || !b || !result || a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            int aval = matrix_get(a, i, j);
            int bval = matrix_get(b, i, j);
            if ((bval > 0 && aval < INT_MIN + bval) || (bval < 0 && aval > INT_MAX + bval)) {
                matrix_free(result);
                return 0;
            }
            int diff = aval - bval;
            matrix_set(result, i, j, diff);
        }
    }
    return 1;
}

int matrix_multiply(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (!a || !b || !result || a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            int sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                int aval = matrix_get(a, i, k);
                int bval = matrix_get(b, k, j);
                if (aval != 0 && bval != 0) {
                    if ((aval > 0 && bval > 0 && aval >