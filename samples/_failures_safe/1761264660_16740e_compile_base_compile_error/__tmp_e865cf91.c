//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
    int *data;
};

int read_int(const char *prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        return value;
    }
}

int matrix_init(struct Matrix *m, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0 || rows > 100 || cols > 100) {
        return 0;
    }
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

void matrix_fill(struct Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("Enter element [%zu][%zu]: ", i, j);
            int val = read_int("", INT_MIN, INT_MAX);
            m->data[i * m->cols + j] = val;
        }
    }
}

void matrix_print(const struct Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%6d ", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

int matrix_add(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    if (!matrix_init(result, a->rows, a->cols)) {
        return 0;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            int sum = a->data[i * a->cols + j] + b->data[i * b->cols + j];
            if ((a->data[i * a->cols + j] > 0 && b->data[i * b->cols + j] > 0 && sum < 0) ||
                (a->data[i * a->cols + j] < 0 && b->data[i * b->cols + j] < 0 && sum > 0)) {
                matrix_free(result);
                return 0;
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    if (!matrix_init(result, a->rows, a->cols)) {
        return 0;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            int diff = a->data[i * a->cols + j] - b->data[i * b->cols + j];
            if ((a->data[i * a->cols + j] > 0 && b->data[i * b->cols + j] < 0 && diff < 0) ||
                (a->data[i * a->cols + j] < 0 && b->data[i * b->cols + j] > 0 && diff > 0)) {
                matrix_free(result);
                return 0;
            }
            result->data[i * result->cols + j] = diff;
        }
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (!matrix_init(result, a->rows, b->cols)) {
        return 0;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            int sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                int product = a->data[i * a->cols + k] * b->data[k * b->cols + j];
                if (a->data[i * a->cols + k] != 0 &&