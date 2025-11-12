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
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                mat[i][j] = 0; \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) \
                printf("%6d", mat[i][j]); \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                dest[i][j] = src1[i][j] + src2[i][j]; \
    } while (0)

#define MATRIX_MULT(dest, mat1, r1, c1, mat2, r2, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                dest[i][j] = 0; \
                for (int k = 0; k < c1; k++) \
                    dest[i][j] += mat1[i][k] * mat2[k][j]; \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void read_matrix(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            mat[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First Matrix:\n");
    rows1 = read_int("Enter number of rows (1-10): ", MIN_DIM, MAX_ROWS);
    cols1 = read_int("Enter number of columns (1-10): ", MIN_DIM, MAX_COLS);
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nSecond Matrix:\n");
    rows2 = read_int("Enter number of rows (1-10): ", MIN_DIM, MAX_ROWS);
    cols2 = read_int("Enter number of columns (1-10): ", MIN_DIM, MAX_COLS);
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nFirst Matrix:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nSecond Matrix:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);
    
    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        MATRIX_INIT(result, rows1, cols1);
        MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
        MATRIX_PRINT(result, rows1, cols1);
    } else {
        printf("Cannot add matrices: dimensions do not match.\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (cols1 == rows2) {
        MATRIX_INIT(result, rows1, cols2);
        MATRIX_MULT(result, matrix1, rows1, cols1, matrix2, rows2, cols2);
        MATRIX_PRINT(result, rows1, cols2);
    } else {
        printf("Cannot multiply matrices: inner dimensions do not match.\n");
    }
    
    return 0;
}