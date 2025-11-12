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
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

void initialize_matrix(Matrix* mat) {
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

void transpose_matrix(const Matrix* input, Matrix* result) {
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

int main() {
    srand(42);
    
    printf("Matrix Operations Benchmark\n");
    printf("===========================\n\n");
    
    int size = read_int("Enter matrix size (1-100): ", 1, MAX_DIM);
    if (size == -1) {
        return 1;
    }
    
    Matrix a, b, result;
    a.rows = size;
    a.cols = size;
    b.rows = size;
    b.cols = size;
    result.rows = size;
    result.cols = size;
    
    printf("\nInitializing matrices...\n");
    initialize_matrix(&a);
    initialize_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nComputing matrix multiplication...\n");
    multiply_matrices(&a, &b, &result);
    printf("Result of A * B:\n");
    print_matrix(&result);
    
    printf("\nComputing transpose of A...\n");
    Matrix transposed;
    transposed.rows = a.cols;
    transposed.cols = a.rows;
    transpose_matrix(&a, &transposed);
    printf("Transpose of A:\n");
    print_matrix(&transposed);
    
    printf("\nPerforming additional operations for benchmarking...\n");
    int iterations = 100;
    for (int iter = 0; iter < iterations; iter++) {
        multiply_matrices(&a, &b, &result);
        transpose_matrix(&a, &transposed);
    }
    
    printf("Benchmark completed.\n");
    
    return 0;
}