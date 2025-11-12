//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dim(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_int_safe(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long result;
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    
    result = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return -1;
    if (result < min_val || result > max_val) return -1;
    if (result > INT_MAX || result < INT_MIN) return -1;
    
    return (int)result;
}

void initialize_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements (%d x %d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int val = read_int_safe(prompt, -1000, 1000);
            if (val == -1) {
                printf("Invalid input. Using default value 0.\n");
                val = 0;
            }
            mat->data[i][j] = val;
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

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > 0 && sum > INT_MAX - product) return 0;
                if (product < 0 && sum < INT_MIN - product) return 0;
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix* src, Matrix* dst) {
    if (src->rows != dst->cols || src->cols != dst->rows) return 0;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result, transposed;
    
    printf("Matrix Operations Program\n");
    
    a.rows = read_int_safe("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    a.cols = read_int_safe("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    if (a.rows == -1 || a.cols == -1) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    b.rows = read_int_safe("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    b.cols = read_int_safe("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    if (b.rows == -1 || b.cols == -1) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    if (!validate_matrix_dim(a.rows, a.cols) || !validate_matrix_dim(b.rows, b.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    read_matrix(&a);
    read_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix Multiplication:\n");
    if (a.cols == b.rows) {
        result.rows = a.rows;
        result.cols = b.cols;
        initialize_matrix(&result);
        
        if (multiply_matrices(&a,