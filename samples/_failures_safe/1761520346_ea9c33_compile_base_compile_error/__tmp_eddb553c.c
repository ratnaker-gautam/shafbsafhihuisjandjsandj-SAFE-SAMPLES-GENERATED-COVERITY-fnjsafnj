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

#define MATRIX_MULTIPLY(res, a, b, ra, ca, cb) \
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

int read_int(const char* prompt, int min_val, int max_val) {
    char input[MAX_INPUT_LEN];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        if (sscanf(input, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min_val, max_val);
    }
}

void read_matrix(double mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            char input[MAX_INPUT_LEN];
            if (fgets(input, sizeof(input), stdin) == NULL) {
                return;
            }
            double value;
            if (sscanf(input, "%lf", &value) != 1) {
                printf("Invalid input. Using 0.0\n");
                value = 0.0;
            }
            mat[i][j] = value;
        }
    }
}

int main(void) {
    double matrix_a[MAX_ROWS][MAX_COLS];
    double matrix_b[MAX_ROWS][MAX_COLS];
    double result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    
    int rows_a = read_int("Enter rows for matrix A (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows_a == -1) return 1;
    int cols_a = read_int("Enter columns for matrix A (1-10): ", MIN_DIM, MAX_COLS);
    if (cols_a == -1) return 1;
    
    printf("Enter values for matrix A:\n");
    read_matrix(matrix_a, rows_a, cols_a);
    
    int operation;
    printf("Select operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    operation = read_int("Enter choice (1-2): ", 1, 2);
    if (operation == -1) return 1;
    
    if (operation == 1) {
        int rows_b = read_int("Enter rows for matrix B (1-10): ", MIN_DIM, MAX_ROWS);
        if (rows_b == -1) return 1;
        int cols_b = read_int("Enter columns for matrix B (1-10): ", MIN_DIM, MAX_COLS);
        if (cols_b == -1) return 1;
        
        if (rows_a != rows_b || cols_a != cols_b) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        
        printf("Enter values for matrix B:\n");
        read_matrix(matrix_b, rows_b, cols_b);
        
        MATRIX_INIT(result, rows_a, cols_a);
        MATRIX_ADD(result, matrix_a, matrix_b, rows_a, cols_a);
        
        printf("Matrix A:\n");
        MATRIX_PRINT(matrix_a, rows_a, cols_a);
        printf("Matrix B:\n");
        MATRIX_PRINT(matrix_b, rows_b, cols_b);
        printf("Result (A + B):\n");
        MATRIX_PRINT(result, rows_a, cols_a);
    } else {
        int rows_b