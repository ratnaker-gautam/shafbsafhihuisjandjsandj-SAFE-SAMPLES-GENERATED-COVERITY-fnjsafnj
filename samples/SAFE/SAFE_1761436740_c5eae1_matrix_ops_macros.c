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
                mat[i][j] = (i + j) % 10; \
            } \
        } \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                printf("%3d", mat[i][j]); \
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

#define MATRIX_MULTIPLY(res, a, b, rows_a, cols_a, cols_b) \
    do { \
        for (int i = 0; i < rows_a; i++) { \
            for (int j = 0; j < cols_b; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < cols_a; k++) { \
                    res[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char input[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return min_val;
        }
        
        value = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

int main(void) {
    int rows_a, cols_a, rows_b, cols_b;
    int matrix_a[MAX_ROWS][MAX_COLS];
    int matrix_b[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows_a = get_valid_int("Enter rows for matrix A (1-10): ", MIN_DIM, MAX_ROWS);
    cols_a = get_valid_int("Enter columns for matrix A (1-10): ", MIN_DIM, MAX_COLS);
    
    rows_b = get_valid_int("Enter rows for matrix B (1-10): ", MIN_DIM, MAX_ROWS);
    cols_b = get_valid_int("Enter columns for matrix B (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nInitializing matrix A...\n");
    MATRIX_INIT(matrix_a, rows_a, cols_a);
    printf("Matrix A:\n");
    MATRIX_PRINT(matrix_a, rows_a, cols_a);
    
    printf("\nInitializing matrix B...\n");
    MATRIX_INIT(matrix_b, rows_b, cols_b);
    printf("Matrix B:\n");
    MATRIX_PRINT(matrix_b, rows_b, cols_b);
    
    printf("\nMatrix Addition:\n");
    if (rows_a == rows_b && cols_a == cols_b) {
        MATRIX_ADD(result, matrix_a, matrix_b, rows_a, cols_a);
        printf("A + B:\n");
        MATRIX_PRINT(result, rows_a, cols_a);
    } else {
        printf("Cannot add matrices: dimensions don't match.\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (cols_a == rows_b) {
        MATRIX_MULTIPLY(result, matrix_a, matrix_b, rows_a, cols_a, cols_b);
        printf("A * B:\n");
        MATRIX_PRINT(result, rows_a, cols_b);
    } else {
        printf("Cannot multiply matrices: columns of A must equal rows of B.\n");
    }
    
    return 0;
}