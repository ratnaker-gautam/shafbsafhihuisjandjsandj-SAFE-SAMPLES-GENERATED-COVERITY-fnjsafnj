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
    double *data;
};

int matrix_init(struct matrix *m, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0 || rows > SIZE_MAX / cols / sizeof(double)) {
        return 0;
    }
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(double));
    return m->data != NULL;
}

void matrix_free(struct matrix *m) {
    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

int matrix_read(struct matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%zu %zu", &m->rows, &m->cols) != 2) {
        return 0;
    }
    if (!matrix_init(m, m->rows, m->cols)) {
        return 0;
    }
    printf("Enter %zu elements row-wise:\n", m->rows * m->cols);
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i * m->cols + j]) != 1) {
                matrix_free(m);
                return 0;
            }
        }
    }
    return 1;
}

void matrix_print(const struct matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

int matrix_add(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    if (!matrix_init(result, a->rows, a->cols)) {
        return 0;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[i * a->cols + j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
        }
    }
    return 1;
}

int matrix_subtract(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    if (!matrix_init(result, a->rows, a->cols)) {
        return 0;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[i * a->cols + j] = a->data[i * a->cols + j] - b->data[i * a->cols + j];
        }
    }
    return 1;
}

int matrix_multiply(const struct matrix *a, const struct matrix *b, struct matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (!matrix_init(result, a->rows, b->cols)) {
        return 0;
    }
    for (size_t i = 0; i < result->rows; i++) {
        for (size_t j = 0; j < result->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const struct matrix *a, struct matrix *result) {
    if (!matrix_init(result, a->cols, a->rows)) {
        return 0;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[j * result->cols + i] = a->data[i * a->cols + j];
        }
    }
    return 1;
}

double matrix_determinant(const struct matrix *m) {
    if (m->rows != m->cols) {
        return 0.0;
    }
    if (m->rows == 1) {
        return m->data[0];
    }
    if (m->rows == 2) {
        return m->data[0] * m->data[3] - m->data[1] * m->data[2];
    }
    double det = 0.0;
    for (size_t j1 = 0; j1 < m->rows; j1++) {
        struct matrix sub;