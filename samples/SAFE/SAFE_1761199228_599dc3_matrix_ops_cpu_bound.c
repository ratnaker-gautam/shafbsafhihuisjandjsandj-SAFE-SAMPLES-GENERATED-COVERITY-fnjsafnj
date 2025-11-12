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
            printf("%4d", mat->data[i][j]);
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
    
    printf("Matrix Operations Demo\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    
    if (rows1 == -1 || cols1 == -1) {
        return 1;
    }
    
    Matrix matA = {rows1, cols1};
    init_matrix(&matA);
    
    printf("\nMatrix A:\n");
    print_matrix(&matA);
    
    int rows2 = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    
    if (rows2 == -1 || cols2 == -1) {
        return 1;
    }
    
    Matrix matB = {rows2, cols2};
    init_matrix(&matB);
    
    printf("\nMatrix B:\n");
    print_matrix(&matB);
    
    if (matA.cols == matB.rows) {
        Matrix result = {matA.rows, matB.cols};
        multiply_matrices(&matA, &matB, &result);
        printf("\nMatrix A * B:\n");
        print_matrix(&result);
    } else {
        printf("\nCannot multiply: incompatible dimensions\n");
    }
    
    Matrix transposed = {matA.cols, matA.rows};
    transpose_matrix(&matA, &transposed);
    printf("\nTranspose of A:\n");
    print_matrix(&transposed);
    
    printf("\nPerforming intensive computation...\n");
    
    Matrix temp = {50, 50};
    init_matrix(&temp);
    
    for (int iter = 0; iter < 1000; iter++) {
        Matrix temp2 = {50, 50};
        transpose_matrix(&temp, &temp2);
        multiply_matrices(&temp, &temp2, &temp);
    }
    
    printf("Computation completed.\n");
    
    return 0;
}