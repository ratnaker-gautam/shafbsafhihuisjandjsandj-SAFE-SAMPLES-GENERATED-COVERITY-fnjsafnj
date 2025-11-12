//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            dest[j][i] = src[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < cols_a) {
                if (a[i][k] > 0 && b[k][j] > INT_MAX / a[i][k]) {
                    printf("Integer overflow detected in multiplication\n");
                    return;
                }
                if (a[i][k] < 0 && b[k][j] < INT_MAX / a[i][k]) {
                    printf("Integer overflow detected in multiplication\n");
                    return;
                }
                result[i][j] += a[i][k] * b[k][j];
                k++;
            }
        }
    }
}

int read_int_safe(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    do {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            exit(1);
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d\n", min_val, max_val);
            continue;
        }
        
        break;
    } while (1);
    
    return (int)value;
}

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int_safe(prompt, -1000, 1000);
        }
    }
}

int main() {
    int choice;
    int rows_a, cols_a, rows_b, cols_b;
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Transpose\n");
    printf("2. Matrix Multiplication\n");
    
    choice = read_int_safe("Enter choice (1-2): ", 1, 2);
    
    switch (choice) {
        case 1: {
            rows_a = read_int_safe("Enter number of rows (1-10): ", 1, MAX_SIZE);
            cols_a = read_int_safe("Enter number of columns (1-10): ", 1, MAX_SIZE);
            
            printf("Enter matrix A:\n");
            read_matrix(matrix_a, rows_a, cols_a);
            
            printf("\nOriginal Matrix:\n");
            print_matrix(matrix_a, rows_a, cols_a);
            
            memset(result, 0, sizeof(result));
            transpose_matrix(matrix_a, result, rows_a, cols_a);
            
            printf("\nTransposed Matrix:\n");
            print_matrix(result, cols_a, rows_a);
            break;
        }
        
        case 2: {
            rows_a = read_int_safe("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
            cols_a = read_int_safe("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
            rows_b = cols_a;
            cols_b = read_int_safe("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
            
            printf("Enter matrix A:\n");
            read_matrix(matrix_a, rows_a, cols_a);
            
            printf("Enter matrix B:\n");
            read_matrix(matrix_b, rows_b, cols_b);
            
            printf("\nMatrix A:\n");
            print_matrix(matrix_a, rows_a, cols_a);
            
            printf("\nMatrix B:\n");
            print_matrix(matrix_b, rows_b, cols_b);
            
            memset(result, 0, sizeof(result));
            multiply_matrices(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
            
            printf("\nResult Matrix:\n");
            print_matrix(result, rows_a, cols_b);
            break;
        }
    }
    
    return 0;
}