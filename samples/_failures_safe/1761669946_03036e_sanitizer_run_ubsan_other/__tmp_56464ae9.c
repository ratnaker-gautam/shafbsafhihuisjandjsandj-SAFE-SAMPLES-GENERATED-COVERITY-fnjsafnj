//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)
#define INPUT_BUFFER_SIZE 256

#define MATRIX_GET(m, r, c, cols) ((m)[(r) * (cols) + (c)])
#define MATRIX_SET(m, r, c, cols, val) ((m)[(r) * (cols) + (c)] = (val))

int read_int_safe(const char* prompt, int min_val, int max_val) {
    char buffer[INPUT_BUFFER_SIZE];
    char* endptr;
    long result;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        result = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (result < min_val || result > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)result;
    }
}

void read_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            MATRIX_SET(matrix, i, j, cols, read_int_safe("", INT_MIN, INT_MAX));
        }
    }
}

void print_matrix(const int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_GET(matrix, i, j, cols));
        }
        printf("\n");
    }
}

void multiply_matrices(const int* a, int a_rows, int a_cols,
                      const int* b, int b_rows, int b_cols,
                      int* result) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += MATRIX_GET(a, i, k, a_cols) * MATRIX_GET(b, k, j, b_cols);
            }
            MATRIX_SET(result, i, j, b_cols, sum);
        }
    }
}

int main(void) {
    int matrix_a[MATRIX_SIZE];
    int matrix_b[MATRIX_SIZE];
    int result[MATRIX_SIZE];
    
    printf("Matrix Multiplication Program\n");
    printf("=============================\n\n");
    
    int rows_a = read_int_safe("Enter rows for matrix A (1-10): ", 1, MAX_ROWS);
    int cols_a = read_int_safe("Enter columns for matrix A (1-10): ", 1, MAX_COLS);
    
    int rows_b = read_int_safe("Enter rows for matrix B (1-10): ", 1, MAX_ROWS);
    int cols_b = read_int_safe("Enter columns for matrix B (1-10): ", 1, MAX_COLS);
    
    if (cols_a != rows_b) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication.\n", cols_a, rows_b);
        return 1;
    }
    
    printf("\nEnter values for Matrix A (%dx%d):\n", rows_a, cols_a);
    read_matrix(matrix_a, rows_a, cols_a);
    
    printf("\nEnter values for Matrix B (%dx%d):\n", rows_b, cols_b);
    read_matrix(matrix_b, rows_b, cols_b);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    memset(result, 0, sizeof(result));
    multiply_matrices(matrix_a, rows_a, cols_a, matrix_b, rows_b, cols_b, result);
    
    printf("\nResult Matrix (%dx%d):\n", rows_a, cols_b);
    print_matrix(result, rows_a, cols_b);
    
    return 0;
}