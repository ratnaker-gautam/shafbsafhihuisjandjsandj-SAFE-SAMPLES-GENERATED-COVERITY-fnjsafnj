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

#define MATRIX_MULT(res, a, b, a_rows, a_cols, b_cols) \
    do { \
        for (int i = 0; i < a_rows; i++) { \
            for (int j = 0; j < b_cols; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < a_cols; k++) { \
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
    int matrix_a[MAX_ROWS][MAX_COLS];
    int matrix_b[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows_a = get_valid_int("Enter rows for matrix A (1-10): ", MIN_DIM, MAX_ROWS);
    int cols_a = get_valid_int("Enter columns for matrix A (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nMatrix A dimensions: %d x %d\n", rows_a, cols_a);
    printf("Enter elements for matrix A:\n");
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "A[%d][%d]: ", i, j);
            matrix_a[i][j] = get_valid_int(prompt, -1000, 1000);
        }
    }
    
    int rows_b, cols_b;
    printf("\nMatrix B dimensions: ");
    if (cols_a <= MAX_ROWS) {
        rows_b = cols_a;
        cols_b = get_valid_int("Enter columns for matrix B (1-10): ", MIN_DIM, MAX_COLS);
        printf("Matrix B dimensions: %d x %d\n", rows_b, cols_b);
    } else {
        rows_b = get_valid_int("Enter rows for matrix B (1-10): ", MIN_DIM, MAX_ROWS);
        cols_b = get_valid_int("Enter columns for matrix B (1-10): ", MIN_DIM, MAX_COLS);
    }
    
    printf("Enter elements for matrix B:\n");
    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "B[%d][%d]: ", i, j);
            matrix_b[i][j] = get_valid_int(prompt, -1000, 1000);
        }
    }
    
    printf("\nMatrix A:\n");
    MATRIX_PRINT(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    MATRIX_PRINT(matrix_b, rows_b, cols_b);
    
    printf("\nPerforming matrix addition...\n");
    if (rows_a == rows_b && cols_a == cols_b) {
        MATRIX_INIT(result, rows_a, cols_a);
        MATRIX_ADD(result, matrix_a, matrix_b, rows_a, cols_a);
        printf("A + B:\n");
        MATRIX_PRINT(result,