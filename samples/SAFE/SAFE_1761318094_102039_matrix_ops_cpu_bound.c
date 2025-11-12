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

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        return 0;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    
    return 1;
}

int transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
    
    return 1;
}

int main() {
    srand(42);
    
    printf("Matrix Operations Program\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    
    if (rows1 == -1 || cols1 == -1) {
        return 1;
    }
    
    int rows2 = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    
    if (rows2 == -1 || cols2 == -1) {
        return 1;
    }
    
    Matrix matA = {rows1, cols1};
    Matrix matB = {rows2, cols2};
    
    initialize_matrix(&matA);
    initialize_matrix(&matB);
    
    printf("\nMatrix A:\n");
    print_matrix(&matA);
    
    printf("\nMatrix B:\n");
    print_matrix(&matB);
    
    Matrix result;
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&matA, &matB, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply matrices: dimension mismatch or overflow risk.\n");
    }
    
    printf("\nTranspose of Matrix A:\n");
    if (transpose_matrix(&matA, &result)) {
        print_matrix(&result);
    }
    
    printf("\nTranspose of Matrix B:\n");
    if (transpose_matrix(&matB, &result)) {
        print_matrix(&result);
    }
    
    return 0;
}