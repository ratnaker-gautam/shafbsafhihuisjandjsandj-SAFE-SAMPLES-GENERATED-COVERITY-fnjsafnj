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

int validate_dimension(int dim) {
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
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
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
            snprintf(prompt, sizeof(prompt), "  [%d,%d]: ", i, j);
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
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    printf("Integer overflow detected in multiplication.\n");
                    exit(1);
                }
                sum += product;
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

int main(void) {
    Matrix a, b, result;
    
    printf("Matrix Multiplication Calculator\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    a.rows = read_int("Enter rows for matrix A: ", 1, MAX_DIM);
    a.cols = read_int("Enter columns for matrix A: ", 1, MAX_DIM);
    
    b.rows = read_int("Enter rows for matrix B: ", 1, MAX_DIM);
    b.cols = read_int("Enter columns for matrix B: ", 1, MAX_DIM);
    
    if (a.cols != b.rows) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication.\n", a.cols, b.rows);
        return 1;
    }
    
    result.rows = a.rows;
    result.cols = b.cols;
    initialize_matrix(&result);
    
    read_matrix(&a, "A");
    read_matrix(&b, "B");
    
    printf("\nPerforming matrix multiplication...\n");
    multiply_matrices(&a, &b, &result);
    
    printf("\n");
    print_matrix(&a, "A");
    print_matrix(&b, "B");
    print_matrix(&result, "A x B");
    
    return 0;
}