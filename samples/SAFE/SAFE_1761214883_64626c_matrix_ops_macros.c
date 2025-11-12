//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_ELEM(m, i, j, cols) ((m)[(i) * (cols) + (j)])

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min_val || temp > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            MATRIX_ELEM(matrix, i, j, cols) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_ELEM(matrix, i, j, cols));
        }
        printf("\n");
    }
}

void multiply_matrices(const int* a, const int* b, int* result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long product = (long)MATRIX_ELEM(a, i, k, a_cols) * (long)MATRIX_ELEM(b, k, j, b_cols);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    return;
                }
                sum += (int)product;
                if ((sum > 0 && product < 0) || (sum < 0 && product > 0)) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
            }
            MATRIX_ELEM(result, i, j, b_cols) = sum;
        }
    }
}

int main() {
    int matrix_a[MAX_ROWS * MAX_COLS];
    int matrix_b[MAX_ROWS * MAX_COLS];
    int result[MAX_ROWS * MAX_COLS];
    
    printf("Matrix Multiplication Program\n");
    printf("=============================\n");
    
    int rows_a = read_int("Enter rows for matrix A (1-10): ", MIN_DIM, MAX_ROWS);
    int cols_a = read_int("Enter columns for matrix A (1-10): ", MIN_DIM, MAX_COLS);
    int rows_b = read_int("Enter rows for matrix B (1-10): ", MIN_DIM, MAX_ROWS);
    int cols_b = read_int("Enter columns for matrix B (1-10): ", MIN_DIM, MAX_COLS);
    
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
    multiply_matrices(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
    
    printf("\nResult Matrix (%dx%d):\n", rows_a, cols_b);
    print_matrix(result, rows_a, cols_b);
    
    return 0;
}