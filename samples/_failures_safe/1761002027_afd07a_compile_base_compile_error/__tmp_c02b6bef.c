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
    double *data;
};

int read_matrix(struct Matrix *m) {
    if (scanf("%zu %zu", &m->rows, &m->cols) != 2) {
        return 0;
    }
    if (m->rows == 0 || m->cols == 0 || m->rows > 100 || m->cols > 100) {
        return 0;
    }
    m->data = malloc(m->rows * m->cols * sizeof(double));
    if (m->data == NULL) {
        return 0;
    }
    for (size_t i = 0; i < m->rows * m->cols; i++) {
        if (scanf("%lf", &m->data[i]) != 1) {
            free(m->data);
            return 0;
        }
    }
    return 1;
}

void print_matrix(const struct Matrix *m) {
    printf("%zu %zu\n", m->rows, m->cols);
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%.2f ", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

int matrix_add(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    result->rows = a->rows;
    result->cols = a->cols;
    result->data = malloc(result->rows * result->cols * sizeof(double));
    if (result->data == NULL) {
        return 0;
    }
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
    return 1;
}

int matrix_subtract(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    result->rows = a->rows;
    result->cols = a->cols;
    result->data = malloc(result->rows * result->cols * sizeof(double));
    if (result->data == NULL) {
        return 0;
    }
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] - b->data[i];
    }
    return 1;
}

int matrix_multiply(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    result->rows = a->rows;
    result->cols = b->cols;
    result->data = calloc(result->rows * result->cols, sizeof(double));
    if (result->data == NULL) {
        return 0;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t k = 0; k < a->cols; k++) {
            for (size_t j = 0; j < b->cols; j++) {
                result->data[i * result->cols + j] += 
                    a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
        }
    }
    return 1;
}

int matrix_transpose(const struct Matrix *a, struct Matrix *result) {
    result->rows = a->cols;
    result->cols = a->rows;
    result->data = malloc(result->rows * result->cols * sizeof(double));
    if (result->data == NULL) {
        return 0;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[j * result->cols + i] = a->data[i * a->cols + j];
        }
    }
    return 1;
}

void cleanup_matrix(struct Matrix *m) {
    if (m->data != NULL) {
        free(m->data);
        m->data = NULL;
    }
}

int main(void) {
    struct Matrix a, b, result;
    int op_choice;
    
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    memset(&result, 0, sizeof(result));
    
    if (scanf("%d", &op_choice) != 1) {
        fprintf(stderr, "Invalid operation input\n");
        return 1;
    }
    
    enum Operation op;
    switch (op_choice) {
        case 0: op = OP_ADD; break;
        case 1: op = OP_SUBTRACT; break;
        case 2: op = OP_MULTIPLY; break;
        case 3: op = OP_TRANSPOSE; break;
        default: op = OP_INVALID; break;
    }
    
    if (op == OP_INVALID