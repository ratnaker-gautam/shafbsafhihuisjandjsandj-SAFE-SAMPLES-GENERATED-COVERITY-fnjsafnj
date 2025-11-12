//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
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

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void transpose_matrix(int* src, int* dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(dest + j * rows + i) = *(src + i * cols + j);
        }
    }
}

int multiply_matrices(int* a, int* b, int* result, int a_rows, int a_cols, int b_rows, int b_cols) {
    if (a_cols != b_rows) {
        return 0;
    }
    
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
            *(result + i * b_cols + j) = sum;
        }
    }
    
    return 1;
}

int main() {
    int matrix_a[MAX_SIZE * MAX_SIZE];
    int matrix_b[MAX_SIZE * MAX_SIZE];
    int result[MAX_SIZE * MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows_a = read_int("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
    int cols_a = read_int("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
    
    printf("\nFilling matrix A:\n");
    fill_matrix(matrix_a, rows_a, cols_a);
    print_matrix(matrix_a, rows_a, cols_a);
    
    int rows_b = read_int("\nEnter rows for matrix B (1-10): ", 1, MAX_SIZE);
    int cols_b = read_int("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
    
    printf("\nFilling matrix B:\n");
    fill_matrix(matrix_b, rows_b, cols_b);
    print_matrix(matrix_b, rows_b, cols_b);
    
    printf("\nMatrix A transpose:\n");
    int transpose[MAX_SIZE * MAX_SIZE];
    transpose_matrix(matrix_a, transpose, rows_a, cols_a);
    print_matrix(transpose, cols_a, rows_a);
    
    printf("\nMatrix B transpose:\n");
    transpose_matrix(matrix_b, transpose, rows_b, cols_b);
    print_matrix(transpose, cols_b, rows_b);
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(matrix_a, matrix_b, result, rows_a, cols_a, rows_b, cols_b)) {
        print_matrix(result, rows_a, cols_b);
    } else {
        printf("Cannot multiply: columns of A (%d) must equal rows of B (%d)\n", cols_a, rows_b);
    }
    
    return 0;
}