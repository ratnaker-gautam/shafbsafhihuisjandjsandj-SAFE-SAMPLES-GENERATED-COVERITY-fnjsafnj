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
                printf("%6.2f", mat[i][j]); \
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

int read_int(const char* prompt, int min_val, int max_val) {
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

int read_matrix(double mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    char input[MAX_INPUT_LEN];
    double val;
    char* endptr;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            while (1) {
                printf("Enter element [%d][%d]: ", i, j);
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    return 0;
                }
                
                input[strcspn(input, "\n")] = '\0';
                val = strtod(input, &endptr);
                
                if (endptr == input || *endptr != '\0') {
                    printf("Invalid input. Please enter a number.\n");
                    continue;
                }
                
                mat[i][j] = val;
                break;
            }
        }
    }
    return 1;
}

int main(void) {
    double matrix_a[MAX_ROWS][MAX_COLS];
    double matrix_b[MAX_ROWS][MAX_COLS];
    double result[MAX_ROWS][MAX_COLS];
    int rows_a, cols_a, rows_b, cols_b;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows_a = read_int("Enter rows for matrix A (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows_a == -1) return 1;
    
    cols_a = read_int("Enter columns for matrix A (1-10): ", MIN_DIM, MAX_COLS);
    if (cols_a == -1) return 1;
    
    printf("\nEnter values for matrix A:\n");
    if (!read_matrix(matrix_a, rows_a, cols_a)) return 1;
    
    printf("\nMatrix A:\n");
    MATRIX_PRINT(matrix_a, rows_a, cols_a);
    
    printf("\nSelect operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    operation = read_int("Enter choice (1-2): ", 1, 2);
    if (operation == -1) return 1;
    
    if (operation == 1) {
        rows_b = read_int("Enter rows for matrix B (1-10): ", MIN_DIM, MAX_ROWS);
        if (rows_b == -1) return 1;
        
        cols_b = read_int("Enter columns for matrix B (1-10): ", MIN_DIM, MAX_COLS);
        if (cols_b == -1) return 1;
        
        if (rows_a != rows_b || cols_a != cols_b