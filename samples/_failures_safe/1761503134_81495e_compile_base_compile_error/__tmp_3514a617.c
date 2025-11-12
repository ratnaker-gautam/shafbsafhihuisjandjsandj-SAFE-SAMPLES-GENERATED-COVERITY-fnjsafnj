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

#define MATRIX_MULT(res, a, b, arows, acols, bcols) \
    do { \
        for (int i = 0; i < arows; i++) { \
            for (int j = 0; j < bcols; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < acols; k++) { \
                    res[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return (int)value;
    }
}

void matrix_fill(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            mat[i][j] = read_int("", INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int matrix_a[MAX_ROWS][MAX_COLS];
    int matrix_b[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    int rows_a, cols_a, rows_b, cols_b;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Enter dimensions for Matrix A:\n");
    rows_a = read_int("Rows (1-10): ", MIN_DIM, MAX_ROWS);
    cols_a = read_int("Columns (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("Enter dimensions for Matrix B:\n");
    rows_b = read_int("Rows (1-10): ", MIN_DIM, MAX_ROWS);
    cols_b = read_int("Columns (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nFilling Matrix A:\n");
    matrix_fill(matrix_a, rows_a, cols_a);
    
    printf("\nFilling Matrix B:\n");
    matrix_fill(matrix_b, rows_b, cols_b);
    
    printf("\nMatrix A:\n");
    MATRIX_PRINT(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    MATRIX_PRINT(matrix_b, rows_b, cols_b);
    
    printf("\nSelect operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    operation = read_int("Choice (1-2): ", 1, 2);
    
    MATRIX_INIT(result, MAX_ROWS, MAX_COLS);
    
    switch (operation) {
        case 1:
            if (rows_a != rows_b || cols_a != cols_b) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                return 1;
            }
            MATRIX_ADD(result, matrix_a, matrix_b, rows_a, cols_a);
            printf("\nMatrix A + B:\n");
            MATRIX_PRINT(result, rows_a, cols_a);
            break;
            
        case 2:
            if (cols_a != rows_b) {
                printf("Error: Columns of A must equal rows of B for multiplication.\n");
                return 1;
            }
            MATRIX_MULT(result, matrix_a, matrix_b, rows_a, cols_a, cols_b);
            printf("\nMatrix A * B:\n");
            MATRIX_PRINT