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
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

void read_matrix(int rows, int cols, int matrix[MAX_ROWS][MAX_COLS]) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_ROWS][MAX_COLS]) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int rows, int cols, int a[MAX_ROWS][MAX_COLS], int b[MAX_ROWS][MAX_COLS], int result[MAX_ROWS][MAX_COLS]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || 
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                printf("Integer overflow detected in addition\n");
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int rows2, int cols2, int a[MAX_ROWS][MAX_COLS], int b[MAX_ROWS][MAX_COLS], int result[MAX_ROWS][MAX_COLS]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication\n");
                    exit(1);
                }
                long long sum = (long long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected in summation\n");
                    exit(1);
                }
                result[i][j] = (int)sum;
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[MAX_ROWS][MAX_COLS], int result[MAX_ROWS][MAX_COLS]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main(void) {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("3. Matrix Transpose\n");
    
    choice = read_int("Select operation (1-3): ", 1, 3);
    
    switch (choice) {
        case 1:
            printf("Matrix Addition\n");
            rows1 = read_int("Enter rows for first matrix (1-10): ", 1, MAX_ROWS);
            cols1 = read_int("Enter columns for first matrix (1-10): ", 1, MAX_COLS);
            read_matrix(rows1, cols1, matrix1);
            
            rows2 = read_int("Enter rows for second matrix (1-10): ", 1, MAX_ROWS);
            cols2 = read_int("Enter columns for second matrix (1-10): ", 1, MAX_COLS);
            
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition\n");
                return 1;
            }
            
            read_matrix(rows2, cols2, matrix2);
            add_matrices(rows1, cols1, matrix1, matrix2, result);
            
            printf("Matrix A:\n");