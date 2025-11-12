//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

int read_positive_int(const char* prompt, int max_val) {
    int value;
    char buffer[32];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value <= 0 || value > max_val) {
            printf("Value must be between 1 and %d.\n", max_val);
            continue;
        }
        
        return value;
    }
}

void recursive_matrix_multiply(int rows_a, int cols_a, int rows_b, int cols_b, 
                              int a[rows_a][cols_a], int b[rows_b][cols_b], 
                              int result[rows_a][cols_b], int i, int j, int k) {
    if (i >= rows_a) return;
    
    if (j >= cols_b) {
        recursive_matrix_multiply(rows_a, cols_a, rows_b, cols_b, a, b, result, i + 1, 0, 0);
        return;
    }
    
    if (k >= cols_a) {
        recursive_matrix_multiply(rows_a, cols_a, rows_b, cols_b, a, b, result, i, j + 1, 0);
        return;
    }
    
    if (k == 0) {
        result[i][j] = 0;
    }
    
    if ((INT_MAX - abs(result[i][j])) < abs(a[i][k] * b[k][j])) {
        printf("Warning: Potential integer overflow detected.\n");
    }
    
    result[i][j] += a[i][k] * b[k][j];
    recursive_matrix_multiply(rows_a, cols_a, rows_b, cols_b, a, b, result, i, j, k + 1);
}

void recursive_matrix_fill(int rows, int cols, int matrix[rows][cols], int i, int j) {
    if (i >= rows) return;
    
    if (j >= cols) {
        recursive_matrix_fill(rows, cols, matrix, i + 1, 0);
        return;
    }
    
    printf("Enter element [%d][%d]: ", i, j);
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        matrix[i][j] = 0;
    } else {
        int value;
        if (sscanf(buffer, "%d", &value) == 1) {
            matrix[i][j] = value;
        } else {
            matrix[i][j] = 0;
        }
    }
    
    recursive_matrix_fill(rows, cols, matrix, i, j + 1);
}

void recursive_matrix_print(int rows, int cols, int matrix[rows][cols], int i, int j) {
    if (i >= rows) {
        printf("\n");
        return;
    }
    
    if (j >= cols) {
        printf("\n");
        recursive_matrix_print(rows, cols, matrix, i + 1, 0);
        return;
    }
    
    printf("%6d ", matrix[i][j]);
    recursive_matrix_print(rows, cols, matrix, i, j + 1);
}

int main() {
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Matrix Multiplication using Recursion\n");
    printf("=====================================\n\n");
    
    rows_a = read_positive_int("Enter rows for matrix A (1-8): ", MAX_SIZE);
    if (rows_a == -1) return 1;
    
    cols_a = read_positive_int("Enter columns for matrix A (1-8): ", MAX_SIZE);
    if (cols_a == -1) return 1;
    
    rows_b = read_positive_int("Enter rows for matrix B (1-8): ", MAX_SIZE);
    if (rows_b == -1) return 1;
    
    cols_b = read_positive_int("Enter columns for matrix B (1-8): ", MAX_SIZE);
    if (cols_b == -1) return 1;
    
    if (cols_a != rows_b) {
        printf("Error: Columns of A must equal rows of B for multiplication.\n");
        return 1;
    }
    
    int matrix_a[MAX_SIZE][MAX_SIZE] = {0};
    int matrix_b[MAX_SIZE][MAX_SIZE] = {0};
    int result[MAX_SIZE][MAX_SIZE] = {0};
    
    printf("\nEnter elements for matrix A (%dx%d):\n", rows_a, cols_a);
    recursive_matrix_fill(rows_a, cols_a, matrix_a, 0, 0);
    
    printf("\nEnter elements for matrix B (%dx%d):\n", rows_b, cols_b);
    recursive_matrix_fill(rows_b, cols_b, matrix_b, 0, 0);
    
    printf("\nMatrix A:\n");
    recursive_matrix_print(rows_a, cols_a, matrix_a, 0, 0);
    
    printf("\nMatrix B:\n");
    recursive_matrix_print(rows_b, cols_b, matrix_b, 0, 0);
    
    printf("\nCalculating A × B...\n");
    recursive_matrix_multiply(rows_a, cols