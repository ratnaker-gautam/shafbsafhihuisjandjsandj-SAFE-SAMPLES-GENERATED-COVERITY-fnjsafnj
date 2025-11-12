//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
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

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void init_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = i * mat->cols + j + 1;
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

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dst) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

int main() {
    Matrix a, b, result, transposed;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    a.rows = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    if (a.rows == -1) return 1;
    
    a.cols = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    if (a.cols == -1) return 1;
    
    b.rows = a.cols;
    b.cols = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    if (b.cols == -1) return 1;
    
    if (!validate_dimension(a.rows) || !validate_dimension(a.cols) || 
        !validate_dimension(b.rows) || !validate_dimension(b.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    init_matrix(&a);
    init_matrix(&b);
    
    printf("\nMatrix A (%dx%d):\n", a.rows, a.cols);
    print_matrix(&a);
    
    printf("\nMatrix B (%dx%d):\n", b.rows, b.cols);
    print_matrix(&b);
    
    if (a.cols == b.rows) {
        result.rows = a.rows;
        result.cols = b.cols;
        multiply_matrices(&a, &b, &result);
        printf("\nMatrix A * B (%dx%d):\n", result.rows, result.cols);
        print_matrix(&result);
    } else {
        printf("\nMatrices cannot be multiplied (A.cols != B.rows)\n");
    }
    
    transposed.rows = a.cols;
    transposed.cols = a.rows;
    transpose_matrix(&a, &transposed);
    printf("\nTranspose of A (%dx%d):\n", transposed.rows, transposed.cols);
    print_matrix(&transposed);
    
    printf("\nOperations completed.\n");
    return 0;
}