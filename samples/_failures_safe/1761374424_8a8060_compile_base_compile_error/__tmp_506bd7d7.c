//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_INIT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                mat[i][j] = 0; \
            } \
        } \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                printf("%6d", mat[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(res, a, b, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                res[i][j] = a[i][j] + b[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULT(res, a, b, ra, ca, cb) \
    do { \
        for (int i = 0; i < ra; i++) { \
            for (int j = 0; j < cb; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < ca; k++) { \
                    res[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

void read_matrix(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            mat[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int matrix_a[MAX_ROWS][MAX_COLS];
    int matrix_b[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows_a = read_int("Enter rows for matrix A (1-10): ", MIN_DIM, MAX_ROWS);
    int cols_a = read_int("Enter columns for matrix A (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nEnter values for matrix A:\n");
    read_matrix(matrix_a, rows_a, cols_a);
    
    int operation = read_int("\nChoose operation:\n1. Matrix Addition\n2. Matrix Multiplication\nEnter choice (1-2): ", 1, 2);
    
    if (operation == 1) {
        int rows_b = read_int("Enter rows for matrix B (1-10): ", MIN_DIM, MAX_ROWS);
        int cols_b = read_int("Enter columns for matrix B (1-10): ", MIN_DIM, MAX_COLS);
        
        if (rows_a != rows_b || cols_a != cols_b) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        
        printf("\nEnter values for matrix B:\n");
        read_matrix(matrix_b, rows_b, cols_b);
        
        MATRIX_ADD(result, matrix_a, matrix_b, rows_a, cols_a);
        
        printf("\nMatrix A:\n");
        MATRIX_PRINT(matrix_a, rows_a, cols_a);
        
        printf("\nMatrix B:\n");
        MATRIX_PRINT(matrix_b, rows_b, cols_b);
        
        printf("\nResult (A + B):\n");
        MATRIX_PRINT(result, rows_a, cols_a);
    } else {
        int rows_b = read_int("Enter rows for matrix B (1-10): ", MIN_DIM, MAX_ROWS);
        int cols_b = read_int("Enter columns for matrix B (1-10): ", MIN_DIM, MAX_COLS);
        
        if (cols_a != rows_b) {
            printf("Error: Columns of A must equal rows of B for multiplication.\n");
            return 1;
        }
        
        printf("\nEnter values for matrix B:\n");
        read_matrix(matrix_b, rows_b, cols_b);
        
        MATRIX_MULT(result, matrix_a, matrix_b, rows_a, cols_a, cols