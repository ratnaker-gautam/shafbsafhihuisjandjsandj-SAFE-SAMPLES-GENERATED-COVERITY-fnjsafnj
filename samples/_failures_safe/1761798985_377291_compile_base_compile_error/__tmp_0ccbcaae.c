//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
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
    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

int matrix_read(struct Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%zu %zu", &m->rows, &m->cols) != 2) return 0;
    if (m->rows == 0 || m->cols == 0) return 0;
    
    if (!matrix_init(m, m->rows, m->cols)) return 0;
    
    printf("Enter %zu elements row-wise:\n", m->rows * m->cols);
    for (size_t i = 0; i < m->rows * m->cols; i++) {
        if (scanf("%lf", &m->data[i]) != 1) {
            matrix_free(m);
            return 0;
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
    
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!matrix_init(result, a->rows, a->cols)) return 0;
    
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] - b->data[i];
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
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[j * result->cols + i] = a->data[i * a->cols + j];
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
    if (choice < 0 || choice > 3) return OP_INVALID;
    return (enum Operation)choice;
}

int main(void) {
    struct Matrix a, b, result;
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    memset(&result, 0, sizeof(result));
    
    enum Operation op = get_operation();
    if (op == OP_INVALID) {
        printf("Invalid operation choice.\n");
        return 1;
    }
    
    printf("Matrix A:\n");
    if (!matrix_read(&a)) {
        printf("Failed to read matrix A.\n");
        return 1;
    }
    
    if (op != OP_TRANSPOSE) {
        printf("Matrix B:\n");
        if (!matrix_read(&b)) {
            printf