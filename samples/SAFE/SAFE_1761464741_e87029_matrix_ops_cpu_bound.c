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
            mat->data[i][j] = 0;
        }
    }
}

void read_matrix(Matrix* mat, const char* name) {
    printf("Enter values for matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > 0 && sum > LONG_MAX - product) {
                    printf("Integer overflow detected in multiplication.\n");
                    exit(1);
                }
                if (product < 0 && sum < LONG_MIN - product) {
                    printf("Integer overflow detected in multiplication.\n");
                    exit(1);
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Result exceeds integer range.\n");
                exit(1);
            }
            result->data[i][j] = (int)sum;
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    a.rows = read_int("Enter rows for matrix A: ", 1, MAX_DIM);
    a.cols = read_int("Enter columns for matrix A: ", 1, MAX_DIM);
    
    b.rows = read_int("Enter rows for matrix B: ", 1, MAX_DIM);
    b.cols = read_int("Enter columns for matrix B: ", 1, MAX_DIM);
    
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
    initialize_matrix(&result);
    
    read_matrix(&a, "A");
    read_matrix(&b, "B");
    
    printf("\nInput matrices:\n");
    print_matrix(&a, "A");
    print_matrix(&b, "B");
    
    printf("\nComputing A × B...\n");
    multiply_matrices(&a, &b, &result);
    
    printf("\nResult:\n");
    print_matrix(&result, "A × B");
    
    return 0;
}