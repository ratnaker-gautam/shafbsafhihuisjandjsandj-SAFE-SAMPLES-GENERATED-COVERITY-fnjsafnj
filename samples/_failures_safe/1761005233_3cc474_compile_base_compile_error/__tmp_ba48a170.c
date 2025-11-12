//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_INVALID
};

struct Matrix {
    size_t rows;
    size_t cols;
    int *data;
};

int matrix_init(struct Matrix *m, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(int));
    return m->data != NULL;
}

void matrix_free(struct Matrix *m) {
    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

int matrix_get(const struct Matrix *m, size_t row, size_t col) {
    if (row >= m->rows || col >= m->cols) return 0;
    return m->data[row * m->cols + col];
}

void matrix_set(struct Matrix *m, size_t row, size_t col, int value) {
    if (row < m->rows && col < m->cols) {
        m->data[row * m->cols + col] = value;
    }
}

int matrix_read(struct Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%zu %zu", &m->rows, &m->cols) != 2) return 0;
    if (m->rows == 0 || m->cols == 0) return 0;
    if (!matrix_init(m, m->rows, m->cols)) return 0;
    
    printf("Enter %zu elements row by row:\n", m->rows * m->cols);
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i * m->cols + j]) != 1) {
                matrix_free(m);
                return 0;
            }
        }
    }
    return 1;
}

void matrix_print(const struct Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%6d", matrix_get(m, i, j));
        }
        printf("\n");
    }
}

int matrix_add(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            int sum = matrix_get(a, i, j) + matrix_get(b, i, j);
            matrix_set(result, i, j, sum);
        }
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            int diff = matrix_get(a, i, j) - matrix_get(b, i, j);
            matrix_set(result, i, j, diff);
        }
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            int sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += matrix_get(a, i, k) * matrix_get(b, k, j);
            }
            matrix_set(result, i, j, sum);
        }
    }
    return 1;
}

int matrix_transpose(const struct Matrix *a, struct Matrix *result) {
    if (!matrix_init(result, a->cols, a->rows)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            matrix_set(result, j, i, matrix_get(a, i, j));
        }
    }
    return 1;
}

enum Operation get_operation() {
    printf("Select operation:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    printf("Enter choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) return OP_INVALID;
    if (choice < 0 || choice > 3) return OP_IN