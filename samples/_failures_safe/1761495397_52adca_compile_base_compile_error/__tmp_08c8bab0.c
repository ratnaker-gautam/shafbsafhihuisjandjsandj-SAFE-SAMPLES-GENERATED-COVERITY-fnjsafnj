//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum MatrixOp {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE
};

struct Matrix {
    size_t rows;
    size_t cols;
    double *data;
};

int matrix_init(struct Matrix *m, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) return 0;
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(double));
    return m->data != NULL;
}

void matrix_free(struct Matrix *m) {
    if (m->data) free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

int matrix_read(struct Matrix *m) {
    printf("Enter matrix %zux%zu:\n", m->rows, m->cols);
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i * m->cols + j]) != 1) return 0;
        }
    }
    return 1;
}

void matrix_print(const struct Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

int matrix_add(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[i * result->cols + j] = a->data[i * a->cols + j] + b->data[i * b->cols + j];
        }
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[i * result->cols + j] = a->data[i * a->cols + j] - b->data[i * b->cols + j];
        }
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!matrix_init(result, a->rows, b->cols)) return 0;
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

int matrix_transpose(const struct Matrix *a, struct Matrix *result) {
    if (!matrix_init(result, a->cols, a->rows)) return 0;
    for (size_t i = 0; i < result->rows; i++) {
        for (size_t j = 0; j < result->cols; j++) {
            result->data[i * result->cols + j] = a->data[j * a->cols + i];
        }
    }
    return 1;
}

int main(void) {
    struct Matrix a, b, result;
    size_t rows, cols;
    int op_choice;
    
    printf("Matrix Operations\n");
    printf("1. Add\n2. Subtract\n3. Multiply\n4. Transpose\n");
    printf("Select operation (1-4): ");
    if (scanf("%d", &op_choice) != 1 || op_choice < 1 || op_choice > 4) {
        printf("Invalid operation choice\n");
        return 1;
    }
    
    enum MatrixOp op = op_choice - 1;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
        case OP_MULTIPLY:
            printf("Enter rows and columns for first matrix: ");
            if (scanf("%zu %zu", &rows, &cols) != 2 || rows == 0 || cols == 0) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            if (!matrix_init(&a, rows, cols)) {
                printf("Memory allocation failed\n");
                return 1;
            }
            if (!matrix_read(&a)) {
                printf("Failed to