//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

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

void read_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void multiply_matrices(int rows1, int cols1, int matrix1[MAX_DIM][MAX_DIM],
                      int rows2, int cols2, int matrix2[MAX_DIM][MAX_DIM],
                      int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)matrix1[i][k] * (long long)matrix2[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    exit(1);
                }
                result[i][j] += (int)product;
                if ((result[i][j] > 0 && matrix1[i][k] > 0 && matrix2[k][j] > 0 && result[i][j] < 0) ||
                    (result[i][j] < 0 && matrix1[i][k] < 0 && matrix2[k][j] < 0 && result[i][j] > 0)) {
                    printf("Integer overflow detected in addition.\n");
                    exit(1);
                }
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Multiplication Program\n");
    printf("=============================\n");
    
    printf("First matrix dimensions:\n");
    rows1 = read_int("Enter rows (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns (1-10): ", 1, MAX_DIM);
    
    printf("Second matrix dimensions:\n");
    rows2 = read_int("Enter rows (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Enter columns (1-10): ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Error: Cannot multiply matrices. Columns of first matrix (%d) must equal rows of second matrix (%d).\n", cols1, rows2);
        return 1;
    }
    
    printf("\nEnter first matrix elements:\n");
    read_matrix(rows1, cols1, matrix1);
    
    printf("\nEnter second matrix elements:\n");
    read_matrix(rows2, cols2, matrix2);
    
    printf("\nFirst matrix:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nSecond matrix:\n");
    print_matrix(rows2, cols2, matrix2);
    
    multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
    
    printf("\nResult matrix:\n");
    print_matrix(rows1, cols2, result);
    
    return 0;
}