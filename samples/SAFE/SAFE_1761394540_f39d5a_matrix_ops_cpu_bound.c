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

void transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    a.rows = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    a.cols = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    
    if (!validate_matrix_dim(a.rows) || !validate_matrix_dim(a.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    b.rows = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    b.cols = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    
    if (!validate_matrix_dim(b.rows) || !validate_matrix_dim(b.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    srand(42);
    initialize_matrix(&a);
    initialize_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nComputing matrix multiplication...\n");
    if (multiply_matrices(&a, &b, &result)) {
        printf("Result of A * B:\n");
        print_matrix(&result);
    } else {
        printf("Matrix multiplication failed: dimension mismatch or overflow.\n");
    }
    
    printf("\nComputing transpose of A...\n");
    transpose_matrix(&a, &result);
    printf("Transpose of A:\n");
    print_matrix(&result);
    
    printf("\nComputing transpose of B...\n");
    transpose_matrix(&b, &result);
    printf("Transpose of B:\n");
    print_matrix(&result);
    
    return 0;
}