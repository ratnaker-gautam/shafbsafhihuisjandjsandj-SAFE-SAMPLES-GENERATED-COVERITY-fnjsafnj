//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
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
    int data[10][10];
};

void print_matrix(const struct Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    long value;
    char *endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return INT_MIN;
    }
    
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return INT_MIN;
    }
    if (value < min || value > max) {
        return INT_MIN;
    }
    
    return (int)value;
}

int read_matrix(struct Matrix *m) {
    int rows = read_int("Enter number of rows (1-10): ", 1, 10);
    if (rows == INT_MIN) return 0;
    m->rows = (size_t)rows;
    
    int cols = read_int("Enter number of columns (1-10): ", 1, 10);
    if (cols == INT_MIN) return 0;
    m->cols = (size_t)cols;
    
    printf("Enter matrix elements row by row:\n");
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            int val = read_int("", INT_MIN + 1, INT_MAX - 1);
            if (val == INT_MIN) return 0;
            m->data[i][j] = val;
        }
    }
    return 1;
}

int add_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            long sum = (long)a->data[i][j] + (long)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int subtract_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            long diff = (long)a->data[i][j] - (long)b->data[i][j];
            if (diff > INT_MAX || diff < INT_MIN) return 0;
            result->data[i][j] = (int)diff;
        }
    }
    return 1;
}

int multiply_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            long sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) return 0;
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

void transpose_matrix(const struct Matrix *a, struct Matrix *result) {
    result->rows = a->cols;
    result->cols = a->rows;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result->data[j][i] = a->data[i][j];
        }
    }
}

enum Operation get_operation() {
    int choice = read_int("Choose operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose): ", 0, 3);
    if (choice == INT_MIN) return OP_INVALID;
    return (enum Operation)choice;
}

int main() {
    struct Matrix mat1