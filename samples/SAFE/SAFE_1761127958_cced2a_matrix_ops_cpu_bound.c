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
    
    printf("Matrix Multiplication Program\n");
    printf("=============================\n\n");
    
    printf("Enter dimensions for first matrix:\n");
    mat1.rows = read_int("Rows (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Columns (1-100): ", 1, MAX_DIM);
    
    printf("\nEnter dimensions for second matrix:\n");
    mat2.rows = read_int("Rows (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Columns (1-100): ", 1, MAX_DIM);
    
    if (mat1.cols != mat2.rows) {
        printf("Error: Cannot multiply matrices. Columns of first matrix must equal rows of second matrix.\n");
        return 1;
    }
    
    if (!validate_matrix_dim(mat1.rows) || !validate_matrix_dim(mat1.cols) ||
        !validate_matrix_dim(mat2.rows) || !validate_matrix_dim(mat2.cols)) {
        printf("Error: Invalid matrix dimensions.\n");
        return 1;
    }
    
    result.rows = mat1.rows;
    result.cols = mat2.cols;
    
    srand(42);
    initialize_matrix(&mat1);
    initialize_matrix(&mat2);
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    multiply_matrices(&mat1, &mat2, &result);
    
    printf("\nResult matrix:\n");
    print_matrix(&result);
    
    int trace = 0;
    if (result.rows == result.cols) {
        for (int i = 0; i < result.rows; i++) {
            trace += result.data[i][i];
        }
        printf("\nTrace of result matrix: %d\n", trace);
    }
    
    return 0;
}