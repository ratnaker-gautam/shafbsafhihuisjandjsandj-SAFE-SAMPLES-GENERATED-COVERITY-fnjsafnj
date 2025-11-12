//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_integer(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer) continue;
        if (*endptr != '\n' && *endptr != '\0') continue;
        if (value < min_val || value > max_val) continue;
        if (value > INT_MAX || value < INT_MIN) continue;
        
        return (int)value;
    }
}

void read_matrix_element(Matrix* mat, int i, int j) {
    char prompt[64];
    snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
    mat->data[i][j] = read_integer(prompt, -1000, 1000);
}

void input_matrix(Matrix* mat) {
    mat->rows = read_integer("Enter number of rows (1-10): ", 1, MAX_DIM);
    mat->cols = read_integer("Enter number of columns (1-10): ", 1, MAX_DIM);
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            read_matrix_element(mat, i, j);
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int can_multiply(const Matrix* a, const Matrix* b) {
    return a->cols == b->rows;
}

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (!can_multiply(a, b)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = safe_multiply(a->data[i][k], b->data[k][j]);
                if (product == 0 && (a->data[i][k] != 0 || b->data[k][j] != 0)) {
                    return 0;
                }
                sum = safe_add(sum, product);
                if (sum == 0 && product != 0) {
                    return 0;
                }
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int can_add(const Matrix* a, const Matrix* b) {
    return a->rows == b->rows && a->cols == b->cols;
}

int add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (!can_add(a, b)) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int sum = safe_add(a->data[i][j], b->data[i][j]);
            if (sum == 0 && (a->data[i][j] != 0 || b->data[i][j] != 0)) {
                return 0;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i <