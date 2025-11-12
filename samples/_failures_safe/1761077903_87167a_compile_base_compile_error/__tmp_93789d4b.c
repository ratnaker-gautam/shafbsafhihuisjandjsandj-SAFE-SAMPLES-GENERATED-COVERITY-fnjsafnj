//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

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
    int32_t *data;
};

int validate_matrix(const struct Matrix *m) {
    if (m == NULL || m->data == NULL) return 0;
    if (m->rows == 0 || m->cols == 0) return 0;
    if (SIZE_MAX / m->rows < m->cols) return 0;
    return 1;
}

int matrix_init(struct Matrix *m, size_t rows, size_t cols) {
    if (m == NULL || rows == 0 || cols == 0) return 0;
    if (SIZE_MAX / rows < cols) return 0;
    
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(int32_t));
    return m->data != NULL;
}

void matrix_free(struct Matrix *m) {
    if (m != NULL) {
        free(m->data);
        m->data = NULL;
        m->rows = 0;
        m->cols = 0;
    }
}

int matrix_read(struct Matrix *m) {
    if (m == NULL) return 0;
    
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%zu %zu", &m->rows, &m->cols) != 2) return 0;
    if (m->rows == 0 || m->cols == 0) return 0;
    if (SIZE_MAX / m->rows < m->cols) return 0;
    
    if (!matrix_init(m, m->rows, m->cols)) return 0;
    
    printf("Enter %zu elements row by row:\n", m->rows * m->cols);
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i * m->cols + j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void matrix_print(const struct Matrix *m) {
    if (!validate_matrix(m)) return;
    
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

int matrix_add(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!validate_matrix(a) || !validate_matrix(b) || result == NULL) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            int64_t sum = (int64_t)a->data[i * a->cols + j] + b->data[i * b->cols + j];
            if (sum > INT32_MAX || sum < INT32_MIN) {
                matrix_free(result);
                return 0;
            }
            result->data[i * result->cols + j] = (int32_t)sum;
        }
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!validate_matrix(a) || !validate_matrix(b) || result == NULL) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            int64_t diff = (int64_t)a->data[i * a->cols + j] - b->data[i * b->cols + j];
            if (diff > INT32_MAX || diff < INT32_MIN) {
                matrix_free(result);
                return 0;
            }
            result->data[i * result->cols + j] = (int32_t)diff;
        }
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (!validate_matrix(a) || !validate_matrix(b) || result == NULL) return 0;
    if (a->cols != b->rows) return 0;
    
    if (!matrix_init(result, a->rows, b->cols)) return 0;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            int64_t sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                int64_t product = (int64_t)a->data[i * a->cols + k] * b->