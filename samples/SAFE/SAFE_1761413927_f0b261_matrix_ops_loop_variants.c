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
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            dst[j][i] = src[i][j];
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
                if (result[i][j] > INT_MAX - a[i][k] * b[k][j]) {
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
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
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
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("1. Transpose matrix\n");
    printf("2. Multiply matrices\n");
    
    choice = read_int_safe("Enter choice (1-2): ", 1, 2);
    
    switch (choice) {
        case 1: {
            rows1 = read_int_safe("Enter number of rows (1-10): ", 1, MAX_SIZE);
            cols1 = read_int_safe("Enter number of columns (1-10): ", 1, MAX_SIZE);
            
            printf("Enter matrix elements:\n");
            read_matrix(matrix1, rows1, cols1);
            
            printf("\nOriginal matrix:\n");
            print_matrix(matrix1, rows1, cols1);
            
            memset(result, 0, sizeof(result));
            transpose_matrix(matrix1, result, rows1, cols1);
            
            printf("\nTransposed matrix:\n");
            print_matrix(result, cols1, rows1);
            break;
        }
        
        case 2: {
            rows1 = read_int_safe("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
            cols1 = read_int_safe("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
            rows2 = cols1;
            cols2 = read_int_safe("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
            
            if (cols1 != rows2) {
                printf("Matrix dimensions incompatible for multiplication\n");
                return 1;
            }
            
            printf("Enter elements for matrix 1:\n");
            read_matrix(matrix1, rows1, cols1);
            
            printf("Enter elements for matrix 2:\n");
            read_matrix(matrix2, rows2, cols2);
            
            printf("\nMatrix 1:\n");
            print_matrix(matrix1, rows1, cols1);
            
            printf("\nMatrix 2:\n");
            print_matrix(matrix2, rows2, cols2);
            
            memset(result, 0, sizeof(result));
            multiply_matrices(matrix1, matrix2, result, rows1, cols1, cols2);
            
            printf("\nResult of multiplication:\n");
            print_matrix(result, rows1, cols2);
            break;
        }
    }
    
    return 0;
}