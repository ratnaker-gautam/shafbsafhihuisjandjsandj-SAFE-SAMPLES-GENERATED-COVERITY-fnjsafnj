//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
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
                printf("%4d", mat[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                dest[i][j] = src1[i][j] + src2[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULT(dest, src1, rows1, cols1, src2, rows2, cols2) \
    do { \
        for (int i = 0; i < rows1; i++) { \
            for (int j = 0; j < cols2; j++) { \
                dest[i][j] = 0; \
                for (int k = 0; k < cols1; k++) { \
                    dest[i][j] += src1[i][k] * src2[k][j]; \
                } \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min_val, int max_val) {
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

void read_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int("Enter rows for matrix 1: ", MIN_DIM, MAX_ROWS);
    int cols1 = read_int("Enter columns for matrix 1: ", MIN_DIM, MAX_COLS);
    
    printf("\nEnter values for matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);
    
    int rows2 = read_int("Enter rows for matrix 2: ", MIN_DIM, MAX_ROWS);
    int cols2 = read_int("Enter columns for matrix 2: ", MIN_DIM, MAX_COLS);
    
    printf("\nEnter values for matrix 2:\n");
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    printf("\nSelect operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    int choice = read_int("Enter choice (1-2): ", 1, 2);
    
    switch (choice) {
        case 1:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                return 1;
            }
            MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
            printf("\nResult of addition:\n");
            MATRIX_PRINT(result, rows1, cols1);
            break;
            
        case 2:
            if (cols1 != rows2) {
                printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
                return 1;
            }
            MATRIX_MULT(result, matrix1, rows1, cols1, matrix2, rows2, cols2);
            printf("\nResult of multiplication:\n");
            MATRIX_PRINT(result, rows1, cols