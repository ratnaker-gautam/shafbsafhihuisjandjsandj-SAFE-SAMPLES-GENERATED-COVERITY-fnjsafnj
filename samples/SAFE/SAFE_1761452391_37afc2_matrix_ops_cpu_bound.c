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

void transpose_matrix(const Matrix* input, Matrix* result) {
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    
    if (!validate_matrix_dim(mat1.rows) || !validate_matrix_dim(mat1.cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    
    mat2.rows = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    
    if (!validate_matrix_dim(mat2.rows) || !validate_matrix_dim(mat2.cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    
    srand(42);
    init_matrix(&mat1);
    init_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    if (mat1.cols == mat2.rows) {
        result.rows = mat1.rows;
        result.cols = mat2.cols;
        multiply_matrices(&mat1, &mat2, &result);
        printf("\nMatrix Multiplication Result:\n");
        print_matrix(&result);
    } else {
        printf("\nMatrices cannot be multiplied (columns of matrix 1 != rows of matrix 2)\n");
    }
    
    if (mat1.rows == mat1.cols) {
        Matrix transposed;
        transposed.rows = mat1.cols;
        transposed.cols = mat1.rows;
        transpose_matrix(&mat1, &transposed);
        printf("\nTranspose of Matrix 1:\n");
        print_matrix(&transposed);
    } else {
        printf("\nMatrix 1 is not square, cannot transpose\n");
    }
    
    if (mat2.rows == mat2.cols) {
        Matrix transposed;
        transposed.rows = mat2.cols;
        transposed.cols = mat2.rows;
        transpose_matrix(&mat2, &transposed);
        printf("\nTranspose of Matrix 2:\n");
        print_matrix(&transposed);
    } else {
        printf("\nMatrix 2 is not square, cannot transpose\n");
    }
    
    return 0;
}