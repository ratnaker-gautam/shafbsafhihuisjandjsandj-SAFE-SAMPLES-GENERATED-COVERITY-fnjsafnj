//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        char* endptr;
        long long_val = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (long_val < min_val || long_val > max_val) {
            printf("Value must be between %d and %d\n", min_val, max_val);
            continue;
        }
        
        value = (int)long_val;
        break;
    }
    
    return value;
}

void read_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int a[MAX_ROWS][MAX_COLS], int b[MAX_ROWS][MAX_COLS], 
                  int result[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long sum = (long)a[i][j] + (long)b[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Integer overflow detected!\n");
                exit(1);
            }
            result[i][j] = (int)sum;
        }
    }
}

void multiply_matrices(int a[MAX_ROWS][MAX_COLS], int b[MAX_ROWS][MAX_COLS], 
                       int result[MAX_ROWS][MAX_COLS], int rows1, int cols1, int cols2) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            long sum = 0;
            for (int k = 0; k < cols1; k++) {
                long product = (long)a[i][k] * (long)b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected!\n");
                    exit(1);
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected!\n");
                    exit(1);
                }
            }
            result[i][j] = (int)sum;
        }
    }
}

void transpose_matrix(int src[MAX_ROWS][MAX_COLS], int dest[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[j][i] = src[i][j];
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    int transpose[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First matrix dimensions:\n");
    rows1 = read_int("Rows (1-10): ", 1, MAX_ROWS);
    cols1 = read_int("Columns (1-10): ", 1, MAX_COLS);
    
    printf("Second matrix dimensions:\n");
    rows2 = read_int("Rows (1-10): ", 1, MAX_ROWS);
    cols2 = read_int("Columns (1-10): ", 1, MAX_COLS);
    
    printf("\n");
    read_matrix(matrix1, rows1, cols1);
    printf("\n");
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nFirst ");
    print_matrix(matrix1, rows1, cols1);
    printf("\nSecond ");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        add_matrices(matrix1, matrix2, result, rows1, cols1);
        print_matrix(result, rows1, cols1);
    } else {
        printf("Cannot add matrices: dimensions don't match\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (