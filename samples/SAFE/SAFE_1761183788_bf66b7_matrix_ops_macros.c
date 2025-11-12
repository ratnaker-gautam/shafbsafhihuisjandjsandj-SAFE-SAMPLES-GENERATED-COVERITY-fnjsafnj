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

int get_valid_int(const char* prompt, int min_val, int max_val) {
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
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

void matrix_input(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            mat[i][j] = get_valid_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int matrix_a[MAX_ROWS][MAX_COLS];
    int matrix_b[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows_a = get_valid_int("Enter rows for matrix A (1-10): ", MIN_DIM, MAX_ROWS);
    int cols_a = get_valid_int("Enter columns for matrix A (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nInput for matrix A:\n");
    matrix_input(matrix_a, rows_a, cols_a);
    
    int rows_b = get_valid_int("Enter rows for matrix B (1-10): ", MIN_DIM, MAX_ROWS);
    int cols_b = get_valid_int("Enter columns for matrix B (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nInput for matrix B:\n");
    matrix_input(matrix_b, rows_b, cols_b);
    
    printf("\nMatrix A:\n");
    MATRIX_PRINT(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    MATRIX_PRINT(matrix_b, rows_b, cols_b);
    
    printf("\nMatrix Addition:\n");
    if (rows_a == rows_b && cols_a == cols_b) {
        MATRIX_ADD(result, matrix_a, matrix_b, rows_a, cols_a);
        MATRIX_PRINT(result, rows_a, cols_a);
    } else {
        printf("Cannot add: matrices have different dimensions.\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (cols_a == rows_b) {
        MATRIX_MULT(result, matrix_a, matrix_b, rows_a, cols_a, cols_b);
        MATRIX_PRINT(result, rows_a, cols_b);
    } else {
        printf("Cannot multiply: columns of A must equal rows of B.\n");
    }
    
    printf("\nMatrix Transpose of A:\n");
    for (int i = 0; i < cols_a; i++) {
        for (int j = 0; j < rows_a; j++) {
            printf("%6d", matrix_a[j][i]);
        }
        printf("\n");
    }
    
    return 0;
}