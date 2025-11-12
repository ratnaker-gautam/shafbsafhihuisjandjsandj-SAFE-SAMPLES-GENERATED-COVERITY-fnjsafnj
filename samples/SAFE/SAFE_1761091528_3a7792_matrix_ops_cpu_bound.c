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
        
        char* endptr;
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (val < min || val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)val;
        break;
    }
    
    return value;
}

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
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

void matrix_transpose(const Matrix* src, Matrix* dst) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

void fill_matrix_random(Matrix* mat) {
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

int main(void) {
    srand(42);
    
    printf("Matrix Operations Benchmark\n");
    printf("===========================\n\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    if (rows1 == -1) return 1;
    
    int cols1 = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    if (cols1 == -1) return 1;
    
    int rows2 = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    if (rows2 == -1) return 1;
    
    int cols2 = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    if (cols2 == -1) return 1;
    
    if (cols1 != rows2) {
        printf("Error: Matrix A columns must equal Matrix B rows for multiplication.\n");
        return 1;
    }
    
    Matrix matA = {rows1, cols1};
    Matrix matB = {rows2, cols2};
    Matrix matResult = {rows1, cols2};
    Matrix matTranspose = {cols1, rows1};
    
    fill_matrix_random(&matA);
    fill_matrix_random(&matB);
    
    printf("\nMatrix A:\n");
    print_matrix(&matA);
    
    printf("\nMatrix B:\n");
    print_matrix(&matB);
    
    printf("\nMatrix Multiplication Result:\n");
    matrix_multiply(&matA, &matB, &matResult);
    print_matrix(&matResult);
    
    printf("\nMatrix A Transpose:\n");
    matrix_transpose(&matA, &matTranspose);
    print_matrix(&matTranspose);
    
    int iterations = read_int("\nEnter number of benchmark iterations (1-1000): ", 1, 1000);
    if (iterations == -1) return 1;
    
    printf("Running %d iterations of matrix operations...\n", iterations);
    
    for (int i = 0; i < iterations; i++) {
        matrix_multiply(&matA, &matB, &matResult);
        matrix_transpose(&matA, &matTranspose);
        matrix_multiply(&matResult, &matTranspose, &matResult);
    }
    
    printf("Benchmark completed.\n");
    
    return 0;
}