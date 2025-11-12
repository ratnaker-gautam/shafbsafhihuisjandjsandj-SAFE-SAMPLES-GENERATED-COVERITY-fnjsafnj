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

int validate_matrix_dim(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

void init_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = rand() % 100;
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

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Multiplication Benchmark\n");
    printf("===============================\n\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    if (mat1.rows == -1) return 1;
    
    mat1.cols = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    if (mat1.cols == -1) return 1;
    
    mat2.rows = mat1.cols;
    printf("Matrix 2 rows must match matrix 1 columns: %d\n", mat2.rows);
    
    mat2.cols = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    if (mat2.cols == -1) return 1;
    
    if (!validate_matrix_dim(mat1.rows) || !validate_matrix_dim(mat1.cols) ||
        !validate_matrix_dim(mat2.rows) || !validate_matrix_dim(mat2.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    if (mat1.cols != mat2.rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }
    
    result.rows = mat1.rows;
    result.cols = mat2.cols;
    
    printf("\nInitializing matrices with random values...\n");
    init_matrix(&mat1);
    init_matrix(&mat2);
    
    printf("\nMatrix 1 (%dx%d):\n", mat1.rows, mat1.cols);
    print_matrix(&mat1);
    
    printf("\nMatrix 2 (%dx%d):\n", mat2.rows, mat2.cols);
    print_matrix(&mat2);
    
    printf("\nPerforming matrix multiplication...\n");
    multiply_matrices(&mat1, &mat2, &result);
    
    printf("\nResult Matrix (%dx%d):\n", result.rows, result.cols);
    print_matrix(&result);
    
    printf("\nBenchmark complete.\n");
    return 0;
}