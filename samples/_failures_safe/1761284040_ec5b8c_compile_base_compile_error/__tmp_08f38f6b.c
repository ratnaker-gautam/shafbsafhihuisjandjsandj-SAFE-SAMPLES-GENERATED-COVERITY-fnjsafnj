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

int read_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min_val || temp > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
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
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    choice = read_int("Enter choice (1-2): ", 1, 2);
    if (choice == -1) {
        return 1;
    }
    
    rows1 = read_int("Enter rows for first matrix (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) return 1;
    cols1 = read_int("Enter columns for first matrix (1-10): ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) return 1;
    
    printf("Enter first matrix:\n");
    read_matrix(matrix1, rows1, cols1);
    
    if (choice == 1) {
        rows2 = read_int("Enter rows for second matrix (1-10): ", MIN_DIM, MAX_ROWS);
        if (rows2 == -1) return 1;
        cols2 = read_int("Enter columns for second matrix (1-10): ", MIN_DIM, MAX_COLS);
        if (cols2 == -1) return 1;
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        
        printf("Enter second matrix:\n");
        read_matrix(matrix2, rows2, cols2);
        
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        
        printf("Result of addition:\n");
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        rows2 = read_int("Enter rows for second matrix (1-10): ", MIN_DIM, MAX_ROWS);
        if (rows2 == -1) return 1;
        cols2 = read_int("Enter columns for second matrix (1-10): ", MIN_DIM, MAX_COLS);
        if (cols2 == -1