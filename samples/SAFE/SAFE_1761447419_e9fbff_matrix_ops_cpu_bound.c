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
            printf("%4d", mat->data[i][j]);
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
    
    mat1.rows = read_int("Enter rows for first matrix (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for first matrix (1-100): ", 1, MAX_DIM);
    
    printf("\nFirst matrix dimensions: %dx%d\n", mat1.rows, mat1.cols);
    initialize_matrix(&mat1);
    printf("First matrix (random values):\n");
    print_matrix(&mat1);
    
    printf("\n");
    mat2.rows = read_int("Enter rows for second matrix (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for second matrix (1-100): ", 1, MAX_DIM);
    
    printf("\nSecond matrix dimensions: %dx%d\n", mat2.rows, mat2.cols);
    initialize_matrix(&mat2);
    printf("Second matrix (random values):\n");
    print_matrix(&mat2);
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        printf("Result of multiplication:\n");
        print_matrix(&result);
    } else {
        printf("Cannot multiply: incompatible dimensions (%dx%d and %dx%d)\n",
               mat1.rows, mat1.cols, mat2.rows, mat2.cols);
    }
    
    printf("\nMatrix Transposition:\n");
    printf("First matrix transposed:\n");
    transpose_matrix(&mat1, &result);
    print_matrix(&result);
    
    printf("\nSecond matrix transposed:\n");
    transpose_matrix(&mat2, &result);
    print_matrix(&result);
    
    return 0;
}