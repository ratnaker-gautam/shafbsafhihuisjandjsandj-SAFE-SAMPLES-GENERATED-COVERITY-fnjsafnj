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
    int rows;
    int cols;
    int data[10][10];
};

int read_matrix(struct Matrix *m) {
    if (scanf("%d %d", &m->rows, &m->cols) != 2) return 0;
    if (m->rows <= 0 || m->rows > 10 || m->cols <= 0 || m->cols > 10) return 0;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const struct Matrix *m) {
    printf("%d %d\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int add_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    result->rows = a->rows;
    result->cols = a->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int subtract_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    result->rows = a->rows;
    result->cols = a->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return 1;
}

int multiply_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    result->rows = a->rows;
    result->cols = b->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

void transpose_matrix(const struct Matrix *a, struct Matrix *result) {
    result->rows = a->cols;
    result->cols = a->rows;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[j][i] = a->data[i][j];
        }
    }
}

enum Operation get_operation(void) {
    char input[16];
    if (scanf("%15s", input) != 1) return OP_INVALID;
    if (strcmp(input, "add") == 0) return OP_ADD;
    if (strcmp(input, "subtract") == 0) return OP_SUBTRACT;
    if (strcmp(input, "multiply") == 0) return OP_MULTIPLY;
    if (strcmp(input, "transpose") == 0) return OP_TRANSPOSE;
    return OP_INVALID;
}

int main(void) {
    struct Matrix a, b, result;
    enum Operation op = get_operation();
    if (op == OP_INVALID) {
        printf("Invalid operation\n");
        return 1;
    }
    
    if (op != OP_TRANSPOSE) {
        if (!read_matrix(&a) || !read_matrix(&b)) {
            printf("Invalid matrix input\n");
            return 1;
        }
    } else {
        if (!read_matrix(&a)) {
            printf("Invalid matrix input\n");
            return 1;
        }
    }
    
    int success = 1;
    switch (op) {
        case OP_ADD:
            success = add_matrices(&a, &b, &result);
            break;
        case OP_SUBTRACT:
            success = subtract_matrices(&a, &b, &result);
            break;
        case OP_MULTIPLY:
            success = multiply_matrices(&a, &b, &result);
            break;
        case OP_TRANSPOSE:
            transpose_matrix(&a, &result);
            success = 1;
            break;
        default:
            success = 0;
            break;
    }
    
    if (!success) {
        printf("Operation failed\n");