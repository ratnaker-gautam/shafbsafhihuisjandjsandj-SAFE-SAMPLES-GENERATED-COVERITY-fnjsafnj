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
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
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
    srand(42);
    
    printf("Matrix Operations (CPU Bound)\n");
    printf("=============================\n\n");
    
    Matrix a, b, result;
    
    printf("Matrix A dimensions:\n");
    a.rows = read_int("Rows: ", 1, MAX_DIM);
    a.cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("\nMatrix B dimensions:\n");
    b.rows = read_int("Rows: ", 1, MAX_DIM);
    b.cols = read_int("Columns: ", 1, MAX_DIM);
    
    if (a.cols != b.rows) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication.\n", a.cols, b.rows);
        return 1;
    }
    
    if (!validate_matrix_dim(a.rows) || !validate_matrix_dim(a.cols) ||
        !validate_matrix_dim(b.rows) || !validate_matrix_dim(b.cols)) {
        printf("Error: Invalid matrix dimensions.\n");
        return 1;
    }
    
    result.rows = a.rows;
    result.cols = b.cols;
    
    printf("\nInitializing matrices with random values...\n");
    initialize_matrix(&a);
    initialize_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMultiplying matrices (CPU intensive operation)...\n");
    multiply_matrices(&a, &b, &result);
    
    printf("\nResult (A × B):\n");
    print_matrix(&result);
    
    printf("\nOperation completed successfully.\n");
    return 0;
}