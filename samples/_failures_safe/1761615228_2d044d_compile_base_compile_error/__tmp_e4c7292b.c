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
    int rows;
    int cols;
    int *data;
};

int matrix_init(struct matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > 100 || cols > 100) return 0;
    if (SIZE_MAX / sizeof(int) < (size_t)(rows * cols)) return 0;
    m->data = malloc(rows * cols * sizeof(int));
    if (!m->data) return 0;
    m->rows = rows;
    m->cols = cols;
    return 1;
}

void matrix_free(struct matrix *m) {
    if (m && m->data) {
        free(m->data);
        m->data = NULL;
    }
}

int matrix_get(struct matrix *m, int row, int col) {
    if (!m || !m->data || row < 0 || row >= m->rows || col < 0 || col >= m->cols) return 0;
    return m->data[row * m->cols + col];
}

int matrix_set(struct matrix *m, int row, int col, int value) {
    if (!m || !m->data || row < 0 || row >= m->rows || col < 0 || col >= m->cols) return 0;
    m->data[row * m->cols + col] = value;
    return 1;
}

void matrix_print(struct matrix *m) {
    if (!m || !m->data) return;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", matrix_get(m, i, j));
        }
        printf("\n");
    }
}

int matrix_add(struct matrix *a, struct matrix *b, struct matrix *result) {
    if (!a || !b || !result || a->rows != b->rows || a->cols != b->cols) return 0;
    if (a->rows != result->rows || a->cols != result->cols) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int val_a = matrix_get(a, i, j);
            int val_b = matrix_get(b, i, j);
            if ((val_a > 0 && val_b > INT_MAX - val_a) || (val_a < 0 && val_b < INT_MIN - val_a)) return 0;
            matrix_set(result, i, j, val_a + val_b);
        }
    }
    return 1;
}

int matrix_subtract(struct matrix *a, struct matrix *b, struct matrix *result) {
    if (!a || !b || !result || a->rows != b->rows || a->cols != b->cols) return 0;
    if (a->rows != result->rows || a->cols != result->cols) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int val_a = matrix_get(a, i, j);
            int val_b = matrix_get(b, i, j);
            if ((val_b > 0 && val_a < INT_MIN + val_b) || (val_b < 0 && val_a > INT_MAX + val_b)) return 0;
            matrix_set(result, i, j, val_a - val_b);
        }
    }
    return 1;
}

int matrix_multiply(struct matrix *a, struct matrix *b, struct matrix *result) {
    if (!a || !b || !result || a->cols != b->rows) return 0;
    if (result->rows != a->rows || result->cols != b->cols) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int val_a = matrix_get(a, i, k);
                int val_b = matrix_get(b, k, j);
                long long prod = (long long)val_a * val_b;
                if (prod > INT_MAX || prod < INT_MIN) return 0;
                if ((prod > 0 && sum > INT_MAX - (int)prod) || (prod < 0 && sum < INT_MIN - (int)prod)) return 0;
                sum += (int)prod;
            }
            matrix_set(result, i, j, sum);
        }
    }
    return 1;
}

int matrix_transpose(struct matrix *a, struct matrix *result) {
    if (!a || !result) return 0;
    if (result->rows != a->cols || result->cols != a->rows) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            matrix_set(result, j, i, matrix_get(a,