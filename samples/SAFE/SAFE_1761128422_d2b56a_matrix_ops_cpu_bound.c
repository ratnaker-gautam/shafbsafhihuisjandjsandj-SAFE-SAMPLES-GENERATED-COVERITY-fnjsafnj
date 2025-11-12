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

void transpose_matrix(const Matrix* src, Matrix* dst) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

int main() {
    srand(42);
    
    printf("Matrix Operations\n");
    printf("=================\n\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    
    if (rows1 == -1 || cols1 == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    Matrix matA = {rows1, cols1};
    init_matrix(&matA);
    
    printf("\nMatrix A:\n");
    print_matrix(&matA);
    
    int rows2 = read_int("\nEnter rows for matrix B (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    
    if (rows2 == -1 || cols2 == -1) {
        printf("Input error.\n");
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
        printf("\nMatrices cannot be multiplied (A.cols != B.rows)\n");
    }
    
    if (matA.rows <= MAX_DIM && matA.cols <= MAX_DIM) {
        Matrix transA = {matA.cols, matA.rows};
        transpose_matrix(&matA, &transA);
        printf("\nTranspose of A:\n");
        print_matrix(&transA);
    }
    
    if (matB.rows <= MAX_DIM && matB.cols <= MAX_DIM) {
        Matrix transB = {matB.cols, matB.rows};
        transpose_matrix(&matB, &transB);
        printf("\nTranspose of B:\n");
        print_matrix(&transB);
    }
    
    return 0;
}