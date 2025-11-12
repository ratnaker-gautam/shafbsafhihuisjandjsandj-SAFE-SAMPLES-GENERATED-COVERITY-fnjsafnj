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
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = input->data[j][i];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    
    if (!validate_matrix_dim(mat1.rows) || !validate_matrix_dim(mat1.cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("\nMatrix 1 dimensions: %d x %d\n", mat1.rows, mat1.cols);
    init_matrix(&mat1);
    printf("Matrix 1 (random values):\n");
    print_matrix(&mat1);
    
    printf("\n");
    mat2.rows = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    
    if (!validate_matrix_dim(mat2.rows) || !validate_matrix_dim(mat2.cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("\nMatrix 2 dimensions: %d x %d\n", mat2.rows, mat2.cols);
    init_matrix(&mat2);
    printf("Matrix 2 (random values):\n");
    print_matrix(&mat2);
    
    printf("\nPerforming matrix operations...\n\n");
    
    printf("Transpose of Matrix 1:\n");
    transpose_matrix(&mat1, &result);
    print_matrix(&result);
    
    printf("\nTranspose of Matrix 2:\n");
    transpose_matrix(&mat2, &result);
    print_matrix(&result);
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        printf("Result of Matrix 1 * Matrix 2:\n");
        print_matrix(&result);
    } else {
        printf("Cannot multiply matrices: incompatible dimensions\n");
    }
    
    printf("\nMatrix multiplication (reverse order):\n");
    if (multiply_matrices(&mat2, &mat1, &result)) {
        printf("Result of Matrix 2 * Matrix 1:\n");
        print_matrix(&result);
    } else {
        printf("Cannot multiply matrices: incompatible dimensions\n");
    }
    
    return 0;
}