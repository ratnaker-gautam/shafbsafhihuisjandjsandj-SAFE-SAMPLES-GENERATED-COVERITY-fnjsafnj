//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void read_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
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

void add_matrices(int rows, int cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int a[MAX_DIM][MAX_DIM], int rows2, int cols2, int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main() {
    int matrix_a[MAX_DIM][MAX_DIM];
    int matrix_b[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    printf("========================\n");
    
    int rows_a = read_int("Enter rows for matrix A (1-10): ", 1, MAX_DIM);
    int cols_a = read_int("Enter columns for matrix A (1-10): ", 1, MAX_DIM);
    
    printf("\nEnter elements for matrix A:\n");
    read_matrix(rows_a, cols_a, matrix_a);
    
    int rows_b = read_int("Enter rows for matrix B (1-10): ", 1, MAX_DIM);
    int cols_b = read_int("Enter columns for matrix B (1-10): ", 1, MAX_DIM);
    
    printf("\nEnter elements for matrix B:\n");
    read_matrix(rows_b, cols_b, matrix_b);
    
    printf("\nMatrix A:\n");
    print_matrix(rows_a, cols_a, matrix_a);
    
    printf("\nMatrix B:\n");
    print_matrix(rows_b, cols_b, matrix_b);
    
    printf("\nSelect operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("3. Matrix Transpose (A)\n");
    printf("4. Matrix Transpose (B)\n");
    
    int choice = read_int("Enter choice (1-4): ", 1, 4);
    
    switch (choice) {
        case 1:
            if (rows_a == rows_b && cols_a == cols_b) {
                add_matrices(rows_a, cols_a, matrix_a, matrix_b, result);
                printf("\nMatrix A + B:\n");
                print_matrix(rows_a, cols_a, result);
            } else {
                printf("Error: Matrices must have same dimensions for addition.\n");
            }
            break;
            
        case 2:
            if (cols_a == rows_b) {
                multiply_matrices(rows_a, cols_a, matrix_a, rows_b, cols_b, matrix_b, result);
                printf("\nMatrix A * B:\n");
                print_matrix(rows_a, cols_b, result);
            } else {
                printf("Error: Columns of A must equal rows of B for multiplication.\n");
            }
            break;
            
        case 3:
            transpose_matrix(rows_a, cols_a