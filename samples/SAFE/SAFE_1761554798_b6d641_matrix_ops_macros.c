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
            printf("Invalid input. Please enter a valid integer.\n");
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

void read_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Enter dimensions for first matrix:\n");
    rows1 = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    cols1 = read_int("Columns: ", MIN_DIM, MAX_COLS);
    
    printf("Enter dimensions for second matrix:\n");
    rows2 = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    cols2 = read_int("Columns: ", MIN_DIM, MAX_COLS);
    
    printf("\nEnter elements for first matrix:\n");
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nEnter elements for second matrix:\n");
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nFirst matrix:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nSecond matrix:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    if (rows1 == rows2 && cols1 == cols2) {
        MATRIX_INIT(result, rows1, cols1);
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        printf("\nMatrix addition result:\n");
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        printf("\nMatrices cannot be added (dimensions differ).\n");
    }
    
    if (cols1 == rows2) {
        MATRIX_INIT(result, rows1, cols2);
        MATRIX_MULTIPLY(result, matrix1, matrix2, rows1, cols1, cols2);
        printf("\nMatrix multiplication result:\n");
        MATRIX_PRINT(result, rows1, cols2);
    } else {
        printf("\nMatrices cannot be multiplied (columns of first != rows of second).\n");
    }
    
    return 0;
}