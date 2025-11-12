//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_INPUT_LEN 100

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
    char input[MAX_INPUT_LEN];
    long val;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        input[strcspn(input, "\n")] = '\0';
        val = strtol(input, &endptr, 10);
        
        if (endptr == input || *endptr != '\0') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (val < min_val || val > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)val;
    }
}

int main(void) {
    int matrix_a[MAX_ROWS][MAX_COLS];
    int matrix_b[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows_a = get_valid_int("Enter rows for matrix A (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows_a == -1) return 1;
    
    int cols_a = get_valid_int("Enter columns for matrix A (1-10): ", MIN_DIM, MAX_COLS);
    if (cols_a == -1) return 1;
    
    printf("\nEnter values for matrix A (%dx%d):\n", rows_a, cols_a);
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "A[%d][%d]: ", i, j);
            matrix_a[i][j] = get_valid_int(prompt, INT_MIN, INT_MAX);
            if (matrix_a[i][j] == -1) return 1;
        }
    }
    
    int rows_b = get_valid_int("Enter rows for matrix B (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows_b == -1) return 1;
    
    int cols_b = get_valid_int("Enter columns for matrix B (1-10): ", MIN_DIM, MAX_COLS);
    if (cols_b == -1) return 1;
    
    printf("\nEnter values for matrix B (%dx%d):\n", rows_b, cols_b);
    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "B[%d][%d]: ", i, j);
            matrix_b[i][j] = get_valid_int(prompt, INT_MIN, INT_MAX);
            if (matrix_b[i][j] == -1) return 1;
        }
    }
    
    printf("\nMatrix A:\n");
    MATRIX_PRINT(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    MATRIX_PRINT(matrix_b, rows_b, cols_b);
    
    printf("\nSelect operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    int choice = get_valid_int("Enter choice (1-2): ", 1, 2);