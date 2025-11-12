//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
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
            printf("Input error\n");
            exit(1);
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d\n", min, max);
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

void multiply_matrices(int* a, int* b, int* result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
            *(result + i * b_cols + j) = sum;
        }
    }
}

int main() {
    int matrix_a[MAX_SIZE * MAX_SIZE];
    int matrix_b[MAX_SIZE * MAX_SIZE];
    int result[MAX_SIZE * MAX_SIZE];
    
    printf("Matrix Operations Program\n\n");
    
    int rows_a = read_int("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
    int cols_a = read_int("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
    
    printf("\nFilling matrix A:\n");
    fill_matrix(matrix_a, rows_a, cols_a);
    
    int operation = read_int("\nChoose operation:\n1. Transpose\n2. Multiply with another matrix\nChoice: ", 1, 2);
    
    if (operation == 1) {
        transpose_matrix(matrix_a, result, rows_a, cols_a);
        printf("\nOriginal matrix:\n");
        print_matrix(matrix_a, rows_a, cols_a);
        printf("\nTransposed matrix:\n");
        print_matrix(result, cols_a, rows_a);
    } else {
        int rows_b = read_int("Enter rows for matrix B (1-10): ", 1, MAX_SIZE);
        int cols_b = read_int("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
        
        if (cols_a != rows_b) {
            printf("Matrix multiplication not possible: columns of A (%d) must equal rows of B (%d)\n", cols_a, rows_b);
            return 1;
        }
        
        printf("\nFilling matrix B:\n");
        fill_matrix(matrix_b, rows_b, cols_b);
        
        multiply_matrices(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
        
        printf("\nMatrix A:\n");
        print_matrix(matrix_a, rows_a, cols_a);
        printf("\nMatrix B:\n");
        print_matrix(matrix_b, rows_b, cols_b);
        printf("\nResult of multiplication:\n");
        print_matrix(result, rows_a, cols_b);
    }
    
    return 0;
}