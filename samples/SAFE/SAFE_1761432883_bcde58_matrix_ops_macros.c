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
                printf("%3d ", mat[i][j]); \
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
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        break;
    }
    return value;
}

void read_matrix(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    choice = read_int("Select operation (1-2): ", 1, 2);
    if (choice == -1) {
        return 1;
    }
    
    printf("Enter dimensions for first matrix:\n");
    rows1 = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    cols1 = read_int("Columns: ", MIN_DIM, MAX_COLS);
    if (rows1 == -1 || cols1 == -1) {
        return 1;
    }
    
    read_matrix(matrix1, rows1, cols1);
    
    printf("Enter dimensions for second matrix:\n");
    rows2 = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    cols2 = read_int("Columns: ", MIN_DIM, MAX_COLS);
    if (rows2 == -1 || cols2 == -1) {
        return 1;
    }
    
    read_matrix(matrix2, rows2, cols2);
    
    if (choice == 1) {
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        printf("Result of matrix addition:\n");
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        if (cols1 != rows2) {
            printf("Error: Columns of first matrix must equal rows of second matrix.\n");
            return 1;
        }
        
        MATRIX_INIT(result, rows1, cols2);
        MATRIX_MULTIPLY(result, matrix1, matrix2, rows1, cols1, cols2);
        printf("Result of matrix multiplication:\n");
        MATRIX_PRINT(result, rows1, cols2);
    }
    
    return 0;
}