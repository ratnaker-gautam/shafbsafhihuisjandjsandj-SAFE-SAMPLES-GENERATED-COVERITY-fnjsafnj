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
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(int));
    return m->data != NULL;
}

void matrix_free(struct matrix *m) {
    if (m->data) free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

int matrix_get(struct matrix *m, int row, int col) {
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
    if (m->rows <= 0 || m->cols <= 0 || m->rows > 100 || m->cols > 100) return 0;
    if (!matrix_init(m, m->rows, m->cols)) return 0;
    printf("Enter %d elements row by row:\n", m->rows * m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            int value;
            if (scanf("%d", &value) != 1) return 0;
            matrix_set(m, i, j, value);
        }
    }
    return 1;
}

void matrix_print(struct matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", matrix_get(m, i, j));
        }
        printf("\n");
    }
}

int matrix_add(struct matrix *a, struct matrix *b, struct matrix *result) {
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

int matrix_subtract(struct matrix *a, struct matrix *b, struct matrix *result) {
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

int matrix_multiply(struct matrix *a, struct matrix *b, struct matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int val_a = matrix_get(a, i, k);
                int val_b = matrix_get(b, k, j);
                if (val_a != 0 && val_b != 0) {
                    if (val_a > 0) {
                        if (val_b > INT_MAX / val_a || val_b < INT_MIN / val_a) {
                            matrix_free(result);
                            return 0;
                        }
                    } else {
                        if (val_b > 0